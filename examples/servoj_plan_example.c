#include <Elite_Sdk_C.h>
//保证在不同平台上都能正确使用时间相关的函数和字符串处理函数
#include "example_portability.h" 

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(__linux__) || defined(linux) || defined(__linux)
#include <pthread.h>
#include <sys/mman.h>
#endif

typedef struct trapezoidal_point_t {
  double t;
  double pos;
  double vel;
  double acc;
} trapezoidal_point_t;

typedef struct trapezoidal_plan_t {
  trapezoidal_point_t *points;
  size_t count;
} trapezoidal_plan_t;

static void print_status(const char *name, elite_c_status_t status,
                         int32_t value) {
  printf("[%-40s] status=%d result=%d\n", name, (int)status, (int)value);
}

static void print_usage(const char *program) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr,
          "  %s <robot-ip> [--local-ip <ip>] [--headless] [--max-speed "
          "<value>] [--max-acc <value>] "
          "[--script-file <path>]\n",
          program);
  fprintf(stderr, "Example:\n");
  fprintf(stderr,
          "  %s 192.168.1.200 --headless --max-speed 2.0 --max-acc 2.0\n",
          program);
}

static int parse_double_arg(const char *text, double *out_value) {
  char *end = NULL;
  double value = 0.0;

  if (!text || !out_value) {
    return 0;
  }

  value = strtod(text, &end);
  if (end == text || (end && *end != '\0')) {
    return 0;
  }

  *out_value = value;
  return 1;
}

static int require_ok(const char *name, elite_c_status_t status, int32_t ok,
                      const char *error_text) {
  print_status(name, status, ok);
  if (status == ELITE_C_STATUS_OK && ok) {
    return 1;
  }
  if (error_text) {
    fprintf(stderr, "%s failed: %s\n", name, error_text);
  } else {
    fprintf(stderr, "%s failed\n", name);
  }
  return 0;
}

static int require_status(const char *name, elite_c_status_t status,
                          const char *error_text) {
  printf("[%-40s] status=%d\n", name, (int)status);
  if (status == ELITE_C_STATUS_OK) {
    return 1;
  }
  if (error_text) {
    fprintf(stderr, "%s failed: %s\n", name, error_text);
  } else {
    fprintf(stderr, "%s failed\n", name);
  }
  return 0;
}

static void free_plan(trapezoidal_plan_t *plan) {
  if (!plan) {
    return;
  }
  free(plan->points);
  plan->points = NULL;
  plan->count = 0;
}

static int append_plan_point(trapezoidal_plan_t *plan, size_t *capacity,
                             double t, double pos, double vel, double acc) {
  trapezoidal_point_t *new_points = NULL;
  size_t new_capacity = 0;

  if (!plan || !capacity) {
    return 0;
  }

  if (plan->count == *capacity) {
    new_capacity = (*capacity == 0) ? 128 : (*capacity * 2);
    new_points = (trapezoidal_point_t *)realloc(
        plan->points, new_capacity * sizeof(trapezoidal_point_t));
    if (!new_points) {
      return 0;
    }
    plan->points = new_points;
    *capacity = new_capacity;
  }

  plan->points[plan->count].t = t;
  plan->points[plan->count].pos = pos;
  plan->points[plan->count].vel = vel;
  plan->points[plan->count].acc = acc;
  plan->count += 1;
  return 1;
}

static int trapezoidal_speed_plan(double start, double end, double vmax,
                                  double amax, double dt,
                                  trapezoidal_plan_t *out_plan) {
  double dq = 0.0;
  double dir = 0.0;
  double t_acc = 0.0;
  double d_acc = 0.0;
  double t_flat = 0.0;
  double total_time = 0.0;
  size_t capacity = 0;
  double t = 0.0;

  if (!out_plan || dt <= 0.0 || vmax <= 0.0 || amax <= 0.0) {
    return 0;
  }

  out_plan->points = NULL;
  out_plan->count = 0;

  dq = end - start;
  dir = (dq >= 0.0) ? 1.0 : -1.0;
  dq = fabs(dq);

  t_acc = vmax / amax;
  d_acc = 0.5 * amax * t_acc * t_acc;

  if (dq < 2.0 * d_acc) {
    t_acc = sqrt(dq / amax);
    d_acc = 0.5 * amax * t_acc * t_acc;
    t_flat = 0.0;
  } else {
    t_flat = (dq - 2.0 * d_acc) / vmax;
  }

  total_time = 2.0 * t_acc + t_flat;

  for (t = 0.0; t <= total_time + 1e-6; t += dt) {
    double pos = 0.0;
    double vel = 0.0;
    double acc = 0.0;

    if (t < t_acc) {
      acc = dir * amax;
      vel = acc * t;
      pos = start + dir * (0.5 * amax * t * t);
    } else if (t < t_acc + t_flat) {
      acc = 0.0;
      vel = dir * vmax;
      pos = start + dir * (d_acc + vmax * (t - t_acc));
    } else if (t <= total_time) {
      double td = t - (t_acc + t_flat);
      acc = -dir * amax;
      vel = dir * vmax - amax * td * dir;
      pos = end - dir * (0.5 * amax * (total_time - t) * (total_time - t));
    } else {
      acc = 0.0;
      vel = 0.0;
      pos = end;
    }

    if (!append_plan_point(out_plan, &capacity, t, pos, vel, acc)) {
      free_plan(out_plan);
      return 0;
    }
  }

  if (out_plan->count == 0 ||
      fabs(out_plan->points[out_plan->count - 1].pos - end) > 1e-9) {
    if (!append_plan_point(out_plan, &capacity, total_time, end, 0.0, 0.0)) {
      free_plan(out_plan);
      return 0;
    }
  }

  return 1;
}

int main(int argc, char **argv) {
  elite_driver_config_t config;
  elite_driver_handle_t *driver = NULL;
  elite_rtsi_io_handle_t *rtsi = NULL;
  elite_dashboard_handle_t *dashboard = NULL;
  elite_version_info_t version = {0};
  elite_c_status_t status;
  int32_t ok = 0;
  int32_t connected = 0;
  const char *robot_ip = NULL;
  const char *local_ip = "";
  const char *script_file_path = "external_control.script";
  int32_t headless_mode = 0;
  double max_speed = 2.0;
  double max_acc = 2.0;
  double actual_joint[6] = {0};
  double target_joint[6] = {0};
  const double joint_final_target = 3.0;
  trapezoidal_plan_t plan = {0};
  int64_t next_tick_ns = 0;
  int i;
  int exit_code = 1;

#if defined(__linux__) || defined(linux) || defined(__linux)
  mlockall(MCL_CURRENT | MCL_FUTURE);
  (void)pthread_self();
#endif

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  robot_ip = argv[1];

  for (i = 2; i < argc; ++i) {
    if (strcmp(argv[i], "--local-ip") == 0 && i + 1 < argc) {
      local_ip = argv[++i];
    } else if (strcmp(argv[i], "--headless") == 0) {
      headless_mode = 1;
    } else if (strcmp(argv[i], "--max-speed") == 0 && i + 1 < argc) {
      if (!parse_double_arg(argv[++i], &max_speed)) {
        fprintf(stderr, "invalid --max-speed value\n");
        return 1;
      }
    } else if (strcmp(argv[i], "--max-acc") == 0 && i + 1 < argc) {
      if (!parse_double_arg(argv[++i], &max_acc)) {
        fprintf(stderr, "invalid --max-acc value\n");
        return 1;
      }
    } else if (strcmp(argv[i], "--script-file") == 0 && i + 1 < argc) {
      script_file_path = argv[++i];
    } else {
      fprintf(stderr, "unknown argument: %s\n", argv[i]);
      print_usage(argv[0]);
      return 1;
    }
  }

  if (headless_mode) {
    printf(
        "Use headless mode. Please ensure the robot is not in local mode.\n");
  } else {
    printf("Non-headless mode requires the External Control plugin in the task "
           "tree.\n");
  }

  elite_driver_config_set_default(&config);
  config.robot_ip = robot_ip;
  config.local_ip = local_ip;
  config.script_file_path = script_file_path;
  config.headless_mode = headless_mode;
  config.servoj_time = 0.004f;

  status = elite_dashboard_create(&dashboard);
  if (status != ELITE_C_STATUS_OK || !dashboard) {
    fprintf(stderr, "elite_dashboard_create failed: %s\n",
            elite_dashboard_global_last_error_message());
    goto cleanup;
  }

  status = elite_rtsi_io_create("actual_joint_positions", "", 250.0, &rtsi);
  if (status != ELITE_C_STATUS_OK || !rtsi) {
    fprintf(stderr, "elite_rtsi_io_create failed: %s\n",
            elite_c_last_error_message());
    goto cleanup;
  }

  status = elite_driver_create(&config, &driver);
  if (status != ELITE_C_STATUS_OK || !driver) {
    fprintf(stderr, "elite_driver_create failed: %s\n",
            elite_c_last_error_message());
    goto cleanup;
  }

  status = elite_dashboard_connect(dashboard, robot_ip, 29999, &ok);
  if (!require_ok("elite_dashboard_connect", status, ok,
                  elite_dashboard_last_error_message(dashboard))) {
    goto cleanup;
  }

  status = elite_rtsi_io_connect(rtsi, robot_ip, &ok);
  if (!require_ok("elite_rtsi_io_connect", status, ok,
                  elite_rtsi_io_last_error_message(rtsi))) {
    goto cleanup;
  }

  status = elite_rtsi_io_get_controller_version(rtsi, &version);
  if (!require_status("elite_rtsi_io_get_controller_version", status,
                      elite_rtsi_io_last_error_message(rtsi))) {
    goto cleanup;
  }
  printf("controller version: %u.%u.%u.%u\n", version.major, version.minor,
         version.bugfix, version.build);

  status = elite_dashboard_power_on(dashboard, &ok);
  if (!require_ok("elite_dashboard_power_on", status, ok,
                  elite_dashboard_last_error_message(dashboard))) {
    goto cleanup;
  }

  status = elite_dashboard_brake_release(dashboard, &ok);
  if (!require_ok("elite_dashboard_brake_release", status, ok,
                  elite_dashboard_last_error_message(dashboard))) {
    goto cleanup;
  }

  status = elite_driver_is_robot_connected(driver, &connected);
  if (!require_status("elite_driver_is_robot_connected", status,
                      elite_driver_last_error_message(driver))) {
    goto cleanup;
  }

  if (headless_mode) {
    if (!connected) {
      status = elite_driver_send_external_control_script(driver, &ok);
      if (!require_ok("elite_driver_send_external_control_script", status, ok,
                      elite_driver_last_error_message(driver))) {
        goto cleanup;
      }
    }
  } else {
    status = elite_dashboard_play_program(dashboard, &ok);
    if (!require_ok("elite_dashboard_play_program", status, ok,
                    elite_dashboard_last_error_message(dashboard))) {
      goto cleanup;
    }
  }

  printf("Wait external control script run...\n");
  for (i = 0; i < 3000; ++i) {
    status = elite_driver_is_robot_connected(driver, &connected);
    if (status == ELITE_C_STATUS_OK && connected) {
      break;
    }
    example_sleep_us(10000);
  }
  if (!connected) {
    fprintf(stderr, "robot did not enter external control mode in time\n");
    goto cleanup;
  }
  printf("External control script is running\n");

  status = elite_rtsi_io_get_actual_joint_positions(rtsi, actual_joint);
  if (!require_status("elite_rtsi_io_get_actual_joint_positions", status,
                      elite_rtsi_io_last_error_message(rtsi))) {
    goto cleanup;
  }
  memcpy(target_joint, actual_joint, sizeof(target_joint));

  if (!trapezoidal_speed_plan(target_joint[5], joint_final_target, max_speed,
                              max_acc, config.servoj_time, &plan)) {
    fprintf(stderr, "failed to create positive servoj plan\n");
    goto cleanup;
  }

  next_tick_ns = example_monotonic_now_ns();
  for (i = 0; i < (int)plan.count; ++i) {
    target_joint[5] = plan.points[i].pos;
    status = elite_driver_write_servoj(driver, target_joint, 100, 0, &ok);
    if (!require_ok("elite_driver_write_servoj", status, ok,
                    elite_driver_last_error_message(driver))) {
      free_plan(&plan);
      goto cleanup;
    }
    next_tick_ns += 4000000LL;
    example_sleep_until_monotonic_ns(next_tick_ns);
  }
  free_plan(&plan);

  if (!trapezoidal_speed_plan(target_joint[5], -joint_final_target, max_speed,
                              max_acc, config.servoj_time, &plan)) {
    fprintf(stderr, "failed to create negative servoj plan\n");
    goto cleanup;
  }

  for (i = 0; i < (int)plan.count; ++i) {
    target_joint[5] = plan.points[i].pos;
    status = elite_driver_write_servoj(driver, target_joint, 100, 0, &ok);
    if (!require_ok("elite_driver_write_servoj", status, ok,
                    elite_driver_last_error_message(driver))) {
      free_plan(&plan);
      goto cleanup;
    }
    next_tick_ns += 4000000LL;
    example_sleep_until_monotonic_ns(next_tick_ns);
  }
  free_plan(&plan);

  status = elite_driver_stop_control(driver, 1000, &ok);
  if (!require_ok("elite_driver_stop_control", status, ok,
                  elite_driver_last_error_message(driver))) {
    goto cleanup;
  }

  printf("Motion finish\n");
  exit_code = 0;

cleanup:
  free_plan(&plan);
  if (rtsi) {
    elite_rtsi_io_disconnect(rtsi);
    elite_rtsi_io_destroy(rtsi);
  }
  if (driver) {
    elite_driver_destroy(driver);
  }
  if (dashboard) {
    elite_dashboard_disconnect(dashboard);
    elite_dashboard_destroy(dashboard);
  }
  return exit_code;
}

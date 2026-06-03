#include <Elite_Sdk_C.h>

#include "example_portability.h"

#include <stdint.h>
#include <stdio.h>

static void print_usage(const char *program) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr,
          "  %s <robot-ip> <kinematics-plugin-lib> [plugin-class]\n",
          program);
  fprintf(stderr, "Example:\n");
  fprintf(stderr,
          "  %s 192.168.1.200 "
          "/path/to/libelite_kdl_kinematics.so\n",
          program);
}

static void print_vector6(const char *name, const double *value6) {
  int i;
  printf("%s = [", name);
  for (i = 0; i < 6; ++i) {
    printf("%.6f%s", value6[i], (i + 1 < 6) ? ", " : "");
  }
  printf("]\n");
}

static int require_ok(const char *name, elite_c_status_t status, int32_t ok,
                      const char *error_text) {
  printf("[%-36s] status=%d result=%d\n", name, (int)status, (int)ok);
  if (status == ELITE_C_STATUS_OK && ok) {
    return 1;
  }
  fprintf(stderr, "%s failed: %s\n", name,
          error_text ? error_text : "(unknown error)");
  return 0;
}

static int require_status(const char *name, elite_c_status_t status,
                          const char *error_text) {
  printf("[%-36s] status=%d\n", name, (int)status);
  if (status == ELITE_C_STATUS_OK) {
    return 1;
  }
  fprintf(stderr, "%s failed: %s\n", name,
          error_text ? error_text : "(unknown error)");
  return 0;
}

int main(int argc, char **argv) {
  const char *robot_ip = NULL;
  const char *plugin_lib = NULL;
  const char *plugin_class = NULL;
  elite_primary_handle_t *primary = NULL;
  elite_rtsi_io_handle_t *rtsi = NULL;
  elite_kinematics_handle_t *kin = NULL;
  elite_c_status_t status;
  int32_t ok = 0;
  int exit_code = 1;
  double dh_a6[6] = {0};
  double dh_d6[6] = {0};
  double dh_alpha6[6] = {0};
  double current_joint6[6] = {0};
  double current_tcp6[6] = {0};
  double fk_pose6[6] = {0};
  double ik_joint6[6] = {0};
  elite_kinematics_result_t ik_result = {0};

  if (argc < 3) {
    print_usage(argv[0]);
    return 1;
  }

  robot_ip = argv[1];
  plugin_lib = argv[2];
  plugin_class = (argc >= 4) ? argv[3] : NULL;

  status = elite_primary_create(&primary);
  if (status != ELITE_C_STATUS_OK || !primary) {
    fprintf(stderr, "elite_primary_create failed\n");
    goto cleanup;
  }

  status = elite_primary_connect(primary, robot_ip, 30001, &ok);
  if (!require_ok("elite_primary_connect", status, ok, NULL)) {
    goto cleanup;
  }

  status = elite_primary_get_kinematics_info(primary, 200, dh_a6, dh_d6,
                                             dh_alpha6, &ok);
  if (!require_ok("elite_primary_get_kinematics_info", status, ok, NULL)) {
    goto cleanup;
  }
  elite_primary_disconnect(primary);

  status = elite_rtsi_io_create("actual_joint_positions,actual_TCP_pose", "",
                                250.0, &rtsi);
  if (status != ELITE_C_STATUS_OK || !rtsi) {
    fprintf(stderr, "elite_rtsi_io_create failed\n");
    goto cleanup;
  }

  status = elite_rtsi_io_connect(rtsi, robot_ip, &ok);
  if (!require_ok("elite_rtsi_io_connect", status, ok,
                  elite_rtsi_io_last_error_message(rtsi))) {
    goto cleanup;
  }

  example_sleep_seconds(1);

  status = elite_rtsi_io_get_actual_joint_positions(rtsi, current_joint6);
  if (!require_status("elite_rtsi_io_get_actual_joint_positions", status,
                      elite_rtsi_io_last_error_message(rtsi))) {
    goto cleanup;
  }

  status = elite_rtsi_io_get_actual_tcp_pose(rtsi, current_tcp6);
  if (!require_status("elite_rtsi_io_get_actual_tcp_pose", status,
                      elite_rtsi_io_last_error_message(rtsi))) {
    goto cleanup;
  }

  status = elite_kinematics_create(plugin_lib, plugin_class, &kin);
  if (status != ELITE_C_STATUS_OK || !kin) {
    fprintf(stderr, "elite_kinematics_create failed: %s\n",
            elite_kinematics_global_last_error_message());
    goto cleanup;
  }

  status = elite_kinematics_set_mdh(kin, dh_alpha6, dh_a6, dh_d6);
  if (!require_status("elite_kinematics_set_mdh", status,
                      elite_kinematics_last_error_message(kin))) {
    goto cleanup;
  }

  status =
      elite_kinematics_get_position_fk(kin, current_joint6, fk_pose6, &ok);
  if (!require_ok("elite_kinematics_get_position_fk", status, ok,
                  elite_kinematics_last_error_message(kin))) {
    goto cleanup;
  }

  status = elite_kinematics_get_position_ik(kin, current_tcp6, current_joint6,
                                            ik_joint6, &ik_result, &ok);
  if (!require_ok("elite_kinematics_get_position_ik", status, ok,
                  elite_kinematics_last_error_message(kin))) {
    fprintf(stderr, "IK result error=%d\n", (int)ik_result.kinematic_error);
    goto cleanup;
  }

  printf("\n=== Kinematics Example (C wrapper) ===\n");
  print_vector6("Current TCP pose", current_tcp6);
  print_vector6("FK pose", fk_pose6);
  print_vector6("IK result joints", ik_joint6);
  print_vector6("Current joints", current_joint6);
  exit_code = 0;

cleanup:
  if (rtsi) {
    elite_rtsi_io_disconnect(rtsi);
    elite_rtsi_io_destroy(rtsi);
  }
  if (primary) {
    elite_primary_disconnect(primary);
    elite_primary_destroy(primary);
  }
  if (kin) {
    elite_kinematics_destroy(kin);
  }
  return exit_code;
}

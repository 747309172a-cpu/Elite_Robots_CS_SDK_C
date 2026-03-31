#include <Elite_Sdk_C.h>

#include "example_portability.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static volatile int g_trajectory_result = -1;

typedef struct trajectory_control_t {
    elite_driver_handle_t* driver;
    elite_dashboard_handle_t* dashboard;
    elite_driver_config_t config;
} trajectory_control_t;

static void print_status(const char* name, elite_c_status_t status, int32_t value) {
    printf("[%-40s] status=%d result=%d\n", name, (int)status, (int)value);
}

static int require_ok(const char* name, elite_c_status_t status, int32_t ok, const char* error_text) {
    print_status(name, status, ok);
    if (status == ELITE_C_STATUS_OK && ok) {
        return 1;
    }
    fprintf(stderr, "%s failed: %s\n", name, error_text ? error_text : "(unknown error)");
    return 0;
}

static int require_status(const char* name, elite_c_status_t status, const char* error_text) {
    printf("[%-40s] status=%d\n", name, (int)status);
    if (status == ELITE_C_STATUS_OK) {
        return 1;
    }
    fprintf(stderr, "%s failed: %s\n", name, error_text ? error_text : "(unknown error)");
    return 0;
}

static void on_trajectory_result(elite_trajectory_motion_result_t result, void* user_data) {
    (void)user_data;
    g_trajectory_result = (int)result;
    printf("[trajectory callback] result=%d\n", (int)result);
}

static void print_usage(const char* program) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]\n", program);
}

static int trajectory_control_init(trajectory_control_t* control,
                                   const elite_driver_config_t* config,
                                   const char* robot_ip) {
    elite_c_status_t status;
    int32_t ok = 0;

    if (!control || !config || !robot_ip) {
        return 0;
    }

    memset(control, 0, sizeof(*control));
    control->config = *config;

    status = elite_driver_create(&control->config, &control->driver);
    if (status != ELITE_C_STATUS_OK || !control->driver) {
        fprintf(stderr, "elite_driver_create failed: %s\n", elite_c_last_error_message());
        return 0;
    }

    status = elite_dashboard_create(&control->dashboard);
    if (status != ELITE_C_STATUS_OK || !control->dashboard) {
        fprintf(stderr, "elite_dashboard_create failed: %s\n", elite_dashboard_global_last_error_message());
        return 0;
    }

    status = elite_dashboard_connect(control->dashboard, robot_ip, 29999, &ok);
    if (!require_ok("elite_dashboard_connect", status, ok, elite_dashboard_last_error_message(control->dashboard))) {
        return 0;
    }

    status = elite_driver_set_trajectory_result_callback(control->driver, on_trajectory_result, NULL);
    if (!require_status("elite_driver_set_trajectory_result_callback", status, elite_driver_last_error_message(control->driver))) {
        return 0;
    }

    return 1;
}

static void trajectory_control_destroy(trajectory_control_t* control) {
    int32_t ok = 0;

    if (!control) {
        return;
    }

    if (control->driver) {
        elite_driver_stop_control(control->driver, 1000, &ok);
        elite_driver_destroy(control->driver);
        control->driver = NULL;
    }

    if (control->dashboard) {
        elite_dashboard_disconnect(control->dashboard);
        elite_dashboard_destroy(control->dashboard);
        control->dashboard = NULL;
    }
}

static int trajectory_control_start(trajectory_control_t* control) {
    elite_c_status_t status;
    int32_t ok = 0;
    int32_t connected = 0;
    int i;

    if (!control || !control->driver || !control->dashboard) {
        return 0;
    }

    status = elite_dashboard_power_on(control->dashboard, &ok);
    if (!require_ok("elite_dashboard_power_on", status, ok, elite_dashboard_last_error_message(control->dashboard))) {
        return 0;
    }

    status = elite_dashboard_brake_release(control->dashboard, &ok);
    if (!require_ok("elite_dashboard_brake_release", status, ok, elite_dashboard_last_error_message(control->dashboard))) {
        return 0;
    }

    status = elite_driver_is_robot_connected(control->driver, &connected);
    if (!require_status("elite_driver_is_robot_connected", status, elite_driver_last_error_message(control->driver))) {
        return 0;
    }

    if (control->config.headless_mode) {
        if (!connected) {
            status = elite_driver_send_external_control_script(control->driver, &ok);
            if (!require_ok("elite_driver_send_external_control_script", status, ok,
                            elite_driver_last_error_message(control->driver))) {
                return 0;
            }
        }
    } else {
        status = elite_dashboard_play_program(control->dashboard, &ok);
        if (!require_ok("elite_dashboard_play_program", status, ok, elite_dashboard_last_error_message(control->dashboard))) {
            return 0;
        }
    }

    printf("Wait external control script run...\n");
    for (i = 0; i < 3000; ++i) {
        status = elite_driver_is_robot_connected(control->driver, &connected);
        if (status == ELITE_C_STATUS_OK && connected) {
            break;
        }
        example_sleep_us(10000);
    }

    if (!connected) {
        fprintf(stderr, "robot did not enter external control mode in time\n");
        return 0;
    }

    printf("External control script is running\n");
    return 1;
}

static int trajectory_control_move_trajectory(trajectory_control_t* control,
                                              const double* points,
                                              int32_t point_count,
                                              float point_time,
                                              float blend_radius,
                                              int32_t is_cartesian) {
    elite_c_status_t status;
    int32_t ok = 0;
    int32_t i;

    if (!control || !control->driver || !points || point_count <= 0) {
        return 0;
    }

    g_trajectory_result = -1;
    status = elite_driver_write_trajectory_control_action(control->driver,
                                                          ELITE_TRAJECTORY_ACTION_START,
                                                          point_count,
                                                          200,
                                                          &ok);
    if (!require_ok("elite_driver_write_trajectory_control_action_start", status, ok,
                    elite_driver_last_error_message(control->driver))) {
        return 0;
    }

    for (i = 0; i < point_count; ++i) {
        const double* point = points + (size_t)i * 6U;

        status = elite_driver_write_trajectory_point(control->driver, point, point_time, blend_radius, is_cartesian, &ok);
        if (!require_ok("elite_driver_write_trajectory_point", status, ok,
                        elite_driver_last_error_message(control->driver))) {
            return 0;
        }

        status = elite_driver_write_trajectory_control_action(control->driver,
                                                              ELITE_TRAJECTORY_ACTION_NOOP,
                                                              0,
                                                              200,
                                                              &ok);
        if (!require_ok("elite_driver_write_trajectory_control_action_noop", status, ok,
                        elite_driver_last_error_message(control->driver))) {
            return 0;
        }
    }

    while (g_trajectory_result < 0) {
        example_sleep_us(50000);
        status = elite_driver_write_trajectory_control_action(control->driver,
                                                              ELITE_TRAJECTORY_ACTION_NOOP,
                                                              0,
                                                              200,
                                                              &ok);
        if (!require_ok("elite_driver_write_trajectory_control_action_noop", status, ok,
                        elite_driver_last_error_message(control->driver))) {
            return 0;
        }
    }

    status = elite_driver_write_idle(control->driver, 0, &ok);
    if (!require_ok("elite_driver_write_idle", status, ok, elite_driver_last_error_message(control->driver))) {
        return 0;
    }

    return g_trajectory_result == ELITE_TRAJECTORY_MOTION_RESULT_SUCCESS;
}

static int trajectory_control_move_to(trajectory_control_t* control,
                                      const double* point,
                                      float time_s,
                                      int32_t is_cartesian) {
    return trajectory_control_move_trajectory(control, point, 1, time_s, 0.0f, is_cartesian);
}

int main(int argc, char** argv) {
    elite_driver_config_t config;
    trajectory_control_t control;
    elite_rtsi_io_handle_t* rtsi = NULL;
    elite_c_status_t status;
    int32_t ok = 0;
    const char* robot_ip = NULL;
    const char* local_ip = "";
    const char* script_file_path = "external_control.script";
    int32_t headless_mode = 0;
    double actual_joints[6] = {0};
    double actual_pose[6] = {0};
    double cartesian_points[18] = {0};
    int i;
    int exit_code = 1;

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
        } else if (strcmp(argv[i], "--script-file") == 0 && i + 1 < argc) {
            script_file_path = argv[++i];
        } else {
            fprintf(stderr, "unknown argument: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (headless_mode) {
        printf("Use headless mode. Please ensure the robot is not in local mode.\n");
    } else {
        printf("Non-headless mode requires the External Control plugin in the task tree.\n");
    }

    elite_driver_config_set_default(&config);
    config.robot_ip = robot_ip;
    config.local_ip = local_ip;
    config.script_file_path = script_file_path;
    config.headless_mode = headless_mode;

    if (!trajectory_control_init(&control, &config, robot_ip)) {
        goto cleanup;
    }

    status = elite_rtsi_io_create("actual_joint_positions,actual_TCP_pose", "", 250.0, &rtsi);
    if (status != ELITE_C_STATUS_OK || !rtsi) {
        fprintf(stderr, "elite_rtsi_io_create failed: %s\n", elite_c_last_error_message());
        goto cleanup;
    }

    status = elite_rtsi_io_connect(rtsi, robot_ip, &ok);
    if (!require_ok("elite_rtsi_io_connect", status, ok, elite_rtsi_io_last_error_message(rtsi))) {
        goto cleanup;
    }

    if (!trajectory_control_start(&control)) {
        goto cleanup;
    }

    status = elite_rtsi_io_get_actual_joint_positions(rtsi, actual_joints);
    if (!require_status("elite_rtsi_io_get_actual_joint_positions", status, elite_rtsi_io_last_error_message(rtsi))) {
        goto cleanup;
    }

    actual_joints[3] = -1.57;
    printf("Moving joints to target: [%.6f, %.6f, %.6f, %.6f, %.6f, %.6f]\n",
           actual_joints[0],
           actual_joints[1],
           actual_joints[2],
           actual_joints[3],
           actual_joints[4],
           actual_joints[5]);
    if (!trajectory_control_move_to(&control, actual_joints, 3.0f, 0)) {
        fprintf(stderr, "Failed to move joints to target\n");
        goto cleanup;
    }

    status = elite_rtsi_io_get_actual_tcp_pose(rtsi, actual_pose);
    if (!require_status("elite_rtsi_io_get_actual_tcp_pose", status, elite_rtsi_io_last_error_message(rtsi))) {
        goto cleanup;
    }

    memcpy(cartesian_points, actual_pose, sizeof(actual_pose));
    cartesian_points[2] -= 0.2;

    memcpy(cartesian_points + 6, cartesian_points, sizeof(actual_pose));
    cartesian_points[6 + 1] -= 0.2;

    memcpy(cartesian_points + 12, cartesian_points + 6, sizeof(actual_pose));
    cartesian_points[12 + 1] += 0.2;
    cartesian_points[12 + 2] += 0.2;

    printf("Moving cartesian trajectory...\n");
    if (!trajectory_control_move_trajectory(&control, cartesian_points, 3, 3.0f, 0.0f, 1)) {
        fprintf(stderr, "Failed to move cartesian trajectory\n");
        goto cleanup;
    }

    printf("Trajectory motion finish\n");
    exit_code = 0;

cleanup:
    if (rtsi) {
        elite_rtsi_io_disconnect(rtsi);
        elite_rtsi_io_destroy(rtsi);
    }
    trajectory_control_destroy(&control);
    return exit_code;
}

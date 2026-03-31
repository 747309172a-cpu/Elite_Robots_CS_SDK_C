#include <Elite_Sdk_C.h>

#include "example_portability.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

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

static void print_usage(const char* program) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <robot-ip> [--local-ip <ip>] [--headless] [--script-file <path>]\n", program);
}

int main(int argc, char** argv) {
    elite_driver_config_t config;
    elite_driver_handle_t* driver = NULL;
    elite_dashboard_handle_t* dashboard = NULL;
    elite_c_status_t status;
    int32_t ok = 0;
    int32_t connected = 0;
    const char* robot_ip = NULL;
    const char* local_ip = "";
    const char* script_file_path = "external_control.script";
    int32_t headless_mode = 0;
    double speedl_vector[6] = {0, 0, -0.02, 0, 0, 0};
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

    status = elite_dashboard_create(&dashboard);
    if (status != ELITE_C_STATUS_OK || !dashboard) {
        fprintf(stderr, "elite_dashboard_create failed: %s\n", elite_dashboard_global_last_error_message());
        goto cleanup;
    }

    status = elite_driver_create(&config, &driver);
    if (status != ELITE_C_STATUS_OK || !driver) {
        fprintf(stderr, "elite_driver_create failed: %s\n", elite_c_last_error_message());
        goto cleanup;
    }

    status = elite_dashboard_connect(dashboard, robot_ip, 29999, &ok);
    if (!require_ok("elite_dashboard_connect", status, ok, elite_dashboard_last_error_message(dashboard))) {
        goto cleanup;
    }

    status = elite_dashboard_power_on(dashboard, &ok);
    if (!require_ok("elite_dashboard_power_on", status, ok, elite_dashboard_last_error_message(dashboard))) {
        goto cleanup;
    }

    status = elite_dashboard_brake_release(dashboard, &ok);
    if (!require_ok("elite_dashboard_brake_release", status, ok, elite_dashboard_last_error_message(dashboard))) {
        goto cleanup;
    }

    status = elite_driver_is_robot_connected(driver, &connected);
    if (!require_status("elite_driver_is_robot_connected", status, elite_driver_last_error_message(driver))) {
        goto cleanup;
    }

    if (headless_mode) {
        if (!connected) {
            status = elite_driver_send_external_control_script(driver, &ok);
            if (!require_ok("elite_driver_send_external_control_script", status, ok, elite_driver_last_error_message(driver))) {
                goto cleanup;
            }
        }
    } else {
        status = elite_dashboard_play_program(dashboard, &ok);
        if (!require_ok("elite_dashboard_play_program", status, ok, elite_dashboard_last_error_message(dashboard))) {
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

    status = elite_driver_write_speedl(driver, speedl_vector, 0, &ok);
    if (!require_ok("elite_driver_write_speedl_down", status, ok, elite_driver_last_error_message(driver))) {
        goto cleanup;
    }
    example_sleep_seconds(5);

    speedl_vector[2] = 0.02;
    status = elite_driver_write_speedl(driver, speedl_vector, 0, &ok);
    if (!require_ok("elite_driver_write_speedl_up", status, ok, elite_driver_last_error_message(driver))) {
        goto cleanup;
    }
    example_sleep_seconds(5);

    status = elite_driver_stop_control(driver, 1000, &ok);
    if (!require_ok("elite_driver_stop_control", status, ok, elite_driver_last_error_message(driver))) {
        goto cleanup;
    }

    printf("Speedl motion finish\n");
    exit_code = 0;

cleanup:
    if (driver) {
        elite_driver_destroy(driver);
    }
    if (dashboard) {
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
    }
    return exit_code;
}

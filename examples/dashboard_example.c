#include <Elite_Sdk_C.h>

#include "example_portability.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_status(const char* name, elite_c_status_t status, int32_t value) {
    printf("[%-32s] status=%d result=%d\n", name, (int)status, (int)value);
}

static int require_ok(const char* name, elite_c_status_t status, int32_t value) {
    print_status(name, status, value);
    return status == ELITE_C_STATUS_OK && value;
}

static int require_status(const char* name, elite_c_status_t status) {
    printf("[%-32s] status=%d\n", name, (int)status);
    return status == ELITE_C_STATUS_OK;
}

// Helper function to get string value from dashboard API that follows the pattern:
// 1) Call API with NULL buffer to get required length
// 2) Allocate buffer and call API again to get the value
static int dashboard_get_string(elite_dashboard_handle_t* dashboard,
                                const char* api_name,
                                elite_c_status_t (*fn)(elite_dashboard_handle_t*, char*, int32_t, int32_t*),
                                char** out_value) {
    elite_c_status_t status;
    int32_t required_len = 0;
    char* buffer = NULL;

    if (!out_value) {
        return 0;
    }

    *out_value = NULL;
    status = fn(dashboard, NULL, 0, &required_len);
    printf("[%-32s] status=%d required_len=%d\n", api_name, (int)status, (int)required_len);
    if (status != ELITE_C_STATUS_OK || required_len <= 0) {
        return 0;
    }

    buffer = (char*)malloc((size_t)required_len);
    if (!buffer) {
        fprintf(stderr, "malloc failed for %s\n", api_name);
        return 0;
    }

    status = fn(dashboard, buffer, required_len, &required_len);
    printf("[%-32s] status=%d value=%s\n", api_name, (int)status, buffer);
    if (status != ELITE_C_STATUS_OK) {
        free(buffer);
        return 0;
    }

    *out_value = buffer;
    return 1;
}

static void print_usage(const char* program) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <robot-ip> [task-file]\n", program);
    fprintf(stderr, "Note:\n");
    fprintf(stderr, "  The robot should already contain the task file, default: test.task\n");
}

int main(int argc, char** argv) {
    elite_dashboard_handle_t* dashboard = NULL;
    elite_c_status_t status;
    int32_t ok = 0;
    int32_t task_status = 0;
    const char* robot_ip = NULL;
    const char* task_file = "test.task";
    char* loaded_task_path = NULL;

    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    robot_ip = argv[1];
    if (argc >= 3) {
        task_file = argv[2];
    }

    status = elite_dashboard_create(&dashboard);
    if (status != ELITE_C_STATUS_OK || !dashboard) {
        fprintf(stderr, "elite_dashboard_create failed: %s\n", elite_dashboard_global_last_error_message());
        return 1;
    }

    status = elite_dashboard_connect(dashboard, robot_ip, 29999, &ok);
    if (!require_ok("elite_dashboard_connect", status, ok)) {
        fprintf(stderr, "dashboard connect failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_echo(dashboard, &ok);
    if (!require_ok("elite_dashboard_echo", status, ok)) {
        fprintf(stderr, "dashboard echo failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_power_off(dashboard, &ok);
    if (!require_ok("elite_dashboard_power_off", status, ok)) {
        fprintf(stderr, "dashboard power off failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_close_safety_dialog(dashboard, &ok);
    print_status("elite_dashboard_close_safety_dialog", status, ok);

    status = elite_dashboard_power_on(dashboard, &ok);
    if (!require_ok("elite_dashboard_power_on", status, ok)) {
        fprintf(stderr, "dashboard power on failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_brake_release(dashboard, &ok);
    if (!require_ok("elite_dashboard_brake_release", status, ok)) {
        fprintf(stderr, "dashboard brake release failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_load_task(dashboard, task_file, &ok);
    if (!require_ok("elite_dashboard_load_task", status, ok)) {
        fprintf(stderr, "dashboard load task failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    if (!dashboard_get_string(dashboard, "elite_dashboard_get_task_path", elite_dashboard_get_task_path, &loaded_task_path)) {
        fprintf(stderr, "dashboard get task path failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    if (strcmp(loaded_task_path, task_file) != 0) {
        fprintf(stderr, "loaded task mismatch: expected=%s actual=%s\n", task_file, loaded_task_path);
        free(loaded_task_path);
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }
    free(loaded_task_path);

    status = elite_dashboard_get_task_status(dashboard, &task_status);
    print_status("elite_dashboard_get_task_status", status, task_status);
    if (status != ELITE_C_STATUS_OK || task_status != ELITE_TASK_STATUS_STOPPED) {
        fprintf(stderr, "task should be stopped before play\n");
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_play_program(dashboard, &ok);
    if (!require_ok("elite_dashboard_play_program", status, ok)) {
        fprintf(stderr, "dashboard play program failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }
    example_sleep_seconds(5);
    status = elite_dashboard_get_task_status(dashboard, &task_status);
    print_status("elite_dashboard_get_task_status", status, task_status);
    if (status != ELITE_C_STATUS_OK || task_status != ELITE_TASK_STATUS_PLAYING) {
        fprintf(stderr, "task should be playing after play_program\n");
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_pause_program(dashboard, &ok);
    if (!require_ok("elite_dashboard_pause_program", status, ok)) {
        fprintf(stderr, "dashboard pause program failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_get_task_status(dashboard, &task_status);
    print_status("elite_dashboard_get_task_status", status, task_status);
    if (status != ELITE_C_STATUS_OK || task_status != ELITE_TASK_STATUS_PAUSED) {
        fprintf(stderr, "task should be paused after pause_program\n");
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_stop_program(dashboard, &ok);
    if (!require_ok("elite_dashboard_stop_program", status, ok)) {
        fprintf(stderr, "dashboard stop program failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_get_task_status(dashboard, &task_status);
    print_status("elite_dashboard_get_task_status", status, task_status);
    if (status != ELITE_C_STATUS_OK || task_status != ELITE_TASK_STATUS_STOPPED) {
        fprintf(stderr, "task should be stopped after stop_program\n");
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_is_task_saved(dashboard, &ok);
    if (!require_ok("elite_dashboard_is_task_saved", status, ok)) {
        fprintf(stderr, "dashboard is_task_saved failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_popup(dashboard, "-s", "Hello Robot", &ok);
    if (!require_ok("elite_dashboard_popup", status, ok)) {
        fprintf(stderr, "dashboard popup failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_disconnect(dashboard);
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    status = elite_dashboard_disconnect(dashboard);
    if (!require_status("elite_dashboard_disconnect", status)) {
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    elite_dashboard_destroy(dashboard);
    return 0;
}

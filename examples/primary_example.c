#include <Elite_Sdk_C.h>

#include "example_portability.h"

#include <stdint.h>
#include <stdio.h>

static void print_status(const char *name, elite_c_status_t status,
                         int32_t value) {
  printf("[%-32s] status=%d result=%d\n", name, (int)status, (int)value);
}

static int require_ok(const char *name, elite_c_status_t status, int32_t ok) {
  print_status(name, status, ok);
  return status == ELITE_C_STATUS_OK && ok;
}

static void on_robot_exception(const elite_primary_robot_exception_t *ex,
                               void *user_data) {
  (void)user_data;
  if (!ex) {
    return;
  }
  printf("[primary exception] type=%d timestamp=%llu code=%d sub_code=%d "
         "line=%d column=%d message=%s\n",
         (int)ex->type, (unsigned long long)ex->timestamp, (int)ex->error_code,
         (int)ex->sub_error_code, (int)ex->line, (int)ex->column,
         ex->message ? ex->message : "(null)");
}

static void print_dh_array(const char *name, const double *values6) {
  int i;
  printf("%s:", name);
  for (i = 0; i < 6; ++i) {
    printf(" %.6f", values6[i]);
  }
  printf("\n");
}

static void print_usage(const char *program) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s <robot-ip>\n", program);
}

int main(int argc, char **argv) {
  elite_primary_handle_t *primary = NULL;
  elite_c_status_t status;
  int32_t ok = 0;
  double dh_a6[6] = {0};
  double dh_d6[6] = {0};
  double dh_alpha6[6] = {0};

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  status = elite_primary_create(&primary);
  if (status != ELITE_C_STATUS_OK || !primary) {
    fprintf(stderr, "elite_primary_create failed\n");
    return 1;
  }

  status = elite_primary_connect(primary, argv[1], 30001, &ok);
  if (!require_ok("elite_primary_connect", status, ok)) {
    elite_primary_destroy(primary);
    return 1;
  }

  status = elite_primary_register_robot_exception_callback(
      primary, on_robot_exception, NULL);
  printf("[%-32s] status=%d\n", "elite_primary_register_callback", (int)status);
  if (status != ELITE_C_STATUS_OK) {
    elite_primary_disconnect(primary);
    elite_primary_destroy(primary);
    return 1;
  }

  status = elite_primary_get_kinematics_info(primary, 200, dh_a6, dh_d6,
                                             dh_alpha6, &ok);
  print_status("elite_primary_get_kinematics_info", status, ok);
  if (status == ELITE_C_STATUS_OK && ok) {
    print_dh_array("DH parameter a", dh_a6);
    print_dh_array("DH parameter d", dh_d6);
    print_dh_array("DH parameter alpha", dh_alpha6);
  }

  status = elite_primary_send_script(
      primary, "def hello():\n\ttextmsg(\"hello world\")\nend\n", &ok);
  if (!require_ok("elite_primary_send_script_hello", status, ok)) {
    elite_primary_disconnect(primary);
    elite_primary_destroy(primary);
    return 1;
  }

  status =
      elite_primary_send_script(primary, "def exFunc():\n\t1abcd\nend\n", &ok);
  if (!require_ok("elite_primary_send_script_error", status, ok)) {
    elite_primary_disconnect(primary);
    elite_primary_destroy(primary);
    return 1;
  }

  example_sleep_seconds(1);

  status = elite_primary_disconnect(primary);
  printf("[%-32s] status=%d\n", "elite_primary_disconnect", (int)status);

  elite_primary_destroy(primary);
  return 0;
}

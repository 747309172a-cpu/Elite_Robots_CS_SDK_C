#include <Elite_Sdk_C.h>

#include <stdint.h>
#include <stdio.h>

static void print_usage(const char *program) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s <pose-algebra-plugin-lib> [plugin-class]\n",
          program);
  fprintf(stderr, "Example:\n");
  fprintf(stderr,
          "  %s /path/to/libelite_eigen_pose_algebra.so\n",
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

static void print_pose_matrix(const char *name, const elite_pose_matrix_t *m) {
  int row;
  int col;
  printf("%s =\n", name);
  for (row = 0; row < 4; ++row) {
    printf("  [");
    for (col = 0; col < 4; ++col) {
      printf("%.6f%s", m->data[row * 4 + col], (col + 1 < 4) ? ", " : "");
    }
    printf("]\n");
  }
}

static int check_pose_result(const char *name, elite_c_status_t status,
                             int32_t ok,
                             const elite_pose_algebra_result_t *result,
                             elite_pose_algebra_handle_t *handle) {
  printf("[%-40s] status=%d result=%d", name, (int)status, (int)ok);
  if (result) {
    printf(" error=%d", (int)result->error);
  }
  printf("\n");

  if (status == ELITE_C_STATUS_OK && ok) {
    return 1;
  }

  if (status != ELITE_C_STATUS_OK) {
    fprintf(stderr, "%s failed: %s\n", name,
            elite_pose_algebra_last_error_message(handle));
  } else if (result && result->message) {
    fprintf(stderr, "%s failed: %s\n", name, result->message);
  } else {
    fprintf(stderr, "%s failed\n", name);
  }
  return 0;
}

int main(int argc, char **argv) {
  const char *plugin_lib = NULL;
  const char *plugin_class = NULL;
  elite_pose_algebra_handle_t *pose_alg = NULL;
  elite_c_status_t status;
  int32_t ok = 0;
  int exit_code = 1;
  elite_pose_algebra_result_t result = {0};
  const double base_pose6[6] = {0.4, -0.2, 0.5, 0.1, 0.2, -0.3};
  const double tool_offset6[6] = {0.05, 0.0, 0.12, 0.0, 0.0, 1.570796};
  elite_pose_matrix_t base_matrix = {{0}};
  elite_pose_matrix_t tool_matrix = {{0}};
  elite_pose_matrix_t composed_matrix = {{0}};
  elite_pose_matrix_t tool_in_base_matrix = {{0}};
  elite_pose_matrix_t recovered_world_matrix = {{0}};
  elite_pose_matrix_t inverse_base_matrix = {{0}};
  elite_pose_matrix_t identity_check = {{0}};
  double composed_pose6[6] = {0};
  double tool_in_base_pose6[6] = {0};
  double recovered_world_pose6[6] = {0};
  double added_pose6[6] = {0};
  double recovered_pose6[6] = {0};
  elite_pose_distance_t distance = {0};

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  plugin_lib = argv[1];
  plugin_class = (argc >= 3) ? argv[2] : NULL;

  status = elite_pose_algebra_create(plugin_lib, plugin_class, &pose_alg);
  if (status != ELITE_C_STATUS_OK || !pose_alg) {
    fprintf(stderr, "elite_pose_algebra_create failed: %s\n",
            elite_pose_algebra_global_last_error_message());
    goto cleanup;
  }

  status = elite_pose_algebra_vector_to_matrix(pose_alg, base_pose6,
                                               &base_matrix, &result, &ok);
  if (!check_pose_result("vector_to_matrix(base_pose)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_vector_to_matrix(pose_alg, tool_offset6,
                                               &tool_matrix, &result, &ok);
  if (!check_pose_result("vector_to_matrix(tool_offset)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_multiply_matrix(
      pose_alg, &base_matrix, &tool_matrix, &composed_matrix, &result, &ok);
  if (!check_pose_result("multiply(base_matrix, tool_matrix)", status, ok,
                         &result, pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_world_to_local_matrix(
      pose_alg, &base_matrix, &composed_matrix, &tool_in_base_matrix, &result,
      &ok);
  if (!check_pose_result("world_to_local(matrix)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_local_to_world_matrix(
      pose_alg, &base_matrix, &tool_in_base_matrix, &recovered_world_matrix,
      &result, &ok);
  if (!check_pose_result("local_to_world(matrix)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_inverse_matrix(
      pose_alg, &base_matrix, &inverse_base_matrix, &result, &ok);
  if (!check_pose_result("inverse(base_matrix)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_multiply_matrix(
      pose_alg, &base_matrix, &inverse_base_matrix, &identity_check, &result,
      &ok);
  if (!check_pose_result("multiply(base, inverse_base)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_matrix_to_vector(
      pose_alg, &composed_matrix, composed_pose6, &result, &ok);
  if (!check_pose_result("matrix_to_vector(composed_matrix)", status, ok,
                         &result, pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_world_to_local_vector(
      pose_alg, base_pose6, composed_pose6, tool_in_base_pose6, &result, &ok);
  if (!check_pose_result("world_to_local(vector)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_local_to_world_vector(
      pose_alg, base_pose6, tool_in_base_pose6, recovered_world_pose6, &result,
      &ok);
  if (!check_pose_result("local_to_world(vector)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_add_vector(pose_alg, base_pose6, tool_offset6,
                                         added_pose6, &result, &ok);
  if (!check_pose_result("add(base_pose, tool_offset)", status, ok, &result,
                         pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_subtract_vector(
      pose_alg, added_pose6, tool_offset6, recovered_pose6, &result, &ok);
  if (!check_pose_result("subtract(added_pose, tool_offset)", status, ok,
                         &result, pose_alg)) {
    goto cleanup;
  }

  status = elite_pose_algebra_distance_vector(
      pose_alg, base_pose6, composed_pose6, &distance, &result, &ok);
  if (!check_pose_result("distance(base_pose, composed_pose)", status, ok,
                         &result, pose_alg)) {
    goto cleanup;
  }

  printf("\n=== Pose Algebra Example (C wrapper) ===\n");
  print_vector6("base_pose", base_pose6);
  print_vector6("tool_offset", tool_offset6);
  print_pose_matrix("base_matrix", &base_matrix);
  print_pose_matrix("composed_matrix", &composed_matrix);
  print_pose_matrix("tool_in_base_matrix", &tool_in_base_matrix);
  print_pose_matrix("recovered_world_matrix", &recovered_world_matrix);
  print_pose_matrix("identity_check", &identity_check);
  print_vector6("composed_pose", composed_pose6);
  print_vector6("tool_in_base_pose", tool_in_base_pose6);
  print_vector6("recovered_world_pose", recovered_world_pose6);
  print_vector6("added_pose", added_pose6);
  print_vector6("recovered_pose", recovered_pose6);
  printf("distance.linear_distance  = %.6f\n", distance.linear_distance);
  printf("distance.angular_distance = %.6f\n", distance.angular_distance);
  exit_code = 0;

cleanup:
  if (pose_alg) {
    elite_pose_algebra_destroy(pose_alg);
  }
  return exit_code;
}

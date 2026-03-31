#include <Elite_Sdk_C.h>

#include "example_portability.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_status_i32(const char *name, elite_c_status_t status,
                             int32_t value) {
  printf("[%-32s] status=%d result=%d\n", name, (int)status, (int)value);
}

static void print_status_u32(const char *name, elite_c_status_t status,
                             uint32_t value) {
  printf("[%-32s] status=%d result=%" PRIu32 "\n", name, (int)status, value);
}

static int require_ok(const char *name, elite_c_status_t status, int32_t ok,
                      const char *error_text) {
  print_status_i32(name, status, ok);
  if (status == ELITE_C_STATUS_OK && ok) {
    return 1;
  }
  fprintf(stderr, "%s failed: %s\n", name,
          error_text ? error_text : "(unknown error)");
  return 0;
}

static int require_status(const char *name, elite_c_status_t status,
                          const char *error_text) {
  printf("[%-32s] status=%d\n", name, (int)status);
  if (status == ELITE_C_STATUS_OK) {
    return 1;
  }
  fprintf(stderr, "%s failed: %s\n", name,
          error_text ? error_text : "(unknown error)");
  return 0;
}

static char *read_text_file(const char *path) {
  FILE *fp = NULL;
  long file_size = 0;
  size_t read_size = 0;
  char *buffer = NULL;

  if (!path) {
    return NULL;
  }

  fp = fopen(path, "rb");
  if (!fp) {
    return NULL;
  }

  if (fseek(fp, 0, SEEK_END) != 0) {
    fclose(fp);
    return NULL;
  }

  file_size = ftell(fp);
  if (file_size < 0) {
    fclose(fp);
    return NULL;
  }

  if (fseek(fp, 0, SEEK_SET) != 0) {
    fclose(fp);
    return NULL;
  }

  buffer = (char *)malloc((size_t)file_size + 1U);
  if (!buffer) {
    fclose(fp);
    return NULL;
  }

  read_size = fread(buffer, 1U, (size_t)file_size, fp);
  fclose(fp);

  if (read_size != (size_t)file_size) {
    free(buffer);
    return NULL;
  }

  buffer[file_size] = '\0';
  return buffer;
}

static void trim_inplace(char *text) {
  size_t len = 0;

  if (!text) {
    return;
  }

  len = strlen(text);
  while (len > 0 && (text[len - 1] == ' ' || text[len - 1] == '\t' ||
                     text[len - 1] == '\r' || text[len - 1] == '\n')) {
    text[len - 1] = '\0';
    len--;
  }
}

static char *recipe_text_to_csv(const char *text) {
  char *copy = NULL;
  char *token = NULL;
  char *saveptr = NULL;
  char *csv = NULL;
  size_t csv_len = 0;
  int first = 1;

  if (!text) {
    return NULL;
  }

  copy = example_strdup(text);
  if (!copy) {
    return NULL;
  }

  token = example_strtok_r(copy, "\r\n", &saveptr);
  while (token) {
    size_t token_len;
    char *new_csv;

    while (*token == ' ' || *token == '\t') {
      token++;
    }
    trim_inplace(token);
    if (*token == '\0') {
      token = example_strtok_r(NULL, "\r\n", &saveptr);
      continue;
    }

    token_len = strlen(token);
    new_csv = (char *)realloc(csv, csv_len + token_len + (first ? 1U : 2U));
    if (!new_csv) {
      free(csv);
      free(copy);
      return NULL;
    }
    csv = new_csv;

    if (!first) {
      csv[csv_len++] = ',';
    }
    memcpy(csv + csv_len, token, token_len);
    csv_len += token_len;
    csv[csv_len] = '\0';
    first = 0;

    token = example_strtok_r(NULL, "\r\n", &saveptr);
  }

  free(copy);
  return csv;
}

static void print_usage(const char *program) {
  fprintf(stderr, "Usage:\n");
  fprintf(stderr, "  %s <robot-ip>\n", program);
}

int main(int argc, char **argv) {
  elite_rtsi_io_handle_t *io = NULL;
  elite_c_status_t status;
  int32_t ok = 0;
  elite_version_info_t version = {0};
  const char *output_recipe_path = "examples/resource/output_recipe.txt";
  const char *input_recipe_path = "examples/resource/input_recipe.txt";
  char *output_recipe_text = NULL;
  char *input_recipe_text = NULL;
  char *output_recipe_csv = NULL;
  char *input_recipe_csv = NULL;
  uint32_t digital_output_bits = 0;
  int64_t start_ns = 0;
  int64_t end_ns = 0;
  int i;

  if (argc < 2) {
    print_usage(argv[0]);
    return 1;
  }

  output_recipe_text = read_text_file(output_recipe_path);
  input_recipe_text = read_text_file(input_recipe_path);
  if (!output_recipe_text || !input_recipe_text) {
    fprintf(stderr, "failed to read recipe files: %s, %s\n", output_recipe_path,
            input_recipe_path);
    free(output_recipe_text);
    free(input_recipe_text);
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }

  output_recipe_csv = recipe_text_to_csv(output_recipe_text);
  input_recipe_csv = recipe_text_to_csv(input_recipe_text);
  free(output_recipe_text);
  free(input_recipe_text);
  if (!output_recipe_csv || !input_recipe_csv) {
    fprintf(stderr, "failed to convert recipe files to csv format\n");
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }

  status =
      elite_rtsi_io_create(output_recipe_csv, input_recipe_csv, 250.0, &io);
  if (status != ELITE_C_STATUS_OK || !io) {
    fprintf(stderr, "elite_rtsi_io_create failed\n");
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }

  status = elite_rtsi_io_connect(io, argv[1], &ok);
  if (!require_ok("elite_rtsi_io_connect", status, ok,
                  elite_rtsi_io_last_error_message(io))) {
    elite_rtsi_io_destroy(io);
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }

  status = elite_rtsi_io_get_controller_version(io, &version);
  if (!require_status("elite_rtsi_io_get_controller_version", status,
                      elite_rtsi_io_last_error_message(io))) {
    elite_rtsi_io_destroy(io);
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }
  printf("controller version: %u.%u.%u.%u\n", version.major, version.minor,
         version.bugfix, version.build);

  status = elite_rtsi_io_get_digital_output_bits(io, &digital_output_bits);
  print_status_u32("elite_rtsi_io_get_digital_output_bits", status,
                   digital_output_bits);
  if (status != ELITE_C_STATUS_OK) {
    elite_rtsi_io_disconnect(io);
    elite_rtsi_io_destroy(io);
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }

  if (digital_output_bits & 0x00000001U) {
    start_ns = example_monotonic_now_ns();
    status = elite_rtsi_io_set_standard_digital(io, 0, 0, &ok);
    if (!require_ok("elite_rtsi_io_set_standard_digital_low", status, ok,
                    elite_rtsi_io_last_error_message(io))) {
      elite_rtsi_io_disconnect(io);
      elite_rtsi_io_destroy(io);
      return 1;
    }

    for (i = 0; i < 100000; ++i) {
      status = elite_rtsi_io_get_digital_output_bits(io, &digital_output_bits);
      if (status != ELITE_C_STATUS_OK) {
        break;
      }
      if ((digital_output_bits & 0x00000001U) == 0U) {
        break;
      }
    }
    end_ns = example_monotonic_now_ns();
    print_status_u32("elite_rtsi_io_get_digital_output_bits", status,
                     digital_output_bits);
    if (status != ELITE_C_STATUS_OK) {
      elite_rtsi_io_disconnect(io);
      elite_rtsi_io_destroy(io);
      free(output_recipe_csv);
      free(input_recipe_csv);
      return 1;
    }
    printf("Setting low level cost time: %.6f s\n",
           example_monotonic_elapsed_seconds(start_ns, end_ns));
  }

  start_ns = example_monotonic_now_ns();
  status = elite_rtsi_io_set_standard_digital(io, 0, 1, &ok);
  if (!require_ok("elite_rtsi_io_set_standard_digital_high", status, ok,
                  elite_rtsi_io_last_error_message(io))) {
    elite_rtsi_io_disconnect(io);
    elite_rtsi_io_destroy(io);
    return 1;
  }

  for (i = 0; i < 100000; ++i) {
    status = elite_rtsi_io_get_digital_output_bits(io, &digital_output_bits);
    if (status != ELITE_C_STATUS_OK) {
      break;
    }
    if (digital_output_bits & 0x00000001U) {
      break;
    }
  }
  end_ns = example_monotonic_now_ns();
  print_status_u32("elite_rtsi_io_get_digital_output_bits", status,
                   digital_output_bits);
  if (status != ELITE_C_STATUS_OK) {
    elite_rtsi_io_disconnect(io);
    elite_rtsi_io_destroy(io);
    free(output_recipe_csv);
    free(input_recipe_csv);
    return 1;
  }
  printf("Setting high level cost time: %.6f s\n",
         example_monotonic_elapsed_seconds(start_ns, end_ns));

  status = elite_rtsi_io_disconnect(io);
  if (!require_status("elite_rtsi_io_disconnect", status,
                      elite_rtsi_io_last_error_message(io))) {
    elite_rtsi_io_destroy(io);
    return 1;
  }

  elite_rtsi_io_destroy(io);
  free(output_recipe_csv);
  free(input_recipe_csv);
  return 0;
}

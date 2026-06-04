# PoseAlgebra Module

## Overview

The PoseAlgebra module exposes pose geometry plugin APIs through the C wrapper. It loads a pose algebra plugin at runtime and provides operations for 6D pose vectors and 4x4 homogeneous matrices.

## Header

```c
#include <Elite/PoseAlgebra_C.hpp>
```

The header is also included by:

```c
#include <Elite_Sdk_C.h>
```

## Plugin

Common plugin classes:

- `ELITE::EigenPoseAlgebra`
- `ELITE::ElitePoseAlgebra`

Common installed plugin paths on Linux:

```text
/usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_eigen_pose_algebra.so
/usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_pose_algebra.so
```

If `plugin_class_name` is `NULL` or empty, `ELITE::EigenPoseAlgebra` is used.

## Types

See [CommonTypes.md](./CommonTypes.md) for:

- `elite_pose_matrix_t`
- `elite_pose_distance_t`
- `elite_pose_algebra_error_t`
- `elite_pose_algebra_result_t`

`elite_pose_matrix_t::data` stores a row-major 4x4 matrix.

## Handle Type

```c
typedef struct elite_pose_algebra_handle_t elite_pose_algebra_handle_t;
```

- ***Description***

  Opaque handle that owns the plugin loader and pose algebra instance.

## API Reference

### Create Pose Algebra Handle

```c
elite_c_status_t elite_pose_algebra_create(
    const char* plugin_lib_path,
    const char* plugin_class_name,
    elite_pose_algebra_handle_t** out_handle
);
```

- ***Description***

  Loads a pose algebra plugin library and creates a pose algebra instance.

- ***Parameters***

- `plugin_lib_path`: Path to the pose algebra plugin shared library.
- `plugin_class_name`: Plugin class name. If `NULL` or empty, `ELITE::EigenPoseAlgebra` is used.
- `out_handle`: Returns the created pose algebra handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy Pose Algebra Handle

```c
void elite_pose_algebra_destroy(elite_pose_algebra_handle_t* handle);
```

- ***Description***

  Releases the pose algebra handle and associated resources.

## Matrix Operations

```c
elite_c_status_t elite_pose_algebra_inverse_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose, elite_pose_matrix_t* out_inverse_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_multiply_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose, const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_add_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose, const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_subtract_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose, const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***Description***

  Performs inverse, multiply, add, or subtract operations on 4x4 pose matrices.

- ***Parameters***

- `handle`: Pose algebra handle.
- `pose`: Input matrix.
- `left_pose`, `right_pose`: Matrix operands.
- `out_inverse_pose`, `out_pose`: Output matrix.
- `out_result`: Operation result details.
- `out_success`: Returns whether the operation succeeded.

## Vector Operations

```c
elite_c_status_t elite_pose_algebra_inverse_vector(elite_pose_algebra_handle_t* handle, const double* pose6, double* out_inverse_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_multiply_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6, const double* right_pose6, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_add_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6, const double* right_pose6, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_subtract_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6, const double* right_pose6, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***Description***

  Performs inverse, multiply, add, or subtract operations on 6D pose vectors.

- ***Parameters***

- `pose6`, `left_pose6`, `right_pose6`: 6-element pose vectors.
- `out_inverse_pose6`, `out_pose6`: Output 6-element pose vector.
- `out_result`: Operation result details.
- `out_success`: Returns whether the operation succeeded.

## Matrix and Vector Conversion

```c
elite_c_status_t elite_pose_algebra_vector_to_matrix(elite_pose_algebra_handle_t* handle, const double* pose6, elite_pose_matrix_t* out_pose_matrix, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_matrix_to_vector(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose_matrix, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***Description***

  Converts between a 6D pose vector and a 4x4 pose matrix.

## Distance

```c
elite_c_status_t elite_pose_algebra_distance_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose_a, const elite_pose_matrix_t* pose_b, elite_pose_distance_t* out_distance, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_distance_vector(elite_pose_algebra_handle_t* handle, const double* pose_a6, const double* pose_b6, elite_pose_distance_t* out_distance, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***Description***

  Calculates linear and angular distance between two poses.

## Frame Conversion

```c
elite_c_status_t elite_pose_algebra_world_to_local_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* world_ref_pose, const elite_pose_matrix_t* world_pose, elite_pose_matrix_t* out_local_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_world_to_local_vector(elite_pose_algebra_handle_t* handle, const double* world_ref_pose6, const double* world_pose6, double* out_local_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_local_to_world_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* world_ref_pose, const elite_pose_matrix_t* local_pose, elite_pose_matrix_t* out_world_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_local_to_world_vector(elite_pose_algebra_handle_t* handle, const double* world_ref_pose6, const double* local_pose6, double* out_world_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***Description***

  Converts poses between world coordinates and a local reference frame.

## Error Helpers

```c
const char* elite_pose_algebra_last_error_message(elite_pose_algebra_handle_t* handle);
const char* elite_pose_algebra_global_last_error_message(void);
```

- ***Description***

  Gets the most recent pose algebra wrapper error string.

- ***Notes***

  `elite_pose_algebra_result_t::message` is valid until the next call on the same pose algebra handle.

## Example

Run with the Eigen plugin:

```bash
./build-c/examples/pose_algebra_example \
  /usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_eigen_pose_algebra.so \
  ELITE::EigenPoseAlgebra
```

Run with the Elite plugin:

```bash
./build-c/examples/pose_algebra_example \
  /usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_pose_algebra.so \
  ELITE::ElitePoseAlgebra
```

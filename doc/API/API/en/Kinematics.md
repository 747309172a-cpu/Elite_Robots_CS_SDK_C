# Kinematics Module

## Overview

The Kinematics module exposes the upstream kinematics plugin interface through the C API. It loads a kinematics plugin library at runtime, creates a solver instance, sets robot MDH parameters, and provides FK/IK calculation APIs.

## Header

```c
#include <Elite/Kinematics_C.hpp>
```

The header is also included by:

```c
#include <Elite_Sdk_C.h>
```

## Plugin

The kinematics implementation is provided by a runtime plugin. The default class name is:

```text
ELITE::KdlKinematicsPlugin
```

Common installed plugin path on Linux:

```text
/usr/local/lib/elite-cs-series-sdk-plugins/kinematics/libelite_kdl_kinematics.so
```

## Handle Type

```c
typedef struct elite_kinematics_handle_t elite_kinematics_handle_t;
```

- ***Description***

  Opaque handle that owns the plugin loader and the kinematics solver instance.

## API Reference

### Create Kinematics Handle

```c
elite_c_status_t elite_kinematics_create(
    const char* plugin_lib_path,
    const char* plugin_class_name,
    elite_kinematics_handle_t** out_handle
);
```

- ***Description***

  Loads the kinematics plugin library and creates a solver instance.

- ***Parameters***

- `plugin_lib_path`: Path to the kinematics plugin shared library.
- `plugin_class_name`: Plugin class name. If `NULL` or empty, `ELITE::KdlKinematicsPlugin` is used.
- `out_handle`: Returns the created kinematics handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy Kinematics Handle

```c
void elite_kinematics_destroy(elite_kinematics_handle_t* handle);
```

- ***Description***

  Releases the kinematics handle and unloads associated resources.

### Set MDH Parameters

```c
elite_c_status_t elite_kinematics_set_mdh(
    elite_kinematics_handle_t* handle,
    const double* alpha6,
    const double* a6,
    const double* d6
);
```

- ***Description***

  Sets robot MDH parameters used by the kinematics solver.

- ***Parameters***

- `handle`: Kinematics handle.
- `alpha6`: 6-element MDH alpha array.
- `a6`: 6-element MDH a array.
- `d6`: 6-element MDH d array.

- ***Notes***

  DH parameters can be read from the robot using `elite_primary_get_kinematics_info()` or `elite_driver_get_primary_package()`.

### Forward Kinematics

```c
elite_c_status_t elite_kinematics_get_position_fk(
    elite_kinematics_handle_t* handle,
    const double* joint_angles6,
    double* out_pose6,
    int32_t* out_success
);
```

- ***Description***

  Calculates TCP pose from joint angles.

- ***Parameters***

- `handle`: Kinematics handle.
- `joint_angles6`: 6-element joint angle array.
- `out_pose6`: Returns the 6-element TCP pose.
- `out_success`: Returns whether FK succeeded.

### Inverse Kinematics

```c
elite_c_status_t elite_kinematics_get_position_ik(
    elite_kinematics_handle_t* handle,
    const double* pose6,
    const double* near6,
    double* out_solution6,
    elite_kinematics_result_t* out_result,
    int32_t* out_success
);
```

- ***Description***

  Calculates one joint solution for a target TCP pose.

- ***Parameters***

- `handle`: Kinematics handle.
- `pose6`: 6-element target TCP pose.
- `near6`: 6-element seed joint position used to choose the nearest solution.
- `out_solution6`: Returns the selected joint solution.
- `out_result`: Returns solver result details. See [CommonTypes.md](./CommonTypes.md).
- `out_success`: Returns whether IK succeeded.

### Inverse Kinematics All Solutions

```c
elite_c_status_t elite_kinematics_get_position_ik_all(
    elite_kinematics_handle_t* handle,
    const double* pose6,
    const double* near6,
    double* out_solutions6,
    int32_t max_solutions,
    int32_t* out_solution_count,
    elite_kinematics_result_t* out_result,
    int32_t* out_success
);
```

- ***Description***

  Calculates multiple joint solutions for a target TCP pose.

- ***Parameters***

- `out_solutions6`: Output buffer containing `max_solutions * 6` doubles. May be `NULL` when only querying solution count.
- `max_solutions`: Maximum number of solutions to copy to `out_solutions6`.
- `out_solution_count`: Returns the total number of solutions found.

### Default Timeout

```c
elite_c_status_t elite_kinematics_set_default_timeout(elite_kinematics_handle_t* handle, double timeout);
elite_c_status_t elite_kinematics_get_default_timeout(elite_kinematics_handle_t* handle, double* out_timeout);
```

- ***Description***

  Sets or gets the solver default timeout in seconds.

### Error Helpers

```c
const char* elite_kinematics_last_error_message(elite_kinematics_handle_t* handle);
const char* elite_kinematics_global_last_error_message(void);
```

- ***Description***

  Gets the most recent kinematics wrapper error string.

## Example

Build examples with `-DELITE_BUILD_EXAMPLES=ON`, then run:

```bash
./build-c/examples/kinematics_example \
  192.168.1.200 \
  /usr/local/lib/elite-cs-series-sdk-plugins/kinematics/libelite_kdl_kinematics.so
```

Optional explicit class name:

```bash
./build-c/examples/kinematics_example \
  192.168.1.200 \
  /usr/local/lib/elite-cs-series-sdk-plugins/kinematics/libelite_kdl_kinematics.so \
  ELITE::KdlKinematicsPlugin
```

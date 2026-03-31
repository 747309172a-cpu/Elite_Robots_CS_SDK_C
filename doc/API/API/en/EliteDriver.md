# EliteDriver Module

## Overview

EliteDriver is the main module used for external robot control and data interaction. It creates the required communication channels, sends external control scripts to the robot, exchanges motion data, and reports motion results when needed.

## Header

```c
#include <Elite/EliteDriver_C.h>
```

## Callback and Data Types

### Trajectory Result Callback
```c
typedef void (*elite_driver_trajectory_result_cb_t)(
    elite_trajectory_motion_result_t result,
    void* user_data
);
```

- ***Description***

  Callback used to receive the result of a trajectory motion after it finishes.

- ***Callback Parameters***

- `result`: Trajectory motion result. See `elite_trajectory_motion_result_t` in [CommonTypes.md](./CommonTypes.md).
- `user_data`: Opaque pointer provided during callback registration.

### Robot Exception Payload
```c
typedef struct elite_driver_robot_exception_t {
    int32_t type;
    uint64_t timestamp;
    int32_t error_code;
    int32_t sub_error_code;
    int32_t error_source;
    int32_t error_level;
    int32_t error_data_type;
    uint32_t data_u32;
    int32_t data_i32;
    float data_f32;
    int32_t line;
    int32_t column;
    const char* message;
} elite_driver_robot_exception_t;
```

- ***Description***

  Payload structure used by driver robot exception callbacks.

- ***Field Description***

- `type`: Exception type.
- `timestamp`: Exception timestamp.
- `error_code` and `sub_error_code`: Error code and sub error code.
- `error_source`: Error source.
- `error_level`: Error severity level.
- `error_data_type`: Type of optional data payload.
- `data_u32`, `data_i32`, `data_f32`: Optional additional error data.
- `line` and `column`: Script line and column for script-related exceptions.
- `message`: Human-readable exception message.

- ***Notes***

  The `message` pointer is only valid during the callback. Copy the string if it must be retained after the callback returns.

### Robot Exception Callback
```c
typedef void (*elite_driver_robot_exception_cb_t)(
    const elite_driver_robot_exception_t* ex,
    void* user_data
);
```

- ***Description***

  Callback type used to receive robot exception notifications from the driver.

- ***Callback Parameters***

- `ex`: Exception payload pointer.
- `user_data`: Opaque pointer provided during callback registration.

## API Reference

### Set Default Driver Configuration
```c
void elite_driver_config_set_default(elite_driver_config_t* config);
```

- ***Description***

  Initializes `elite_driver_config_t` with SDK default values.

- ***Parameters***

- `config`: Driver configuration struct to initialize.

- ***Returns***

  No return value.

- ***Notes***

  Default values are documented in [EliteDriverConfig.md](./EliteDriverConfig.md).

### Create Driver Handle
```c
elite_c_status_t elite_driver_create(
    const elite_driver_config_t* config,
    elite_driver_handle_t** out_handle
);
```

- ***Description***

  Creates an EliteDriver handle and initializes internal resources required for robot communication.

- ***Parameters***

- `config`: Driver configuration. See [EliteDriverConfig.md](./EliteDriverConfig.md).
- `out_handle`: Returns the created driver handle.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  `config->robot_ip` and `config->script_file_path` must both be non-`NULL`.

### Destroy Driver Handle
```c
void elite_driver_destroy(elite_driver_handle_t* handle);
```

- ***Description***

  Releases the driver handle and its associated resources.

- ***Parameters***

- `handle`: Driver handle to destroy.

- ***Returns***

  No return value.

### Query Robot Connection State
```c
elite_c_status_t elite_driver_is_robot_connected(
    elite_driver_handle_t* handle,
    int32_t* out_connected
);
```

- ***Description***

  Checks whether the driver is currently connected to the robot.

- ***Parameters***

- `handle`: Driver handle.
- `out_connected`: Returns connection state; `1` means connected and `0` means disconnected.

- ***Returns***

  Returns `elite_c_status_t`.

### Send External Control Script
```c
elite_c_status_t elite_driver_send_external_control_script(
    elite_driver_handle_t* handle,
    int32_t* out_success
);
```

- ***Description***

  Sends the external control script to the robot to establish or restore external control.

- ***Parameters***

- `handle`: Driver handle.
- `out_success`: Returns whether the script was sent successfully.

- ***Returns***

  Returns `elite_c_status_t`.

### Stop External Control
```c
elite_c_status_t elite_driver_stop_control(
    elite_driver_handle_t* handle,
    int32_t wait_ms,
    int32_t* out_success
);
```

- ***Description***

  Stops the current external control session and waits for robot disconnect if requested.

- ***Parameters***

- `handle`: Driver handle.
- `wait_ms`: Blocking wait time in milliseconds for robot disconnect.
- `out_success`: Returns whether stopping control succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Motion Commands
```c
elite_c_status_t elite_driver_write_servoj(elite_driver_handle_t* handle, const double* pos6, int32_t timeout_ms, int32_t cartesian, int32_t* out_success);
elite_c_status_t elite_driver_write_speedj(elite_driver_handle_t* handle, const double* vel6, int32_t timeout_ms, int32_t* out_success);
elite_c_status_t elite_driver_write_speedl(elite_driver_handle_t* handle, const double* vel6, int32_t timeout_ms, int32_t* out_success);
elite_c_status_t elite_driver_write_idle(elite_driver_handle_t* handle, int32_t timeout_ms, int32_t* out_success);
```

- ***Description***

  Sends servo position, joint speed, TCP speed, or idle keepalive commands to the robot.

- ***Parameters***

- `handle`: Driver handle.
- `pos6`: 6-element target position array for `servoj`.
- `vel6`: 6-element velocity array for `speedj` or `speedl`.
- `timeout_ms`: Timeout for the robot to receive the next command. Values less than or equal to `0` mean wait indefinitely.
- `cartesian`: For `servoj`, pass `1` for a Cartesian point and `0` for a joint point.
- `out_success`: Returns whether the command was sent successfully.

- ***Returns***

  Returns `elite_c_status_t`.

### Trajectory Callback Registration
```c
elite_c_status_t elite_driver_set_trajectory_result_callback(
    elite_driver_handle_t* handle,
    elite_driver_trajectory_result_cb_t cb,
    void* user_data
);
```

- ***Description***

  Registers the callback used to receive trajectory execution results.

- ***Parameters***

- `handle`: Driver handle.
- `cb`: Trajectory result callback. Passing `NULL` unregisters the callback.
- `user_data`: Opaque pointer passed back to the callback.

- ***Returns***

  Returns `elite_c_status_t`.

### Trajectory Commands
```c
elite_c_status_t elite_driver_write_trajectory_point(
    elite_driver_handle_t* handle,
    const double* positions6,
    float time,
    float blend_radius,
    int32_t cartesian,
    int32_t* out_success
);
elite_c_status_t elite_driver_write_trajectory_control_action(
    elite_driver_handle_t* handle,
    elite_trajectory_control_action_t action,
    int32_t point_number,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***Description***

  Sends trajectory points and trajectory control actions such as `START`, `NOOP`, or `CANCEL`.

- ***Parameters***

- `handle`: Driver handle.
- `positions6`: 6-element joint or Cartesian target point.
- `time`: Motion time for the point.
- `blend_radius`: Blend radius for the point.
- `cartesian`: Pass `1` for a Cartesian point and `0` for a joint point.
- `action`: Trajectory control action. See `elite_trajectory_control_action_t` in [CommonTypes.md](./CommonTypes.md).
- `point_number`: Expected number of points for the trajectory command.
- `timeout_ms`: Timeout for the next expected control message.
- `out_success`: Returns whether the command succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  After sending `START`, keep sending the next command within the timeout window. `NOOP` may be used as a keepalive when needed.

### Freedrive Command
```c
elite_c_status_t elite_driver_write_freedrive(
    elite_driver_handle_t* handle,
    elite_freedrive_action_t action,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***Description***

  Sends freedrive control actions to the robot.

- ***Parameters***

- `handle`: Driver handle.
- `action`: Freedrive action. See `elite_freedrive_action_t` in [CommonTypes.md](./CommonTypes.md).
- `timeout_ms`: Timeout for the next expected control message.
- `out_success`: Returns whether the command succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Force and Payload Configuration
```c
elite_c_status_t elite_driver_zero_ft_sensor(elite_driver_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_driver_set_payload(elite_driver_handle_t* handle, double mass, const double* cog3, int32_t* out_success);
elite_c_status_t elite_driver_set_tool_voltage(elite_driver_handle_t* handle, elite_tool_voltage_t voltage, int32_t* out_success);
elite_c_status_t elite_driver_start_force_mode(elite_driver_handle_t* handle, const double* reference_frame6, const int32_t* selection_vector6, const double* wrench6, elite_force_mode_t mode, const double* limits6, int32_t* out_success);
elite_c_status_t elite_driver_end_force_mode(elite_driver_handle_t* handle, int32_t* out_success);
```

- ***Description***

  Supports force-torque sensor zeroing, payload mass and center-of-gravity configuration, tool voltage setting, and force mode control.

- ***Parameters***

- `handle`: Driver handle.
- `mass`: Payload mass.
- `cog3`: 3-element payload center of gravity vector.
- `voltage`: Tool voltage. See `elite_tool_voltage_t` in [CommonTypes.md](./CommonTypes.md).
- `reference_frame6`: 6-element force mode reference frame.
- `selection_vector6`: 6-element selection vector.
- `wrench6`: 6-element target wrench.
- `mode`: Force mode. See `elite_force_mode_t` in [CommonTypes.md](./CommonTypes.md).
- `limits6`: 6-element force mode limits.
- `out_success`: Returns whether the command succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Primary and Script Helpers
```c
elite_c_status_t elite_driver_send_script(elite_driver_handle_t* handle, const char* script, int32_t* out_success);
elite_c_status_t elite_driver_get_primary_package(elite_driver_handle_t* handle, int32_t timeout_ms, double* out_dh_a6, double* out_dh_d6, double* out_dh_alpha6, int32_t* out_success);
elite_c_status_t elite_driver_primary_reconnect(elite_driver_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_driver_register_robot_exception_callback(elite_driver_handle_t* handle, elite_driver_robot_exception_cb_t cb, void* user_data);
```

- ***Description***

  Sends raw scripts, reads kinematics information from the Primary channel, reconnects Primary, and registers robot exception callbacks.

- ***Parameters***

- `handle`: Driver handle.
- `script`: Script text to send.
- `timeout_ms`: Wait timeout in milliseconds for reading Primary data.
- `out_dh_a6`: Returns the 6-element DH `a` array.
- `out_dh_d6`: Returns the 6-element DH `d` array.
- `out_dh_alpha6`: Returns the 6-element DH `alpha` array.
- `cb`: Robot exception callback. Passing `NULL` unregisters the callback.
- `user_data`: Opaque pointer passed back to the callback.
- `out_success`: Returns whether the operation succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### RS485 Helpers
```c
elite_c_status_t elite_driver_start_tool_rs485(
    elite_driver_handle_t* handle,
    const elite_serial_config_t* config,
    const char* ssh_password,
    int32_t tcp_port,
    elite_serial_comm_handle_t** out_comm
);
elite_c_status_t elite_driver_end_tool_rs485(
    elite_driver_handle_t* handle,
    elite_serial_comm_handle_t* comm,
    const char* ssh_password,
    int32_t* out_success
);
elite_c_status_t elite_driver_start_board_rs485(
    elite_driver_handle_t* handle,
    const elite_serial_config_t* config,
    const char* ssh_password,
    int32_t tcp_port,
    elite_serial_comm_handle_t** out_comm
);
elite_c_status_t elite_driver_end_board_rs485(
    elite_driver_handle_t* handle,
    elite_serial_comm_handle_t* comm,
    const char* ssh_password,
    int32_t* out_success
);
```

- ***Description***

  Starts and stops RS485 forwarding for the tool-side or controller-board serial interfaces.

- ***Parameters***

- `handle`: Driver handle.
- `config`: Serial configuration. See `elite_serial_config_t` in [SerialCommunication.md](./SerialCommunication.md).
- `ssh_password`: Controller SSH password.
- `tcp_port`: TCP port used to forward RS485 data. The caller must pass it explicitly.
- `out_comm`: Returns the created serial communication handle.
- `comm`: Serial communication handle to stop.
- `out_success`: Returns whether the stop operation succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Error Helpers
```c
const char* elite_driver_last_error_message(elite_driver_handle_t* handle);
const char* elite_c_last_error_message(void);
```

- ***Description***

  Gets the most recent driver-specific error string or the most recent global C wrapper error string.

- ***Parameters***

- `handle`: Driver handle for per-handle error retrieval.

- ***Returns***

  Returns the most recent error string. For argument validation failures and other errors that only update the thread-local global error, use `elite_c_last_error_message()`.

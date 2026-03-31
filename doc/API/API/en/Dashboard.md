# Dashboard Module

## Overview

The Dashboard module provides access to the robot dashboard server for controller management, task control, status queries, and string-based shell commands.

## Header

```c
#include <Dashboard/DashboardClient_C.hpp>
```

## API Reference

### Create Handle
```c
elite_c_status_t elite_dashboard_create(elite_dashboard_handle_t** out_handle);
```

- ***Description***

  Creates a Dashboard client handle.

- ***Parameters***

- `out_handle`: Returns the created Dashboard handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy Handle
```c
void elite_dashboard_destroy(elite_dashboard_handle_t* handle);
```

- ***Description***

  Releases a Dashboard client handle.

- ***Parameters***

- `handle`: Dashboard handle to destroy.

- ***Returns***

  No return value.

### Connect Server
```c
elite_c_status_t elite_dashboard_connect(
    elite_dashboard_handle_t* handle,
    const char* ip,
    int32_t port,
    int32_t* out_success
);
```

- ***Description***

  Connects to the robot dashboard server.

- ***Parameters***

- `handle`: Dashboard handle.
- `ip`: Robot IP address.
- `port`: Dashboard port, typically `29999`, and it must be provided explicitly by the caller.
- `out_success`: Returns whether the connection succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Disconnect Server
```c
elite_c_status_t elite_dashboard_disconnect(elite_dashboard_handle_t* handle);
```

- ***Description***

  Disconnects the current Dashboard session.

- ***Parameters***

- `handle`: Dashboard handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Connectivity Check
```c
elite_c_status_t elite_dashboard_echo(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***Description***

  Checks whether the Dashboard shell server is reachable.

- ***Parameters***

- `handle`: Dashboard handle.
- `out_success`: Returns whether the echo check succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Controller Power and Safety Commands
```c
elite_c_status_t elite_dashboard_power_on(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_power_off(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_brake_release(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_close_safety_dialog(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_unlock_protective_stop(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_safety_system_restart(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***Description***

  Controls controller power, brake release, and common safety recovery operations.

- ***Parameters***

- `handle`: Dashboard handle.
- `out_success`: Returns whether the requested operation succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Write Controller Log and Popup
```c
elite_c_status_t elite_dashboard_log(
    elite_dashboard_handle_t* handle,
    const char* message,
    int32_t* out_success
);
elite_c_status_t elite_dashboard_popup(
    elite_dashboard_handle_t* handle,
    const char* arg,
    const char* message,
    int32_t* out_success
);
```

- ***Description***

  Writes a controller log message or shows a popup on the controller side.

- ***Parameters***

- `handle`: Dashboard handle.
- `message`: Log text for `elite_dashboard_log`.
- `arg`: Extra popup argument string for `elite_dashboard_popup`; it must not be `NULL`.
- `message`: Popup message text for `elite_dashboard_popup`; `NULL` is treated as an empty string.
- `out_success`: Returns whether the operation succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Program Exit and Controller Restart Commands
```c
elite_c_status_t elite_dashboard_quit(elite_dashboard_handle_t* handle);
elite_c_status_t elite_dashboard_reboot(elite_dashboard_handle_t* handle);
elite_c_status_t elite_dashboard_shutdown(elite_dashboard_handle_t* handle);
```

- ***Description***

  Exits the current program or restarts and shuts down the controller.

- ***Parameters***

- `handle`: Dashboard handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Status Query APIs
```c
elite_c_status_t elite_dashboard_robot_mode(elite_dashboard_handle_t* handle, int32_t* out_mode);
elite_c_status_t elite_dashboard_safety_mode(elite_dashboard_handle_t* handle, int32_t* out_mode);
elite_c_status_t elite_dashboard_running_status(elite_dashboard_handle_t* handle, int32_t* out_status);
elite_c_status_t elite_dashboard_get_task_status(elite_dashboard_handle_t* handle, int32_t* out_status);
elite_c_status_t elite_dashboard_speed_scaling(elite_dashboard_handle_t* handle, int32_t* out_scaling);
elite_c_status_t elite_dashboard_set_speed_scaling(elite_dashboard_handle_t* handle, int32_t scaling, int32_t* out_success);
elite_c_status_t elite_dashboard_task_is_running(elite_dashboard_handle_t* handle, int32_t* out_running);
elite_c_status_t elite_dashboard_is_task_saved(elite_dashboard_handle_t* handle, int32_t* out_saved);
```

- ***Description***

  Queries robot mode, safety mode, runtime state, task state, speed scaling, and task saved or running state. It also supports setting speed scaling.

- ***Parameters***

- `handle`: Dashboard handle.
- `out_mode`: Returns robot mode or safety mode. Values are interpretable as `elite_robot_mode_t` or `elite_safety_mode_t`.
- `out_status`: Returns controller running status or task status. Task status values are interpretable as `elite_task_status_t`.
- `out_scaling`: Returns the speed scaling percentage.
- `scaling`: Speed scaling percentage to set.
- `out_success`: Returns whether speed scaling was set successfully.
- `out_running`: Returns whether a task is currently running; `1` means yes and `0` means no.
- `out_saved`: Returns whether the current task is saved; `1` means yes and `0` means no.

- ***Returns***

  Returns `elite_c_status_t`.

### Program and Configuration Control
```c
elite_c_status_t elite_dashboard_play_program(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_pause_program(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_stop_program(elite_dashboard_handle_t* handle, int32_t* out_success);
elite_c_status_t elite_dashboard_load_configuration(elite_dashboard_handle_t* handle, const char* path, int32_t* out_success);
elite_c_status_t elite_dashboard_load_task(elite_dashboard_handle_t* handle, const char* path, int32_t* out_success);
elite_c_status_t elite_dashboard_is_configuration_modify(elite_dashboard_handle_t* handle, int32_t* out_modified);
```

- ***Description***

  Plays, pauses, and stops programs, loads configurations or tasks, and checks whether the current configuration has been modified.

- ***Parameters***

- `handle`: Dashboard handle.
- `path`: Configuration or task file path.
- `out_success`: Returns whether the requested command succeeded.
- `out_modified`: Returns whether the configuration has been modified; `1` means modified and `0` means not modified.

- ***Returns***

  Returns `elite_c_status_t`.

### String Query APIs
```c
elite_c_status_t elite_dashboard_version(elite_dashboard_handle_t* handle, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_help(elite_dashboard_handle_t* handle, const char* cmd, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_usage(elite_dashboard_handle_t* handle, const char* cmd, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_robot_type(elite_dashboard_handle_t* handle, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_robot_serial_number(elite_dashboard_handle_t* handle, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_robot_id(elite_dashboard_handle_t* handle, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_configuration_path(elite_dashboard_handle_t* handle, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_get_task_path(elite_dashboard_handle_t* handle, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
elite_c_status_t elite_dashboard_send_and_receive(elite_dashboard_handle_t* handle, const char* cmd, char* out_buffer, int32_t buffer_len, int32_t* out_required_len);
```

- ***Description***

  Reads version and identity strings, command help text, file paths, and raw Dashboard shell responses.

- ***Parameters***

- `handle`: Dashboard handle.
- `cmd`: Command string used by `help`, `usage`, or raw send-and-receive.
- `out_buffer`: Output string buffer.
- `buffer_len`: Size of `out_buffer`.
- `out_required_len`: Returns the required string length including the trailing null terminator.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  For all string output APIs on this page, `out_required_len` must not be `NULL`.
  If `out_buffer == NULL` or `buffer_len <= 0`, the function still succeeds and reports the full required length through `out_required_len`. If the buffer is too small, the output is truncated and null-terminated.

### Error Message Helpers
```c
const char* elite_dashboard_last_error_message(elite_dashboard_handle_t* handle);
const char* elite_dashboard_global_last_error_message(void);
```

- ***Description***

  Gets the most recent Dashboard error string. If no handle is available, the per-handle helper also returns the current thread's global Dashboard error.

- ***Parameters***

- `handle`: Dashboard handle for per-handle error retrieval. Passing `NULL` returns the current thread's global Dashboard error string.

- ***Returns***

  Returns the most recent error string.

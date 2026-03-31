# Primary Module

## Overview

The Primary module is used to access the controller Primary interface, send scripts, read kinematics information, and receive robot exception messages.

## Header

```c
#include <Primary/PrimaryPortInterface_C.hpp>
```

## API Reference

### Create Handle
```c
elite_c_status_t elite_primary_create(elite_primary_handle_t** out_handle);
```

- ***Description***

  Creates a Primary communication handle.

- ***Parameters***

- `out_handle`: Returns the created Primary handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy Handle
```c
void elite_primary_destroy(elite_primary_handle_t* handle);
```

- ***Description***

  Releases a Primary communication handle.

- ***Parameters***

- `handle`: Primary handle to destroy.

- ***Returns***

  No return value.

### Connect Primary Port
```c
elite_c_status_t elite_primary_connect(
    elite_primary_handle_t* handle,
    const char* ip,
    int32_t port,
    int32_t* out_success
);
```

- ***Description***

  Connects to the robot Primary port.

- ***Parameters***

- `handle`: Primary handle.
- `ip`: Robot IP address.
- `port`: Primary port.
- `out_success`: Returns whether the connection succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Disconnect Primary Port
```c
elite_c_status_t elite_primary_disconnect(elite_primary_handle_t* handle);
```

- ***Description***

  Disconnects the current Primary session.

- ***Parameters***

- `handle`: Primary handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Send Script
```c
elite_c_status_t elite_primary_send_script(
    elite_primary_handle_t* handle,
    const char* script,
    int32_t* out_success
);
```

- ***Description***

  Sends a script string through the Primary channel for controller execution.

- ***Parameters***

- `handle`: Primary handle.
- `script`: Script text to send.
- `out_success`: Returns whether sending the script succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Local IP
```c
elite_c_status_t elite_primary_get_local_ip(
    elite_primary_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***Description***

  Gets the local IP address used by the current Primary connection.

- ***Parameters***

- `handle`: Primary handle.
- `out_buffer`: Output string buffer.
- `buffer_len`: Size of `out_buffer`.
- `out_required_len`: Returns the required length including the trailing null terminator.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  `out_required_len` must not be `NULL`.
  If `out_buffer == NULL` or `buffer_len <= 0`, the function still succeeds and reports the full required length through `out_required_len`.

### Get Kinematics Information
```c
elite_c_status_t elite_primary_get_kinematics_info(
    elite_primary_handle_t* handle,
    int32_t timeout_ms,
    double* out_dh_a6,
    double* out_dh_d6,
    double* out_dh_alpha6,
    int32_t* out_success
);
```

- ***Description***

  Reads the robot DH parameters from a Primary package.

- ***Parameters***

- `handle`: Primary handle.
- `timeout_ms`: Wait timeout in milliseconds.
- `out_dh_a6`: Returns the 6-element DH `a` array.
- `out_dh_d6`: Returns the 6-element DH `d` array.
- `out_dh_alpha6`: Returns the 6-element DH `alpha` array.
- `out_success`: Returns whether reading succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Robot Exception Payload
```c
typedef struct elite_primary_robot_exception_t {
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
} elite_primary_robot_exception_t;
```

- ***Description***

  Carries robot exception information reported through the Primary callback.

- ***Field Description***

- `type`: Exception type.
- `timestamp`: Exception timestamp.
- `error_code` and `sub_error_code`: Error code and sub error code.
- `error_source`: Error source.
- `error_level`: Error severity level.
- `error_data_type`: Type of extra error data.
- `data_u32`, `data_i32`, `data_f32`: Optional extra data payload.
- `line` and `column`: Script line and column for script-related exceptions.
- `message`: Human-readable exception message.

- ***Notes***

  The `message` pointer is only valid during the callback. Copy it if it needs to outlive the callback.

### Robot Exception Callback Type
```c
typedef void (*elite_primary_robot_exception_cb_t)(
    const elite_primary_robot_exception_t* ex,
    void* user_data
);
```

- ***Description***

  Callback type used to receive robot exception notifications.

- ***Callback Parameters***

- `ex`: Exception payload.
- `user_data`: Opaque pointer provided during registration.

### Register Robot Exception Callback
```c
elite_c_status_t elite_primary_register_robot_exception_callback(
    elite_primary_handle_t* handle,
    elite_primary_robot_exception_cb_t cb,
    void* user_data
);
```

- ***Description***

  Registers a callback for robot exception messages. Passing `NULL` as `cb` unregisters the current callback.

- ***Parameters***

- `handle`: Primary handle.
- `cb`: Exception callback function. Passing `NULL` unregisters the current callback.
- `user_data`: Opaque user pointer passed back to the callback.

- ***Returns***

  Returns `elite_c_status_t`.

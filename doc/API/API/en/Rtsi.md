# RTSI Module

## Overview

The RTSI module contains three parts:

- RTSI Client: low-level real-time connection, protocol negotiation, recipe setup, and data exchange
- RTSI Recipe: recipe field read/write helpers
- RTSI IO: higher-level real-time IO, state, and register access

## Header

```c
#include <Rtsi/Elite_Rtsi_C.h>
```

## RTSI Client

### Create RTSI Client
```c
elite_c_status_t elite_rtsi_client_create(elite_rtsi_client_handle_t** out_handle);
```

- ***Description***

  Creates an RTSI client handle.

- ***Parameters***

- `out_handle`: Returns the created RTSI client handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy RTSI Client
```c
void elite_rtsi_client_destroy(elite_rtsi_client_handle_t* handle);
```

- ***Description***

  Releases an RTSI client handle.

- ***Parameters***

- `handle`: RTSI client handle to destroy.

- ***Returns***

  No return value.

### Connect Controller
```c
elite_c_status_t elite_rtsi_client_connect(
    elite_rtsi_client_handle_t* handle,
    const char* ip,
    int32_t port
);
```

- ***Description***

  Connects to the robot RTSI service.

- ***Parameters***

- `handle`: RTSI client handle.
- `ip`: Robot IP address.
- `port`: RTSI port.

- ***Returns***

  Returns `elite_c_status_t`. This API does not provide an extra `out_success` flag; failures are reported directly through the return status or the last-error helper.

### Disconnect
```c
elite_c_status_t elite_rtsi_client_disconnect(elite_rtsi_client_handle_t* handle);
```

- ***Description***

  Disconnects the current RTSI session.

- ***Parameters***

- `handle`: RTSI client handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Negotiate Protocol Version
```c
elite_c_status_t elite_rtsi_client_negotiate_protocol_version(
    elite_rtsi_client_handle_t* handle,
    uint16_t version,
    int32_t* out_success
);
```

- ***Description***

  Negotiates the RTSI protocol version with the controller.

- ***Parameters***

- `handle`: RTSI client handle.
- `version`: Protocol version number.
- `out_success`: Returns whether negotiation succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Controller Version
```c
elite_c_status_t elite_rtsi_client_get_controller_version(
    elite_rtsi_client_handle_t* handle,
    elite_version_info_t* out_version
);
```

- ***Description***

  Gets controller version information.

- ***Parameters***

- `handle`: RTSI client handle.
- `out_version`: Returns the controller version information.

- ***Returns***

  Returns `elite_c_status_t`.

### Setup Output Recipe
```c
elite_c_status_t elite_rtsi_client_setup_output_recipe(
    elite_rtsi_client_handle_t* handle,
    const char* recipe_csv,
    double frequency,
    elite_rtsi_recipe_handle_t** out_recipe
);
```

- ***Description***

  Creates a recipe used to receive controller output data.

- ***Parameters***

- `handle`: RTSI client handle.
- `recipe_csv`: Comma-separated output recipe field list, for example `actual_q,actual_qd,target_q`.
- `frequency`: Output recipe frequency in Hz.
- `out_recipe`: Returns the created recipe handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Setup Input Recipe
```c
elite_c_status_t elite_rtsi_client_setup_input_recipe(
    elite_rtsi_client_handle_t* handle,
    const char* recipe_csv,
    elite_rtsi_recipe_handle_t** out_recipe
);
```

- ***Description***

  Creates a recipe used to send input data to the controller.

- ***Parameters***

- `handle`: RTSI client handle.
- `recipe_csv`: Comma-separated input recipe field list.
- `out_recipe`: Returns the created recipe handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy Recipe
```c
void elite_rtsi_recipe_destroy(elite_rtsi_recipe_handle_t* recipe);
```

- ***Description***

  Releases an RTSI recipe handle.

- ***Parameters***

- `recipe`: Recipe handle to destroy.

- ***Returns***

  No return value.

### Start RTSI
```c
elite_c_status_t elite_rtsi_client_start(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_success
);
```

- ***Description***

  Starts the RTSI data stream.

- ***Parameters***

- `handle`: RTSI client handle.
- `out_success`: Returns whether start succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Pause RTSI
```c
elite_c_status_t elite_rtsi_client_pause(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_success
);
```

- ***Description***

  Pauses the RTSI data stream.

- ***Parameters***

- `handle`: RTSI client handle.
- `out_success`: Returns whether pause succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Send Input Recipe Data
```c
elite_c_status_t elite_rtsi_client_send(
    elite_rtsi_client_handle_t* handle,
    elite_rtsi_recipe_handle_t* recipe
);
```

- ***Description***

  Sends one frame of input recipe data to the controller.

- ***Parameters***

- `handle`: RTSI client handle.
- `recipe`: Input recipe handle to send.

- ***Returns***

  Returns `elite_c_status_t`.

### Receive Single Recipe Data
```c
elite_c_status_t elite_rtsi_client_receive_data(
    elite_rtsi_client_handle_t* handle,
    elite_rtsi_recipe_handle_t* recipe,
    int32_t read_newest,
    int32_t* out_success
);
```

- ***Description***

  Receives one frame of data for a single recipe.

- ***Parameters***

- `handle`: RTSI client handle.
- `recipe`: Recipe handle that receives the decoded data.
- `read_newest`: Non-zero reads the newest frame; `0` reads in queue order.
- `out_success`: Returns whether receive succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Receive Multiple Recipe Data
```c
elite_c_status_t elite_rtsi_client_receive_data_multi(
    elite_rtsi_client_handle_t* handle,
    elite_rtsi_recipe_handle_t** recipes,
    int32_t recipe_count,
    int32_t read_newest,
    int32_t* out_received_index
);
```

- ***Description***

  Receives data across multiple recipes in one call.

- ***Parameters***

- `handle`: RTSI client handle.
- `recipes`: Array of recipe handles.
- `recipe_count`: Number of recipe handles in `recipes`.
- `read_newest`: Non-zero reads the newest frame.
- `out_received_index`: Returns the index of the recipe that received data; typically `-1` if no data was available.

- ***Returns***

  Returns `elite_c_status_t`.

### Query Client State
```c
elite_c_status_t elite_rtsi_client_is_connected(elite_rtsi_client_handle_t* handle, int32_t* out_connected);
elite_c_status_t elite_rtsi_client_is_started(elite_rtsi_client_handle_t* handle, int32_t* out_started);
elite_c_status_t elite_rtsi_client_is_read_available(elite_rtsi_client_handle_t* handle, int32_t* out_available);
```

- ***Description***

  Queries connection state, stream start state, and whether unread data is available.

- ***Parameters***

- `handle`: RTSI client handle.
- `out_connected`: Returns connection state; `1` means connected and `0` means disconnected.
- `out_started`: Returns stream state; `1` means started and `0` means not started.
- `out_available`: Returns whether unread data is available; `1` means available and `0` means unavailable.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Last RTSI Client Error
```c
const char* elite_rtsi_client_last_error_message(elite_rtsi_client_handle_t* handle);
```

- ***Description***

  Gets the most recent error message stored on the RTSI client handle.

- ***Parameters***

- `handle`: RTSI client handle.

- ***Returns***

  Returns the most recent error string for this handle.

## RTSI Recipe

### Get Recipe ID
```c
elite_c_status_t elite_rtsi_recipe_get_id(
    elite_rtsi_recipe_handle_t* recipe,
    int32_t* out_id
);
```

- ***Description***

  Gets the recipe ID.

- ***Parameters***

- `recipe`: Recipe handle.
- `out_id`: Returns the recipe ID.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Recipe CSV
```c
elite_c_status_t elite_rtsi_recipe_get_recipe_csv(
    elite_rtsi_recipe_handle_t* recipe,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***Description***

  Gets the field list string of the current recipe.

- ***Parameters***

- `recipe`: Recipe handle.
- `out_buffer`: Output buffer.
- `buffer_len`: Size of `out_buffer`.
- `out_required_len`: Returns the required string length including the trailing null terminator.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  `out_required_len` must not be `NULL`.
  If `out_buffer == NULL` or `buffer_len <= 0`, the function still succeeds and reports the full required length through `out_required_len`.

### Read Recipe Field Values
```c
elite_c_status_t elite_rtsi_recipe_get_value_double(elite_rtsi_recipe_handle_t* recipe, const char* name, double* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_get_value_int32(elite_rtsi_recipe_handle_t* recipe, const char* name, int32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_get_value_uint32(elite_rtsi_recipe_handle_t* recipe, const char* name, uint32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_get_value_bool(elite_rtsi_recipe_handle_t* recipe, const char* name, int32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_get_value_vector6d(elite_rtsi_recipe_handle_t* recipe, const char* name, double* out_value6, int32_t* out_success);
```

- ***Description***

  Reads a named field value from a recipe.

- ***Parameters***

- `recipe`: Recipe handle.
- `name`: Field name.
- `out_value`: Returns scalar values; boolean values are returned as `0/1`.
- `out_value6`: Returns a 6-element vector value.
- `out_success`: Returns whether the field read succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Write Recipe Field Values
```c
elite_c_status_t elite_rtsi_recipe_set_value_double(elite_rtsi_recipe_handle_t* recipe, const char* name, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_set_value_int32(elite_rtsi_recipe_handle_t* recipe, const char* name, int32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_set_value_uint32(elite_rtsi_recipe_handle_t* recipe, const char* name, uint32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_set_value_bool(elite_rtsi_recipe_handle_t* recipe, const char* name, int32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_recipe_set_value_vector6d(elite_rtsi_recipe_handle_t* recipe, const char* name, const double* value6, int32_t* out_success);
```

- ***Description***

  Writes a named field value into a recipe.

- ***Parameters***

- `recipe`: Recipe handle.
- `name`: Field name.
- `value`: Scalar value to write.
- `value6`: 6-element vector value to write.
- `out_success`: Returns whether the field write succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

## RTSI IO

### Create RTSI IO Handle
```c
elite_c_status_t elite_rtsi_io_create(
    const char* output_recipe_csv,
    const char* input_recipe_csv,
    double frequency,
    elite_rtsi_io_handle_t** out_handle
);
```

- ***Description***

  Creates an RTSI IO handle backed by input and output recipes.

- ***Parameters***

- `output_recipe_csv`: Comma-separated output recipe field list.
- `input_recipe_csv`: Comma-separated input recipe field list.
- `frequency`: RTSI communication frequency in Hz.
- `out_handle`: Returns the created RTSI IO handle.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  `output_recipe_csv` and `input_recipe_csv` may be `NULL`; they are treated as empty field lists in that case.

### Destroy RTSI IO Handle
```c
void elite_rtsi_io_destroy(elite_rtsi_io_handle_t* handle);
```

- ***Description***

  Releases an RTSI IO handle.

- ***Parameters***

- `handle`: RTSI IO handle to destroy.

- ***Returns***

  No return value.

### Connection and State
```c
elite_c_status_t elite_rtsi_io_connect(elite_rtsi_io_handle_t* handle, const char* ip, int32_t* out_success);
elite_c_status_t elite_rtsi_io_disconnect(elite_rtsi_io_handle_t* handle);
elite_c_status_t elite_rtsi_io_is_connected(elite_rtsi_io_handle_t* handle, int32_t* out_connected);
elite_c_status_t elite_rtsi_io_is_started(elite_rtsi_io_handle_t* handle, int32_t* out_started);
elite_c_status_t elite_rtsi_io_get_controller_version(elite_rtsi_io_handle_t* handle, elite_version_info_t* out_version);
```

- ***Description***

  Connects or disconnects RTSI IO, queries connection and stream state, and reads controller version information.

- ***Parameters***

- `handle`: RTSI IO handle.
- `ip`: Robot IP address.
- `out_success`: Returns whether connect succeeded; `1` means success and `0` means failure.
- `out_connected`: Returns connection state; `1` means connected and `0` means disconnected.
- `out_started`: Returns whether the internal sync thread is running; `1` means started and `0` means not started.
- `out_version`: Returns controller version information.

- ***Returns***

  Returns `elite_c_status_t`; `elite_rtsi_io_disconnect` has no additional output parameter.

### Set Realtime Outputs and Control Values
```c
elite_c_status_t elite_rtsi_io_set_speed_scaling(elite_rtsi_io_handle_t* handle, double scaling, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_standard_digital(elite_rtsi_io_handle_t* handle, int32_t index, int32_t level, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_configure_digital(elite_rtsi_io_handle_t* handle, int32_t index, int32_t level, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_analog_output_voltage(elite_rtsi_io_handle_t* handle, int32_t index, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_analog_output_current(elite_rtsi_io_handle_t* handle, int32_t index, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_external_force_torque(elite_rtsi_io_handle_t* handle, const double* value6, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_tool_digital_output(elite_rtsi_io_handle_t* handle, int32_t index, int32_t level, int32_t* out_success);
```

- ***Description***

  Sets speed scaling, standard digital outputs, configurable digital outputs, analog outputs, external wrench values, and tool digital outputs.

- ***Parameters***

- `handle`: RTSI IO handle.
- `scaling`: Speed scaling value, typically in the range `0.0` to `1.0`.
- `index`: Target IO channel index.
- `level`: Digital output level; `0` means low and non-zero means high.
- `value`: Analog output value.
- `value6`: 6-element external force/torque array.
- `out_success`: Returns whether the write succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Scalar and Vector Status Values
```c
elite_c_status_t elite_rtsi_io_get_timestamp(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_payload_mass(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_payload_cog(elite_rtsi_io_handle_t* handle, double* out_value3);
elite_c_status_t elite_rtsi_io_get_script_control_line(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_target_joint_positions(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_target_joint_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_positions(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_torques(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_current(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_temperatures(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_tcp_pose(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_tcp_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_tcp_force(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_target_tcp_pose(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_target_tcp_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_speed_scaling(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_target_speed_scaling(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_robot_voltage(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_robot_current(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_elbow_position(elite_rtsi_io_handle_t* handle, double* out_value3);
elite_c_status_t elite_rtsi_io_get_elbow_velocity(elite_rtsi_io_handle_t* handle, double* out_value3);
elite_c_status_t elite_rtsi_io_get_io_current(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_input(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_output(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_output_voltage(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_output_current(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_output_temperature(elite_rtsi_io_handle_t* handle, double* out_value);
```

- ***Description***

  Reads cached scalar and vector state values such as timestamp, payload, joints, TCP, speed scaling, electrical status, elbow state, and tool-side analog values.

- ***Parameters***

- `handle`: RTSI IO handle.
- `out_value`: Returns a scalar value.
- `out_value3`: Returns a 3-element vector value.
- `out_value6`: Returns a 6-element vector value.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Modes, Bitfields, and Digital Status
```c
elite_c_status_t elite_rtsi_io_get_digital_input_bits(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_digital_output_bits(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_robot_mode(elite_rtsi_io_handle_t* handle, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_joint_mode(elite_rtsi_io_handle_t* handle, int32_t* out_value6);
elite_c_status_t elite_rtsi_io_get_safety_status(elite_rtsi_io_handle_t* handle, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_robot_status(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_runtime_state(elite_rtsi_io_handle_t* handle, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_safety_status_bits(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_analog_io_types(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_mode(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_input_type(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_output_type(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_digital_mode(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_digital_output_mode(elite_rtsi_io_handle_t* handle, int32_t index, uint32_t* out_value);
```

- ***Description***

  Reads digital input and output bits, robot and safety modes, runtime state, status bitfields, and tool-related mode information.

- ***Parameters***

- `handle`: RTSI IO handle.
- `index`: Tool digital output channel index.
- `out_value`: Returns a scalar value or bit mask.
- `out_value6`: Returns a 6-element joint mode array.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Standard Analog Channels
```c
elite_c_status_t elite_rtsi_io_get_analog_input(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
elite_c_status_t elite_rtsi_io_get_analog_output(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
```

- ***Description***

  Gets the current value of a specified standard analog input or output channel.

- ***Parameters***

- `handle`: RTSI IO handle.
- `index`: Analog channel index.
- `out_value`: Returns the channel value.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Boolean and Numeric Registers
```c
elite_c_status_t elite_rtsi_io_get_out_bool_registers0_to_31(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_out_bool_registers32_to_63(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_bool_registers0_to_31(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_bool_registers32_to_63(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_bool_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_out_bool_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_int_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_out_int_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_double_register(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
elite_c_status_t elite_rtsi_io_get_out_double_register(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
```

- ***Description***

  Gets packed boolean register bitfields and indexed boolean, integer, and double register values.

- ***Parameters***

- `handle`: RTSI IO handle.
- `index`: Register index.
- `out_value`: Returns the bitfield value, scalar register value, or boolean value as `0/1`.

- ***Returns***

  Returns `elite_c_status_t`.

### Read Output Recipe Field Values
```c
elite_c_status_t elite_rtsi_io_get_recipe_value_double(elite_rtsi_io_handle_t* handle, const char* name, double* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_int32(elite_rtsi_io_handle_t* handle, const char* name, int32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_uint32(elite_rtsi_io_handle_t* handle, const char* name, uint32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_bool(elite_rtsi_io_handle_t* handle, const char* name, int32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_vector3d(elite_rtsi_io_handle_t* handle, const char* name, double* out_value3, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_vector6d(elite_rtsi_io_handle_t* handle, const char* name, double* out_value6, int32_t* out_success);
```

- ***Description***

  Reads a named field value from the output recipe maintained by RTSI IO.

- ***Parameters***

- `handle`: RTSI IO handle.
- `name`: Field name.
- `out_value`: Returns scalar values; booleans are returned as `0/1`.
- `out_value3`: Returns a 3-element vector value.
- `out_value6`: Returns a 6-element vector value.
- `out_success`: Returns whether the field read succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Write Input Recipe Field Values
```c
elite_c_status_t elite_rtsi_io_set_input_recipe_value_double(elite_rtsi_io_handle_t* handle, const char* name, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_int32(elite_rtsi_io_handle_t* handle, const char* name, int32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_uint32(elite_rtsi_io_handle_t* handle, const char* name, uint32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_bool(elite_rtsi_io_handle_t* handle, const char* name, int32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_vector6d(elite_rtsi_io_handle_t* handle, const char* name, const double* value6, int32_t* out_success);
```

- ***Description***

  Writes a named field value into the input recipe used by RTSI IO.

- ***Parameters***

- `handle`: RTSI IO handle.
- `name`: Field name.
- `value`: Scalar value to write; booleans use `0/1`.
- `value6`: 6-element vector value to write.
- `out_success`: Returns whether the field write succeeded; `1` means success and `0` means failure.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Last RTSI IO Error
```c
const char* elite_rtsi_io_last_error_message(elite_rtsi_io_handle_t* handle);
```

- ***Description***

  Gets the most recent error message stored on the RTSI IO handle.

- ***Parameters***

- `handle`: RTSI IO handle.

- ***Returns***

  Returns the most recent error string for this handle.

# SerialCommunication Module

## Overview

The robot provides RS485 communication interfaces on the tool side and controller side, and the SDK exposes corresponding APIs for them.

Internally, SDK serial communication works by forwarding RS485 data through a TCP port, and the SDK reads and writes that TCP channel directly.

## Header

```c
#include <Elite/SerialCommunication_C.hpp>
```

## Configuration Type

```c
typedef struct elite_serial_config_t {
    elite_serial_baud_rate_t baud_rate;
    elite_serial_parity_t parity;
    elite_serial_stop_bits_t stop_bits;
} elite_serial_config_t;
```

- `baud_rate`: Serial baud rate. See `elite_serial_baud_rate_t` in [CommonTypes.md](./CommonTypes.md).
- `parity`: Serial parity mode. See `elite_serial_parity_t` in [CommonTypes.md](./CommonTypes.md).
- `stop_bits`: Serial stop bit configuration. See `elite_serial_stop_bits_t` in [CommonTypes.md](./CommonTypes.md).

## API Reference

### Connect Serial Channel
```c
elite_c_status_t elite_serial_comm_connect(
    elite_serial_comm_handle_t* comm,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***Description***

  Connects the serial communication channel.

- ***Parameters***

- `comm`: Serial communication handle.
- `timeout_ms`: Connection timeout in milliseconds.
- `out_success`: Returns whether the connection succeeded.

- ***Returns***

  Returns `elite_c_status_t`.

### Disconnect Serial Channel
```c
elite_c_status_t elite_serial_comm_disconnect(elite_serial_comm_handle_t* comm);
```

- ***Description***

  Disconnects the current serial connection.

- ***Parameters***

- `comm`: Serial communication handle.

- ***Returns***

  Returns `elite_c_status_t`.

### Check Connection State
```c
elite_c_status_t elite_serial_comm_is_connected(
    elite_serial_comm_handle_t* comm,
    int32_t* out_connected
);
```

- ***Description***

  Checks whether the serial channel is currently connected.

- ***Parameters***

- `comm`: Serial communication handle.
- `out_connected`: Returns connection state; `1` means connected and `0` means disconnected.

- ***Returns***

  Returns `elite_c_status_t`.

### Get Socat Process ID
```c
elite_c_status_t elite_serial_comm_get_socat_pid(
    elite_serial_comm_handle_t* comm,
    int32_t* out_pid
);
```

- ***Description***

  Gets the associated `socat` process ID used by the serial forwarding channel.

- ***Parameters***

- `comm`: Serial communication handle.
- `out_pid`: Returns the associated process ID.

- ***Returns***

  Returns `elite_c_status_t`.

### Write Serial Data
```c
elite_c_status_t elite_serial_comm_write(
    elite_serial_comm_handle_t* comm,
    const uint8_t* data,
    int32_t size,
    int32_t* out_written
);
```

- ***Description***

  Writes binary data to the serial channel.

- ***Parameters***

- `comm`: Serial communication handle.
- `data`: Buffer containing the bytes to write.
- `size`: Number of bytes requested to write.
- `out_written`: Returns the actual number of bytes written.

- ***Returns***

  Returns `elite_c_status_t`.

### Read Serial Data
```c
elite_c_status_t elite_serial_comm_read(
    elite_serial_comm_handle_t* comm,
    uint8_t* out_data,
    int32_t size,
    int32_t timeout_ms,
    int32_t* out_read
);
```

- ***Description***

  Reads binary data from the serial channel.

- ***Parameters***

- `comm`: Serial communication handle.
- `out_data`: Output buffer for received bytes.
- `size`: Maximum number of bytes to read.
- `timeout_ms`: Read timeout in milliseconds.
- `out_read`: Returns the actual number of bytes read.

- ***Returns***

  Returns `elite_c_status_t`.

### Destroy Serial Handle
```c
void elite_serial_comm_destroy(elite_serial_comm_handle_t* comm);
```

- ***Description***

  Destroys the serial communication handle.

- ***Parameters***

- `comm`: Serial communication handle to destroy.

- ***Returns***

  No return value.

- ***Notes***

  This handle is typically created by `elite_driver_start_tool_rs485()` or `elite_driver_start_board_rs485()`.

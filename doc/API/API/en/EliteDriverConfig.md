# EliteDriverConfig Module

## Overview

This document describes the `elite_driver_config_t` configuration structure and its default initialization helper.

## Header

```c
#include <Elite/EliteDriver_C.h>
```

## Configuration Structure

```c
typedef struct elite_driver_config_t {
    const char* robot_ip;
    const char* script_file_path;
    const char* local_ip;

    int32_t headless_mode;
    int32_t script_sender_port;
    int32_t reverse_port;
    int32_t trajectory_port;
    int32_t script_command_port;

    float servoj_time;
    float servoj_lookahead_time;
    int32_t servoj_gain;
    float stopj_acc;
} elite_driver_config_t;
```

## Field Description

- `robot_ip`: Robot IP address.
- `script_file_path`: External control script file path.
- `local_ip`: Local machine IP address.
- `headless_mode`: Whether headless mode is enabled.
- `script_sender_port`: Script sender port.
- `reverse_port`: Reverse communication port.
- `trajectory_port`: Trajectory communication port.
- `script_command_port`: Script command port.
- `servoj_time`: `servoj` control cycle parameter.
- `servoj_lookahead_time`: `servoj` lookahead time.
- `servoj_gain`: `servoj` servo gain.
- `stopj_acc`: `stopj` deceleration in rad/s^2.

## Default Values

After calling `elite_driver_config_set_default()`, the fields are initialized as follows:

- `robot_ip = NULL`
- `script_file_path = NULL`
- `local_ip = ""`
- `headless_mode = 0`
- `script_sender_port = 50002`
- `reverse_port = 50001`
- `trajectory_port = 50003`
- `script_command_port = 50004`
- `servoj_time = 0.008f`
- `servoj_lookahead_time = 0.1f`
- `servoj_gain = 300`
- `stopj_acc = 8.0f`

## API Reference

### Set Default Configuration
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

  It is recommended to call this function before manually overriding individual fields.
  Before calling `elite_driver_create()`, both `robot_ip` and `script_file_path` must be set to non-`NULL` values.

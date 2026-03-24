# Elite Robots CS SDK C Wrapper

[中文说明](./README_CN.md)

`Elite_Robots_CS_SDK_C` is a C ABI wrapper for the Elite Robots CS Series SDK.
It exposes a stable C interface on top of the upstream C++ SDK so that C projects
and foreign-function interfaces can integrate with Elite CS controllers more easily.

## Overview

This repository builds a shared library named `elite_cs_series_sdk_c` and links it
against the upstream `elite-cs-series-sdk` shared library.

The wrapper currently covers these areas:

- `Dashboard`: controller management, power control, program/task operations, status queries
- `EliteDriver`: motion control, script execution, payload/tool settings, force mode, RS485 helpers
- `RTSI`: real-time data exchange, recipes, IO access, registers
- `Primary`: primary port access, script sending, kinematics info, exception callbacks
- `Utility APIs`: version parsing/comparison, logging, controller log access

The public umbrella header is [`include/Elite_Sdk_C.h`](./include/Elite_Sdk_C.h).

## Repository Layout

```text
include/
  Elite_Sdk_C.h              Main public header
  Elite_C_Types.h            Shared C types and status codes
  Dashboard/                 Dashboard client C APIs
  Elite/                     Driver, serial, log, version APIs
  Primary/                   Primary port APIs
  Rtsi/                      RTSI client and IO APIs
source/
  ...                        C wrapper implementations
CMakeLists.txt               Build definition
```

## Requirements

- CMake 3.16 or newer
- A C++17-capable toolchain
- The upstream `elite-cs-series-sdk` library and headers

## Build

### Option 1: Use a locally installed upstream SDK

Point `CMAKE_PREFIX_PATH` to a location where `elite-cs-series-sdk` exports
`elite-cs-series-sdk::shared`.

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/elite-cs-sdk
cmake --build build
```

### Option 2: Auto-fetch the upstream SDK

If the upstream SDK is not installed locally, CMake can fetch it from GitHub or Gitee.

```bash
cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON
cmake --build build
```

You can override the upstream repository with `ELITE_CS_SDK_REPO` if needed.

## Install

Installation is enabled when `ELITE_INSTALL` is set.

```bash
cmake -S . -B build \
  -DELITE_INSTALL=ON \
  -DCMAKE_INSTALL_PREFIX=/your/install/prefix \
  -DCMAKE_PREFIX_PATH=/path/to/elite-cs-sdk
cmake --build build
cmake --install build
```

## Minimal Example

```c
#include <Elite_Sdk_C.h>
#include <stdio.h>

int main(void) {
    elite_dashboard_handle_t* dashboard = NULL;
    int32_t ok = 0;

    if (elite_dashboard_create(&dashboard) != ELITE_C_STATUS_OK) {
        fprintf(stderr, "failed to create dashboard handle\n");
        return 1;
    }

    if (elite_dashboard_connect(dashboard, "192.168.1.200", 29999, &ok) != ELITE_C_STATUS_OK || !ok) {
        fprintf(stderr, "connect failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    if (elite_dashboard_robot_mode(dashboard, &ok) == ELITE_C_STATUS_OK) {
        printf("robot mode: %d\n", ok);
    }

    elite_dashboard_disconnect(dashboard);
    elite_dashboard_destroy(dashboard);
    return 0;
}
```

## Error Handling

Most APIs return `elite_c_status_t`:

- `ELITE_C_STATUS_OK`
- `ELITE_C_STATUS_INVALID_ARGUMENT`
- `ELITE_C_STATUS_ALLOCATION_FAILED`
- `ELITE_C_STATUS_EXCEPTION`

Many modules also expose `*_last_error_message(...)` helpers to retrieve a readable
error message after a failure.

## Notes

- The API is designed as a C interface, but some public headers use `.hpp` filenames.
- This repository is a wrapper layer, not a standalone robot SDK.
- Feature availability ultimately depends on the upstream SDK and controller firmware.

## License

See the license information in this repository and in the upstream Elite Robots SDK.

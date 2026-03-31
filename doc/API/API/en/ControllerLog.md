# ControllerLog Module

## Overview

The ControllerLog module provides the ability to download system logs from the robot controller.

## Header

```c
#include <Elite/ControllerLog_C.hpp>
```

## API Reference

### Download System Log
```c
elite_c_status_t elite_controller_log_download_system_log(
    const char* robot_ip,
    const char* password,
    const char* path,
    elite_controller_log_progress_cb_t cb,
    void* user_data,
    int32_t* out_success
);
```

- ***Description***

  Downloads controller system logs to a local path.

- ***Parameters***

- `robot_ip`: Robot IP address.
- `password`: Controller SSH password.
- `path`: Local destination path for the log file.
- `cb`: Download progress callback.
- `user_data`: Opaque user pointer passed back to the callback.
- `out_success`: Returns whether the download succeeded.

- ***Callback Parameters***

- `file_size`: Total log file size in bytes.
- `recv_size`: Current downloaded size in bytes.
- `err`: Error message string; typically `NULL` when no error occurred.
- `user_data`: Opaque user pointer passed through unchanged.

- ***Returns***

  Returns `elite_c_status_t`.

- ***Notes***

  1. On Linux, if `libssh` is not installed, the machine running the SDK must provide `scp`, `ssh`, and `sshpass`.
  2. On Windows, this API is unavailable if `libssh` is not installed.

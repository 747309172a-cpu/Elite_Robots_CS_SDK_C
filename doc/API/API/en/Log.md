# Log Module

## Overview

The Log module provides SDK logging configuration, log callback registration, and message output helpers.

## Header

```c
#include <Elite/Log_C.hpp>
```

## Callback and Types

### Log Level Enum
```c
typedef enum elite_log_level_t {
    ELITE_LOG_LEVEL_DEBUG = 0,
    ELITE_LOG_LEVEL_INFO = 1,
    ELITE_LOG_LEVEL_WARN = 2,
    ELITE_LOG_LEVEL_ERROR = 3,
    ELITE_LOG_LEVEL_FATAL = 4,
    ELITE_LOG_LEVEL_NONE = 5
} elite_log_level_t;
```

- ***Description***

  Defines the supported SDK log levels.

### Log Handler Callback
```c
typedef void (*elite_log_handler_cb_t)(
    const char* file,
    int32_t line,
    int32_t loglevel,
    const char* log,
    void* user_data
);
```

- ***Description***

  Callback type used to receive SDK log messages.

- ***Callback Parameters***

- `file`: Source file path of the log message.
- `line`: Source line number of the log message.
- `loglevel`: Log level value, corresponding to `elite_log_level_t`.
- `log`: Log text.
- `user_data`: Opaque user pointer passed during callback registration.

## API Reference

### Register Log Callback
```c
elite_c_status_t elite_register_log_handler(elite_log_handler_cb_t cb, void* user_data);
```

- ***Description***

  Registers an SDK log callback handler.

- ***Parameters***

- `cb`: Log callback function.
- `user_data`: Opaque user pointer.

- ***Returns***

  Returns `elite_c_status_t`.

### Unregister Log Handler
```c
void elite_unregister_log_handler();
```

- ***Description***

  Unregisters the current log handler and restores the default log handler.

- ***Returns***

  No return value.

### Set Log Level
```c
void elite_set_log_level(elite_log_level_t level);
```

- ***Description***

  Sets the SDK output log level.

- ***Parameters***

- `level`: Log level. Supported values are `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`, and `NONE`.

- ***Returns***

  No return value.

### Log Message with Explicit Level
```c
void elite_log_message(const char* file, int32_t line, elite_log_level_t level, const char* message);
```

- ***Description***

  Emits one log message at the specified log level.

- ***Parameters***

- `file`: Source file name, usually `__FILE__`.
- `line`: Source line number, usually `__LINE__`.
- `level`: Log level.
- `message`: Log text.

- ***Returns***

  No return value.

### Convenience Log Helpers
```c
void elite_log_debug_message(const char* file, int32_t line, const char* message);
void elite_log_info_message(const char* file, int32_t line, const char* message);
void elite_log_warn_message(const char* file, int32_t line, const char* message);
void elite_log_error_message(const char* file, int32_t line, const char* message);
void elite_log_fatal_message(const char* file, int32_t line, const char* message);
void elite_log_none_message(const char* file, int32_t line, const char* message);
```

- ***Description***

  Emits log messages at fixed levels: `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`, or `NONE`.

- ***Parameters***

- `file`: Source file name, usually `__FILE__`.
- `line`: Source line number, usually `__LINE__`.
- `message`: Log text.

- ***Returns***

  No return value.

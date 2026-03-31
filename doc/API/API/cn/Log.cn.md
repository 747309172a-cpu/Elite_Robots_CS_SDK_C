# Log 模块

## 简介

Log模块提供了Elite_Robots_CS_SDK中的日志功能相关设置，包括日志级别定义、日志处理器接口和日志输出功能。

## 导入

```c
#include <Elite/Log_C.hpp>
```

## 接口说明

### 注册日志回调
```c
elite_c_status_t elite_register_log_handler(elite_log_handler_cb_t cb, void* user_data);
```

- ***功能***

  注册 SDK 日志处理回调。

- ***参数***

- `cb`：日志回调函数。
- `user_data`：用户透传指针。

- ***返回值***

  返回 `elite_c_status_t`。

### 注销日志处理器
```c
void elite_unregister_log_handler();
```

- ***功能***

  注销当前日志处理器，将启用默认日志处理器

- ***返回值***

  无返回值。

### 设置日志等级
```c
void elite_set_log_level(elite_log_level_t level);
```

- ***功能***

  设置 SDK 输出日志等级。

- ***参数***

- `level`：日志等级，支持 `DEBUG/INFO/WARN/ERROR/FATAL/NONE`。

- ***返回值***

  无返回值。

### 输出指定级别日志
```c
void elite_log_message(const char* file, int32_t line, elite_log_level_t level, const char* message);
```

- ***功能***

  按指定日志等级输出一条日志消息。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `level`：日志等级。
- `message`：日志文本。

- ***返回值***

  无返回值。

### 输出调试日志
```c
void elite_log_debug_message(const char* file, int32_t line, const char* message);
```

- ***功能***

  以 `DEBUG` 等级输出日志。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `message`：日志文本。

- ***返回值***

  无返回值。

### 输出信息日志
```c
void elite_log_info_message(const char* file, int32_t line, const char* message);
```

- ***功能***

  以 `INFO` 等级输出日志。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `message`：日志文本。

- ***返回值***

  无返回值。

### 输出警告日志
```c
void elite_log_warn_message(const char* file, int32_t line, const char* message);
```

- ***功能***

  以 `WARN` 等级输出日志。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `message`：日志文本。

- ***返回值***

  无返回值。

### 输出错误日志
```c
void elite_log_error_message(const char* file, int32_t line, const char* message);
```

- ***功能***

  以 `ERROR` 等级输出日志。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `message`：日志文本。

- ***返回值***

  无返回值。

### 输出致命日志
```c
void elite_log_fatal_message(const char* file, int32_t line, const char* message);
```

- ***功能***

  以 `FATAL` 等级输出日志。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `message`：日志文本。

- ***返回值***

  无返回值。

### 输出空级别日志
```c
void elite_log_none_message(const char* file, int32_t line, const char* message);
```

- ***功能***

  以 `NONE` 等级输出日志。

- ***参数***

- `file`：日志所属文件名，建议传 `__FILE__`。
- `line`：日志所属代码行号，建议传 `__LINE__`。
- `message`：日志文本。

- ***返回值***

  无返回值。

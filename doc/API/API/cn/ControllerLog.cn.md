# ControllerLog 模块

## 简介

ControllerLog 模块提供从机器人控制器下载系统日志的能力。

## 导入

```c
#include <Elite/ControllerLog_C.hpp>
```

## 接口说明

### 下载系统日志
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

- ***功能***

  从机器人控制器下载系统日志到本地路径。

- ***参数***

- `robot_ip`：机器人 IP 地址。
- `password`：机器人 SSH 密码。
- `path`：日志保存路径。
- `cb`：下载进度回调函数。
- `user_data`：用户自定义上下文指针，会原样传回回调。
- `out_success`：下载是否成功。

- ***回调函数参数***

- `file_size`：日志文件总大小，单位字节。
- `recv_size`：当前已下载大小，单位字节。
- `err`：错误信息，无错误时通常为 `NULL`。
- `user_data`：用户透传指针。

- ***返回值***

  返回 `elite_c_status_t`，表示接口调用结果。

- ***注意事项***


  1. 在Linux系统下，如果未安装`libssh`，需要确保运行SDK的计算机具有`scp`、`ssh`和`sshpass`命令可用
  2. 在Windows系统下，如果未安装libssh，则此接口不可用

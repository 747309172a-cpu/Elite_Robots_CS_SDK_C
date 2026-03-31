# Dashboard 模块

## 简介

DashboardClient类提供了与机器人仪表盘服务器的交互接口，用于执行各种机器人控制命令和状态查询。

## 导入

```c
#include <Dashboard/DashboardClient_C.hpp>
```

## 接口说明

### 创建句柄
```c
elite_c_status_t elite_dashboard_create(elite_dashboard_handle_t** out_handle);
```

- ***功能***

  创建 Dashboard 客户端句柄。

- ***参数***

- `out_handle`：返回创建出的 Dashboard 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 销毁句柄
```c
void elite_dashboard_destroy(elite_dashboard_handle_t* handle);
```

- ***功能***

  释放 Dashboard 客户端句柄。

- ***参数***

- `handle`：需要销毁的 Dashboard 句柄。

- ***返回值***

  无返回值。

### 连接服务器
```c
elite_c_status_t elite_dashboard_connect(
    elite_dashboard_handle_t* handle,
    const char* ip,
    int32_t port,
    int32_t* out_success
);
```

- ***功能***

  连接仪表盘服务器

- ***参数***

- `handle`：Dashboard 句柄。
- `ip`：机器人 IP 地址。
- `port`：Dashboard 端口，通常使用 `29999`，需由调用方显式传入。
- `out_success`：连接是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 断开连接
```c
elite_c_status_t elite_dashboard_disconnect(elite_dashboard_handle_t* handle);
```

- ***功能***

  断开当前 Dashboard 连接。

- ***参数***

- `handle`：Dashboard 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 连通性检测
```c
elite_c_status_t elite_dashboard_echo(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  检查与仪表盘shell服务器的连接状态

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回连通性检测是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 控制器上电
```c
elite_c_status_t elite_dashboard_power_on(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  控制机器人控制器上电。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回上电操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 控制器下电
```c
elite_c_status_t elite_dashboard_power_off(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  控制机器人控制器下电。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回下电操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 释放刹车
```c
elite_c_status_t elite_dashboard_brake_release(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  释放机器人抱闸。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 关闭安全弹窗
```c
elite_c_status_t elite_dashboard_close_safety_dialog(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  关闭控制器上的安全消息弹窗。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 解锁保护性停止
```c
elite_c_status_t elite_dashboard_unlock_protective_stop(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  解锁控制器的保护性停止状态。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 重启安全系统
```c
elite_c_status_t elite_dashboard_safety_system_restart(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  重启控制器安全系统。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 写入控制器日志
```c
elite_c_status_t elite_dashboard_log(
    elite_dashboard_handle_t* handle,
    const char* message,
    int32_t* out_success
);
```

- ***功能***

  向控制器日志系统写入一条日志消息。

- ***参数***

- `handle`：Dashboard 句柄。
- `message`：日志内容，必须为非 `NULL`。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 弹出提示框
```c
elite_c_status_t elite_dashboard_popup(
    elite_dashboard_handle_t* handle,
    const char* arg,
    const char* message,
    int32_t* out_success
);
```

- ***功能***

  在控制器侧执行操作或显示交互信息。

- ***参数***

- `handle`：Dashboard 句柄。
- `arg`：弹窗附加参数，必须为非 `NULL`。
- `message`：弹窗消息内容；传 `NULL` 时按空字符串处理。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 退出程序
```c
elite_c_status_t elite_dashboard_quit(elite_dashboard_handle_t* handle);
```

### 重启控制器
```c
elite_c_status_t elite_dashboard_reboot(elite_dashboard_handle_t* handle);
```

### 关闭控制器
```c
elite_c_status_t elite_dashboard_shutdown(elite_dashboard_handle_t* handle);
```

- ***功能***

  对控制器执行退出、重启或关机操作。

- ***参数***

- `handle`：Dashboard 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取机器人模式
```c
elite_c_status_t elite_dashboard_robot_mode(elite_dashboard_handle_t* handle, int32_t* out_mode);
```

- ***功能***

  获取机器人当前模式。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_mode`：返回机器人模式，取值可按 `elite_robot_mode_t` 解释，见 [CommonTypes.cn.md](./CommonTypes.cn.md)。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取安全模式
```c
elite_c_status_t elite_dashboard_safety_mode(elite_dashboard_handle_t* handle, int32_t* out_mode);
```

- ***功能***

  获取机器人当前安全模式。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_mode`：返回安全模式，取值可按 `elite_safety_mode_t` 解释，见 [CommonTypes.cn.md](/home/dukang/Elite_Robots_CS_SDK_C/doc/API/API/cn/CommonTypes.cn.md)。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取运行状态
```c
elite_c_status_t elite_dashboard_running_status(elite_dashboard_handle_t* handle, int32_t* out_status);
```

- ***功能***

  获取控制器当前运行状态。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_status`：返回运行状态值。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取任务状态
```c
elite_c_status_t elite_dashboard_get_task_status(elite_dashboard_handle_t* handle, int32_t* out_status);
```

- ***功能***

  获取当前任务状态。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_status`：返回任务状态，取值可按 `elite_task_status_t` 解释，见 [CommonTypes.cn.md](/home/dukang/Elite_Robots_CS_SDK_C/doc/API/API/cn/CommonTypes.cn.md)。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取速度比例
```c
elite_c_status_t elite_dashboard_speed_scaling(elite_dashboard_handle_t* handle, int32_t* out_scaling);
```

- ***功能***

  获取机器人速度比例百分比。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_scaling`：返回速度比例百分比。

- ***返回值***

  返回 `elite_c_status_t`。

### 设置速度比例
```c
elite_c_status_t elite_dashboard_set_speed_scaling(
    elite_dashboard_handle_t* handle,
    int32_t scaling,
    int32_t* out_success
);
```

- ***功能***

  设置机器人速度比例百分比。

- ***参数***

- `handle`：Dashboard 句柄。
- `scaling`：需要设置的速度比例百分比。
- `out_success`：返回设置是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断任务是否运行中
```c
elite_c_status_t elite_dashboard_task_is_running(elite_dashboard_handle_t* handle, int32_t* out_running);
```

- ***功能***

  判断当前任务是否正在运行。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_running`：返回任务是否运行中，`1` 表示运行中，`0` 表示未运行。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断任务是否已保存
```c
elite_c_status_t elite_dashboard_is_task_saved(elite_dashboard_handle_t* handle, int32_t* out_saved);
```

- ***功能***

  判断当前任务是否已经保存。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_saved`：返回任务是否已保存，`1` 表示已保存，`0` 表示未保存。

- ***返回值***

  返回 `elite_c_status_t`。

### 运行程序
```c
elite_c_status_t elite_dashboard_play_program(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  启动当前已加载程序。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回运行是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 暂停程序
```c
elite_c_status_t elite_dashboard_pause_program(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  暂停当前运行中的程序。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回暂停是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 停止程序
```c
elite_c_status_t elite_dashboard_stop_program(elite_dashboard_handle_t* handle, int32_t* out_success);
```

- ***功能***

  停止当前运行中的程序。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_success`：返回停止是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 加载配置
```c
elite_c_status_t elite_dashboard_load_configuration(
    elite_dashboard_handle_t* handle,
    const char* path,
    int32_t* out_success
);
```

- ***功能***

  加载指定的控制器配置文件。

- ***参数***

- `handle`：Dashboard 句柄。
- `path`：控制器中的配置文件路径。
- `out_success`：返回加载是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 加载任务
```c
elite_c_status_t elite_dashboard_load_task(
    elite_dashboard_handle_t* handle,
    const char* path,
    int32_t* out_success
);
```

- ***功能***

  加载指定任务文件。

- ***参数***

- `handle`：Dashboard 句柄。
- `path`：控制器中的任务文件路径。
- `out_success`：返回加载是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断配置是否被修改
```c
elite_c_status_t elite_dashboard_is_configuration_modify(
    elite_dashboard_handle_t* handle,
    int32_t* out_modified
);
```

- ***功能***

  查询当前配置是否已被修改但未保存。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_modified`：返回配置是否被修改。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取控制器版本
```c
elite_c_status_t elite_dashboard_version(
    elite_dashboard_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***注意事项***

  本页所有字符串输出接口都要求 `out_required_len` 非空。
  如果 `out_buffer == NULL` 或 `buffer_len <= 0`，接口仍会返回成功，并通过 `out_required_len` 告知完整长度；若缓冲区不足，输出会被截断并自动补 `\0`。

- ***功能***

  获取控制器版本字符串。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取命令帮助
```c
elite_c_status_t elite_dashboard_help(
    elite_dashboard_handle_t* handle,
    const char* cmd,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取指定 Dashboard 命令的帮助信息。

- ***参数***

- `handle`：Dashboard 句柄。
- `cmd`：命令字符串。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取命令用法
```c
elite_c_status_t elite_dashboard_usage(
    elite_dashboard_handle_t* handle,
    const char* cmd,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取指定 Dashboard 命令的用法说明。

- ***参数***

- `handle`：Dashboard 句柄。
- `cmd`：命令字符串。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取机器人型号
```c
elite_c_status_t elite_dashboard_robot_type(
    elite_dashboard_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取机器人型号信息。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取机器人序列号
```c
elite_c_status_t elite_dashboard_robot_serial_number(
    elite_dashboard_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取机器人序列号。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取机器人 ID
```c
elite_c_status_t elite_dashboard_robot_id(
    elite_dashboard_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取机器人 ID。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取配置路径
```c
elite_c_status_t elite_dashboard_configuration_path(
    elite_dashboard_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取当前配置文件路径。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取任务路径
```c
elite_c_status_t elite_dashboard_get_task_path(
    elite_dashboard_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取当前任务文件路径。

- ***参数***

- `handle`：Dashboard 句柄。
- `out_buffer`：输出字符串缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：返回实际所需缓冲区长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 发送原始命令并接收结果
```c
elite_c_status_t elite_dashboard_send_and_receive(
    elite_dashboard_handle_t* handle,
    const char* cmd,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  发送原始 Dashboard 命令并获取返回结果。

- ***参数***

- `handle`：Dashboard 句柄。
- `cmd`：Dashboard 命令字符串。
- `out_buffer`：输出结果缓冲区。
- `buffer_len`：输出缓冲区长度。
- `out_required_len`：结果所需长度。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取最近错误信息
```c
const char* elite_dashboard_last_error_message(elite_dashboard_handle_t* handle);
```

- ***功能***

  获取该 Dashboard 句柄最近一次记录的错误信息。

- ***参数***

- `handle`：Dashboard 句柄；传 `NULL` 时返回当前线程的全局错误字符串。

- ***返回值***

  返回最近一次记录的错误字符串。

### 获取全局最近错误信息
```c
const char* elite_dashboard_global_last_error_message(void);
```

- ***功能***

  获取最近一次 Dashboard 调用失败时记录的全局错误信息。

- ***返回值***

  返回全局错误字符串。

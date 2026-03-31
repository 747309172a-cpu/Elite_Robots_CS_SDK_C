# EliteDriverConfig 模块

## 简介

本文档说明 `elite_driver_config_t` 配置结构体及其默认初始化接口。

## 导入

```c
#include <Elite/EliteDriver_C.h>
```

## 配置结构体

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

## 字段说明

- `robot_ip`：机器人 IP 地址。
- `script_file_path`：外部控制脚本文件路径。
- `local_ip`：本机 IP 地址。
- `headless_mode`：是否启用无头模式。
- `script_sender_port`：脚本发送端口。
- `reverse_port`：反向通信端口。
- `trajectory_port`：轨迹通信端口。
- `script_command_port`：脚本命令端口。
- `servoj_time`：`servoj` 控制周期参数。
- `servoj_lookahead_time`：`servoj` 前瞻时间。
- `servoj_gain`：`servoj` 伺服增益。
- `stopj_acc`：`stopj` ：停止运动的加速度 (rad/s²)。

## 默认值

调用 `elite_driver_config_set_default()` 后，各字段默认值如下：

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
## 接口说明

### 设置默认配置
```c
void elite_driver_config_set_default(elite_driver_config_t* config);
```

- ***功能***

  使用 SDK 默认参数初始化 `elite_driver_config_t`。

- ***注意事项***

  建议在手动修改配置项前先调用该接口。
  调用 `elite_driver_create()` 前，`robot_ip` 和 `script_file_path` 必须被设置为非 `NULL`。

- ***参数***

- `config`：需要初始化的驱动配置结构体。

- ***返回值***

  无返回值。

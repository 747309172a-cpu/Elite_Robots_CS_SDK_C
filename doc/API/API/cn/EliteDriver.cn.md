# EliteDriver 模块

## 简介

EliteDriver 是用于与机器人进行数据交互的主要类。它负责建立所有必要的套接字连接，并处理与机器人的数据交换。EliteDriver  会向机器人发送控制脚本，机器人在运行控制脚本后，会和 EliteDriver 建立通讯，接收运动数据，并且必要时会发送运动结果。

## 导入

```c
#include <Elite/EliteDriver_C.h>
```

## 接口说明

## 回调与数据类型

### 轨迹结果回调类型
```c
typedef void (*elite_driver_trajectory_result_cb_t)(
    elite_trajectory_motion_result_t result,
    void* user_data
);
```

- ***功能***

  轨迹运动完成后，SDK 会通过该回调通知调用方轨迹执行结果。

- ***参数***

- `result`：轨迹执行结果，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_trajectory_motion_result_t`。
- `user_data`：注册回调时传入的用户透传指针。

### 机器人异常结构体
```c
typedef struct elite_driver_robot_exception_t {
    int32_t type;
    uint64_t timestamp;
    int32_t error_code;
    int32_t sub_error_code;
    int32_t error_source;
    int32_t error_level;
    int32_t error_data_type;
    uint32_t data_u32;
    int32_t data_i32;
    float data_f32;
    int32_t line;
    int32_t column;
    const char* message;
} elite_driver_robot_exception_t;
```

- ***功能***

  用于承载机器人异常回调中的异常信息。

- ***字段说明***

- `type`：异常类型。
- `timestamp`：异常时间戳。
- `error_code` / `sub_error_code`：错误码及子错误码。
- `error_source`：错误来源。
- `error_level`：错误等级。
- `error_data_type`：附加数据类型。
- `data_u32` / `data_i32` / `data_f32`：异常附加数据。
- `line` / `column`：脚本异常对应的行列号。
- `message`：异常描述字符串。

- ***注意事项***

  `message` 指针仅在回调执行期间有效；如果调用方需要长期保存，应自行拷贝字符串内容。

### 机器人异常回调类型
```c
typedef void (*elite_driver_robot_exception_cb_t)(
    const elite_driver_robot_exception_t* ex,
    void* user_data
);
```

- ***功能***

  当驱动从机器人收到异常信息时，会调用该回调。

- ***参数***

- `ex`：异常数据结构体指针。
- `user_data`：注册回调时传入的用户透传指针。

### 设置默认驱动配置
```c
void elite_driver_config_set_default(elite_driver_config_t* config);
```

- ***功能***

  使用 SDK 默认参数初始化 `elite_driver_config_t`。

- ***参数***

- `config`：需要初始化的驱动配置结构体。

- ***返回值***

  无返回值。

- ***注意事项***

  建议在手动设置 `robot_ip`、`script_file_path`、端口和运动参数前先调用该接口。
  默认值见 [EliteDriverConfig.cn.md](./EliteDriverConfig.cn.md)。

### 创建驱动句柄
```c
elite_c_status_t elite_driver_create(
    const elite_driver_config_t* config,
    elite_driver_handle_t** out_handle
);
```

- ***功能***

  创建 EliteDriver 驱动句柄，并根据配置初始化与机器人通信所需的内部资源。该接口会基于配置中的机器人ip、脚本路径、本机ip及端口参数创建底层驱动对象。
  
- ***参数***

- `config`：驱动配置结构体，具体字段见 `EliteDriverConfig` 文档。
- `out_handle`：返回创建出的驱动句柄。

- ***返回值***

  返回 `elite_c_status_t`。成功时返回 `ELITE_C_STATUS_OK`。

- ***注意事项***

  `config->robot_ip` 和 `config->script_file_path` 必须为非 `NULL`。

---

### 销毁驱动句柄
```c
void elite_driver_destroy(elite_driver_handle_t* handle);
```

- ***功能***

  释放驱动句柄及其关联资源。销毁时会清理内部回调注册，并释放底层通信对象。
  
- ***参数***

- `handle`：需要销毁的驱动句柄。

- ***返回值***

  无返回值。

---

### 是否与机器人连接
```c
elite_c_status_t elite_driver_is_robot_connected(
    elite_driver_handle_t* handle,
    int32_t* out_connected
);
```

- ***功能***

  查询是否已经与机器人建立连接。

- ***参数***

- `handle`：驱动句柄。
- `out_connected`：返回机器人连接状态，`1` 表示已连接，`0` 表示未连接。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 发送控制脚本
```c
elite_c_status_t elite_driver_send_external_control_script(
    elite_driver_handle_t* handle,
    int32_t* out_success
);
```

- ***功能***

  向机器人发送外部控制脚本。可用于建立或恢复与机器人的控制。
  
- ***参数***

- `handle`：驱动句柄。
- `out_success`：返回脚本下发是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 停止外部控制
```c
elite_c_status_t elite_driver_stop_control(
    elite_driver_handle_t* handle,
    int32_t wait_ms,
    int32_t* out_success
);
```

- ***功能***

  向机器人发送停止控制指令。机器人会退出当前外部控制脚本，并停止接收来自 PC 的控制命令。
  
- ***参数***

- `handle`：驱动句柄。
- `wait_ms`：阻塞等待机器人断开连接的时间（毫秒）。
- `out_success`：返回停止控制是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

## 运动控制

### 控制关节位置
```c
elite_c_status_t elite_driver_write_servoj(
    elite_driver_handle_t* handle,
    const double* pos6,
    int32_t timeout_ms,
    int32_t cartesian,
    int32_t* out_success
);
```

- ***功能***

  向机器人发送伺服运动指令。

- ***参数***

- `handle`：驱动句柄。
- `pos6`：长度为 6 的目标位置数组。
- `timeout_ms`：设置机器人读取下一条指令的超时时间，小于等于 `0` 时会无限等待。
- `cartesian`：如果发送的是笛卡尔点位，则传 `1`；如果发送的是关节点位，则传 `0`。
- `out_success`：返回命令发送是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 控制关节速度
```c
elite_c_status_t elite_driver_write_speedj(
    elite_driver_handle_t* handle,
    const double* vel6,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***功能***

  向机器人发送关节速度控制指令。

- ***参数***

- `handle`：驱动句柄。
- `vel6`：长度为 6 的关节速度数组。
- `timeout_ms`：设置机器人读取下一条指令的超时时间，小于等于 `0` 时会无限等待。
- `out_success`：返回命令发送是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 控制末端速度
```c
elite_c_status_t elite_driver_write_speedl(
    elite_driver_handle_t* handle,
    const double* vel6,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***功能***

  向机器人发送线速度控制指令。

- ***参数***

- `handle`：驱动句柄。
- `vel6`：线速度数组 `[x, y, z, rx, ry, rz]`。
- `timeout_ms`：设置机器人读取下一条指令的超时时间，小于等于 `0` 时会无限等待。
- `out_success`：返回命令发送是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 使机器人空闲
```c
elite_c_status_t elite_driver_write_idle(
    elite_driver_handle_t* handle,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***功能***

  发送空闲指令，如果机器人当前处于运动状态，发送该指令后会使机器人停止运动。
  
- ***参数***

- `handle`：驱动句柄。
- `timeout_ms`：设置机器人读取下一条指令的超时时间，小于等于 `0` 时会无限等待。
- `out_success`：返回命令发送是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### Freedrive
```c
elite_c_status_t elite_driver_write_freedrive(
    elite_driver_handle_t* handle,
    elite_freedrive_action_t action,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***功能***

  发送 Freedrive 模式指令，如开启 Freedrive、停止 Freedrive 操作。

- ***参数***

- `handle`：驱动句柄。
- `action`：Freedrive 动作，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_freedrive_action_t`。
- `timeout_ms`：设置机器人读取下一条指令的超时时间，小于等于 `0` 时会无限等待。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

## 轨迹运动

### 设置轨迹运动结果回调
```c
elite_c_status_t elite_driver_set_trajectory_result_callback(
    elite_driver_handle_t* handle,
    elite_driver_trajectory_result_cb_t cb,
    void* user_data
);
```

- ***功能***

  注册轨迹完成时的回调函数。 控制机器人的一种方式是将路点一次性发给机器人，当执行完成时，这里注册的回调函数将被触发。
  
- ***参数***

- `handle`：驱动句柄。
- `cb`：轨迹结果回调函数，回调结果类型为 `elite_trajectory_motion_result_t`，传 `NULL` 可取消注册。
- `user_data`：用户透传指针，会原样传回回调。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 写入轨迹路点
```c
elite_c_status_t elite_driver_write_trajectory_point(
    elite_driver_handle_t* handle,
    const double* positions6,
    float time,
    float blend_radius,
    int32_t cartesian,
    int32_t* out_success
);
```

- ***功能***

  向专门的socket写入轨迹路点。

- ***参数***

- `handle`：驱动句柄。
- `positions6`：长度为 6 的轨迹路点位置数组。
- `time`：到达该路点的时间。
- `blend_radius`：两个路点之间的过渡半径。
- `cartesian`：如果发送的是笛卡尔点位，则传 `1`；如果发送的是关节点位，则传 `0`。
- `out_success`：返回轨迹点下发是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 轨迹控制动作
```c
elite_c_status_t elite_driver_write_trajectory_control_action(
    elite_driver_handle_t* handle,
    elite_trajectory_control_action_t action,
    int32_t point_number,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***功能***

  发送轨迹控制指令，例如启动轨迹执行、继续、结束或空操作等。

- ***参数***

- `handle`：驱动句柄。
- `action`：轨迹控制动作，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_trajectory_control_action_t`。
- `point_number`：轨迹路点数量。
- `timeout_ms`：设置机器人读取下一条指令的超时时间，小于等于 `0` 时会无限等待。
- `out_success`：返回操作是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意***

  写入 `START` 动作之后，需要在超时时间内继续写入下一条指令，必要时可写入 `NOOP`。

---

## 机器人配置

### 力传感器去皮
```c
elite_c_status_t elite_driver_zero_ft_sensor(
    elite_driver_handle_t* handle,
    int32_t* out_success
);
```

- ***功能***

  将力/力矩传感器测量的施加在工具 TCP 上的力/力矩值清零（去皮），所述力/力矩值为 get_tcp_force(True) 脚本指令获取的施加在工具 TCP 上的力/力矩矢量，该矢量已进行负载补偿等处理。 该指令执行后，当前的力/力矩测量值会被作为力/力矩参考值保存，后续所有的力/力矩测量值都会减去该力/力矩参考值（去皮）。 请注意，上述力/力矩参考值会在该指令执行时更新，在控制器重启后将重置为 0。
  
- ***参数***

- `handle`：驱动句柄。
- `out_success`：返回力传感器零点校准是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 设置末端负载
```c
elite_c_status_t elite_driver_set_payload(
    elite_driver_handle_t* handle,
    double mass,
    const double* cog3,
    int32_t* out_success
);
```

- ***功能***

  该命令用于设置机器人载荷的质量和重心。

- ***参数***

- `handle`：驱动句柄。
- `mass`：负载质量。
- `cog3`：有效载荷重心坐标，长度为 3，相对于法兰坐标系定义。
- `out_success`：返回设置是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 设置工具电压
```c
elite_c_status_t elite_driver_set_tool_voltage(
    elite_driver_handle_t* handle,
    elite_tool_voltage_t voltage,
    int32_t* out_success
);
```

- ***功能***

  设置工具端输出电压。

- ***参数***

- `handle`：驱动句柄。
- `voltage`：工具电压，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_tool_voltage_t`。
- `out_success`：返回设置是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 开启力控模式
```c
elite_c_status_t elite_driver_start_force_mode(
    elite_driver_handle_t* handle,
    const double* reference_frame6,
    const int32_t* selection_vector6,
    const double* wrench6,
    elite_force_mode_t mode,
    const double* limits6,
    int32_t* out_success
);
```

- ***功能***

  开启机器人力控模式。

- ***参数***

- `handle`：驱动句柄。
- `reference_frame6`：定义力控参考坐标系的位姿矢量，长度为 6。
- `selection_vector6`：长度为 6 的选择向量，`1` 表示力控轴，`0` 表示非力控轴。
- `wrench6`：目标力/力矩，格式为 `[Fx, Fy, Fz, Mx, My, Mz]`。
- `mode`：力控模式，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_force_mode_t`。
- `limits6`：长度为 6 的速度限制参数。
- `out_success`：返回启动是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 关闭力控模式
```c
elite_c_status_t elite_driver_end_force_mode(
    elite_driver_handle_t* handle,
    int32_t* out_success
);
```

- ***功能***

  关闭当前力控模式。

- ***参数***

- `handle`：驱动句柄。
- `out_success`：返回结束力控是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

## 其余

### 发送脚本
```c
elite_c_status_t elite_driver_send_script(
    elite_driver_handle_t* handle,
    const char* script,
    int32_t* out_success
);
```

- ***功能***

  向机器人的 30001 端口发送可执行脚本。

- ***参数***

- `handle`：驱动句柄。
- `script`：要发送到控制器执行的脚本字符串。
- `out_success`：返回脚本发送是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 获取机器人 Primary 端口的数据包
```c
elite_c_status_t elite_driver_get_primary_package(
    elite_driver_handle_t* handle,
    int32_t timeout_ms,
    double* out_dh_a6,
    double* out_dh_d6,
    double* out_dh_alpha6,
    int32_t* out_success
);
```

- ***功能***

  获取机器人 Primary 端口中的运动学相关数据包。  
  当前 C 接口中，该接口主要用于读取机器人 DH 参数。

- ***参数***

- `handle`：驱动句柄。
- `timeout_ms`：获取超时时间，单位毫秒。
- `out_dh_a6`：返回长度为 6 的 DH 参数 `a`。
- `out_dh_d6`：返回长度为 6 的 DH 参数 `d`。
- `out_dh_alpha6`：返回长度为 6 的 DH 参数 `alpha`。
- `out_success`：返回读取是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 重新连接机器人 Primary 端口
```c
elite_c_status_t elite_driver_primary_reconnect(
    elite_driver_handle_t* handle,
    int32_t* out_success
);
```

- ***功能***

  重新建立与机器人的 30001端口连接。

- ***参数***

- `handle`：驱动句柄。
- `out_success`：返回重连是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 注册机器人异常回调
```c
elite_c_status_t elite_driver_register_robot_exception_callback(
    elite_driver_handle_t* handle,
    elite_driver_robot_exception_cb_t cb,
    void* user_data
);
```

- ***功能***

  注册机器人异常回调函数。当从机器人的 Primary 端口接收到异常报文时，将调用该回调函数。
  
- ***参数***

- `handle`：驱动句柄。
- `cb`：机器人异常回调函数，传 `NULL` 可取消注册。
- `user_data`：用户透传指针，会原样传回回调。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 启用工具 RS485 通讯
```c
elite_c_status_t elite_driver_start_tool_rs485(
    elite_driver_handle_t* handle,
    const elite_serial_config_t* config,
    const char* ssh_password,
    int32_t tcp_port,
    elite_serial_comm_handle_t** out_comm
);
```

- ***功能***

  启用工具RS485通讯。此接口会通过ssh登录机器人控制柜操作系统，并在机器人控制器上启动一个 socat 进程，将工具RS485串口的数据转发到指定的 TCP/IP 端口。
  
- ***参数***

- `handle`：驱动句柄。
- `config`：串口配置，包含波特率、校验位、停止位。
- `ssh_password`：机器人控制柜操作系统 SSH 登录密码。
- `tcp_port`：用于转发 RS485 数据的 TCP 端口，由调用方显式指定。
- `out_comm`：返回创建出的串口通信句柄。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 停止工具 RS485 通讯
```c
elite_c_status_t elite_driver_end_tool_rs485(
    elite_driver_handle_t* handle,
    elite_serial_comm_handle_t* comm,
    const char* ssh_password,
    int32_t* out_success
);
```

- ***功能***

  停止工具端 RS485 通讯。

- ***参数***

- `handle`：驱动句柄。
- `comm`：操作串口的对象句柄。
- `ssh_password`：机器人控制柜操作系统 SSH 登录密码。
- `out_success`：返回关闭是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 启用主板 RS485 通讯
```c
elite_c_status_t elite_driver_start_board_rs485(
    elite_driver_handle_t* handle,
    const elite_serial_config_t* config,
    const char* ssh_password,
    int32_t tcp_port,
    elite_serial_comm_handle_t** out_comm
);
```

- ***功能***

  启用主板 RS485 通讯。此接口会通过 ssh 登录机器人控制柜操作系统，并在机器人控制器上启动一个 socat 进程，将主板 RS485 串口的数据转发到指定的 TCP/IP 端口。
  
- ***参数***

- `handle`：驱动句柄。
- `config`：串口配置，包含波特率、校验位、停止位。
- `ssh_password`：机器人控制柜操作系统 SSH 登录密码。
- `tcp_port`：用于转发 RS485 数据的 TCP 端口，由调用方显式指定。
- `out_comm`：返回创建出的串口通信句柄。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 停止主板 RS485 通讯
```c
elite_c_status_t elite_driver_end_board_rs485(
    elite_driver_handle_t* handle,
    elite_serial_comm_handle_t* comm,
    const char* ssh_password,
    int32_t* out_success
);
```

- ***功能***

  停止主板 RS485 通讯。

- ***参数***

- `handle`：驱动句柄。
- `comm`：操作串口的对象句柄。
- `ssh_password`：机器人控制柜操作系统 SSH 登录密码。
- `out_success`：返回关闭是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

---

### 获取驱动最近错误信息
```c
const char* elite_driver_last_error_message(elite_driver_handle_t* handle);
```

- ***功能***

  获取该驱动句柄最近一次记录的错误信息。

- ***参数***

- `handle`：驱动句柄。

- ***返回值***

  返回最近一次该句柄记录的错误字符串。参数校验失败等未写入句柄错误区的情况，请改用 `elite_c_last_error_message()`。

---

### 获取全局最近错误信息
```c
const char* elite_c_last_error_message(void);
```

- ***功能***

  获取当前线程中最近一次 C 接口调用失败时记录的全局错误信息。

- ***返回值***

  返回全局错误字符串。

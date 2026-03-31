# RTSI 模块

## 简介

RTSI 模块包含三部分内容：

- RTSI Client：底层实时通道连接、协议协商、配方创建和实时数据收发
- RTSI Recipe：配方字段读写
- RTSI IO：常用实时 IO 与状态访问的高级封装

## 导入

```c
#include <Rtsi/Elite_Rtsi_C.h>
```

## RTSI Client

### 创建 RTSI 客户端
```c
elite_c_status_t elite_rtsi_client_create(elite_rtsi_client_handle_t** out_handle);
```

- ***功能***

  创建 RTSI 客户端句柄。

- ***参数***

- `out_handle`：返回创建出的 RTSI 客户端句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 销毁 RTSI 客户端
```c
void elite_rtsi_client_destroy(elite_rtsi_client_handle_t* handle);
```

- ***功能***

  释放 RTSI 客户端句柄。

- ***参数***

- `handle`：需要销毁的 RTSI 客户端句柄。

- ***返回值***

  无返回值。

### 连接控制器
```c
elite_c_status_t elite_rtsi_client_connect(
    elite_rtsi_client_handle_t* handle,
    const char* ip,
    int32_t port
);
```

- ***功能***

  连接机器人 RTSI 服务。

- ***参数***

- `handle`：RTSI 客户端句柄。
- `ip`：机器人 IP 地址。
- `port`：RTSI 端口。

- ***返回值***

  返回 `elite_c_status_t`。该接口没有额外的 `out_success` 输出参数，连接失败时会直接通过返回值或错误字符串体现。

### 断开连接
```c
elite_c_status_t elite_rtsi_client_disconnect(elite_rtsi_client_handle_t* handle);
```

- ***功能***

  断开当前 RTSI 连接。

- ***参数***

- `handle`：RTSI 客户端句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 协商协议版本
```c
elite_c_status_t elite_rtsi_client_negotiate_protocol_version(
    elite_rtsi_client_handle_t* handle,
    uint16_t version,
    int32_t* out_success
);
```

- ***功能***

  与控制器协商 RTSI 协议版本。

- ***参数***

- `handle`：RTSI 客户端句柄。
- `version`：协议版本号。
- `out_success`：返回协商是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取控制器版本
```c
elite_c_status_t elite_rtsi_client_get_controller_version(
    elite_rtsi_client_handle_t* handle,
    elite_version_info_t* out_version
);
```

- ***功能***

  获取控制器版本信息。

- ***参数***

- `handle`：RTSI 客户端句柄。
- `out_version`：返回控制器版本信息。

- ***返回值***

  返回 `elite_c_status_t`。

### 创建输出配方
```c
elite_c_status_t elite_rtsi_client_setup_output_recipe(
    elite_rtsi_client_handle_t* handle,
    const char* recipe_csv,
    double frequency,
    elite_rtsi_recipe_handle_t** out_recipe
);
```

- ***功能***

  创建用于接收控制器输出数据的 recipe。

- ***参数***

- `handle`：RTSI Client 句柄。
- `recipe_csv`：输出配方字段列表，使用逗号分隔，例如 `actual_q,actual_qd,target_q`。
- `frequency`：输出配方接收频率，单位 Hz。
- `out_recipe`：返回创建出的 recipe 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 创建输入配方
```c
elite_c_status_t elite_rtsi_client_setup_input_recipe(
    elite_rtsi_client_handle_t* handle,
    const char* recipe_csv,
    elite_rtsi_recipe_handle_t** out_recipe
);
```

- ***功能***

  创建用于发送输入数据到控制器的 recipe。

- ***参数***

- `handle`：RTSI Client 句柄。
- `recipe_csv`：输入配方字段列表，使用逗号分隔。
- `out_recipe`：返回创建出的 recipe 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 销毁配方
```c
void elite_rtsi_recipe_destroy(elite_rtsi_recipe_handle_t* recipe);
```

- ***功能***

  释放 RTSI recipe 句柄。

- ***参数***

- `recipe`：需要销毁的 recipe 句柄。

- ***返回值***

  无返回值。

### 启动 RTSI
```c
elite_c_status_t elite_rtsi_client_start(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_success
);
```

- ***功能***

  启动 RTSI 数据流。

- ***参数***

- `handle`：RTSI Client 句柄。
- `out_success`：返回启动是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 暂停 RTSI
```c
elite_c_status_t elite_rtsi_client_pause(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_success
);
```

- ***功能***

  暂停 RTSI 数据流。

- ***参数***

- `handle`：RTSI Client 句柄。
- `out_success`：返回暂停是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 发送输入配方数据
```c
elite_c_status_t elite_rtsi_client_send(
    elite_rtsi_client_handle_t* handle,
    elite_rtsi_recipe_handle_t* recipe
);
```

- ***功能***

  发送一次输入 recipe 数据到控制器。

- ***参数***

- `handle`：RTSI Client 句柄。
- `recipe`：需要发送的输入 recipe 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 接收单个配方数据
```c
elite_c_status_t elite_rtsi_client_receive_data(
    elite_rtsi_client_handle_t* handle,
    elite_rtsi_recipe_handle_t* recipe,
    int32_t read_newest,
    int32_t* out_success
);
```

- ***功能***

  接收单个 recipe 的一帧数据。

- ***参数***

- `handle`：RTSI Client 句柄。
- `recipe`：接收数据写回的 recipe 句柄。
- `read_newest`：是否读取最新数据帧，非 `0` 表示读取最新，`0` 表示按队列顺序读取。
- `out_success`：返回接收是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 接收多个配方数据
```c
elite_c_status_t elite_rtsi_client_receive_data_multi(
    elite_rtsi_client_handle_t* handle,
    elite_rtsi_recipe_handle_t** recipes,
    int32_t recipe_count,
    int32_t read_newest,
    int32_t* out_received_index
);
```

- ***功能***

  一次接收多个 recipe 中的数据。

- ***参数***

- `handle`：RTSI Client 句柄。
- `recipes`：recipe 句柄数组。
- `recipe_count`：recipe 数组长度。
- `read_newest`：是否读取最新数据帧，非 `0` 表示读取最新。
- `out_received_index`：返回收到数据的 recipe 下标；若没有可用数据，通常返回 `-1`。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断是否已连接
```c
elite_c_status_t elite_rtsi_client_is_connected(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_connected
);
```

- ***功能***

  判断 RTSI 客户端是否已连接。

- ***参数***

- `handle`：RTSI Client 句柄。
- `out_connected`：返回连接状态，`1` 表示已连接，`0` 表示未连接。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断是否已启动
```c
elite_c_status_t elite_rtsi_client_is_started(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_started
);
```

- ***功能***

  判断 RTSI 数据流是否已启动。

- ***参数***

- `handle`：RTSI Client 句柄。
- `out_started`：返回启动状态，`1` 表示已启动，`0` 表示未启动。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断是否有可读数据
```c
elite_c_status_t elite_rtsi_client_is_read_available(
    elite_rtsi_client_handle_t* handle,
    int32_t* out_available
);
```

- ***功能***

  判断当前是否有可读取的数据。

- ***参数***

- `handle`：RTSI Client 句柄。
- `out_available`：返回是否有可读数据，`1` 表示有，`0` 表示无。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取 RTSI Client 最后错误信息
```c
const char* elite_rtsi_client_last_error_message(elite_rtsi_client_handle_t* handle);
```

- ***功能***

  获取 RTSI Client 句柄最近一次错误消息。

- ***参数***

- `handle`：RTSI Client 句柄。

- ***返回值***

  返回该句柄最近一次错误字符串。

## RTSI Recipe

### 获取配方 ID
```c
elite_c_status_t elite_rtsi_recipe_get_id(
    elite_rtsi_recipe_handle_t* recipe,
    int32_t* out_id
);
```

- ***功能***

  获取当前 recipe 的 ID。

- ***参数***

- `recipe`：recipe 句柄。
- `out_id`：返回配方 ID。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取配方 CSV 字符串
```c
elite_c_status_t elite_rtsi_recipe_get_recipe_csv(
    elite_rtsi_recipe_handle_t* recipe,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取当前 recipe 的字段列表字符串。

- ***参数***

- `recipe`：recipe 句柄。
- `out_buffer`：输出缓冲区。
- `buffer_len`：缓冲区长度。
- `out_required_len`：返回完整字符串所需长度，包含结尾空字符。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  `out_required_len` 不能为空。
  如果 `out_buffer == NULL` 或 `buffer_len <= 0`，接口仍会返回成功，并通过 `out_required_len` 告知完整长度。

### 读取 recipe 字段
```c
elite_c_status_t elite_rtsi_recipe_get_value_double(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    double* out_value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_get_value_int32(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    int32_t* out_value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_get_value_uint32(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    uint32_t* out_value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_get_value_bool(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    int32_t* out_value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_get_value_vector6d(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    double* out_value6,
    int32_t* out_success
);
```

- ***功能***

  读取 recipe 中指定字段的值。

- ***参数***

- `recipe`：recipe 句柄。
- `name`：字段名。
- `out_value`：返回 `double`、`int32_t`、`uint32_t` 或 `bool` 字段值；其中布尔值通过 `0/1` 表示。
- `out_value6`：返回长度为 6 的向量字段值。
- `out_success`：返回字段读取是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 写入 recipe 字段
```c
elite_c_status_t elite_rtsi_recipe_set_value_double(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    double value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_set_value_int32(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    int32_t value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_set_value_uint32(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    uint32_t value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_set_value_bool(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    int32_t value,
    int32_t* out_success
);
elite_c_status_t elite_rtsi_recipe_set_value_vector6d(
    elite_rtsi_recipe_handle_t* recipe,
    const char* name,
    const double* value6,
    int32_t* out_success
);
```

- ***功能***

  向 recipe 中写入指定字段的值。

- ***参数***

- `recipe`：recipe 句柄。
- `name`：字段名。
- `value`：要写入的标量值。
- `value6`：要写入的长度为 6 的向量值。
- `out_success`：返回字段写入是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

## RTSI IO

### 创建 RTSI IO 句柄
```c
elite_c_status_t elite_rtsi_io_create(
    const char* output_recipe_csv,
    const char* input_recipe_csv,
    double frequency,
    elite_rtsi_io_handle_t** out_handle
);
```

- ***功能***

  创建 RTSI IO 句柄，并基于输入/输出 recipe 建立高级实时 IO 访问对象。

- ***参数***

- `output_recipe_csv`：输出 recipe 字段列表，使用逗号分隔。
- `input_recipe_csv`：输入 recipe 字段列表，使用逗号分隔。
- `frequency`：RTSI 通信频率，单位 Hz。
- `out_handle`：返回创建出的 RTSI IO 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  `output_recipe_csv` 和 `input_recipe_csv` 可以传 `NULL`，此时分别按空字段列表处理。

### 销毁 RTSI IO 句柄
```c
void elite_rtsi_io_destroy(elite_rtsi_io_handle_t* handle);
```

- ***功能***

  释放 RTSI IO 句柄。

- ***参数***

- `handle`：需要销毁的 RTSI IO 句柄。

- ***返回值***

  无返回值。

### 连接与状态
```c
elite_c_status_t elite_rtsi_io_connect(elite_rtsi_io_handle_t* handle, const char* ip, int32_t* out_success);
elite_c_status_t elite_rtsi_io_disconnect(elite_rtsi_io_handle_t* handle);
elite_c_status_t elite_rtsi_io_is_connected(elite_rtsi_io_handle_t* handle, int32_t* out_connected);
elite_c_status_t elite_rtsi_io_is_started(elite_rtsi_io_handle_t* handle, int32_t* out_started);
elite_c_status_t elite_rtsi_io_get_controller_version(elite_rtsi_io_handle_t* handle, elite_version_info_t* out_version);
```

- ***功能***

  完成 RTSI IO 的连接、断开、状态查询以及控制器版本读取。

- ***参数***

- `handle`：RTSI IO 句柄。
- `ip`：机器人 IP 地址。
- `out_success`：返回连接是否成功，`1` 表示成功，`0` 表示失败。
- `out_connected`：返回连接状态，`1` 表示已连接，`0` 表示未连接。
- `out_started`：返回内部同步线程是否已启动，`1` 表示已启动，`0` 表示未启动。
- `out_version`：返回控制器版本信息。

- ***返回值***

  返回 `elite_c_status_t`；`elite_rtsi_io_disconnect` 不额外输出结果参数。

### 设置实时输出与控制量
```c
elite_c_status_t elite_rtsi_io_set_speed_scaling(elite_rtsi_io_handle_t* handle, double scaling, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_standard_digital(elite_rtsi_io_handle_t* handle, int32_t index, int32_t level, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_configure_digital(elite_rtsi_io_handle_t* handle, int32_t index, int32_t level, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_analog_output_voltage(elite_rtsi_io_handle_t* handle, int32_t index, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_analog_output_current(elite_rtsi_io_handle_t* handle, int32_t index, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_external_force_torque(elite_rtsi_io_handle_t* handle, const double* value6, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_tool_digital_output(elite_rtsi_io_handle_t* handle, int32_t index, int32_t level, int32_t* out_success);
```

- ***功能***

  设置速度比例、标准数字输出、可配置数字输出、模拟输出、外部力/力矩以及工具数字输出。

- ***参数***

- `handle`：RTSI IO 句柄。
- `scaling`：需要设置的速度比例，通常为 `0.0` 到 `1.0`。
- `index`：目标 IO 通道索引。
- `level`：数字输出电平，`0` 表示低电平，非 `0` 表示高电平。
- `value`：模拟输出值。
- `value6`：长度为 6 的外部力/力矩值数组。
- `out_success`：返回设置是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取基础标量与向量状态
```c
elite_c_status_t elite_rtsi_io_get_timestamp(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_payload_mass(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_payload_cog(elite_rtsi_io_handle_t* handle, double* out_value3);
elite_c_status_t elite_rtsi_io_get_script_control_line(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_target_joint_positions(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_target_joint_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_positions(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_torques(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_current(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_joint_temperatures(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_tcp_pose(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_tcp_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_tcp_force(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_target_tcp_pose(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_target_tcp_velocity(elite_rtsi_io_handle_t* handle, double* out_value6);
elite_c_status_t elite_rtsi_io_get_actual_speed_scaling(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_target_speed_scaling(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_robot_voltage(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_robot_current(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_elbow_position(elite_rtsi_io_handle_t* handle, double* out_value3);
elite_c_status_t elite_rtsi_io_get_elbow_velocity(elite_rtsi_io_handle_t* handle, double* out_value3);
elite_c_status_t elite_rtsi_io_get_io_current(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_input(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_output(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_output_voltage(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_output_current(elite_rtsi_io_handle_t* handle, double* out_value);
elite_c_status_t elite_rtsi_io_get_tool_output_temperature(elite_rtsi_io_handle_t* handle, double* out_value);
```

- ***功能***

  读取 RTSI IO 当前缓存中的时间戳、负载、关节、TCP、速度比例、电气状态、肘部状态及工具侧标量/向量数据。

- ***参数***

- `handle`：RTSI IO 句柄。
- `out_value`：返回标量值。
- `out_value3`：返回长度为 3 的向量值。
- `out_value6`：返回长度为 6 的向量值。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取模式、状态位与数字量
```c
elite_c_status_t elite_rtsi_io_get_digital_input_bits(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_digital_output_bits(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_robot_mode(elite_rtsi_io_handle_t* handle, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_joint_mode(elite_rtsi_io_handle_t* handle, int32_t* out_value6);
elite_c_status_t elite_rtsi_io_get_safety_status(elite_rtsi_io_handle_t* handle, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_robot_status(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_runtime_state(elite_rtsi_io_handle_t* handle, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_safety_status_bits(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_analog_io_types(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_mode(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_input_type(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_analog_output_type(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_digital_mode(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_tool_digital_output_mode(elite_rtsi_io_handle_t* handle, int32_t index, uint32_t* out_value);
```

- ***功能***

  读取数字输入/输出位、机器人模式、关节模式、安全状态、运行状态、工具模式及相关状态位信息。

- ***参数***

- `handle`：RTSI IO 句柄。
- `index`：工具数字输出通道索引。
- `out_value`：返回标量或位掩码值。
- `out_value6`：返回长度为 6 的关节模式数组。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取标准模拟量
```c
elite_c_status_t elite_rtsi_io_get_analog_input(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
elite_c_status_t elite_rtsi_io_get_analog_output(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
```

- ***功能***

  获取指定标准模拟输入或标准模拟输出通道的当前值。

- ***参数***

- `handle`：RTSI IO 句柄。
- `index`：模拟通道索引。
- `out_value`：返回通道值。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取布尔寄存器与数值寄存器
```c
elite_c_status_t elite_rtsi_io_get_out_bool_registers0_to_31(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_out_bool_registers32_to_63(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_bool_registers0_to_31(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_bool_registers32_to_63(elite_rtsi_io_handle_t* handle, uint32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_bool_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_out_bool_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_int_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_out_int_register(elite_rtsi_io_handle_t* handle, int32_t index, int32_t* out_value);
elite_c_status_t elite_rtsi_io_get_in_double_register(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
elite_c_status_t elite_rtsi_io_get_out_double_register(elite_rtsi_io_handle_t* handle, int32_t index, double* out_value);
```

- ***功能***

  获取输入/输出布尔寄存器位图，以及指定索引的布尔、整型和双精度寄存器值。

- ***参数***

- `handle`：RTSI IO 句柄。
- `index`：寄存器索引。
- `out_value`：返回位图值、布尔值、整型值或双精度值；布尔寄存器单值通过 `0/1` 表示。

- ***返回值***

  返回 `elite_c_status_t`。

### 读取输出 recipe 字段值
```c
elite_c_status_t elite_rtsi_io_get_recipe_value_double(elite_rtsi_io_handle_t* handle, const char* name, double* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_int32(elite_rtsi_io_handle_t* handle, const char* name, int32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_uint32(elite_rtsi_io_handle_t* handle, const char* name, uint32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_bool(elite_rtsi_io_handle_t* handle, const char* name, int32_t* out_value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_vector3d(elite_rtsi_io_handle_t* handle, const char* name, double* out_value3, int32_t* out_success);
elite_c_status_t elite_rtsi_io_get_recipe_value_vector6d(elite_rtsi_io_handle_t* handle, const char* name, double* out_value6, int32_t* out_success);
```

- ***功能***

  从 RTSI IO 当前维护的输出 recipe 中读取指定字段值。

- ***参数***

- `handle`：RTSI IO 句柄。
- `name`：字段名。
- `out_value`：返回标量值；布尔值通过 `0/1` 表示。
- `out_value3`：返回长度为 3 的向量值。
- `out_value6`：返回长度为 6 的向量值。
- `out_success`：返回字段读取是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 写入输入 recipe 字段值
```c
elite_c_status_t elite_rtsi_io_set_input_recipe_value_double(elite_rtsi_io_handle_t* handle, const char* name, double value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_int32(elite_rtsi_io_handle_t* handle, const char* name, int32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_uint32(elite_rtsi_io_handle_t* handle, const char* name, uint32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_bool(elite_rtsi_io_handle_t* handle, const char* name, int32_t value, int32_t* out_success);
elite_c_status_t elite_rtsi_io_set_input_recipe_value_vector6d(elite_rtsi_io_handle_t* handle, const char* name, const double* value6, int32_t* out_success);
```

- ***功能***

  向 RTSI IO 使用的输入 recipe 中写入指定字段值。

- ***参数***

- `handle`：RTSI IO 句柄。
- `name`：字段名。
- `value`：要写入的标量值；布尔值使用 `0/1` 表示。
- `value6`：要写入的长度为 6 的向量值。
- `out_success`：返回字段写入是否成功，`1` 表示成功，`0` 表示失败。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取 RTSI IO 最后错误信息
```c
const char* elite_rtsi_io_last_error_message(elite_rtsi_io_handle_t* handle);
```

- ***功能***

  获取 RTSI IO 句柄最近一次错误消息。

- ***参数***

- `handle`：RTSI IO 句柄。

- ***返回值***

  返回该句柄最近一次错误字符串。

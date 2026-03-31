# SerialCommunication 模块

## 简介

机器人的末端和控制柜有RS485通讯接口，SDK 提供了相应的功能。

SDK 读写机器人串口本质是，机器人将 RS485 接口的数据转发到指定的 TCP 端口，SDK直接去读写该TCP端口。

## 导入

```c
#include <Elite/SerialCommunication_C.hpp>
```

## 配置类型

```c
typedef struct elite_serial_config_t {
    elite_serial_baud_rate_t baud_rate;
    elite_serial_parity_t parity;
    elite_serial_stop_bits_t stop_bits;
} elite_serial_config_t;
```

- `baud_rate`：串口波特率，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_serial_baud_rate_t`
- `parity`：串口校验位，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_serial_parity_t`
- `stop_bits`：串口停止位，取值见 [CommonTypes.cn.md](./CommonTypes.cn.md) 中的 `elite_serial_stop_bits_t`

## 接口说明

### 连接串口通道
```c
elite_c_status_t elite_serial_comm_connect(
    elite_serial_comm_handle_t* comm,
    int32_t timeout_ms,
    int32_t* out_success
);
```

- ***参数***

- `comm`：串口通信句柄。
- `timeout_ms`：连接超时时间，单位毫秒。
- `out_success`：返回连接是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 断开串口连接
```c
elite_c_status_t elite_serial_comm_disconnect(elite_serial_comm_handle_t* comm);
```

- ***参数***

- `comm`：串口通信句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 判断是否已连接
```c
elite_c_status_t elite_serial_comm_is_connected(
    elite_serial_comm_handle_t* comm,
    int32_t* out_connected
);
```

- ***参数***

- `comm`：串口通信句柄。
- `out_connected`：返回连接状态。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取 socat 进程 ID
```c
elite_c_status_t elite_serial_comm_get_socat_pid(
    elite_serial_comm_handle_t* comm,
    int32_t* out_pid
);
```

- ***参数***

- `comm`：串口通信句柄。
- `out_pid`：返回关联的 socat 进程 ID。

- ***返回值***

  返回 `elite_c_status_t`。

### 写入串口数据
```c
elite_c_status_t elite_serial_comm_write(
    elite_serial_comm_handle_t* comm,
    const uint8_t* data,
    int32_t size,
    int32_t* out_written
);
```

- ***参数***

- `comm`：串口通信句柄。
- `data`：待写入数据缓存。
- `size`：待写入字节数。
- `out_written`：返回实际写入字节数。

- ***返回值***

  返回 `elite_c_status_t`。

### 读取串口数据
```c
elite_c_status_t elite_serial_comm_read(
    elite_serial_comm_handle_t* comm,
    uint8_t* out_data,
    int32_t size,
    int32_t timeout_ms,
    int32_t* out_read
);
```

- ***参数***

- `comm`：串口通信句柄。
- `out_data`：读取结果缓冲区。
- `size`：期望读取的最大字节数。
- `timeout_ms`：读取超时时间，单位毫秒。
- `out_read`：返回实际读取字节数。

- ***返回值***

  返回 `elite_c_status_t`。

### 销毁串口句柄
```c
void elite_serial_comm_destroy(elite_serial_comm_handle_t* comm);
```

- ***功能***

  销毁串口通信句柄并释放相关资源。

- ***参数***

- `comm`：需要销毁的串口通信句柄。

- ***返回值***

  无返回值。

- ***注意事项***

  该句柄通常由 `elite_driver_start_tool_rs485()` 或 `elite_driver_start_board_rs485()` 创建。

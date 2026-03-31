# Primary 模块

## 简介

Primary 模块用于访问控制器 Primary 接口，可发送脚本、获取运动学参数并接收异常消息。

## 导入

```c
#include <Primary/PrimaryPortInterface_C.hpp>
```

## 接口说明

### 创建句柄
```c
elite_c_status_t elite_primary_create(elite_primary_handle_t** out_handle);
```

- ***功能***

  创建 Primary 通信句柄。

- ***参数***

- `out_handle`：返回创建出的 Primary 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  `out_required_len` 不能为空。
  如果 `out_buffer == NULL` 或 `buffer_len <= 0`，接口仍会返回成功，并通过 `out_required_len` 告知完整长度。

### 销毁句柄
```c
void elite_primary_destroy(elite_primary_handle_t* handle);
```

- ***功能***

  释放 Primary 通信句柄。

- ***参数***

- `handle`：需要销毁的 Primary 句柄。

- ***返回值***

  无返回值。

### 连接 Primary 端口
```c
elite_c_status_t elite_primary_connect(
    elite_primary_handle_t* handle,
    const char* ip,
    int32_t port,
    int32_t* out_success
);
```

- ***功能***

  连接机器人 Primary 端口。

- ***参数***

- `handle`：Primary 句柄。
- `ip`：机器人 IP 地址。
- `port`：Primary 端口。
- `out_success`：返回连接是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  `out_required_len` 不能为空。
  如果 `out_buffer == NULL` 或 `buffer_len <= 0`，接口仍会返回成功，并通过 `out_required_len` 告知完整长度。

### 断开 Primary 端口
```c
elite_c_status_t elite_primary_disconnect(elite_primary_handle_t* handle);
```

- ***功能***

  断开当前 Primary 连接。

- ***参数***

- `handle`：Primary 句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 发送脚本
```c
elite_c_status_t elite_primary_send_script(
    elite_primary_handle_t* handle,
    const char* script,
    int32_t* out_success
);
```

- ***功能***

  通过 Primary 通道发送脚本字符串到控制器执行。

- ***参数***

- `handle`：Primary 句柄。
- `script`：要发送的脚本字符串。
- `out_success`：返回脚本发送是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 获取本地 IP
```c
elite_c_status_t elite_primary_get_local_ip(
    elite_primary_handle_t* handle,
    char* out_buffer,
    int32_t buffer_len,
    int32_t* out_required_len
);
```

- ***功能***

  获取当前 Primary 连接使用的本地 IP 地址。

- ***参数***

- `handle`：Primary 句柄。
- `out_buffer`：输出缓冲区。
- `buffer_len`：缓冲区长度。
- `out_required_len`：实际所需长度。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  `out_required_len` 不能为空。
  如果 `out_buffer == NULL` 或 `buffer_len <= 0`，接口仍会返回成功，并通过 `out_required_len` 告知完整长度。

### 获取运动学信息
```c
elite_c_status_t elite_primary_get_kinematics_info(
    elite_primary_handle_t* handle,
    int32_t timeout_ms,
    double* out_dh_a6,
    double* out_dh_d6,
    double* out_dh_alpha6,
    int32_t* out_success
);
```

- ***功能***

  从 Primary 数据包中读取机器人 DH 参数。

- ***参数***

- `timeout_ms`：等待超时时间。
- `handle`：Primary 句柄。
- `out_dh_a6`：DH 参数 `a` 数组。
- `out_dh_d6`：DH 参数 `d` 数组。
- `out_dh_alpha6`：DH 参数 `alpha` 数组。
- `out_success`：返回读取是否成功。

- ***返回值***

  返回 `elite_c_status_t`。

### 注册机器人异常回调
```c
elite_c_status_t elite_primary_register_robot_exception_callback(
    elite_primary_handle_t* handle,
    elite_primary_robot_exception_cb_t cb,
    void* user_data
);
```

- ***功能***

  注册机器人异常消息回调。

- ***回调参数***

- `ex`：异常信息结构体，包含错误码、子错误码、来源、级别和文本消息等。
- `user_data`：用户透传指针。

- ***参数***

- `handle`：Primary 句柄。
- `cb`：机器人异常回调函数，传 `NULL` 可取消注册。
- `user_data`：用户透传指针，会原样传回回调。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  回调中的 `ex->message` 指针仅在回调执行期间有效；如果需要在回调返回后继续使用，应自行拷贝。

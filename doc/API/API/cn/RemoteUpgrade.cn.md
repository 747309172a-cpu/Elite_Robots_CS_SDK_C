# RemoteUpgrade 模块

## 简介

RemoteUpgrade 模块提供控制器软件远程升级能力。

## 导入

```c
#include <Elite/RemoteUpgrade_C.hpp>
```

## 接口说明

### 升级控制器软件
```c
elite_c_status_t elite_upgrade_control_software(
    const char* ip,
    const char* file,
    const char* password,
    int32_t* out_success
);
```

- ***功能***

  将指定升级包发送到控制器并触发控制软件升级。

- ***参数***

- `ip`：机器人 IP 地址。
- `file`：升级文件路径。
- `password`：控制器 SSH 密码。
- `out_success`：升级流程是否成功启动。

- ***返回值***

  返回 `elite_c_status_t`。

- ***注意事项***

  升级前应确认升级包版本与控制器型号匹配。

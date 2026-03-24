# Elite Robots CS SDK C 封装库

[English](./README.md)

`Elite_Robots_CS_SDK_C` 是 Elite Robots CS 系列 SDK 的 C ABI 封装层。
它在上游 C++ SDK 之上提供稳定的 C 接口，方便纯 C 项目以及其他语言通过 FFI
方式接入 Elite CS 控制器。

## 项目说明

该仓库会构建一个名为 `elite_cs_series_sdk_c` 的共享库，并链接上游
`elite-cs-series-sdk` 的共享库。

当前封装的能力主要包括：

- `Dashboard`：控制器管理、上下电、程序/任务操作、状态查询
- `EliteDriver`：运动控制、脚本下发、负载与工具设置、力控、RS485 辅助接口
- `RTSI`：实时数据收发、recipe 配置、IO 与寄存器访问
- `Primary`：Primary 端口访问、脚本发送、运动学信息读取、异常回调
- `工具接口`：版本解析与比较、日志、控制器日志访问

统一对外头文件是 [`include/Elite_Sdk_C.h`](./include/Elite_Sdk_C.h)。

## 仓库结构

```text
include/
  Elite_Sdk_C.h              对外总入口头文件
  Elite_C_Types.h            共享类型与状态码
  Dashboard/                 Dashboard C 接口
  Elite/                     Driver、串口、日志、版本接口
  Primary/                   Primary 端口接口
  Rtsi/                      RTSI 客户端与 IO 接口
source/
  ...                        C 封装实现
CMakeLists.txt               编译规则
```

## 依赖要求

- CMake 3.16 或更高版本
- 支持 C++17 的编译工具链
- 上游 `elite-cs-series-sdk` 库及其头文件

## 构建方式

### 方式一：使用本地已安装的上游 SDK

将 `CMAKE_PREFIX_PATH` 指向已经导出 `elite-cs-series-sdk::shared` 的 SDK 安装路径。

```bash
cmake -S . -B build -DCMAKE_PREFIX_PATH=/path/to/elite-cs-sdk
cmake --build build
```

### 方式二：自动拉取上游 SDK

如果本地没有安装上游 SDK，可以通过 CMake 自动从 GitHub 或 Gitee 拉取。

```bash
cmake -S . -B build -DELITE_AUTO_FETCH_SDK=ON
cmake --build build
```

如有需要，也可以通过 `ELITE_CS_SDK_REPO` 覆盖默认上游仓库地址。

## 安装

设置 `ELITE_INSTALL` 后可执行安装。

```bash
cmake -S . -B build \
  -DELITE_INSTALL=ON \
  -DCMAKE_INSTALL_PREFIX=/your/install/prefix \
  -DCMAKE_PREFIX_PATH=/path/to/elite-cs-sdk
cmake --build build
cmake --install build
```

## 最小示例

```c
#include <Elite_Sdk_C.h>
#include <stdio.h>

int main(void) {
    elite_dashboard_handle_t* dashboard = NULL;
    int32_t ok = 0;

    if (elite_dashboard_create(&dashboard) != ELITE_C_STATUS_OK) {
        fprintf(stderr, "failed to create dashboard handle\n");
        return 1;
    }

    if (elite_dashboard_connect(dashboard, "192.168.1.200", 29999, &ok) != ELITE_C_STATUS_OK || !ok) {
        fprintf(stderr, "connect failed: %s\n", elite_dashboard_last_error_message(dashboard));
        elite_dashboard_destroy(dashboard);
        return 1;
    }

    if (elite_dashboard_robot_mode(dashboard, &ok) == ELITE_C_STATUS_OK) {
        printf("robot mode: %d\n", ok);
    }

    elite_dashboard_disconnect(dashboard);
    elite_dashboard_destroy(dashboard);
    return 0;
}
```

## 错误处理

大多数接口返回 `elite_c_status_t`：

- `ELITE_C_STATUS_OK`
- `ELITE_C_STATUS_INVALID_ARGUMENT`
- `ELITE_C_STATUS_ALLOCATION_FAILED`
- `ELITE_C_STATUS_EXCEPTION`

很多模块还提供了 `*_last_error_message(...)` 接口，便于在失败后获取可读错误信息。

## 说明

- 对外接口是 C 风格，但部分公开头文件使用了 `.hpp` 文件名。
- 该仓库是封装层，不是独立的机器人 SDK。
- 实际可用能力仍取决于上游 SDK 以及控制器固件版本。

## 许可证

请结合本仓库以及上游 Elite Robots SDK 中的许可证信息使用。

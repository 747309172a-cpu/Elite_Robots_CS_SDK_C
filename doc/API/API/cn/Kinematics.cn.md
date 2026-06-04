# 运动学模块

## 简介

运动学模块通过 C 接口封装上游 SDK 的运动学插件接口。该模块在运行时加载运动学插件库，创建求解器实例，设置机器人 MDH 参数，并提供 FK/IK 计算接口。

## 导入

```c
#include <Elite/Kinematics_C.hpp>
```

总入口头文件也包含该模块：

```c
#include <Elite_Sdk_C.h>
```

## 插件

运动学实现由运行时插件提供。默认插件类名为：

```text
ELITE::KdlKinematicsPlugin
```

Linux 下常见安装路径：

```text
/usr/local/lib/elite-cs-series-sdk-plugins/kinematics/libelite_kdl_kinematics.so
```

## 句柄类型

```c
typedef struct elite_kinematics_handle_t elite_kinematics_handle_t;
```

- ***功能***

  不透明句柄，内部持有插件加载器和运动学求解器实例。

## 接口说明

### 创建运动学句柄

```c
elite_c_status_t elite_kinematics_create(
    const char* plugin_lib_path,
    const char* plugin_class_name,
    elite_kinematics_handle_t** out_handle
);
```

- ***功能***

  加载运动学插件库，并创建求解器实例。

- ***参数***

- `plugin_lib_path`：运动学插件动态库路径。
- `plugin_class_name`：插件类名。传 `NULL` 或空字符串时默认使用 `ELITE::KdlKinematicsPlugin`。
- `out_handle`：返回创建出的运动学句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 销毁运动学句柄

```c
void elite_kinematics_destroy(elite_kinematics_handle_t* handle);
```

- ***功能***

  释放运动学句柄及其关联资源。

### 设置 MDH 参数

```c
elite_c_status_t elite_kinematics_set_mdh(
    elite_kinematics_handle_t* handle,
    const double* alpha6,
    const double* a6,
    const double* d6
);
```

- ***功能***

  设置运动学求解器使用的机器人 MDH 参数。

- ***参数***

- `handle`：运动学句柄。
- `alpha6`：长度为 6 的 MDH alpha 数组。
- `a6`：长度为 6 的 MDH a 数组。
- `d6`：长度为 6 的 MDH d 数组。

- ***注意事项***

  DH 参数可通过 `elite_primary_get_kinematics_info()` 或 `elite_driver_get_primary_package()` 从机器人读取。

### 正运动学

```c
elite_c_status_t elite_kinematics_get_position_fk(
    elite_kinematics_handle_t* handle,
    const double* joint_angles6,
    double* out_pose6,
    int32_t* out_success
);
```

- ***功能***

  根据关节角计算 TCP 位姿。

- ***参数***

- `handle`：运动学句柄。
- `joint_angles6`：长度为 6 的关节角数组。
- `out_pose6`：返回长度为 6 的 TCP 位姿。
- `out_success`：返回 FK 是否成功。

### 逆运动学

```c
elite_c_status_t elite_kinematics_get_position_ik(
    elite_kinematics_handle_t* handle,
    const double* pose6,
    const double* near6,
    double* out_solution6,
    elite_kinematics_result_t* out_result,
    int32_t* out_success
);
```

- ***功能***

  根据目标 TCP 位姿计算一组关节解。

- ***参数***

- `handle`：运动学句柄。
- `pose6`：长度为 6 的目标 TCP 位姿。
- `near6`：长度为 6 的种子关节位置，用于选择最接近的解。
- `out_solution6`：返回选中的关节解。
- `out_result`：返回求解器结果信息，见 [CommonTypes.cn.md](./CommonTypes.cn.md)。
- `out_success`：返回 IK 是否成功。

### 逆运动学多解

```c
elite_c_status_t elite_kinematics_get_position_ik_all(
    elite_kinematics_handle_t* handle,
    const double* pose6,
    const double* near6,
    double* out_solutions6,
    int32_t max_solutions,
    int32_t* out_solution_count,
    elite_kinematics_result_t* out_result,
    int32_t* out_success
);
```

- ***功能***

  根据目标 TCP 位姿计算多组关节解。

- ***参数***

- `out_solutions6`：输出缓冲区，大小应至少为 `max_solutions * 6` 个 `double`。只查询解数量时可传 `NULL`。
- `max_solutions`：最多复制到输出缓冲区的解数量。
- `out_solution_count`：返回求解器找到的总解数量。

### 默认超时时间

```c
elite_c_status_t elite_kinematics_set_default_timeout(elite_kinematics_handle_t* handle, double timeout);
elite_c_status_t elite_kinematics_get_default_timeout(elite_kinematics_handle_t* handle, double* out_timeout);
```

- ***功能***

  设置或读取求解器默认超时时间，单位 s。

### 错误信息接口

```c
const char* elite_kinematics_last_error_message(elite_kinematics_handle_t* handle);
const char* elite_kinematics_global_last_error_message(void);
```

- ***功能***

  获取最近一次运动学 C 包装层错误信息。

## 示例

打开 `ELITE_BUILD_EXAMPLES` 后，可运行：

```bash
./build-c/examples/kinematics_example \
  192.168.1.200 \
  /usr/local/lib/elite-cs-series-sdk-plugins/kinematics/libelite_kdl_kinematics.so
```

也可以显式指定类名：

```bash
./build-c/examples/kinematics_example \
  192.168.1.200 \
  /usr/local/lib/elite-cs-series-sdk-plugins/kinematics/libelite_kdl_kinematics.so \
  ELITE::KdlKinematicsPlugin
```

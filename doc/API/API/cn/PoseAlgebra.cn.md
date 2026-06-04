# 位姿代数模块

## 简介

位姿代数模块通过 C 包装层暴露上游 SDK 的位姿几何插件接口。该模块在运行时加载位姿代数插件，并提供 6 维位姿向量和 4x4 齐次矩阵的运算接口。

## 导入

```c
#include <Elite/PoseAlgebra_C.hpp>
```

总入口头文件也包含该模块：

```c
#include <Elite_Sdk_C.h>
```

## 插件

常用插件类名：

- `ELITE::EigenPoseAlgebra`
- `ELITE::ElitePoseAlgebra`

Linux 下常见安装路径：

```text
/usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_eigen_pose_algebra.so
/usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_pose_algebra.so
```

如果 `plugin_class_name` 传 `NULL` 或空字符串，默认使用 `ELITE::EigenPoseAlgebra`。

## 类型

以下类型见 [CommonTypes.cn.md](./CommonTypes.cn.md)：

- `elite_pose_matrix_t`
- `elite_pose_distance_t`
- `elite_pose_algebra_error_t`
- `elite_pose_algebra_result_t`

`elite_pose_matrix_t::data` 按行优先顺序存储 4x4 矩阵。

## 句柄类型

```c
typedef struct elite_pose_algebra_handle_t elite_pose_algebra_handle_t;
```

- ***功能***

  不透明句柄，内部持有插件加载器和位姿代数实例。

## 接口说明

### 创建位姿代数句柄

```c
elite_c_status_t elite_pose_algebra_create(
    const char* plugin_lib_path,
    const char* plugin_class_name,
    elite_pose_algebra_handle_t** out_handle
);
```

- ***功能***

  加载位姿代数插件库，并创建位姿代数实例。

- ***参数***

- `plugin_lib_path`：位姿代数插件动态库路径。
- `plugin_class_name`：插件类名。传 `NULL` 或空字符串时默认使用 `ELITE::EigenPoseAlgebra`。
- `out_handle`：返回创建出的位姿代数句柄。

- ***返回值***

  返回 `elite_c_status_t`。

### 销毁位姿代数句柄

```c
void elite_pose_algebra_destroy(elite_pose_algebra_handle_t* handle);
```

- ***功能***

  释放位姿代数句柄及其关联资源。

## 矩阵运算

```c
elite_c_status_t elite_pose_algebra_inverse_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose, elite_pose_matrix_t* out_inverse_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_multiply_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose, const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_add_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose, const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_subtract_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose, const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***功能***

  对 4x4 位姿矩阵进行求逆、乘法、加法或减法运算。

- ***参数***

- `handle`：位姿代数句柄。
- `pose`：输入矩阵。
- `left_pose` / `right_pose`：矩阵运算输入。
- `out_inverse_pose` / `out_pose`：输出矩阵。
- `out_result`：返回运算结果信息。
- `out_success`：返回运算是否成功。

## 向量运算

```c
elite_c_status_t elite_pose_algebra_inverse_vector(elite_pose_algebra_handle_t* handle, const double* pose6, double* out_inverse_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_multiply_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6, const double* right_pose6, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_add_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6, const double* right_pose6, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_subtract_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6, const double* right_pose6, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***功能***

  对 6 维位姿向量进行求逆、乘法、加法或减法运算。

- ***参数***

- `pose6` / `left_pose6` / `right_pose6`：长度为 6 的位姿向量。
- `out_inverse_pose6` / `out_pose6`：输出位姿向量。
- `out_result`：返回运算结果信息。
- `out_success`：返回运算是否成功。

## 矩阵和向量转换

```c
elite_c_status_t elite_pose_algebra_vector_to_matrix(elite_pose_algebra_handle_t* handle, const double* pose6, elite_pose_matrix_t* out_pose_matrix, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_matrix_to_vector(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose_matrix, double* out_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***功能***

  在 6 维位姿向量和 4x4 位姿矩阵之间转换。

## 距离计算

```c
elite_c_status_t elite_pose_algebra_distance_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose_a, const elite_pose_matrix_t* pose_b, elite_pose_distance_t* out_distance, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_distance_vector(elite_pose_algebra_handle_t* handle, const double* pose_a6, const double* pose_b6, elite_pose_distance_t* out_distance, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***功能***

  计算两个位姿之间的位置距离和姿态角距离。

## 坐标系转换

```c
elite_c_status_t elite_pose_algebra_world_to_local_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* world_ref_pose, const elite_pose_matrix_t* world_pose, elite_pose_matrix_t* out_local_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_world_to_local_vector(elite_pose_algebra_handle_t* handle, const double* world_ref_pose6, const double* world_pose6, double* out_local_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_local_to_world_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* world_ref_pose, const elite_pose_matrix_t* local_pose, elite_pose_matrix_t* out_world_pose, elite_pose_algebra_result_t* out_result, int32_t* out_success);
elite_c_status_t elite_pose_algebra_local_to_world_vector(elite_pose_algebra_handle_t* handle, const double* world_ref_pose6, const double* local_pose6, double* out_world_pose6, elite_pose_algebra_result_t* out_result, int32_t* out_success);
```

- ***功能***

  在世界坐标系和指定局部参考坐标系之间转换位姿。

## 错误信息接口

```c
const char* elite_pose_algebra_last_error_message(elite_pose_algebra_handle_t* handle);
const char* elite_pose_algebra_global_last_error_message(void);
```

- ***功能***

  获取最近一次位姿代数 C 包装层错误信息。

- ***注意事项***

  `elite_pose_algebra_result_t::message` 在同一个位姿代数句柄下一次调用前有效。

## 示例

使用 Eigen 插件运行：

```bash
./build-c/examples/pose_algebra_example \
  /usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_eigen_pose_algebra.so \
  ELITE::EigenPoseAlgebra
```

使用 Elite 插件运行：

```bash
./build-c/examples/pose_algebra_example \
  /usr/local/lib/elite-cs-series-sdk-plugins/pose-algebra/libelite_pose_algebra.so \
  ELITE::ElitePoseAlgebra
```

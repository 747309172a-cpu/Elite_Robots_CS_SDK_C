# 共享类型

## 简介

本文档说明多个模块共用的基础类型和状态码。

## 导入

```c
#include <Elite_C_Types.h>
```

## 状态码

```c
typedef enum elite_c_status_t {
    ELITE_C_STATUS_OK = 0,
    ELITE_C_STATUS_INVALID_ARGUMENT = 1,
    ELITE_C_STATUS_ALLOCATION_FAILED = 2,
    ELITE_C_STATUS_EXCEPTION = 3,
} elite_c_status_t;
```

- `ELITE_C_STATUS_OK`：接口调用成功
- `ELITE_C_STATUS_INVALID_ARGUMENT`：参数非法
- `ELITE_C_STATUS_ALLOCATION_FAILED`：内存分配失败
- `ELITE_C_STATUS_EXCEPTION`：内部异常

## 版本结构体

```c
typedef struct elite_version_info_t {
    uint32_t major;
    uint32_t minor;
    uint32_t bugfix;
    uint32_t build;
} elite_version_info_t;
```

- ***功能***

  用于描述控制器或 SDK 版本号。

## 运动与控制枚举

### 轨迹运动结果

```c
typedef enum elite_trajectory_motion_result_t {
    ELITE_TRAJECTORY_MOTION_RESULT_SUCCESS = 0,
    ELITE_TRAJECTORY_MOTION_RESULT_CANCELED = 1,
    ELITE_TRAJECTORY_MOTION_RESULT_FAILURE = 2,
} elite_trajectory_motion_result_t;
```

- `ELITE_TRAJECTORY_MOTION_RESULT_SUCCESS`：轨迹执行成功
- `ELITE_TRAJECTORY_MOTION_RESULT_CANCELED`：轨迹被取消
- `ELITE_TRAJECTORY_MOTION_RESULT_FAILURE`：轨迹执行失败

### 轨迹控制动作

```c
typedef enum elite_trajectory_control_action_t {
    ELITE_TRAJECTORY_ACTION_CANCEL = -1,
    ELITE_TRAJECTORY_ACTION_NOOP = 0,
    ELITE_TRAJECTORY_ACTION_START = 1,
} elite_trajectory_control_action_t;
```

- `ELITE_TRAJECTORY_ACTION_CANCEL`：取消当前轨迹
- `ELITE_TRAJECTORY_ACTION_NOOP`：空操作，用于保活防止超时
- `ELITE_TRAJECTORY_ACTION_START`：启动新的轨迹运动

### 轨迹反馈消息类型

```c
typedef enum elite_trajectory_feedback_message_type_t {
    ELITE_TRAJECTORY_FEEDBACK_ACTIVE_POINT = 1,
    ELITE_TRAJECTORY_FEEDBACK_POINT_DONE = 2,
    ELITE_TRAJECTORY_FEEDBACK_RESULT = 3,
} elite_trajectory_feedback_message_type_t;
```

- `ELITE_TRAJECTORY_FEEDBACK_ACTIVE_POINT`：机器人反馈当前活动轨迹点。
- `ELITE_TRAJECTORY_FEEDBACK_POINT_DONE`：机器人反馈某个轨迹点已完成。
- `ELITE_TRAJECTORY_FEEDBACK_RESULT`：机器人反馈轨迹最终结果。

### 轨迹运动反馈结构体

```c
typedef struct elite_trajectory_motion_feedback_t {
    elite_trajectory_feedback_message_type_t message_type;
    int32_t point_index;
    int32_t total_points;
    int32_t result;
    double point[6];
} elite_trajectory_motion_feedback_t;
```

- `message_type`：反馈消息类型。
- `point_index`：当前轨迹点序号。
- `total_points`：轨迹总点数。
- `result`：结果反馈帧中的结果值。
- `point`：机器人反馈的 6 维关节或笛卡尔点位。

### Freedrive 动作

```c
typedef enum elite_freedrive_action_t {
    ELITE_FREEDRIVE_END = -1,
    ELITE_FREEDRIVE_NOOP = 0,
    ELITE_FREEDRIVE_START = 1,
} elite_freedrive_action_t;
```

- `ELITE_FREEDRIVE_END`：结束 Freedrive 模式
- `ELITE_FREEDRIVE_NOOP`：保持 Freedrive 模式
- `ELITE_FREEDRIVE_START`：启动 Freedrive 模式

### 工具电压

```c
typedef enum elite_tool_voltage_t {
    ELITE_TOOL_VOLTAGE_OFF = 0,
    ELITE_TOOL_VOLTAGE_12V = 12,
    ELITE_TOOL_VOLTAGE_24V = 24,
} elite_tool_voltage_t;
```

- `ELITE_TOOL_VOLTAGE_OFF`：0V
- `ELITE_TOOL_VOLTAGE_12V`：12V
- `ELITE_TOOL_VOLTAGE_24V`：24V

### 力控模式

```c
typedef enum elite_force_mode_t {
    ELITE_FORCE_MODE_FIX = 0,
    ELITE_FORCE_MODE_POINT = 1,
    ELITE_FORCE_MODE_MOTION = 2,
    ELITE_FORCE_MODE_TCP = 3,
} elite_force_mode_t;
```

- `ELITE_FORCE_MODE_FIX`：固定模式
- `ELITE_FORCE_MODE_POINT`：点模式
- `ELITE_FORCE_MODE_MOTION`：运动模式
- `ELITE_FORCE_MODE_TCP`：TCP 模式

## 状态枚举

### 机器人模式

```c
typedef enum elite_robot_mode_t {
    ELITE_ROBOT_MODE_UNKNOWN = -2,
    ELITE_ROBOT_MODE_NO_CONTROLLER = -1,
    ELITE_ROBOT_MODE_DISCONNECTED = 0,
    ELITE_ROBOT_MODE_CONFIRM_SAFETY = 1,
    ELITE_ROBOT_MODE_BOOTING = 2,
    ELITE_ROBOT_MODE_POWER_OFF = 3,
    ELITE_ROBOT_MODE_POWER_ON = 4,
    ELITE_ROBOT_MODE_IDLE = 5,
    ELITE_ROBOT_MODE_BACKDRIVE = 6,
    ELITE_ROBOT_MODE_RUNNING = 7,
    ELITE_ROBOT_MODE_UPDATING_FIRMWARE = 8,
    ELITE_ROBOT_MODE_WAITING_CALIBRATION = 9,
} elite_robot_mode_t;
```

### 关节模式

```c
typedef enum elite_joint_mode_t {
    ELITE_JOINT_MODE_RESET = 235,
    ELITE_JOINT_MODE_SHUTTING_DOWN = 236,
    ELITE_JOINT_MODE_BACKDRIVE = 238,
    ELITE_JOINT_MODE_POWER_OFF = 239,
    ELITE_JOINT_MODE_READY_FOR_POWEROFF = 240,
    ELITE_JOINT_MODE_NOT_RESPONDING = 245,
    ELITE_JOINT_MODE_MOTOR_INITIALISATION = 246,
    ELITE_JOINT_MODE_BOOTING = 247,
    ELITE_JOINT_MODE_BOOTLOADER = 249,
    ELITE_JOINT_MODE_VIOLATION = 251,
    ELITE_JOINT_MODE_FAULT = 252,
    ELITE_JOINT_MODE_RUNNING = 253,
    ELITE_JOINT_MODE_IDLE = 255,
} elite_joint_mode_t;
```

### 安全模式

```c
typedef enum elite_safety_mode_t {
    ELITE_SAFETY_MODE_UNKNOWN = -2,
    ELITE_SAFETY_MODE_NORMAL = 1,
    ELITE_SAFETY_MODE_REDUCED = 2,
    ELITE_SAFETY_MODE_PROTECTIVE_STOP = 3,
    ELITE_SAFETY_MODE_RECOVERY = 4,
    ELITE_SAFETY_MODE_SAFEGUARD_STOP = 5,
    ELITE_SAFETY_MODE_SYSTEM_EMERGENCY_STOP = 6,
    ELITE_SAFETY_MODE_ROBOT_EMERGENCY_STOP = 7,
    ELITE_SAFETY_MODE_VIOLATION = 8,
    ELITE_SAFETY_MODE_FAULT = 9,
    ELITE_SAFETY_MODE_VALIDATE_JOINT_ID = 10,
    ELITE_SAFETY_MODE_UNDEFINED_SAFETY_MODE = 11,
    ELITE_SAFETY_MODE_AUTOMATIC_MODE_SAFEGUARD_STOP = 12,
    ELITE_SAFETY_MODE_SYSTEM_THREE_POSITION_ENABLING_STOP = 13,
    ELITE_SAFETY_MODE_TP_THREE_POSITION_ENABLING_STOP = 14,
} elite_safety_mode_t;
```

### 任务状态

```c
typedef enum elite_task_status_t {
    ELITE_TASK_STATUS_UNKNOWN = 0,
    ELITE_TASK_STATUS_PLAYING = 1,
    ELITE_TASK_STATUS_PAUSED = 2,
    ELITE_TASK_STATUS_STOPPED = 3,
} elite_task_status_t;
```

## 工具与串口枚举

### 工具模式

```c
typedef enum elite_tool_mode_t {
    ELITE_TOOL_MODE_RESET = 235,
    ELITE_TOOL_MODE_SHUTTING_DOWN = 236,
    ELITE_TOOL_MODE_POWER_OFF = 239,
    ELITE_TOOL_MODE_NOT_RESPONDING = 245,
    ELITE_TOOL_MODE_BOOTING = 247,
    ELITE_TOOL_MODE_BOOTLOADER = 249,
    ELITE_TOOL_MODE_FAULT = 252,
    ELITE_TOOL_MODE_RUNNING = 253,
    ELITE_TOOL_MODE_IDLE = 255,
} elite_tool_mode_t;
```

### 工具数字模式

```c
typedef enum elite_tool_digital_mode_t {
    ELITE_TOOL_DIGITAL_MODE_SINGLE_NEEDLE = 0,
    ELITE_TOOL_DIGITAL_MODE_DOUBLE_NEEDLE_1 = 1,
    ELITE_TOOL_DIGITAL_MODE_DOUBLE_NEEDLE_2 = 2,
    ELITE_TOOL_DIGITAL_MODE_TRIPLE_NEEDLE = 3,
} elite_tool_digital_mode_t;
```

### 工具数字输出模式

```c
typedef enum elite_tool_digital_output_mode_t {
    ELITE_TOOL_DIGITAL_OUTPUT_PUSH_PULL_MODE = 0,
    ELITE_TOOL_DIGITAL_OUTPUT_SOURCING_PNP_MODE = 1,
    ELITE_TOOL_DIGITAL_OUTPUT_SINKING_NPN_MODE = 2,
} elite_tool_digital_output_mode_t;
```

### 串口波特率

```c
typedef enum elite_serial_baud_rate_t {
    ELITE_SERIAL_BAUD_RATE_2400 = 2400,
    ELITE_SERIAL_BAUD_RATE_4800 = 4800,
    ELITE_SERIAL_BAUD_RATE_9600 = 9600,
    ELITE_SERIAL_BAUD_RATE_19200 = 19200,
    ELITE_SERIAL_BAUD_RATE_38400 = 38400,
    ELITE_SERIAL_BAUD_RATE_57600 = 57600,
    ELITE_SERIAL_BAUD_RATE_115200 = 115200,
    ELITE_SERIAL_BAUD_RATE_460800 = 460800,
    ELITE_SERIAL_BAUD_RATE_1000000 = 1000000,
    ELITE_SERIAL_BAUD_RATE_2000000 = 2000000,
} elite_serial_baud_rate_t;
```

### 串口校验位

```c
typedef enum elite_serial_parity_t {
    ELITE_SERIAL_PARITY_NONE = 0,
    ELITE_SERIAL_PARITY_ODD = 1,
    ELITE_SERIAL_PARITY_EVEN = 2,
} elite_serial_parity_t;
```

### 串口停止位

```c
typedef enum elite_serial_stop_bits_t {
    ELITE_SERIAL_STOP_BITS_ONE = 1,
    ELITE_SERIAL_STOP_BITS_TWO = 2,
} elite_serial_stop_bits_t;
```

## 运动学类型

### 运动学错误码

```c
typedef enum elite_kinematic_error_t {
    ELITE_KINEMATIC_ERROR_OK = 1,
    ELITE_KINEMATIC_ERROR_SOLVER_NOT_ACTIVE = 2,
    ELITE_KINEMATIC_ERROR_NO_SOLUTION = 3,
} elite_kinematic_error_t;
```

- `ELITE_KINEMATIC_ERROR_OK`：无错误。
- `ELITE_KINEMATIC_ERROR_SOLVER_NOT_ACTIVE`：求解器未激活。
- `ELITE_KINEMATIC_ERROR_NO_SOLUTION`：未找到有效 IK 解。

### 运动学结果结构体

```c
typedef struct elite_kinematics_result_t {
    elite_kinematic_error_t kinematic_error;
} elite_kinematics_result_t;
```

- `kinematic_error`：运动学求解结果错误码。

## 位姿代数类型

### 位姿矩阵

```c
typedef struct elite_pose_matrix_t {
    double data[16];
} elite_pose_matrix_t;
```

- ***功能***

  按行优先顺序存储的 4x4 齐次位姿矩阵。

### 位姿距离结构体

```c
typedef struct elite_pose_distance_t {
    double linear_distance;
    double angular_distance;
} elite_pose_distance_t;
```

- `linear_distance`：位置距离，单位 m。
- `angular_distance`：姿态角距离，单位 rad。

### 位姿代数错误码

```c
typedef enum elite_pose_algebra_error_t {
    ELITE_POSE_ALGEBRA_ERROR_SUCCESS = 0,
    ELITE_POSE_ALGEBRA_ERROR_INVALID_INPUT = 1,
    ELITE_POSE_ALGEBRA_ERROR_SINGULAR_MATRIX = 2,
    ELITE_POSE_ALGEBRA_ERROR_INVALID_ROTATION_MATRIX = 3,
    ELITE_POSE_ALGEBRA_ERROR_NUMERICAL_ERROR = 4,
    ELITE_POSE_ALGEBRA_ERROR_UNSUPPORTED_OPERATION = 5,
    ELITE_POSE_ALGEBRA_ERROR_INTERNAL_ERROR = 6,
} elite_pose_algebra_error_t;
```

- `ELITE_POSE_ALGEBRA_ERROR_SUCCESS`：操作成功。
- `ELITE_POSE_ALGEBRA_ERROR_INVALID_INPUT`：输入数据非法。
- `ELITE_POSE_ALGEBRA_ERROR_SINGULAR_MATRIX`：矩阵奇异，无法求逆。
- `ELITE_POSE_ALGEBRA_ERROR_INVALID_ROTATION_MATRIX`：旋转矩阵非法。
- `ELITE_POSE_ALGEBRA_ERROR_NUMERICAL_ERROR`：数值计算错误。
- `ELITE_POSE_ALGEBRA_ERROR_UNSUPPORTED_OPERATION`：不支持的操作。
- `ELITE_POSE_ALGEBRA_ERROR_INTERNAL_ERROR`：内部错误。

### 位姿代数结果结构体

```c
typedef struct elite_pose_algebra_result_t {
    elite_pose_algebra_error_t error;
    const char* message;
} elite_pose_algebra_result_t;
```

- `error`：位姿代数操作结果错误码。
- `message`：错误说明字符串，该指针在同一个位姿代数句柄下一次调用前有效。

// SPDX-License-Identifier: MIT
// Copyright (c) 2026, Elite Robots.

//
// Shared C ABI types for Elite C wrapper modules.
//c语言接口中使用的共享类型定义，供多个c接口模块使用，避免重复定义和跨模块定义不一致问题
#ifndef __ELITE_C_TYPES_H__
#define __ELITE_C_TYPES_H__

#include <stdint.h>

#if defined(_WIN32) || defined(_WIN64)
#if defined(ELITE_C_EXPORT_LIBRARY)
#define ELITE_C_EXPORT __declspec(dllexport)
#else
#define ELITE_C_EXPORT __declspec(dllimport)
#endif
#else
#define ELITE_C_EXPORT __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum elite_c_status_t {
    ELITE_C_STATUS_OK = 0,
    ELITE_C_STATUS_INVALID_ARGUMENT = 1,
    ELITE_C_STATUS_ALLOCATION_FAILED = 2,
    ELITE_C_STATUS_EXCEPTION = 3,
} elite_c_status_t;

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

typedef enum elite_tool_digital_mode_t {
    ELITE_TOOL_DIGITAL_MODE_SINGLE_NEEDLE = 0,
    ELITE_TOOL_DIGITAL_MODE_DOUBLE_NEEDLE_1 = 1,
    ELITE_TOOL_DIGITAL_MODE_DOUBLE_NEEDLE_2 = 2,
    ELITE_TOOL_DIGITAL_MODE_TRIPLE_NEEDLE = 3,
} elite_tool_digital_mode_t;

typedef enum elite_tool_digital_output_mode_t {
    ELITE_TOOL_DIGITAL_OUTPUT_PUSH_PULL_MODE = 0,
    ELITE_TOOL_DIGITAL_OUTPUT_SOURCING_PNP_MODE = 1,
    ELITE_TOOL_DIGITAL_OUTPUT_SINKING_NPN_MODE = 2,
} elite_tool_digital_output_mode_t;

typedef enum elite_task_status_t {
    ELITE_TASK_STATUS_UNKNOWN = 0,
    ELITE_TASK_STATUS_PLAYING = 1,
    ELITE_TASK_STATUS_PAUSED = 2,
    ELITE_TASK_STATUS_STOPPED = 3,
} elite_task_status_t;

typedef enum elite_trajectory_motion_result_t {
    ELITE_TRAJECTORY_MOTION_RESULT_SUCCESS = 0,
    ELITE_TRAJECTORY_MOTION_RESULT_CANCELED = 1,
    ELITE_TRAJECTORY_MOTION_RESULT_FAILURE = 2,
} elite_trajectory_motion_result_t;

typedef enum elite_trajectory_control_action_t {
    ELITE_TRAJECTORY_ACTION_CANCEL = -1,
    ELITE_TRAJECTORY_ACTION_NOOP = 0,
    ELITE_TRAJECTORY_ACTION_START = 1,
} elite_trajectory_control_action_t;

typedef enum elite_trajectory_feedback_message_type_t {
    ELITE_TRAJECTORY_FEEDBACK_ACTIVE_POINT = 1,
    ELITE_TRAJECTORY_FEEDBACK_POINT_DONE = 2,
    ELITE_TRAJECTORY_FEEDBACK_RESULT = 3,
} elite_trajectory_feedback_message_type_t;

typedef struct elite_trajectory_motion_feedback_t {
    elite_trajectory_feedback_message_type_t message_type;
    int32_t point_index;
    int32_t total_points;
    int32_t result;
    double point[6];
} elite_trajectory_motion_feedback_t;

typedef enum elite_tool_voltage_t {
    ELITE_TOOL_VOLTAGE_OFF = 0,
    ELITE_TOOL_VOLTAGE_12V = 12,
    ELITE_TOOL_VOLTAGE_24V = 24,
} elite_tool_voltage_t;

typedef enum elite_force_mode_t {
    ELITE_FORCE_MODE_FIX = 0,
    ELITE_FORCE_MODE_POINT = 1,
    ELITE_FORCE_MODE_MOTION = 2,
    ELITE_FORCE_MODE_TCP = 3,
} elite_force_mode_t;

typedef enum elite_freedrive_action_t {
    ELITE_FREEDRIVE_END = -1,
    ELITE_FREEDRIVE_NOOP = 0,
    ELITE_FREEDRIVE_START = 1,
} elite_freedrive_action_t;

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

typedef enum elite_serial_parity_t {
    ELITE_SERIAL_PARITY_NONE = 0,
    ELITE_SERIAL_PARITY_ODD = 1,
    ELITE_SERIAL_PARITY_EVEN = 2,
} elite_serial_parity_t;

typedef enum elite_serial_stop_bits_t {
    ELITE_SERIAL_STOP_BITS_ONE = 1,
    ELITE_SERIAL_STOP_BITS_TWO = 2,
} elite_serial_stop_bits_t;

typedef struct elite_version_info_t {
    uint32_t major;
    uint32_t minor;
    uint32_t bugfix;
    uint32_t build;
} elite_version_info_t;

typedef enum elite_kinematic_error_t {
    ELITE_KINEMATIC_ERROR_OK = 1,
    ELITE_KINEMATIC_ERROR_SOLVER_NOT_ACTIVE = 2,
    ELITE_KINEMATIC_ERROR_NO_SOLUTION = 3,
} elite_kinematic_error_t;

typedef struct elite_kinematics_result_t {
    elite_kinematic_error_t kinematic_error;
} elite_kinematics_result_t;

typedef struct elite_pose_matrix_t {
    double data[16];
} elite_pose_matrix_t;

typedef struct elite_pose_distance_t {
    double linear_distance;
    double angular_distance;
} elite_pose_distance_t;

typedef enum elite_pose_algebra_error_t {
    ELITE_POSE_ALGEBRA_ERROR_SUCCESS = 0,
    ELITE_POSE_ALGEBRA_ERROR_INVALID_INPUT = 1,
    ELITE_POSE_ALGEBRA_ERROR_SINGULAR_MATRIX = 2,
    ELITE_POSE_ALGEBRA_ERROR_INVALID_ROTATION_MATRIX = 3,
    ELITE_POSE_ALGEBRA_ERROR_NUMERICAL_ERROR = 4,
    ELITE_POSE_ALGEBRA_ERROR_UNSUPPORTED_OPERATION = 5,
    ELITE_POSE_ALGEBRA_ERROR_INTERNAL_ERROR = 6,
} elite_pose_algebra_error_t;

typedef struct elite_pose_algebra_result_t {
    elite_pose_algebra_error_t error;
    const char* message;
} elite_pose_algebra_result_t;

#ifdef __cplusplus
}
#endif

#endif

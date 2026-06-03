// SPDX-License-Identifier: MIT
// Copyright (c) 2026, Elite Robots.

//
// C ABI wrapper for Elite kinematics plugin APIs.
#ifndef __ELITE_KINEMATICS_C_HPP__
#define __ELITE_KINEMATICS_C_HPP__

#include <Elite_C_Types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct elite_kinematics_handle_t elite_kinematics_handle_t;

ELITE_C_EXPORT elite_c_status_t elite_kinematics_create(const char* plugin_lib_path, const char* plugin_class_name,
                                                        elite_kinematics_handle_t** out_handle);
ELITE_C_EXPORT void elite_kinematics_destroy(elite_kinematics_handle_t* handle);

ELITE_C_EXPORT elite_c_status_t elite_kinematics_set_mdh(elite_kinematics_handle_t* handle, const double* alpha6,
                                                         const double* a6, const double* d6);
ELITE_C_EXPORT elite_c_status_t elite_kinematics_get_position_fk(elite_kinematics_handle_t* handle,
                                                                 const double* joint_angles6, double* out_pose6,
                                                                 int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_kinematics_get_position_ik(elite_kinematics_handle_t* handle, const double* pose6,
                                                                 const double* near6, double* out_solution6,
                                                                 elite_kinematics_result_t* out_result,
                                                                 int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_kinematics_get_position_ik_all(elite_kinematics_handle_t* handle, const double* pose6,
                                                                     const double* near6, double* out_solutions6,
                                                                     int32_t max_solutions,
                                                                     int32_t* out_solution_count,
                                                                     elite_kinematics_result_t* out_result,
                                                                     int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_kinematics_set_default_timeout(elite_kinematics_handle_t* handle, double timeout);
ELITE_C_EXPORT elite_c_status_t elite_kinematics_get_default_timeout(elite_kinematics_handle_t* handle, double* out_timeout);

ELITE_C_EXPORT const char* elite_kinematics_last_error_message(elite_kinematics_handle_t* handle);
ELITE_C_EXPORT const char* elite_kinematics_global_last_error_message(void);

#ifdef __cplusplus
}
#endif

#endif

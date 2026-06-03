// SPDX-License-Identifier: MIT
// Copyright (c) 2026, Elite Robots.

//
// C ABI wrapper for Elite pose algebra plugin APIs.
#ifndef __ELITE_POSE_ALGEBRA_C_HPP__
#define __ELITE_POSE_ALGEBRA_C_HPP__

#include <Elite_C_Types.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct elite_pose_algebra_handle_t elite_pose_algebra_handle_t;

ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_create(const char* plugin_lib_path, const char* plugin_class_name,
                                                          elite_pose_algebra_handle_t** out_handle);
ELITE_C_EXPORT void elite_pose_algebra_destroy(elite_pose_algebra_handle_t* handle);

ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_inverse_matrix(elite_pose_algebra_handle_t* handle,
                                                                  const elite_pose_matrix_t* pose,
                                                                  elite_pose_matrix_t* out_inverse_pose,
                                                                  elite_pose_algebra_result_t* out_result,
                                                                  int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_inverse_vector(elite_pose_algebra_handle_t* handle, const double* pose6,
                                                                  double* out_inverse_pose6,
                                                                  elite_pose_algebra_result_t* out_result,
                                                                  int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_multiply_matrix(elite_pose_algebra_handle_t* handle,
                                                                   const elite_pose_matrix_t* left_pose,
                                                                   const elite_pose_matrix_t* right_pose,
                                                                   elite_pose_matrix_t* out_pose,
                                                                   elite_pose_algebra_result_t* out_result,
                                                                   int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_multiply_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6,
                                                                   const double* right_pose6, double* out_pose6,
                                                                   elite_pose_algebra_result_t* out_result,
                                                                   int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_add_matrix(elite_pose_algebra_handle_t* handle,
                                                              const elite_pose_matrix_t* left_pose,
                                                              const elite_pose_matrix_t* right_pose,
                                                              elite_pose_matrix_t* out_pose,
                                                              elite_pose_algebra_result_t* out_result,
                                                              int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_add_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6,
                                                              const double* right_pose6, double* out_pose6,
                                                              elite_pose_algebra_result_t* out_result,
                                                              int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_subtract_matrix(elite_pose_algebra_handle_t* handle,
                                                                   const elite_pose_matrix_t* left_pose,
                                                                   const elite_pose_matrix_t* right_pose,
                                                                   elite_pose_matrix_t* out_pose,
                                                                   elite_pose_algebra_result_t* out_result,
                                                                   int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_subtract_vector(elite_pose_algebra_handle_t* handle,
                                                                   const double* left_pose6,
                                                                   const double* right_pose6, double* out_pose6,
                                                                   elite_pose_algebra_result_t* out_result,
                                                                   int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_vector_to_matrix(elite_pose_algebra_handle_t* handle, const double* pose6,
                                                                    elite_pose_matrix_t* out_pose_matrix,
                                                                    elite_pose_algebra_result_t* out_result,
                                                                    int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_matrix_to_vector(elite_pose_algebra_handle_t* handle,
                                                                    const elite_pose_matrix_t* pose_matrix,
                                                                    double* out_pose6,
                                                                    elite_pose_algebra_result_t* out_result,
                                                                    int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_distance_matrix(elite_pose_algebra_handle_t* handle,
                                                                   const elite_pose_matrix_t* pose_a,
                                                                   const elite_pose_matrix_t* pose_b,
                                                                   elite_pose_distance_t* out_distance,
                                                                   elite_pose_algebra_result_t* out_result,
                                                                   int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_distance_vector(elite_pose_algebra_handle_t* handle, const double* pose_a6,
                                                                   const double* pose_b6,
                                                                   elite_pose_distance_t* out_distance,
                                                                   elite_pose_algebra_result_t* out_result,
                                                                   int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_world_to_local_matrix(elite_pose_algebra_handle_t* handle,
                                                                         const elite_pose_matrix_t* world_ref_pose,
                                                                         const elite_pose_matrix_t* world_pose,
                                                                         elite_pose_matrix_t* out_local_pose,
                                                                         elite_pose_algebra_result_t* out_result,
                                                                         int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_world_to_local_vector(elite_pose_algebra_handle_t* handle,
                                                                         const double* world_ref_pose6,
                                                                         const double* world_pose6,
                                                                         double* out_local_pose6,
                                                                         elite_pose_algebra_result_t* out_result,
                                                                         int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_local_to_world_matrix(elite_pose_algebra_handle_t* handle,
                                                                         const elite_pose_matrix_t* world_ref_pose,
                                                                         const elite_pose_matrix_t* local_pose,
                                                                         elite_pose_matrix_t* out_world_pose,
                                                                         elite_pose_algebra_result_t* out_result,
                                                                         int32_t* out_success);
ELITE_C_EXPORT elite_c_status_t elite_pose_algebra_local_to_world_vector(elite_pose_algebra_handle_t* handle,
                                                                         const double* world_ref_pose6,
                                                                         const double* local_pose6,
                                                                         double* out_world_pose6,
                                                                         elite_pose_algebra_result_t* out_result,
                                                                         int32_t* out_success);

ELITE_C_EXPORT const char* elite_pose_algebra_last_error_message(elite_pose_algebra_handle_t* handle);
ELITE_C_EXPORT const char* elite_pose_algebra_global_last_error_message(void);

#ifdef __cplusplus
}
#endif

#endif

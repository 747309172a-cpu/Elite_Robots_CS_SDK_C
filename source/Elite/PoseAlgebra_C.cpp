// SPDX-License-Identifier: MIT
// Copyright (c) 2026, Elite Robots.

#include <Elite/ClassLoader.hpp>
#include <Elite/PoseAlgebraBase.hpp>
#include <Elite/PoseAlgebra_C.hpp>

#include <memory>
#include <mutex>
#include <new>
#include <string>

struct elite_pose_algebra_handle_t {
    std::unique_ptr<ELITE::ClassLoader> loader;
    std::unique_ptr<ELITE::PoseAlgebraBase> solver;
    std::string last_error;
    std::string last_result_message;
    std::mutex mutex;
};

namespace {
thread_local std::string g_last_error;

inline void set_global_error(const std::string& msg) { g_last_error = msg; }

inline void set_handle_error(elite_pose_algebra_handle_t* handle, const std::string& msg) {
    if (!handle) {
        set_global_error(msg);
        return;
    }
    std::lock_guard<std::mutex> lock(handle->mutex);
    handle->last_error = msg;
    g_last_error = msg;
}

inline ELITE::vector6d_t to_vector6d(const double* data6) {
    ELITE::vector6d_t out{};
    for (size_t i = 0; i < out.size(); ++i) {
        out[i] = data6[i];
    }
    return out;
}

inline void copy_vector6_to_buffer(const ELITE::vector6d_t& src, double* dst6) {
    for (size_t i = 0; i < src.size(); ++i) {
        dst6[i] = src[i];
    }
}

inline ELITE::PoseMatrix to_pose_matrix(const elite_pose_matrix_t* in) {
    ELITE::PoseMatrix out{};
    for (size_t row = 0; row < 4; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            out.data[row][col] = in->data[row * 4 + col];
        }
    }
    return out;
}

inline void copy_pose_matrix_to_buffer(const ELITE::PoseMatrix& in, elite_pose_matrix_t* out) {
    for (size_t row = 0; row < 4; ++row) {
        for (size_t col = 0; col < 4; ++col) {
            out->data[row * 4 + col] = in.data[row][col];
        }
    }
}

inline void copy_pose_distance_to_buffer(const ELITE::PoseDistance& in, elite_pose_distance_t* out) {
    out->linear_distance = in.linear_distance;
    out->angular_distance = in.angular_distance;
}

inline void fill_pose_result(elite_pose_algebra_handle_t* handle, const ELITE::PoseAlgebraResult& in,
                             elite_pose_algebra_result_t* out_result) {
    if (!out_result) {
        return;
    }
    std::lock_guard<std::mutex> lock(handle->mutex);
    handle->last_result_message = in.message;
    out_result->error = static_cast<elite_pose_algebra_error_t>(in.error);
    out_result->message = handle->last_result_message.c_str();
}

template <typename Fn>
elite_c_status_t run_with_handle(elite_pose_algebra_handle_t* handle, Fn&& fn) {
    if (!handle) {
        set_global_error("handle is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }
    if (!handle->solver) {
        set_handle_error(handle, "pose algebra solver is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    try {
        fn();
        return ELITE_C_STATUS_OK;
    } catch (const std::exception& e) {
        set_handle_error(handle, e.what());
        return ELITE_C_STATUS_EXCEPTION;
    } catch (...) {
        set_handle_error(handle, "unknown exception");
        return ELITE_C_STATUS_EXCEPTION;
    }
}
}  // namespace

elite_c_status_t elite_pose_algebra_create(const char* plugin_lib_path, const char* plugin_class_name,
                                           elite_pose_algebra_handle_t** out_handle) {
    if (!plugin_lib_path || !out_handle) {
        set_global_error("plugin_lib_path or out_handle is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    try {
        auto handle = std::make_unique<elite_pose_algebra_handle_t>();
        handle->loader = std::make_unique<ELITE::ClassLoader>(plugin_lib_path);
        if (!handle->loader->loadLib()) {
            set_global_error("failed to load pose algebra plugin library");
            return ELITE_C_STATUS_EXCEPTION;
        }

        const std::string class_name =
            (plugin_class_name && plugin_class_name[0] != '\0') ? plugin_class_name : "ELITE::EigenPoseAlgebra";
        handle->solver = handle->loader->createUniqueInstance<ELITE::PoseAlgebraBase>(class_name);
        if (!handle->solver) {
            set_global_error("failed to create pose algebra plugin instance");
            return ELITE_C_STATUS_EXCEPTION;
        }

        *out_handle = handle.release();
        set_global_error("");
        return ELITE_C_STATUS_OK;
    } catch (const std::bad_alloc&) {
        set_global_error("allocation failed");
        return ELITE_C_STATUS_ALLOCATION_FAILED;
    } catch (const std::exception& e) {
        set_global_error(e.what());
        return ELITE_C_STATUS_EXCEPTION;
    } catch (...) {
        set_global_error("unknown exception");
        return ELITE_C_STATUS_EXCEPTION;
    }
}

void elite_pose_algebra_destroy(elite_pose_algebra_handle_t* handle) { delete handle; }

elite_c_status_t elite_pose_algebra_inverse_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose,
                                                   elite_pose_matrix_t* out_inverse_pose,
                                                   elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!pose || !out_inverse_pose || !out_success) {
        set_global_error("pose, out_inverse_pose, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->inverse(to_pose_matrix(pose), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_inverse_pose);
        }
    });
}

elite_c_status_t elite_pose_algebra_inverse_vector(elite_pose_algebra_handle_t* handle, const double* pose6,
                                                   double* out_inverse_pose6, elite_pose_algebra_result_t* out_result,
                                                   int32_t* out_success) {
    if (!pose6 || !out_inverse_pose6 || !out_success) {
        set_global_error("pose6, out_inverse_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->inverse(to_vector6d(pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_inverse_pose6);
        }
    });
}

elite_c_status_t elite_pose_algebra_multiply_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose,
                                                    const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose,
                                                    elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!left_pose || !right_pose || !out_pose || !out_success) {
        set_global_error("left_pose, right_pose, out_pose, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->multiply(to_pose_matrix(left_pose), to_pose_matrix(right_pose), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_pose);
        }
    });
}

elite_c_status_t elite_pose_algebra_multiply_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6,
                                                    const double* right_pose6, double* out_pose6,
                                                    elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!left_pose6 || !right_pose6 || !out_pose6 || !out_success) {
        set_global_error("left_pose6, right_pose6, out_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->multiply(to_vector6d(left_pose6), to_vector6d(right_pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_pose6);
        }
    });
}

elite_c_status_t elite_pose_algebra_add_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose,
                                               const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose,
                                               elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!left_pose || !right_pose || !out_pose || !out_success) {
        set_global_error("left_pose, right_pose, out_pose, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->add(to_pose_matrix(left_pose), to_pose_matrix(right_pose), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_pose);
        }
    });
}

elite_c_status_t elite_pose_algebra_add_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6,
                                               const double* right_pose6, double* out_pose6,
                                               elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!left_pose6 || !right_pose6 || !out_pose6 || !out_success) {
        set_global_error("left_pose6, right_pose6, out_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->add(to_vector6d(left_pose6), to_vector6d(right_pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_pose6);
        }
    });
}

elite_c_status_t elite_pose_algebra_subtract_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* left_pose,
                                                    const elite_pose_matrix_t* right_pose, elite_pose_matrix_t* out_pose,
                                                    elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!left_pose || !right_pose || !out_pose || !out_success) {
        set_global_error("left_pose, right_pose, out_pose, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->subtract(to_pose_matrix(left_pose), to_pose_matrix(right_pose), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_pose);
        }
    });
}

elite_c_status_t elite_pose_algebra_subtract_vector(elite_pose_algebra_handle_t* handle, const double* left_pose6,
                                                    const double* right_pose6, double* out_pose6,
                                                    elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!left_pose6 || !right_pose6 || !out_pose6 || !out_success) {
        set_global_error("left_pose6, right_pose6, out_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->subtract(to_vector6d(left_pose6), to_vector6d(right_pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_pose6);
        }
    });
}

elite_c_status_t elite_pose_algebra_vector_to_matrix(elite_pose_algebra_handle_t* handle, const double* pose6,
                                                     elite_pose_matrix_t* out_pose_matrix,
                                                     elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!pose6 || !out_pose_matrix || !out_success) {
        set_global_error("pose6, out_pose_matrix, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->vectorToMatrix(to_vector6d(pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_pose_matrix);
        }
    });
}

elite_c_status_t elite_pose_algebra_matrix_to_vector(elite_pose_algebra_handle_t* handle,
                                                     const elite_pose_matrix_t* pose_matrix, double* out_pose6,
                                                     elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!pose_matrix || !out_pose6 || !out_success) {
        set_global_error("pose_matrix, out_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->matrixToVector(to_pose_matrix(pose_matrix), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_pose6);
        }
    });
}

elite_c_status_t elite_pose_algebra_distance_matrix(elite_pose_algebra_handle_t* handle, const elite_pose_matrix_t* pose_a,
                                                    const elite_pose_matrix_t* pose_b,
                                                    elite_pose_distance_t* out_distance,
                                                    elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!pose_a || !pose_b || !out_distance || !out_success) {
        set_global_error("pose_a, pose_b, out_distance, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseDistance out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->distance(to_pose_matrix(pose_a), to_pose_matrix(pose_b), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_distance_to_buffer(out, out_distance);
        }
    });
}

elite_c_status_t elite_pose_algebra_distance_vector(elite_pose_algebra_handle_t* handle, const double* pose_a6,
                                                    const double* pose_b6, elite_pose_distance_t* out_distance,
                                                    elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!pose_a6 || !pose_b6 || !out_distance || !out_success) {
        set_global_error("pose_a6, pose_b6, out_distance, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseDistance out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->distance(to_vector6d(pose_a6), to_vector6d(pose_b6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_distance_to_buffer(out, out_distance);
        }
    });
}

elite_c_status_t elite_pose_algebra_world_to_local_matrix(elite_pose_algebra_handle_t* handle,
                                                          const elite_pose_matrix_t* world_ref_pose,
                                                          const elite_pose_matrix_t* world_pose,
                                                          elite_pose_matrix_t* out_local_pose,
                                                          elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!world_ref_pose || !world_pose || !out_local_pose || !out_success) {
        set_global_error("world_ref_pose, world_pose, out_local_pose, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success =
            handle->solver->worldToLocal(to_pose_matrix(world_ref_pose), to_pose_matrix(world_pose), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_local_pose);
        }
    });
}

elite_c_status_t elite_pose_algebra_world_to_local_vector(elite_pose_algebra_handle_t* handle,
                                                          const double* world_ref_pose6, const double* world_pose6,
                                                          double* out_local_pose6,
                                                          elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!world_ref_pose6 || !world_pose6 || !out_local_pose6 || !out_success) {
        set_global_error("world_ref_pose6, world_pose6, out_local_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->worldToLocal(to_vector6d(world_ref_pose6), to_vector6d(world_pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_local_pose6);
        }
    });
}

elite_c_status_t elite_pose_algebra_local_to_world_matrix(elite_pose_algebra_handle_t* handle,
                                                          const elite_pose_matrix_t* world_ref_pose,
                                                          const elite_pose_matrix_t* local_pose,
                                                          elite_pose_matrix_t* out_world_pose,
                                                          elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!world_ref_pose || !local_pose || !out_world_pose || !out_success) {
        set_global_error("world_ref_pose, local_pose, out_world_pose, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::PoseMatrix out{};
        ELITE::PoseAlgebraResult result;
        *out_success =
            handle->solver->localToWorld(to_pose_matrix(world_ref_pose), to_pose_matrix(local_pose), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_pose_matrix_to_buffer(out, out_world_pose);
        }
    });
}

elite_c_status_t elite_pose_algebra_local_to_world_vector(elite_pose_algebra_handle_t* handle,
                                                          const double* world_ref_pose6, const double* local_pose6,
                                                          double* out_world_pose6,
                                                          elite_pose_algebra_result_t* out_result, int32_t* out_success) {
    if (!world_ref_pose6 || !local_pose6 || !out_world_pose6 || !out_success) {
        set_global_error("world_ref_pose6, local_pose6, out_world_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t out{};
        ELITE::PoseAlgebraResult result;
        *out_success = handle->solver->localToWorld(to_vector6d(world_ref_pose6), to_vector6d(local_pose6), out, result) ? 1 : 0;
        fill_pose_result(handle, result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(out, out_world_pose6);
        }
    });
}

const char* elite_pose_algebra_last_error_message(elite_pose_algebra_handle_t* handle) {
    if (!handle) {
        return g_last_error.c_str();
    }
    std::lock_guard<std::mutex> lock(handle->mutex);
    return handle->last_error.c_str();
}

const char* elite_pose_algebra_global_last_error_message(void) { return g_last_error.c_str(); }

// SPDX-License-Identifier: MIT
// Copyright (c) 2026, Elite Robots.

#include <Elite/ClassLoader.hpp>
#include <Elite/KinematicsBase.hpp>
#include <Elite/Kinematics_C.hpp>

#include <algorithm>
#include <cstring>
#include <memory>
#include <mutex>
#include <new>
#include <string>
#include <vector>

struct elite_kinematics_handle_t {
    std::unique_ptr<ELITE::ClassLoader> loader;
    std::unique_ptr<ELITE::KinematicsBase> solver;
    std::string last_error;
    std::mutex mutex;
};

namespace {
thread_local std::string g_last_error;

inline void set_global_error(const std::string& msg) { g_last_error = msg; }

inline void set_handle_error(elite_kinematics_handle_t* handle, const std::string& msg) {
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

inline void fill_kinematics_result(const ELITE::KinematicsResult& in, elite_kinematics_result_t* out_result) {
    if (!out_result) {
        return;
    }
    out_result->kinematic_error = static_cast<elite_kinematic_error_t>(in.kinematic_error);
}

template <typename Fn>
elite_c_status_t run_with_handle(elite_kinematics_handle_t* handle, Fn&& fn) {
    if (!handle) {
        set_global_error("handle is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }
    if (!handle->solver) {
        set_handle_error(handle, "kinematics solver is null");
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

elite_c_status_t elite_kinematics_create(const char* plugin_lib_path, const char* plugin_class_name,
                                         elite_kinematics_handle_t** out_handle) {
    if (!plugin_lib_path || !out_handle) {
        set_global_error("plugin_lib_path or out_handle is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    try {
        auto handle = std::make_unique<elite_kinematics_handle_t>();
        handle->loader = std::make_unique<ELITE::ClassLoader>(plugin_lib_path);
        if (!handle->loader->loadLib()) {
            set_global_error("failed to load kinematics plugin library");
            return ELITE_C_STATUS_EXCEPTION;
        }

        const std::string class_name =
            (plugin_class_name && plugin_class_name[0] != '\0') ? plugin_class_name : "ELITE::KdlKinematicsPlugin";
        handle->solver = handle->loader->createUniqueInstance<ELITE::KinematicsBase>(class_name);
        if (!handle->solver) {
            set_global_error("failed to create kinematics plugin instance");
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

void elite_kinematics_destroy(elite_kinematics_handle_t* handle) { delete handle; }

elite_c_status_t elite_kinematics_set_mdh(elite_kinematics_handle_t* handle, const double* alpha6, const double* a6,
                                          const double* d6) {
    if (!alpha6 || !a6 || !d6) {
        set_global_error("alpha6, a6, or d6 is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        handle->solver->setMDH(to_vector6d(alpha6), to_vector6d(a6), to_vector6d(d6));
    });
}

elite_c_status_t elite_kinematics_get_position_fk(elite_kinematics_handle_t* handle, const double* joint_angles6,
                                                  double* out_pose6, int32_t* out_success) {
    if (!joint_angles6 || !out_pose6 || !out_success) {
        set_global_error("joint_angles6, out_pose6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t pose{};
        *out_success = handle->solver->getPositionFK(to_vector6d(joint_angles6), pose) ? 1 : 0;
        if (*out_success) {
            copy_vector6_to_buffer(pose, out_pose6);
        }
    });
}

elite_c_status_t elite_kinematics_get_position_ik(elite_kinematics_handle_t* handle, const double* pose6, const double* near6,
                                                  double* out_solution6, elite_kinematics_result_t* out_result,
                                                  int32_t* out_success) {
    if (!pose6 || !near6 || !out_solution6 || !out_success) {
        set_global_error("pose6, near6, out_solution6, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        ELITE::vector6d_t solution{};
        ELITE::KinematicsResult result{};
        *out_success = handle->solver->getPositionIK(to_vector6d(pose6), to_vector6d(near6), solution, result) ? 1 : 0;
        fill_kinematics_result(result, out_result);
        if (*out_success) {
            copy_vector6_to_buffer(solution, out_solution6);
        }
    });
}

elite_c_status_t elite_kinematics_get_position_ik_all(elite_kinematics_handle_t* handle, const double* pose6,
                                                      const double* near6, double* out_solutions6, int32_t max_solutions,
                                                      int32_t* out_solution_count,
                                                      elite_kinematics_result_t* out_result, int32_t* out_success) {
    if (!pose6 || !near6 || !out_solution_count || !out_success) {
        set_global_error("pose6, near6, out_solution_count, or out_success is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }
    if (max_solutions < 0) {
        set_global_error("max_solutions is negative");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }

    return run_with_handle(handle, [&]() {
        std::vector<ELITE::vector6d_t> solutions;
        ELITE::KinematicsResult result{};
        *out_success = handle->solver->getPositionIK(to_vector6d(pose6), to_vector6d(near6), solutions, result) ? 1 : 0;
        fill_kinematics_result(result, out_result);
        *out_solution_count = static_cast<int32_t>(solutions.size());

        if (!out_solutions6 || max_solutions == 0) {
            return;
        }

        const int32_t copy_count = std::min(max_solutions, *out_solution_count);
        for (int32_t i = 0; i < copy_count; ++i) {
            copy_vector6_to_buffer(solutions[static_cast<size_t>(i)], out_solutions6 + static_cast<size_t>(i) * 6);
        }
    });
}

elite_c_status_t elite_kinematics_set_default_timeout(elite_kinematics_handle_t* handle, double timeout) {
    return run_with_handle(handle, [&]() { handle->solver->setDefaultTimeout(timeout); });
}

elite_c_status_t elite_kinematics_get_default_timeout(elite_kinematics_handle_t* handle, double* out_timeout) {
    if (!out_timeout) {
        set_global_error("out_timeout is null");
        return ELITE_C_STATUS_INVALID_ARGUMENT;
    }
    return run_with_handle(handle, [&]() { *out_timeout = handle->solver->getDefaultTimeout(); });
}

const char* elite_kinematics_last_error_message(elite_kinematics_handle_t* handle) {
    if (!handle) {
        return g_last_error.c_str();
    }
    std::lock_guard<std::mutex> lock(handle->mutex);
    return handle->last_error.c_str();
}

const char* elite_kinematics_global_last_error_message(void) { return g_last_error.c_str(); }

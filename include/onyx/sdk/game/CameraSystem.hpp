// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_GAME_CAMERA_SYSTEM_HPP
#define ONYX_SDK_GAME_CAMERA_SYSTEM_HPP

#include <onyx/sdk/engine/Camera.hpp>
#include <onyx/sdk/Offsets.hpp>
#include <imgui.h>
#include <cmath>

namespace onyx::sdk::game {

    using Vector3 = ::onyx::sdk::Vector3;
    using Matrix4x4 = ::onyx::sdk::Matrix4x4;

/// @brief 相机系统的顶级包装器。
struct CameraSystem : engine::Object {

    engine::Camera main_camera;

    CameraSystem() noexcept : engine::Object(static_cast<uintptr_t>(0)), main_camera() {}

    explicit CameraSystem(uintptr_t addr) noexcept 
        : engine::Object(addr)
        , main_camera() 
    {

        if (is_valid()) {

            main_camera = engine::Camera(read_field<uintptr_t>(offsets::camera_system::k_m_camera));

        }

    }

    /// @brief 兼容性方法：获取视图矩阵。
    [[nodiscard]] Matrix4x4 view_matrix() const noexcept {

        return main_camera.view_matrix;

    }

    /// @brief 兼容性方法：获取相机位置。
    [[nodiscard]] Vector3 position() const noexcept {

        return main_camera.position;

    }

    /// @brief 使用缓存矩阵将世界坐标转换为屏幕空间。
    [[nodiscard]] bool world_to_screen(const Vector3& world_pos, ImVec2& out_screen, float screen_width, float screen_height) const noexcept {

        if (!main_camera.is_valid()) return false;

        const auto& vm = main_camera.view_matrix;

        float w = vm.m30 * world_pos.x + vm.m31 * world_pos.y + vm.m32 * world_pos.z + vm.m33;

        if (w < 0.001f) return false;

        float invW = 1.0f / w;
        float x = (vm.m00 * world_pos.x + vm.m01 * world_pos.y + vm.m02 * world_pos.z + vm.m03) * invW;
        float y = (vm.m10 * world_pos.x + vm.m11 * world_pos.y + vm.m12 * world_pos.z + vm.m13) * invW;

        out_screen.x = (screen_width * 0.5f) * (1.0f + x);
        out_screen.y = (screen_height * 0.5f) * (1.0f - y);

        if (std::isnan(out_screen.x) || std::isnan(out_screen.y) || std::isinf(out_screen.x) || std::isinf(out_screen.y)) {

            return false;

        }

        return true;

    }

};

} // namespace onyx::sdk::game

#endif // ONYX_SDK_GAME_CAMERA_SYSTEM_HPP

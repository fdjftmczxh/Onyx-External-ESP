// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_CAMERA_HPP
#define ONYX_SDK_ENGINE_CAMERA_HPP

#include <onyx/sdk/engine/Component.hpp>
#include <onyx/sdk/engine/Transform.hpp>
#include <onyx/sdk/engine/structures/Matrix4x4.hpp>
#include <onyx/sdk/Offsets.hpp>

namespace onyx::sdk::engine {

/// @brief Unity Camera 组件的原生包装器。
struct Camera : Component {

    Matrix4x4 view_matrix;
    Vector3   position;

    Camera() noexcept : Component(0), view_matrix{}, position{} {}

    explicit Camera(uintptr_t addr) noexcept 
        : Component(addr)
        , view_matrix{}
        , position{} 
    {

        if (is_valid()) {

            const auto native_camera = read_field<uintptr_t>(offsets::unity_object::k_cached_ptr);

            if (utils::Memory::is_valid_ptr(native_camera)) {

                view_matrix = utils::Memory::read_safe<Matrix4x4>(native_camera + offsets::unity_camera::k_view_matrix);
                
                const auto game_object = utils::Memory::read_safe<uintptr_t>(native_camera + offsets::component::k_game_object);
                const auto component_array = utils::Memory::read_safe<uintptr_t>(game_object + offsets::game_object::k_component_array);
                const auto transform_native = utils::Memory::read_safe<uintptr_t>(component_array + offsets::component::k_transform_in_array);

                if (utils::Memory::is_valid_ptr(transform_native)) {

                    position = Transform(transform_native).position();

                }

            }

        }

    }

};

} // namespace onyx::sdk::engine

#endif // ONYX_SDK_ENGINE_CAMERA_HPP

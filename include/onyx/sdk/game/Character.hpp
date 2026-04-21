// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_GAME_CHARACTER_HPP
#define ONYX_SDK_GAME_CHARACTER_HPP

#include <onyx/sdk/engine/Component.hpp>
#include <onyx/sdk/engine/Transform.hpp>
#include <onyx/sdk/engine/GameObject.hpp>
#include <onyx/sdk/engine/structures/Vector2.hpp>
#include <onyx/sdk/Offsets.hpp>

namespace onyx::sdk::game {

/// @brief 游戏角色/玩家的包装器。
struct Character : engine::Component {

    engine::GameObject game_object;
    engine::Transform  transform;

    int32_t            health;

    uintptr_t          rotation_ptr;

    sdk::Vector2       view_angles;

    explicit Character(uintptr_t addr) noexcept : engine::Component(addr), game_object(static_cast<uintptr_t>(0)), transform(static_cast<uintptr_t>(0)), health(0), rotation_ptr(0), view_angles{}
    {

        if (is_valid()) {

            game_object = engine::GameObject(game_object_addr());

            if (game_object.is_valid()) {

                transform = game_object.transform;

            }

            health      = read_field<int32_t>(offsets::character::k_health);
            
            rotation_ptr = read_field<uintptr_t>(offsets::character::k_rotation_ptr);

            if (utils::Memory::is_valid_ptr(rotation_ptr)) {

                view_angles = utils::Memory::read_safe<sdk::Vector2>(rotation_ptr + offsets::rotation_control::k_angles);

            }

        }

    }

    /// @brief 设置角色的视角角度。
    void set_view_angles(const sdk::Vector2& angles) const noexcept {

        utils::Memory::write_safe<sdk::Vector2>(rotation_ptr + offsets::rotation_control::k_angles, angles);

    }

};

} // namespace onyx::sdk::game

#endif // ONYX_SDK_GAME_CHARACTER_HPP

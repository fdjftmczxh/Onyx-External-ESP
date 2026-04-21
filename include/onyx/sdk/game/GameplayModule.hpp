// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_GAME_GAMEPLAY_MODULE_HPP
#define ONYX_SDK_GAME_GAMEPLAY_MODULE_HPP

#include <onyx/sdk/game/GameSystem.hpp>
#include <onyx/sdk/game/CameraSystem.hpp>
#include <onyx/sdk/Offsets.hpp>

namespace onyx::sdk::game {

/// @brief 游戏的主模块入口包装器。
struct GameplayModule : engine::Object {

    GameSystem   game_system;
    CameraSystem camera_system;

    explicit GameplayModule(uintptr_t addr) noexcept : engine::Object(addr), game_system(static_cast<uintptr_t>(0)), camera_system(static_cast<uintptr_t>(0)) {

        if (is_valid()) {

            game_system   = GameSystem(utils::Memory::read_safe<uintptr_t>(address + offsets::gameplay_module::k_game_system));
            camera_system = CameraSystem(utils::Memory::read_safe<uintptr_t>(address + offsets::gameplay_module::k_camera_system));

        }

    }

    /// @brief 获取单例实例 (使用全局基址)。
    [[nodiscard]] static GameplayModule instance() noexcept {

        if (!utils::Memory::s_base_address) return GameplayModule(static_cast<uintptr_t>(0));

        uintptr_t type_info = utils::Memory::read_safe<uintptr_t>(utils::Memory::s_base_address + offsets::k_type_info_gameplay_module);

        if (!type_info) return GameplayModule(static_cast<uintptr_t>(0));

        uintptr_t static_fields = utils::Memory::read_safe<uintptr_t>(type_info + 0xb8);

        if (!static_fields) return GameplayModule(static_cast<uintptr_t>(0));

        return GameplayModule(utils::Memory::read_safe<uintptr_t>(static_fields + 0x0));

    }

};

} // namespace onyx::sdk::game

#endif // ONYX_SDK_GAME_GAMEPLAY_MODULE_HPP

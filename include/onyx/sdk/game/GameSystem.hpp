// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_GAME_GAME_SYSTEM_HPP
#define ONYX_SDK_GAME_GAME_SYSTEM_HPP

#include <onyx/sdk/engine/Object.hpp>
#include <onyx/sdk/engine/structures/Il2CppDictionary.hpp>
#include <onyx/sdk/Offsets.hpp>

namespace onyx::sdk::game {

/// @brief 游戏系统的包装器，管理玩家集合。
struct GameSystem : engine::Object {

    uintptr_t player_dict;
    int32_t   local_player_id;

    explicit GameSystem(uintptr_t addr) noexcept : engine::Object(addr), player_dict(0), local_player_id(-1) {

        if (is_valid()) {

            player_dict = read_field<uintptr_t>(offsets::game_system::k_all_players_dict);
            
            uintptr_t inter = read_field<uintptr_t>(offsets::game_system::k_local_player_interface);

            if (utils::Memory::is_valid_ptr(inter)) {

                if (utils::Memory::read_safe<bool>(inter + 0x38)) {

                    local_player_id = 1;

                } else {

                    uintptr_t l1 = utils::Memory::read_safe<uintptr_t>(inter + 0x10);

                    if (utils::Memory::is_valid_ptr(l1)) {

                        uintptr_t l2 = utils::Memory::read_safe<uintptr_t>(l1 + 0x60);

                        if (utils::Memory::is_valid_ptr(l2)) {

                            uintptr_t l3 = utils::Memory::read_safe<uintptr_t>(l2 + 0x10);

                            if (utils::Memory::is_valid_ptr(l3)) {

                                local_player_id = utils::Memory::read_safe<int32_t>(l3 + 0x10);

                            }

                        }

                    }

                }

            }

        }

    }

};

} // namespace onyx::sdk::game

#endif // ONYX_SDK_GAME_GAME_SYSTEM_HPP

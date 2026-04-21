// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_GAME_LOCAL_PLAYER_HPP
#define ONYX_SDK_GAME_LOCAL_PLAYER_HPP

#include <onyx/sdk/game/Character.hpp>

namespace onyx::sdk::game {

/// @brief 代表当前本地玩家的类。
struct LocalPlayer : Character {

    LocalPlayer() noexcept : Character(0) {}

    explicit LocalPlayer(uintptr_t address) noexcept : Character(address) {}

};

} // namespace onyx::sdk::game

#endif // ONYX_SDK_GAME_LOCAL_PLAYER_HPP

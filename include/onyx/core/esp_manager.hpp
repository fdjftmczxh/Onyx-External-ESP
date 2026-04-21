// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_CORE_ESP_MANAGER_HPP
#define ONYX_CORE_ESP_MANAGER_HPP

#include <onyx/sdk/engine/structures/Vector3.hpp>
#include <onyx/sdk/game/CameraSystem.hpp>
#include <onyx/sdk/game/LocalPlayer.hpp>
#include <vector>
#include <mutex>
#include <imgui.h>

namespace onyx::core {

    namespace sdk = ::onyx::sdk;

/// @brief 单个骨骼在屏幕空间的投影数据。
struct ScreenBone {

    ImVec2  screen_pos;
    int32_t parent_index{-1};

    bool    is_valid{false};

};

constexpr int32_t k_max_bones = 128;

struct PlayerData {

    sdk::Vector3 world_pos;
    ImVec2       screen_pos;

    bool         is_visible{false};

    // 骨骼数据
    ScreenBone bones[k_max_bones];

    int32_t    bone_count{0};

    // 动态包围盒 (从骨骼计算)
    sdk::Vector3 aabb_min;
    sdk::Vector3 aabb_max;

    bool         has_bounds{false};
    int32_t      health{0};

    float        distance{0.0f};

};

class EspManager final {
public:

    EspManager() = default;
    ~EspManager() = default;

    EspManager(const EspManager&) = delete;
    EspManager& operator=(const EspManager&) = delete;

    /// @brief 定期从游戏世界中获取所有玩家数据。
    /// 应在后台线程运行。
    void update_loop(uintptr_t base_address, sdk::game::CameraSystem& camera) noexcept;

    /// @brief 安全地检索当前玩家数据的副本。
    [[nodiscard]] std::vector<PlayerData> players() const noexcept;

    /// @brief 获取当前识别到的本地玩家。
    [[nodiscard]] const sdk::game::LocalPlayer& local_player() const noexcept {

        return m_local_player;

    }

private:

    /// @brief 清除当前玩家列表。
    void clear_players() noexcept;

    mutable std::mutex           m_mutex;

    std::vector<PlayerData>      m_players;
    sdk::game::LocalPlayer       m_local_player;

};

} // namespace onyx::core

#endif // ONYX_CORE_ESP_MANAGER_HPP

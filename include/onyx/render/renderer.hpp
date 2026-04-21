// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_RENDER_RENDERER_HPP
#define ONYX_RENDER_RENDERER_HPP

#include <onyx/core/esp_manager.hpp>
#include <onyx/sdk/game/CameraSystem.hpp>
#include <onyx/core/settings.hpp>
#include <imgui.h>

namespace onyx::render {

class Renderer final {
public:
    Renderer() = default;
    ~Renderer() = default;

    // 不可复制
    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    /// @brief 初始化渲染器 (加载字体、应用样式)。必须在 ImGui 上下文创建后但 BeginFrame 前调用一次。
    void initialize() noexcept;

    /// @brief 编排帧渲染。
    /// @param esp ESP 数据管理器。
    /// @param camera 相机投影系统。
    /// @param base_address
    void render_frame(const core::EspManager& esp,
                      sdk::game::CameraSystem& camera,
                      uintptr_t base_address) noexcept;

private:
    void draw_esp(const core::EspManager& esp, const sdk::game::CameraSystem& camera) noexcept;
    void draw_launcher_bar() noexcept;
    void draw_2d_box(const core::PlayerData& player, const sdk::game::CameraSystem& camera) noexcept;
    void draw_3d_box(const core::PlayerData& player, const sdk::game::CameraSystem& camera) noexcept;
    void draw_health_bar_2d(ImVec2 top_left, ImVec2 bottom_right, int health) noexcept;
    bool get_2d_bounds(const core::PlayerData& player, const sdk::game::CameraSystem& camera, ImVec2& out_top_left, ImVec2& out_bottom_right) noexcept;
    void draw_skeleton(const core::PlayerData& player) noexcept;
    void draw_offscreen_indicator(const core::EspManager& esp, const sdk::game::CameraSystem& camera) noexcept;
    void draw_menu(const core::EspManager& esp, const sdk::game::CameraSystem& camera) noexcept;
    void apply_styling() noexcept;

    bool m_is_initialized{false};
    bool m_is_menu_open{false};
    double m_last_time{0.0};
    ImFont* m_font_roboto{nullptr};
    ImFont* m_font_roboto_small{nullptr};
};

} // namespace onyx::render

#endif // ONYX_RENDER_RENDERER_HPP

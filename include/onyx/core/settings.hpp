// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_CORE_SETTINGS_HPP
#define ONYX_CORE_SETTINGS_HPP

#include <imgui.h>

namespace onyx::core {

/// @brief 全局设置单例，用于管理 ESP 和 UI 配置。
class Settings final {

public:

    static Settings& get() noexcept {

        static Settings instance;
        return instance;

    }

    bool   esp_enabled{true};
    
    int    esp_box_type{2};
    int    esp_box_2d_style{0};
    ImVec4 esp_box_color{0.0f, 1.0f, 0.8f, 1.0f};

    bool   esp_skeleton{true};
    float  esp_skeleton_y_offset{-0.07f};
    ImVec4 esp_skeleton_color{1.0f, 1.0f, 1.0f, 0.9f};
    float  esp_skeleton_thickness{1.5f};

    bool   esp_lines_enabled{false};
    bool   esp_health_enabled{true};
    bool   esp_distance_enabled{true};
    bool   esp_offscreen_indicator{true};

private:

    Settings() = default;
    ~Settings() = default;
    Settings(const Settings&) = delete;

    Settings& operator=(const Settings&) = delete;

};

} // namespace onyx::core

#endif // ONYX_CORE_SETTINGS_HPP

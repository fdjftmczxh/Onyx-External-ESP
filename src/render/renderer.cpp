// 作者: feng_xingzhe_cn_.86

#include <onyx/render/renderer.hpp>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <ctime>
#include <GLES3/gl3.h>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include "../../Deps/Roboto-Regular.h"

namespace onyx::render {

    void Renderer::render_frame(const core::EspManager& esp, sdk::game::CameraSystem& camera, uintptr_t base_address) noexcept {

        ImGuiIO& io = ImGui::GetIO();

        struct timespec ts{};
        clock_gettime(CLOCK_MONOTONIC, &ts);
        double now = static_cast<double>(ts.tv_sec) + ts.tv_nsec / 1e9;

        io.DeltaTime = m_last_time > 0.0 ? static_cast<float>(now - m_last_time) : 1.0f / 60.0f;
        m_last_time = now;

        draw_esp(esp, camera);
        draw_offscreen_indicator(esp, camera);
        draw_launcher_bar();
        draw_menu(esp, camera);

    }

void Renderer::initialize() noexcept {

    if (m_is_initialized) return;

    ImGuiIO& io = ImGui::GetIO();
    
    m_font_roboto = io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, sizeof(Roboto_Regular), 20.0f);
    m_font_roboto_small = io.Fonts->AddFontFromMemoryTTF(Roboto_Regular, sizeof(Roboto_Regular), 17.0f);
    
    apply_styling();

    m_is_initialized = true;

}

void Renderer::draw_esp(const core::EspManager& esp, const sdk::game::CameraSystem& camera) noexcept {

    auto& settings = core::Settings::get();

    if (!settings.esp_enabled) return;

    ImGuiIO& io = ImGui::GetIO();

    auto players = esp.players();

    for (const auto& player : players) {

        if (!player.is_visible) continue;

        ImVec2 tl, br;

        bool has_2d = false;

        if (settings.esp_box_type > 0) {

            has_2d = get_2d_bounds(player, camera, tl, br);

        }

        if (settings.esp_box_type == 1 && has_2d) {

            draw_2d_box(player, camera);

        } else if (settings.esp_box_type == 2) {

            draw_3d_box(player, camera);

        }

        if (settings.esp_health_enabled && player.health > 0) {

            if (settings.esp_box_type == 1 && has_2d) {

                draw_health_bar_2d(tl, br, player.health);

            } else {

                ImDrawList* dl = ImGui::GetBackgroundDrawList();

                std::string hp_text = "HP: " + std::to_string(player.health);
                
                if (m_font_roboto) ImGui::PushFont(m_font_roboto);

                ImVec2 text_size = ImGui::CalcTextSize(hp_text.c_str());
                ImVec2 text_pos = { player.screen_pos.x - text_size.x * 0.5f, player.screen_pos.y + 10.0f };

                dl->AddRectFilled({text_pos.x - 4.0f, text_pos.y}, {text_pos.x + text_size.x + 4.0f, text_pos.y + text_size.y},IM_COL32(0, 0, 0, 160), 4.0f);
                dl->AddText(text_pos, IM_COL32(0, 255, 100, 255), hp_text.c_str());

                if (m_font_roboto) ImGui::PopFont();

            }

        }

        if (settings.esp_distance_enabled) {

            ImDrawList* dl = ImGui::GetBackgroundDrawList();

            std::string dist_text = std::to_string(static_cast<int>(player.distance)) + "m";
            
            if (m_font_roboto) ImGui::PushFont(m_font_roboto);

            ImVec2 text_size = ImGui::CalcTextSize(dist_text.c_str());
            
            if (settings.esp_box_type == 2) {

                ImVec2 text_pos = { player.screen_pos.x - text_size.x * 0.5f, player.screen_pos.y + 30.0f };

                dl->AddRectFilled({text_pos.x - 4.0f, text_pos.y}, {text_pos.x + text_size.x + 4.0f, text_pos.y + text_size.y},IM_COL32(0, 0, 0, 160), 4.0f);
                dl->AddText(text_pos, IM_COL32(0, 255, 100, 255), dist_text.c_str());

            } else if (settings.esp_box_type == 1 && has_2d) {

                ImVec2 text_pos = { (tl.x + br.x) * 0.5f - text_size.x * 0.5f, br.y + 5.0f };

                dl->AddText(text_pos, IM_COL32(255, 255, 255, 255), dist_text.c_str());

            }

            if (m_font_roboto) ImGui::PopFont();

        }

        if (settings.esp_lines_enabled) {

            ImDrawList* dl = ImGui::GetBackgroundDrawList();

            ImVec2 screen_top = { io.DisplaySize.x * 0.5f, 0.0f };
            ImVec2 target_top;

            if (settings.esp_box_type == 1 && has_2d) {

                target_top = { (tl.x + br.x) * 0.5f, tl.y };

            } else {

                target_top = player.screen_pos; // Default to feet if no box

            }
            
            dl->AddLine(screen_top, target_top, IM_COL32(200, 200, 200, 120), 1.0f);

        }

        if (settings.esp_skeleton && player.bone_count > 0) {

            draw_skeleton(player);

        }

    }

}

void Renderer::draw_2d_box(const core::PlayerData& player, const sdk::game::CameraSystem& camera) noexcept {

    ImVec2 tl, br;

    if (!get_2d_bounds(player, camera, tl, br)) return;

    auto& settings = core::Settings::get();

    ImDrawList* dl = ImGui::GetBackgroundDrawList();

    const ImU32 color = ImGui::ColorConvertFloat4ToU32(settings.esp_box_color);
    
    if (settings.esp_box_2d_style == 0) {

        dl->AddRect(tl, br, IM_COL32(0, 0, 0, 255), 0.0f, ImDrawFlags_None, 3.5f);
        dl->AddRect(tl, br, color, 0.0f, ImDrawFlags_None, 1.5f);

    } else {

        float w = br.x - tl.x;
        float h = br.y - tl.y;
        float line_w = w * 0.25f;
        float line_h = h * 0.25f;
        
        auto draw_corner = [&](ImVec2 p, float dx, float dy) {

            dl->AddLine({p.x, p.y}, {p.x + dx, p.y}, IM_COL32(0, 0, 0, 255), 3.5f);
            dl->AddLine({p.x, p.y}, {p.x, p.y + dy}, IM_COL32(0, 0, 0, 255), 3.5f);
            dl->AddLine({p.x, p.y}, {p.x + dx, p.y}, color, 1.5f);
            dl->AddLine({p.x, p.y}, {p.x, p.y + dy}, color, 1.5f);

        };

        draw_corner(tl, line_w, line_h);
        draw_corner({br.x, tl.y}, -line_w, line_h);
        draw_corner({tl.x, br.y}, line_w, -line_h);
        draw_corner(br, -line_w, -line_h);

    }

}

void Renderer::draw_3d_box(const core::PlayerData& player, const sdk::game::CameraSystem& camera) noexcept {

    auto& settings = core::Settings::get();

    sdk::Vector3 bmin, bmax;

    if (player.has_bounds) {
        bmin = player.aabb_min;
        bmax = player.aabb_max;
    }

    sdk::Vector3 corners[8] = {

        {bmin.x, bmin.y, bmin.z},  // 0: 下-左-后
        {bmax.x, bmin.y, bmin.z},  // 1: 下-右-后
        {bmax.x, bmin.y, bmax.z},  // 2: 下-右-前
        {bmin.x, bmin.y, bmax.z},  // 3: 下-左-前
        {bmin.x, bmax.y, bmin.z},  // 4: 上-左-后
        {bmax.x, bmax.y, bmin.z},  // 5: 上-右-后
        {bmax.x, bmax.y, bmax.z},  // 6: 上-右-前
        {bmin.x, bmax.y, bmax.z}   // 7: 上-左-前

    };

    ImVec2 screen_points[8];

    const float screen_w = ImGui::GetIO().DisplaySize.x;
    const float screen_h = ImGui::GetIO().DisplaySize.y;

    for (int i = 0; i < 8; ++i) {

        if (!camera.world_to_screen(corners[i], screen_points[i], screen_w, screen_h)) {

            return;

        }

    }

    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    const ImU32 color = ImGui::ColorConvertFloat4ToU32(settings.esp_box_color);

    draw_list->AddLine(screen_points[0], screen_points[1], color);
    draw_list->AddLine(screen_points[1], screen_points[2], color);
    draw_list->AddLine(screen_points[2], screen_points[3], color);
    draw_list->AddLine(screen_points[3], screen_points[0], color);

    draw_list->AddLine(screen_points[4], screen_points[5], color);
    draw_list->AddLine(screen_points[5], screen_points[6], color);
    draw_list->AddLine(screen_points[6], screen_points[7], color);
    draw_list->AddLine(screen_points[7], screen_points[4], color);

    draw_list->AddLine(screen_points[0], screen_points[4], color);
    draw_list->AddLine(screen_points[1], screen_points[5], color);
    draw_list->AddLine(screen_points[2], screen_points[6], color);
    draw_list->AddLine(screen_points[3], screen_points[7], color);

}

void Renderer::draw_health_bar_2d(ImVec2 tl, ImVec2 br, int health) noexcept {

    ImDrawList* dl = ImGui::GetBackgroundDrawList();
    
    float h = br.y - tl.y;
    float w = 4.0f;
    float x = tl.x - 6.0f;
    
    dl->AddRectFilled({x - 1.0f, tl.y - 1.0f}, {x + w + 1.0f, br.y + 1.0f}, IM_COL32(0, 0, 0, 180));
    
    float health_height = (h * (health / 100.0f));
    ImU32 health_color = IM_COL32(0, 255, 100, 255);
    if (health < 30) health_color = IM_COL32(255, 0, 0, 255);
    else if (health < 60) health_color = IM_COL32(255, 255, 0, 255);

    dl->AddRectFilled({x, br.y - health_height}, {x + w, br.y}, health_color);

}

bool Renderer::get_2d_bounds(const core::PlayerData& player, const sdk::game::CameraSystem& camera, ImVec2& tl, ImVec2& br) noexcept {

    sdk::Vector3 bmin, bmax;

    if (player.has_bounds) {

        bmin = player.aabb_min;
        bmax = player.aabb_max;

    }

    sdk::Vector3 corners[8] = {

        {bmin.x, bmin.y, bmin.z}, {bmax.x, bmin.y, bmin.z},
        {bmax.x, bmin.y, bmax.z}, {bmin.x, bmin.y, bmax.z},
        {bmin.x, bmax.y, bmin.z}, {bmax.x, bmax.y, bmin.z},
        {bmax.x, bmax.y, bmax.z}, {bmin.x, bmax.y, bmax.z}

    };

    float min_x = 10000.0f, min_y = 10000.0f, max_x = -10000.0f, max_y = -10000.0f;

    const float sw = ImGui::GetIO().DisplaySize.x;
    const float sh = ImGui::GetIO().DisplaySize.y;

    bool any_visible = false;

    for (int i = 0; i < 8; ++i) {

        ImVec2 sp;

        if (camera.world_to_screen(corners[i], sp, sw, sh)) {

            min_x = std::min(min_x, sp.x);
            min_y = std::min(min_y, sp.y);
            max_x = std::max(max_x, sp.x);
            max_y = std::max(max_y, sp.y);

            any_visible = true;

        }

    }

    if (!any_visible) return false;

    tl = {min_x, min_y};
    br = {max_x, max_y};

    return true;
}

void Renderer::draw_skeleton(const core::PlayerData& player) noexcept {

    auto& settings = core::Settings::get();

    ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

    const ImU32 color = ImGui::ColorConvertFloat4ToU32(settings.esp_skeleton_color);
    const float thickness = settings.esp_skeleton_thickness;

    for (int32_t i = 0; i < player.bone_count; ++i) {

        const auto& bone = player.bones[i];
        if (!bone.is_valid) continue;

        int32_t pi = bone.parent_index;

        if (pi < 0 || pi >= player.bone_count) continue;
        if (!player.bones[pi].is_valid) continue;

        draw_list->AddLine(player.bones[pi].screen_pos, bone.screen_pos, color, thickness);

    }
}

void Renderer::draw_launcher_bar() noexcept {

    ImGuiIO& io = ImGui::GetIO();

    float screen_w = io.DisplaySize.x;

    std::time_t t = std::time(nullptr);
    std::tm tm = *std::gmtime(&t);

    std::stringstream time_ss;

    time_ss << std::put_time(&tm, "%H:%M:%S");

    std::string text = "Onyx 1.60.0.f3172 | " + time_ss.str() + " | " + std::to_string(static_cast<int>(io.Framerate)) + " FPS";

    if (m_font_roboto) ImGui::PushFont(m_font_roboto);
    
    ImVec2 text_size = ImGui::CalcTextSize(text.c_str());

    float padding = 8.0f;
    float button_w = 90.0f;
    float button_h = 30.0f;
    float window_h = 46.0f;
    float window_w = text_size.x + button_w + (padding * 4.0f);
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, 0.515f));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

    ImVec2 pos = { (screen_w - window_w) * 0.5f, io.DisplaySize.y - window_h - 20.0f };

    ImGui::SetNextWindowPos(pos);
    ImGui::SetNextWindowSize({ window_w, window_h });
    
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBackground;

    if (ImGui::Begin("##LauncherBar", nullptr, flags)) {

        ImDrawList* dl = ImGui::GetWindowDrawList();
        
        dl->AddRectFilled(pos, { pos.x + window_w, pos.y + window_h }, IM_COL32(15, 15, 15, 245));
        
        dl->AddRectFilled(pos, { pos.x + window_w, pos.y + 2.0f }, IM_COL32(0, 255, 230, 255));

        float available_h = window_h - 2.0f;
        float content_start_y = pos.y + 2.0f;
        
        float text_y = content_start_y + (available_h - text_size.y) * 0.5f;
        float button_y = content_start_y + (available_h - button_h) * 0.5f;

        ImGui::SetCursorScreenPos({ pos.x + padding * 1.5f, text_y });
        ImGui::TextColored(ImVec4(0.9f, 0.9f, 0.9f, 1.0f), "%s", text.c_str());
        
        ImGui::SetCursorScreenPos({ pos.x + window_w - button_w - padding, button_y });
        
        if (m_font_roboto_small) {

            ImGui::PopFont();
            ImGui::PushFont(m_font_roboto_small);

        }

        const char* btn_label = m_is_menu_open ? "Close" : "Open";

        if (ImGui::Button(btn_label, { button_w, button_h })) {

            m_is_menu_open = !m_is_menu_open;
        }


        if (m_font_roboto_small) {

            ImGui::PopFont();

            if (m_font_roboto) ImGui::PushFont(m_font_roboto);

        }

    }

    ImGui::End();
    ImGui::PopStyleVar(5);

    if (m_font_roboto) ImGui::PopFont();

}

void Renderer::draw_menu(const core::EspManager& esp, const sdk::game::CameraSystem& camera) noexcept {

    if (!m_is_menu_open) return;

    auto& settings = core::Settings::get();

    ImGuiIO& io = ImGui::GetIO();

    if (m_font_roboto) ImGui::PushFont(m_font_roboto);

    ImGui::SetNextWindowPos({io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f}, ImGuiCond_FirstUseEver, {0.5f, 0.5f});
    ImGui::SetNextWindowSize({io.DisplaySize.x * 0.4f, io.DisplaySize.y * 0.6f}, ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Onyx - External ESP Framework", nullptr, ImGuiWindowFlags_NoCollapse)) {

        if (ImGui::BeginTabBar("MenuTabs")) {
            
            if (ImGui::BeginTabItem("Visuals")) {

                ImGui::Spacing();
                
                ImGui::Text("Box Style:");
                ImGui::RadioButton("None", &settings.esp_box_type, 0); ImGui::SameLine();
                ImGui::RadioButton("2D", &settings.esp_box_type, 1); ImGui::SameLine();
                ImGui::RadioButton("3D", &settings.esp_box_type, 2);

                if (settings.esp_box_type == 1) { // 2D specific options

                    ImGui::Text("2D Style:");
                    ImGui::RadioButton("Full##2D", &settings.esp_box_2d_style, 0); ImGui::SameLine();
                    ImGui::RadioButton("Corners##2D", &settings.esp_box_2d_style, 1);

                }

                if (settings.esp_box_type > 0) {

                    ImGui::ColorEdit4("Box Color", (float*)&settings.esp_box_color, ImGuiColorEditFlags_NoInputs);

                }

                ImGui::Spacing();
                ImGui::Checkbox("Skeleton ESP", &settings.esp_skeleton);

                if (settings.esp_skeleton) {

                    ImGui::ColorEdit4("Skeleton Color", (float*)&settings.esp_skeleton_color, ImGuiColorEditFlags_NoInputs);
                    ImGui::SliderFloat("Thickness", &settings.esp_skeleton_thickness, 0.5f, 5.0f);
                    ImGui::SliderFloat("Y Offset", &settings.esp_skeleton_y_offset, -0.5f, 0.5f);

                }

                ImGui::Separator();

                ImGui::Checkbox("Snap-Lines", &settings.esp_lines_enabled);
                ImGui::Checkbox("Display Health", &settings.esp_health_enabled);
                ImGui::Checkbox("Display Distance", &settings.esp_distance_enabled);
                ImGui::Checkbox("Offscreen Indicator", &settings.esp_offscreen_indicator);
                
                ImGui::EndTabItem();

            }

            if (ImGui::BeginTabItem("Misc")) {

                ImGui::Spacing();
                
                sdk::Vector3 cam_pos = camera.main_camera.position;

                ImGui::Text("Camera World Position:");
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.9f, 1.0f), "X: %.2f  Y: %.2f  Z: %.2f", cam_pos.x, cam_pos.y, cam_pos.z);
                
                ImGui::Spacing();
                
                const auto& lp = esp.local_player();

                if (lp.is_valid()) {

                    ImGui::Text("Local Player Found:");
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "Address: 0x%lX", (unsigned long)lp.address);
                    sdk::Vector3 lp_pos = lp.transform.position();
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "Pos: X: %.2f  Y: %.2f  Z: %.2f", lp_pos.x, lp_pos.y, lp_pos.z);
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.5f, 1.0f), "Health: %d", lp.health);

                } else {

                    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Local Player: Not Identified");

                }
                
                ImGui::Separator();
                ImGui::TextDisabled("Build: " __DATE__ " " __TIME__);
                ImGui::TextDisabled("Platform: Android x86_64");
                ImGui::Separator();
                ImGui::TextWrapped("Onyx External ESP for Critical Ops. Use responsibly.");
                ImGui::EndTabItem();

            }

            ImGui::EndTabBar();

        }

    }

    ImGui::End();
    
    if (m_font_roboto) ImGui::PopFont();

}

void Renderer::apply_styling() noexcept {

    auto& style = ImGui::GetStyle();
    
    style.WindowRounding    = 12.0f;
    style.ChildRounding     = 8.0f;
    style.FrameRounding     = 8.0f;
    style.PopupRounding     = 8.0f;
    style.ScrollbarRounding = 12.0f;
    style.GrabRounding      = 8.0f;
    style.TabRounding       = 8.0f;
    style.WindowBorderSize  = 1.0f;
    style.FrameBorderSize   = 0.0f;

    style.AntiAliasedLines = true;
    style.AntiAliasedFill  = true;
    style.WindowPadding    = { 12, 12 };
    style.ItemSpacing      = { 10, 8 };

    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text]                   = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.08f, 0.08f, 0.09f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_Border]                 = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.05f, 0.05f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.10f, 0.10f, 0.11f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.00f, 1.00f, 0.90f, 1.00f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.00f, 0.80f, 0.70f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.00f, 1.00f, 0.90f, 1.00f);
    colors[ImGuiCol_Button]                 = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.18f, 0.18f, 0.20f, 1.00f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.22f, 0.22f, 0.24f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.12f, 0.11f, 0.14f, 1.00f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.19f, 0.18f, 0.21f, 1.00f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.25f, 0.24f, 0.27f, 1.00f);
    colors[ImGuiCol_Separator]              = ImVec4(0.20f, 0.20f, 0.22f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.30f, 0.30f, 0.32f, 1.00f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.40f, 0.42f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.00f, 1.00f, 0.90f, 0.20f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.00f, 1.00f, 0.90f, 0.67f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.00f, 1.00f, 0.90f, 0.95f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.00f, 1.00f, 0.90f, 0.80f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.00f, 0.80f, 0.70f, 1.00f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.11f, 0.11f, 0.13f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.16f, 1.00f);

}

void Renderer::draw_offscreen_indicator(const core::EspManager& esp, const sdk::game::CameraSystem& camera) noexcept {

    auto& settings = core::Settings::get();

    if (!settings.esp_offscreen_indicator) return;


    ImDrawList* dl = ImGui::GetBackgroundDrawList();

    const ImVec2 display_size = ImGui::GetIO().DisplaySize;
    const ImVec2 screen_center = { display_size.x * 0.5f, display_size.y * 0.5f };

    constexpr float k_radius = 180.0f;
    constexpr float k_arrow_size = 12.0f;
    constexpr float k_half_width = 6.0f;

    const ImU32 color = IM_COL32(0, 255, 230, 255); // Onyx Cyan

    const sdk::Matrix4x4 view = camera.view_matrix();
    const auto players = esp.players();

    for (const auto& player : players) {

        if (player.is_visible) continue;

        const sdk::Vector3 local = view.MultiplyPoint3x4(player.world_pos);

        const float angle = std::atan2(local.x, local.z);

        const float s = std::sin(angle);
        const float c = std::cos(angle);

        const ImVec2 pos = {screen_center.x + s * k_radius,screen_center.y - c * k_radius };

        const ImVec2 p1 = pos;
        const ImVec2 p2 = { pos.x - s * k_arrow_size - c * k_half_width, pos.y + c * k_arrow_size - s * k_half_width };
        const ImVec2 p3 = { pos.x - s * k_arrow_size + c * k_half_width, pos.y + c * k_arrow_size + s * k_half_width };

        dl->AddTriangleFilled(p1, p2, p3, color);
        dl->AddTriangle(p1, p2, p3, IM_COL32(0, 0, 0, 180), 1.0f);

    }

}

} // namespace onyx::render

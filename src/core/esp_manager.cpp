// 作者: feng_xingzhe_cn_.86

#include <onyx/core/esp_manager.hpp>
#include <onyx/core/settings.hpp>
#include <onyx/sdk/game/GameplayModule.hpp>
#include <onyx/sdk/game/Character.hpp>
#include <onyx/sdk/game/Skeleton.hpp>
#include <onyx/sdk/game/LocalPlayer.hpp>
#include <onyx/utils/Memory.hpp>
#include <thread>
#include <chrono>
#include <algorithm>

namespace onyx::core {

    using namespace sdk::game;

    void EspManager::update_loop(uintptr_t base_address, sdk::game::CameraSystem& camera) noexcept {

        using PlayerEntry = ::onyx::Dictionary<int32_t, uintptr_t>::Entry;

        std::vector<PlayerData> current_batch;
        std::vector<PlayerEntry> entry_buffer;

        while (true) {

            std::this_thread::sleep_for(std::chrono::milliseconds(4)); // ~240hz

            current_batch.clear();

            auto gameplay = GameplayModule::instance();

            if (!gameplay.is_valid()) {
                clear_players();
                continue;
            }

            camera = gameplay.camera_system;

            if (!camera.is_valid()) {
                clear_players();
                continue;
            }

            auto game_system = gameplay.game_system;

            if (!game_system.is_valid()) {
                clear_players();
                continue;
            }

            uintptr_t dict_addr = game_system.player_dict;

            if (!utils::Memory::is_valid_ptr(dict_addr)) {
                clear_players();
                continue;
            }

            auto dict = utils::Memory::read<::onyx::Dictionary<int32_t, uintptr_t>>(dict_addr);

            uintptr_t entries_addr = reinterpret_cast<uintptr_t>(dict.entries);

            if (!utils::Memory::is_valid_ptr(reinterpret_cast<uintptr_t>(dict.entries)) || dict.count <= 0 || dict.count >= 1000) {
                clear_players();
                continue;
            }

            current_batch.reserve(dict.count);

            const float screen_w = ImGui::GetIO().DisplaySize.x;
            const float screen_h = ImGui::GetIO().DisplaySize.y;

            const uintptr_t vector_base = entries_addr + 0x20;

            entry_buffer.resize(dict.count);

            utils::Memory::read_raw(vector_base, entry_buffer.data(), dict.count * sizeof(PlayerEntry));

            uintptr_t closest_addr = 0;

            float min_dist = 1000000.0f;

            sdk::Vector3 cam_pos = camera.main_camera.position;

            for (int32_t i = 0; i < dict.count; ++i) {

                PlayerEntry& entry = entry_buffer[i];

                if (entry.hashCode < 0 || !utils::Memory::is_valid_ptr(entry.value)) continue;

                Character character(entry.value);

                float d = sdk::Vector3::Distance(character.transform.position(), cam_pos);

                if (d < min_dist) {

                    min_dist = d;
                    closest_addr = entry.value;

                }

            }

            {

                std::lock_guard<std::mutex> lock(m_mutex);

                if (closest_addr != 0) {

                    m_local_player = sdk::game::LocalPlayer(closest_addr);

                }

            }

            for (int32_t i = 0; i < dict.count; ++i) {

                PlayerEntry& entry = entry_buffer[i];

                if (entry.hashCode < 0 || !utils::Memory::is_valid_ptr(entry.value)) { continue; }

                if (entry.value == closest_addr) { continue; }

                Character character(entry.value);

                sdk::Vector3 world_pos = character.transform.position();

                float d = sdk::Vector3::Distance(world_pos, cam_pos);

                PlayerData& p_data = current_batch.emplace_back();

                if (camera.world_to_screen(world_pos, p_data.screen_pos, screen_w, screen_h)) {

                    p_data.world_pos = world_pos;
                    p_data.health = character.health;
                    p_data.distance = d;
                    p_data.is_visible = true;

                    auto skel = Skeleton::find_from_character(character.transform, world_pos);

                    if (skel) {

                        auto& joints = skel->joints;

                        int32_t num = static_cast<int32_t>(joints.size());

                        if (num > k_max_bones) num = k_max_bones;

                        p_data.bone_count = num;

                        if (num > 0) {

                            sdk::Vector3 bmin = joints[0].world_position;
                            sdk::Vector3 bmax = bmin;

                            for (int32_t b = 0; b < num; ++b) {

                                sdk::Vector3 wp = joints[b].world_position;

                                wp.y += Settings::get().esp_skeleton_y_offset;

                                if (wp.x < bmin.x) bmin.x = wp.x;
                                if (wp.y < bmin.y) bmin.y = wp.y;
                                if (wp.z < bmin.z) bmin.z = wp.z;
                                if (wp.x > bmax.x) bmax.x = wp.x;
                                if (wp.y > bmax.y) bmax.y = wp.y;
                                if (wp.z > bmax.z) bmax.z = wp.z;

                                ImVec2 sp;

                                if (camera.world_to_screen(wp, sp, screen_w, screen_h)) {

                                    p_data.bones[b].screen_pos = sp;
                                    p_data.bones[b].parent_index = joints[b].parent_index;
                                    p_data.bones[b].is_valid = true;

                                }
                            }

                            constexpr float k_padding = 0.12f;

                            bmin.x -= k_padding; bmin.y -= k_padding; bmin.z -= k_padding;
                            bmax.x += k_padding; bmax.y += k_padding; bmax.z += k_padding;

                            p_data.aabb_min = bmin;
                            p_data.aabb_max = bmax;
                            p_data.has_bounds = true;

                        }

                    }

                } else {

                    p_data.world_pos = world_pos;
                    p_data.distance = d;
                    p_data.is_visible = false;
                    p_data.bone_count = 0;
                    p_data.has_bounds = false;

                }

            }

            {

                std::lock_guard<std::mutex> lock(m_mutex);

                m_players = current_batch;

            }

        }

    }

    [[nodiscard]] std::vector<PlayerData> EspManager::players() const noexcept {

        std::lock_guard<std::mutex> lock(m_mutex);

        return m_players;

    }

    void EspManager::clear_players() noexcept {

        std::lock_guard<std::mutex> lock(m_mutex);

        m_players.clear();

    }

} // namespace onyx::core
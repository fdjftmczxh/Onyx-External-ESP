// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_OFFSETS_HPP
#define ONYX_SDK_OFFSETS_HPP

#include <cstdint>

namespace onyx::sdk::offsets {

constexpr uintptr_t k_type_info_gameplay_module = 0x2f7b680; // update for version 1.60.0.f3172

namespace gameplay_module {

    constexpr int32_t k_game_system = 0x30;
    constexpr int32_t k_camera_system = 0x38;

}

namespace game_system {

    constexpr int32_t k_all_players_dict = 0x30;
    constexpr int32_t k_local_player_interface = 0x88;

}

namespace camera_system {

    constexpr int32_t k_m_camera = 0xa8;

}

namespace unity_object {

    constexpr int32_t k_cached_ptr = 0x10;

}

namespace game_object {

    constexpr int32_t k_component_array = 0x20;

}

namespace component {

    constexpr int32_t k_game_object = 0x20;
    constexpr int32_t k_transform_in_array = 0x8;

}

namespace unity_camera {

    constexpr int32_t k_view_matrix = 0xF0;

}

namespace transform {

    constexpr int32_t k_data = 0x38;

    constexpr int32_t k_hierarchy_internal = 0x0;

    constexpr int32_t k_index_internal = 0x8;

    constexpr int32_t k_native_hierarchy = 0x28;

    constexpr int32_t k_native_index = 0x30;

    constexpr int32_t k_transform_data = 0x18;

    constexpr int32_t k_parent_indices = 0x20;

    constexpr int32_t k_data_stride = 0x30;

    constexpr int32_t k_local_position = 0x00;

    constexpr int32_t k_local_rotation = 0x10;

    constexpr int32_t k_local_scale = 0x20;

    constexpr int32_t k_children_array = 0x48;

    constexpr int32_t k_child_count = 0x58;

}

namespace character {

    constexpr int32_t k_health = 0x110;
    constexpr int32_t k_rotation_ptr = 0x108;

}

namespace rotation_control {
    
    constexpr int32_t k_angles = 0x94;

}

} // namespace onyx::sdk::offsets

#endif // ONYX_SDK_OFFSETS_HPP

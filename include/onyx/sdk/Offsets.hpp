// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_OFFSETS_HPP
#define ONYX_SDK_OFFSETS_HPP

#include <cstdint>

namespace onyx::sdk::offsets {

/// @brief OECFB7135E : GameplayModule TypeInfo
constexpr uintptr_t k_type_info_gameplay_module = 0x2f90360;

/// @brief 类: OECFB7135E (GameplayModule)
namespace gameplay_module {
    /// @brief 字段: OE92D655B7 (GameSystem)
    constexpr int32_t k_game_system = 0x30;
    /// @brief 字段: OC751CCC01 (CameraSystem)
    constexpr int32_t k_camera_system = 0x38;
}

/// @brief 类: O6F692BC0C (GameSystem)
namespace game_system {
    /// @brief 字段: OE54334CA1 (Dictionary<int, Character>)
    constexpr int32_t k_all_players_dict = 0x30;
    /// @brief 字段: O6DD90AC9C (Interface for Local User)
    constexpr int32_t k_local_player_interface = 0x88;
}

/// @brief 类: O48D52ECEB (CameraSystem)
namespace camera_system {
    /// @brief 字段: O4D9D04E00 (m_camera)
    constexpr int32_t k_m_camera = 0xa8;
}

/// @brief UnityEngine.Object 核心偏移量
namespace unity_object {
    /// @brief 指向底层原生 Unity C++ 对象的指针 (cachedPtr)
    constexpr int32_t k_cached_ptr = 0x10;
}

/// @brief UnityEngine.GameObject 核心偏移量
namespace game_object {
    /// @brief 指向组件数组的指针
    constexpr int32_t k_component_array = 0x20;
}

/// @brief UnityEngine.Component 核心偏移量
namespace component {
    /// @brief 从 Native Component 获取所属的 Native GameObject
    constexpr int32_t k_game_object = 0x20;
    /// @brief 在 Component Array 中获取 Transform 指针的偏移量 (Index 0)
    constexpr int32_t k_transform_in_array = 0x8;
}

/// @brief UnityEngine.Camera 核心偏移量
namespace unity_camera {
    /// @brief 世界到屏幕/视图 矩阵 (ViewMatrix)
    constexpr int32_t k_view_matrix = 0xF0;
}

/// @brief UnityEngine.Transform 核心偏移量 (底层内存布局)
namespace transform {
    // ─── 基础组件 ───
    /// @brief 变换数据指针 (TransformData)
    constexpr int32_t k_data = 0x38;
    /// @brief 层次结构指针 (在 TransformData 内部偏移)
    constexpr int32_t k_hierarchy_internal = 0x0;
    /// @brief 索引值 (在 TransformData 内部偏移)
    constexpr int32_t k_index_internal = 0x8;

    /// @brief 直接从 Transform 获取层次结构指针 (绕过 Data 指针)
    constexpr int32_t k_native_hierarchy = 0x28;
    /// @brief 直接从 Transform 获取索引值 (绕过 Data 指针)
    constexpr int32_t k_native_index = 0x30;

    /// @brief 变换数据数组 (position/rotation/scale)
    constexpr int32_t k_transform_data = 0x18;
    /// @brief 父节点索引数组 (int32[])
    constexpr int32_t k_parent_indices = 0x20;
    /// @brief 每元素步长 (48 字节: pos+rot+scale)
    constexpr int32_t k_data_stride = 0x30;

    /// @brief 本地位置 (vec4, 16 bytes)
    constexpr int32_t k_local_position = 0x00;
    /// @brief 本地旋转四元数 (vec4, 16 bytes)
    constexpr int32_t k_local_rotation = 0x10;
    /// @brief 本地缩放 (vec4, 16 bytes)
    constexpr int32_t k_local_scale = 0x20;

    /// @brief 子 Transform 指针数组 (Transform**)
    constexpr int32_t k_children_array = 0x48;
    /// @brief 子 Transform 数量
    constexpr int32_t k_child_count = 0x58;
}

/// @brief 类: O7F4294DBE (Character)
namespace character {
    /// @brief 字段: O52FE5729C (Int32)
    constexpr int32_t k_health = 0x110;
    /// @brief 字段: 指向旋转控制器的指针
    constexpr int32_t k_rotation_ptr = 0x108;
}

/// @brief 旋转控制器内部偏移
namespace rotation_control {
    /// @brief 视角角度 (Vector2: Pitch, Yaw)
    constexpr int32_t k_angles = 0x94;
}

} // namespace onyx::sdk::offsets

#endif // ONYX_SDK_OFFSETS_HPP

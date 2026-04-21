// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_TRANSFORM_HPP
#define ONYX_SDK_ENGINE_TRANSFORM_HPP

#include <onyx/sdk/engine/Component.hpp>
#include <onyx/sdk/engine/structures/Quaternion.hpp>

namespace onyx::sdk::engine {

/// @brief Unity Transform 的原生包装器。
struct Transform : Component {

    explicit Transform(uintptr_t addr) noexcept : Component(addr) {

        if (is_valid()) native_ptr = address;

    }

    /// @brief 读取本地坐标 (从层级数据数组中直接读取，不做父链遍历)。
    [[nodiscard]] sdk::Vector3 local_position() const noexcept {

        if (!native_ptr) return {0.f, 0.f, 0.f};

        using namespace offsets::transform;

        const uintptr_t hierarchy = utils::Memory::read_safe<uintptr_t>(native_ptr + k_native_hierarchy);
        const uint32_t index = utils::Memory::read_safe<uint32_t>(native_ptr + k_native_index);

        if (!hierarchy || index == 0xFFFFFFFF) return {0.f, 0.f, 0.f};

        const uintptr_t data_array = utils::Memory::read_safe<uintptr_t>(hierarchy + k_transform_data);

        if (!data_array) return {0.f, 0.f, 0.f};

        uintptr_t elem = data_array + (static_cast<uintptr_t>(index) * k_data_stride);

        return utils::Memory::read_safe<sdk::Vector3>(elem + k_local_position);

    }

    /// @brief 读取本地旋转四元数。
    [[nodiscard]] Quaternion local_rotation() const noexcept {

        if (!native_ptr) return Quaternion::identity();

        using namespace offsets::transform;

        const uintptr_t hierarchy = utils::Memory::read_safe<uintptr_t>(native_ptr + k_native_hierarchy);
        const uint32_t index = utils::Memory::read_safe<uint32_t>(native_ptr + k_native_index);

        if (!hierarchy || index == 0xFFFFFFFF) return Quaternion::identity();

        const uintptr_t data_array = utils::Memory::read_safe<uintptr_t>(hierarchy + k_transform_data);

        if (!data_array) return Quaternion::identity();

        uintptr_t elem = data_array + (static_cast<uintptr_t>(index) * k_data_stride);

        return utils::Memory::read_safe<Quaternion>(elem + k_local_rotation);

    }

    /// @brief 获取世界坐标 (通过层级父链遍历计算)。
    [[nodiscard]] sdk::Vector3 position() const noexcept {

        if (!native_ptr) return {0.f, 0.f, 0.f};

        using namespace offsets::transform;

        const uintptr_t hierarchy = utils::Memory::read_safe<uintptr_t>(native_ptr + k_native_hierarchy);
        const uint32_t index = utils::Memory::read_safe<uint32_t>(native_ptr + k_native_index);

        if (!hierarchy || index == 0xFFFFFFFF) return {0.f, 0.f, 0.f};

        const uintptr_t data_array = utils::Memory::read_safe<uintptr_t>(hierarchy + k_transform_data);
        const uintptr_t parent_array = utils::Memory::read_safe<uintptr_t>(hierarchy + k_parent_indices);

        if (!data_array || !parent_array) return {0.f, 0.f, 0.f};

        uintptr_t elem = data_array + (static_cast<uintptr_t>(index) * k_data_stride);

        sdk::Vector3 pos = utils::Memory::read_safe<sdk::Vector3>(elem + k_local_position);

        int32_t parent_idx = utils::Memory::read_safe<int32_t>(parent_array + (index * 4));
        int32_t safety = 0;

        while (parent_idx >= 0 && safety < 64) {

            uintptr_t p_elem = data_array + (static_cast<uintptr_t>(parent_idx) * k_data_stride);

            sdk::Vector3 p_scale = utils::Memory::read_safe<sdk::Vector3>(p_elem + k_local_scale);

            pos.x *= p_scale.x;
            pos.y *= p_scale.y;
            pos.z *= p_scale.z;

            Quaternion p_rot = utils::Memory::read_safe<Quaternion>(p_elem + k_local_rotation);

            pos = p_rot * pos;

            sdk::Vector3 p_pos = utils::Memory::read_safe<sdk::Vector3>(p_elem + k_local_position);

            pos.x += p_pos.x;
            pos.y += p_pos.y;
            pos.z += p_pos.z;

            parent_idx = utils::Memory::read_safe<int32_t>(parent_array + (parent_idx * 4));

            ++safety;

        }

        return pos;

    }

    /// @brief 获取子 Transform 的数量。
    [[nodiscard]] int32_t child_count() const noexcept {

        if (!native_ptr) return 0;

        return utils::Memory::read_safe<int32_t>(native_ptr + offsets::transform::k_child_count);

    }

    /// @brief 获取指定索引的子 Transform。
    [[nodiscard]] Transform get_child(int32_t index) const noexcept {

        if (!native_ptr) return Transform(static_cast<uintptr_t>(0));

        const int32_t count = child_count();

        if (index < 0 || index >= count) return Transform(static_cast<uintptr_t>(0));

        const uintptr_t children_ptr = utils::Memory::read_safe<uintptr_t>(native_ptr + offsets::transform::k_children_array);

        if (!children_ptr) return Transform(static_cast<uintptr_t>(0));

        uintptr_t child_ptr = utils::Memory::read_safe<uintptr_t>(children_ptr + (static_cast<uintptr_t>(index) * 8));

        return Transform(child_ptr);

    }

};

} // namespace onyx::sdk::engine

#endif // ONYX_SDK_ENGINE_TRANSFORM_HPP

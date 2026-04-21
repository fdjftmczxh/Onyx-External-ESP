// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_GAME_OBJECT_HPP
#define ONYX_SDK_ENGINE_GAME_OBJECT_HPP

#include <onyx/sdk/engine/Object.hpp>
#include <onyx/sdk/engine/Transform.hpp>
#include <onyx/sdk/Offsets.hpp>

namespace onyx::sdk::engine {

/// @brief Unity GameObject 的原生包装器。
struct GameObject : Object {

    Transform transform;

    explicit GameObject(uintptr_t addr) noexcept : Object(addr), transform(static_cast<uintptr_t>(0)) {

        if (is_valid()) {

            const uintptr_t comp_list = utils::Memory::read_safe<uintptr_t>(address + offsets::game_object::k_component_array);

            if (comp_list) {

                transform = Transform(utils::Memory::read_safe<uintptr_t>(comp_list + offsets::component::k_transform_in_array));

            }

        }

    }

    template <typename T>
    [[nodiscard]] T get_component(int32_t index) const noexcept {

        if (!is_valid()) return T(static_cast<uintptr_t>(0));

        const uintptr_t comp_list = utils::Memory::read_safe<uintptr_t>(address + offsets::game_object::k_component_array);

        if (!comp_list) return T(static_cast<uintptr_t>(0));

        return T(utils::Memory::read_safe<uintptr_t>(comp_list + offsets::component::k_transform_in_array + (index * 0x8)));

    }
};

} // namespace onyx::sdk::engine

#endif // ONYX_SDK_ENGINE_GAME_OBJECT_HPP

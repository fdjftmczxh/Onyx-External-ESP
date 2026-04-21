// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_COMPONENT_HPP
#define ONYX_SDK_ENGINE_COMPONENT_HPP

#include <onyx/sdk/engine/Object.hpp>
#include <onyx/sdk/Offsets.hpp>

namespace onyx::sdk::engine {

/// @brief Unity Component 的基础包装器。
struct Component : Object {

    uintptr_t native_ptr;

    explicit Component(uintptr_t addr) noexcept : Object(addr), native_ptr(0) {

        if (is_valid()) {

            native_ptr = utils::Memory::read_safe<uintptr_t>(address + offsets::unity_object::k_cached_ptr);

        }

    }

    /// @brief 获取所属的 Native GameObject 地址。
    [[nodiscard]] uintptr_t game_object_addr() const noexcept {

        return native_ptr ? utils::Memory::read_safe<uintptr_t>(native_ptr + offsets::component::k_game_object) : 0;

    }
};

} // namespace onyx::sdk::engine

#endif // ONYX_SDK_ENGINE_COMPONENT_HPP

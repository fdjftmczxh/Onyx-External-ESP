// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_OBJECT_HPP
#define ONYX_SDK_ENGINE_OBJECT_HPP

#include <onyx/sdk/engine/structures/Matrix4x4.hpp>
#include <onyx/utils/Memory.hpp>
#include <string>

namespace onyx::sdk::engine {

/// @brief Unity Object 的基础包装器。
struct Object {

    uintptr_t address;

    explicit Object(uintptr_t addr) noexcept : address(addr) {}
    explicit Object(void* addr) noexcept : address(reinterpret_cast<uintptr_t>(addr)) {}

    [[nodiscard]] bool is_valid() const noexcept {

        return utils::Memory::is_valid_ptr(address);

    }

    template <typename T>
    [[nodiscard]] T read_field(int32_t offset) const noexcept {
        if (!is_valid()) return T{};
        return utils::Memory::read_safe<T>(address + offset);
    }
};

} // namespace onyx::sdk::engine

#endif // ONYX_SDK_ENGINE_OBJECT_HPP

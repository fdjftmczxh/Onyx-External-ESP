// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_ARRAY_HPP
#define ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_ARRAY_HPP

#include <onyx/sdk/engine/structures/Il2CppTypes.hpp>

typedef struct Il2CppArray {

    Il2CppObject obj;
    void*        bounds;
    uintptr_t    max_length;

} Il2CppArray;

namespace onyx {

/// @brief IL2CPP 原生 Array 结构体的类型安全包装器。
/// @tparam T 元素类型。
template <typename T>
struct ManagedArray : public Il2CppArray {

    T vector[1];

    [[nodiscard]] T& operator[](int32_t i) noexcept { return vector[i]; }
    [[nodiscard]] const T& operator[](int32_t i) const noexcept { return vector[i]; }

    [[nodiscard]] int32_t length() const noexcept {

        return static_cast<int32_t>(max_length);

    }

    [[nodiscard]] bool contains(const T& item) const noexcept {

        for (int32_t i = 0; i < length(); ++i) {

            if (vector[i] == item) return true;

        }

        return false;

    }

};

} // namespace onyx

#endif // ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_ARRAY_HPP

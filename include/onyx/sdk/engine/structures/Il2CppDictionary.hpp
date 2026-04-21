// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_DICTIONARY_HPP
#define ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_DICTIONARY_HPP

#include <onyx/sdk/engine/structures/Il2CppArray.hpp>
#include <optional>

namespace onyx {

/// @brief 镜像原生 IL2CPP Dictionary。
template<typename TKey, typename TValue> 
struct Dictionary {

    struct Entry {

        int32_t hashCode; 
        int32_t next;

        TKey    key;
        TValue  value;

    };

    void* klass;
    void* monitor;
    ManagedArray<int32_t>* buckets;
    ManagedArray<Entry>*   entries;
    int32_t count;
    int32_t version;
    int32_t freeList;
    int32_t freeCount;
    void*   comparer;
    void*   keys;
    void*   values;
    void*   syncRoot;

    [[nodiscard]] int32_t size() const noexcept { return count; }

    [[nodiscard]] std::optional<TValue> try_get_value(const TKey& key) const noexcept {

        int index = find_entry(key);

        if (index >= 0) return entries->vector[index].value;

        return std::nullopt;

    }

    [[nodiscard]] int32_t find_entry(const TKey& key) const noexcept {

        if (!entries) return -1;

        for (int i = 0; i < count; ++i) {

            if (entries->vector[i].key == key) return i;

        }

        return -1;

    }

    [[nodiscard]] bool contains_key(const TKey& key) const noexcept {

        return find_entry(key) >= 0;

    }
};

} // namespace onyx

namespace onyx::sdk {

    template <typename K, typename V>
    using Dictionary = ::onyx::Dictionary<K, V>;

}

#endif // ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_DICTIONARY_HPP

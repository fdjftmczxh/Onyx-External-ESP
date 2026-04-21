// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_UTILS_MEMORY_HPP
#define ONYX_UTILS_MEMORY_HPP

#include <KittyMemoryMgr.hpp>
#include <cstdint>
#include <optional>
#include <vector>
#include <string>

namespace onyx::utils {

    class Memory final {

    public:

        /// @brief 外部内存管理器实例。
        static inline KittyMemoryMgr* s_mgr = nullptr;
        static inline uintptr_t       s_base_address = 0;

        /// @brief 从内存地址安全地读取类型为 T 的值。
        /// @tparam T 要读取的数据类型。
        /// @param address 绝对内存地址。
        /// @returns 读取的值，如果读取失败则返回 T 的默认值。
        template<typename T>
        [[nodiscard]] static T read(uintptr_t address) noexcept {

            T buffer{};

            if (!s_mgr || s_mgr->readMem(address, &buffer, sizeof(T)) <= 0) {

                return T{};

            }

            return buffer;

        }

        /// @brief 批量从内存中读取原始数据（用于减少 Syscall 开销的批量读取）。
        /// @param address 起始绝对内存地址。
        /// @param buffer 目标缓冲区的指针。
        /// @param size 要读取的字节数。
        /// @returns 如果成功读取则返回 true，否则返回 false。
        static bool read_raw(uintptr_t address, void* buffer, size_t size) noexcept {

            if (!is_valid_ptr(address) || !s_mgr || !buffer || size == 0) {

                return false;

            }

            return s_mgr->readMem(address, buffer, size) > 0;

        }

        /// @brief 检查指针是否看起来有效。
        [[nodiscard]] static bool is_valid_ptr(uintptr_t addr) noexcept {

            return addr >= 0x1000 && addr < 0x00007FFFFFFFFFFFULL && (addr % sizeof(void*) == 0);

        }

        /// @brief 安全地从内存中读取。如果读取失败，则返回默认值。
        template <typename T>
        [[nodiscard]] static T read_safe(uintptr_t address, T default_value = T{}) noexcept {

            if (!is_valid_ptr(address)) return default_value;

            return read<T>(address);

        }

        /// @brief 安全地将类型为 T 的值写入内存地址。
        /// @tparam T 要写入的数据类型。
        /// @param address 绝对内存地址。
        /// @param value 要写入的值。
        /// @returns 如果成功写入则返回 true，否则返回 false。
        template<typename T>
        static bool write_safe(uintptr_t address, const T& value) noexcept {

            if (!is_valid_ptr(address) || !s_mgr) return false;

            return s_mgr->writeMem(address, (void*)&value, sizeof(T)) > 0;

        }

    };

} // namespace onyx::utils

#endif // ONYX_UTILS_MEMORY_HPP
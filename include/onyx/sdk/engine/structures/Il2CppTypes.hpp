// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_TYPES_HPP
#define ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_TYPES_HPP

#include <cstdint>

typedef struct Il2CppClass    Il2CppClass;
typedef struct MethodInfo     MethodInfo;
typedef struct FieldInfo      FieldInfo;
typedef struct Il2CppAssembly Il2CppAssembly;
typedef struct Il2CppDomain   Il2CppDomain;
typedef struct Il2CppImage    Il2CppImage;

/// @brief IL2CPP 对象的顶级基础结构。
typedef struct Il2CppObject {

    union {

        Il2CppClass* klass;
        void*        vtable;

    };

    void* monitor;

} Il2CppObject;

/// @brief IL2CPP 托管字符串结构。
typedef struct Il2CppString {

    Il2CppObject object;
    int32_t      length;
    uint16_t     chars[0];

} Il2CppString;

#endif // ONYX_SDK_ENGINE_STRUCTURES_IL2CPP_TYPES_HPP

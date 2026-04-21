// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_STRUCTURES_MATRIX4X4_HPP
#define ONYX_SDK_ENGINE_STRUCTURES_MATRIX4X4_HPP

#include <string>
#include <cmath>
#include <stdexcept>

#include <onyx/sdk/engine/structures/Vector3.hpp>
#include <onyx/sdk/engine/structures/Vector4.hpp>
#include <onyx/sdk/engine/structures/Quaternion.hpp>

/// @brief 基础 4x4 矩阵结构。
struct Matrix4x4 {

    union {

        struct {

            float m00, m10, m20, m30; // Column 0
            float m01, m11, m21, m31; // Column 1
            float m02, m12, m22, m32; // Column 2
            float m03, m13, m23, m33; // Column 3

        };

        float data[16];

    };

    inline constexpr Matrix4x4() : m00(0.f), m10(0.f), m20(0.f), m30(0.f), m01(0.f), m11(0.f), m21(0.f), m31(0.f), m02(0.f), m12(0.f), m22(0.f), m32(0.f), m03(0.f), m13(0.f), m23(0.f), m33(0.f) {}

    inline Matrix4x4(Vector4 c0, Vector4 c1, Vector4 c2, Vector4 c3) {

        m00 = c0.x; m10 = c0.y; m20 = c0.z; m30 = c0.w;
        m01 = c1.x; m11 = c1.y; m21 = c1.z; m31 = c1.w;
        m02 = c2.x; m12 = c2.y; m22 = c2.z; m32 = c2.w;
        m03 = c3.x; m13 = c3.y; m23 = c3.z; m33 = c3.w;

    }

    inline float& operator()(int row, int col) {

        return data[row + col * 4];

    }

    inline float operator()(int row, int col) const {

        return data[row + col * 4];

    }

    inline float& operator[](int index) {

        return data[index];

    }

    inline float operator[](int index) const {

        return data[index];

    }

    static inline Matrix4x4 identity() {

        Matrix4x4 res;

        res.m00 = 1.f; res.m11 = 1.f; res.m22 = 1.f; res.m33 = 1.f;

        return res;

    }

    static inline Matrix4x4 zero() {

        return Matrix4x4();

    }

    inline static Matrix4x4 Scale(Vector3 v) {

        Matrix4x4 m;

        m.m00 = v.x; m.m11 = v.y; m.m22 = v.z; m.m33 = 1.f;

        return m;

    }

    inline static Matrix4x4 Translate(Vector3 v) {

        Matrix4x4 m = identity();

        m.m03 = v.x; m.m13 = v.y; m.m23 = v.z;

        return m;

    }

    inline Vector4 GetColumn(int i) const {

        return { data[0 + i * 4], data[1 + i * 4], data[2 + i * 4], data[3 + i * 4] };

    }

    inline Vector3 GetPosition() const {

        return { m03, m13, m23 };

    }

    inline Vector3 MultiplyPoint(Vector3 v) const {

        Vector3 res;

        res.x = m00 * v.x + m01 * v.y + m02 * v.z + m03;
        res.y = m10 * v.x + m11 * v.y + m12 * v.z + m13;
        res.z = m20 * v.x + m21 * v.y + m22 * v.z + m23;

        float w = m30 * v.x + m31 * v.y + m32 * v.z + m33;

        w = 1.0f / w;

        res.x *= w; res.y *= w; res.z *= w;

        return res;

    }

    inline Vector3 MultiplyPoint3x4(Vector3 v) const {

        Vector3 res;

        res.x = m00 * v.x + m01 * v.y + m02 * v.z + m03;
        res.y = m10 * v.x + m11 * v.y + m12 * v.z + m13;
        res.z = m20 * v.x + m21 * v.y + m22 * v.z + m23;

        return res;

    }

    inline Vector3 MultiplyVector(Vector3 v) const {

        Vector3 res;

        res.x = m00 * v.x + m01 * v.y + m02 * v.z;
        res.y = m10 * v.x + m11 * v.y + m12 * v.z;
        res.z = m20 * v.x + m21 * v.y + m22 * v.z;

        return res;
    }

    inline Matrix4x4 operator*(const Matrix4x4& rhs) const {

        Matrix4x4 res;

        for (int r = 0; r < 4; r++) {

            for (int c = 0; c < 4; c++) {

                res(r, c) = (*this)(r, 0) * rhs(0, c) + (*this)(r, 1) * rhs(1, c) + (*this)(r, 2) * rhs(2, c) + (*this)(r, 3) * rhs(3, c);

            }

        }

        return res;

    }

    inline Vector4 operator*(const Vector4& v) const {

        Vector4 res;

        res.x = m00 * v.x + m01 * v.y + m02 * v.z + m03 * v.w;
        res.y = m10 * v.x + m11 * v.y + m12 * v.z + m13 * v.w;
        res.z = m20 * v.x + m21 * v.y + m22 * v.z + m23 * v.w;
        res.w = m30 * v.x + m31 * v.y + m32 * v.z + m33 * v.w;

        return res;

    }

    inline bool operator==(const Matrix4x4& other) const {

        for (int i = 0; i < 16; i++) if (data[i] != other.data[i]) return false;

        return true;

    }

    inline bool operator!=(const Matrix4x4& other) const { return !(*this == other); }

    inline std::string str() const {

        std::string s = "";

        for (int i = 0; i < 4; i++) {

            s += std::to_string((*this)(i, 0)) + "\t" + std::to_string((*this)(i, 1)) + "\t" + std::to_string((*this)(i, 2)) + "\t" + std::to_string((*this)(i, 3)) + "\n";

        }

        return s;

    }

};

namespace onyx::sdk {

    using Matrix4x4 = ::Matrix4x4;

}

#endif // ONYX_SDK_ENGINE_STRUCTURES_MATRIX4X4_HPP

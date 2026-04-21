// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_STRUCTURES_VECTOR3_HPP
#define ONYX_SDK_ENGINE_STRUCTURES_VECTOR3_HPP

#include <cmath>
#include <string>
#include <algorithm>
#include <limits>

struct Vector3 {

    float x, y, z;

    static constexpr float kEpsilon = 0.00001f;
    static constexpr float kEpsilonNormalSqrt = 1e-15f;

    inline constexpr Vector3() : x(0.f), y(0.f), z(0.f) {}
    inline constexpr Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
    inline constexpr Vector3(float x, float y) : x(x), y(y), z(0.f) {}

    inline static Vector3 Lerp(Vector3 a, Vector3 b, float t) {

        t = std::clamp(t, 0.f, 1.f);

        return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);

    }

    inline static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t) {

        return Vector3(a.x + (b.x - a.x) * t, a.y + (b.y - a.y) * t, a.z + (b.z - a.z) * t);

    }

    inline static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta) {

        float toVector_x = target.x - current.x;
        float toVector_y = target.y - current.y;
        float toVector_z = target.z - current.z;

        float sqdist = toVector_x * toVector_x + toVector_y * toVector_y + toVector_z * toVector_z;

        if (sqdist == 0 || (maxDistanceDelta >= 0 && sqdist <= maxDistanceDelta * maxDistanceDelta)) return target;

        float dist = std::sqrt(sqdist);

        return Vector3(current.x + toVector_x / dist * maxDistanceDelta,current.y + toVector_y / dist * maxDistanceDelta,current.z + toVector_z / dist * maxDistanceDelta);

    }

    inline static Vector3 SmoothDamp(Vector3 current, Vector3 target, Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime) {

        smoothTime = std::max(0.0001f, smoothTime);

        float omega = 2.f / smoothTime;

        float x_val = omega * deltaTime;
        float exp_val = 1.f / (1.f + x_val + 0.48f * x_val * x_val + 0.235f * x_val * x_val * x_val);

        float change_x = current.x - target.x;
        float change_y = current.y - target.y;
        float change_z = current.z - target.z;

        Vector3 originalTo = target;

        float maxChange = maxSpeed * smoothTime;
        float maxChangeSq = maxChange * maxChange;
        float sqrmag = change_x * change_x + change_y * change_y + change_z * change_z;

        if (sqrmag > maxChangeSq) {

            float mag = std::sqrt(sqrmag);

            change_x = change_x / mag * maxChange;
            change_y = change_y / mag * maxChange;
            change_z = change_z / mag * maxChange;

        }

        target.x = current.x - change_x;
        target.y = current.y - change_y;
        target.z = current.z - change_z;

        float temp_x = (currentVelocity.x + omega * change_x) * deltaTime;
        float temp_y = (currentVelocity.y + omega * change_y) * deltaTime;
        float temp_z = (currentVelocity.z + omega * change_z) * deltaTime;

        currentVelocity.x = (currentVelocity.x - omega * temp_x) * exp_val;
        currentVelocity.y = (currentVelocity.y - omega * temp_y) * exp_val;
        currentVelocity.z = (currentVelocity.z - omega * temp_z) * exp_val;

        float output_x = target.x + (change_x + temp_x) * exp_val;
        float output_y = target.y + (change_y + temp_y) * exp_val;
        float output_z = target.z + (change_z + temp_z) * exp_val;

        if ((originalTo.x - current.x) * (output_x - originalTo.x) + (originalTo.y - current.y) * (output_y - originalTo.y) + (originalTo.z - current.z) * (output_z - originalTo.z) > 0) {

            output_x = originalTo.x;
            output_y = originalTo.y;
            output_z = originalTo.z;

            currentVelocity.x = (output_x - originalTo.x) / deltaTime;
            currentVelocity.y = (output_y - originalTo.y) / deltaTime;
            currentVelocity.z = (output_z - originalTo.z) / deltaTime;

        }

        return Vector3(output_x, output_y, output_z);

    }

    inline void Set(float newX, float newY, float newZ) { x = newX; y = newY; z = newZ; }

    inline static Vector3 Scale(Vector3 a, Vector3 b) { return Vector3(a.x * b.x, a.y * b.y, a.z * b.z); }
    inline void Scale(Vector3 scale) { x *= scale.x; y *= scale.y; z *= scale.z; }

    inline static Vector3 Cross(Vector3 lhs, Vector3 rhs) {

        return Vector3(lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x);

    }

    inline static float Dot(Vector3 lhs, Vector3 rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }

    inline float magnitude() const { return std::sqrt(x * x + y * y + z * z); }
    inline float sqrMagnitude() const { return x * x + y * y + z * z; }

    inline static Vector3 Normalize(Vector3 value) {

        float mag = value.magnitude();

        if (mag > kEpsilon) return value / mag;

        return zero();

    }

    inline void Normalize() {

        float mag = magnitude();

        if (mag > kEpsilon) *this = *this / mag;

        else *this = zero();

    }

    inline Vector3 normalized() const { return Vector3::Normalize(*this); }

    inline static float Distance(Vector3 a, Vector3 b) {

        return (a - b).magnitude();

    }

    inline static Vector3 Project(Vector3 vector, Vector3 onNormal) {

        float sqrMag = Dot(onNormal, onNormal);

        if (sqrMag < std::numeric_limits<float>::epsilon()) return zero();

        return onNormal * Dot(vector, onNormal) / sqrMag;

    }

    inline float operator[](int index) const {

        if (index == 0) return x;
        if (index == 1) return y;

        return z;

    }

    inline Vector3 operator+(const Vector3& b) const { return Vector3(x + b.x, y + b.y, z + b.z); }
    inline Vector3 operator-(const Vector3& b) const { return Vector3(x - b.x, y - b.y, z - b.z); }
    inline Vector3 operator-() const { return Vector3(-x, -y, -z); }
    inline Vector3 operator*(float d) const { return Vector3(x * d, y * d, z * d); }
    inline Vector3 operator/(float d) const { return Vector3(x / d, y / d, z / d); }

    inline bool operator==(const Vector3& rhs) const {

        Vector3 diff = *this - rhs;

        return diff.sqrMagnitude() < (kEpsilon * kEpsilon);

    }

    inline bool operator!=(const Vector3& rhs) const { return !(*this == rhs); }

    static inline Vector3 zero() { return Vector3(0.f, 0.f, 0.f); }
    static inline Vector3 one() { return Vector3(1.f, 1.f, 1.f); }
    static inline Vector3 forward() { return Vector3(0.f, 0.f, 1.f); }
    static inline Vector3 back() { return Vector3(0.f, 0.f, -1.f); }
    static inline Vector3 up() { return Vector3(0.f, 1.f, 0.f); }
    static inline Vector3 down() { return Vector3(0.f, -1.f, 0.f); }
    static inline Vector3 left() { return Vector3(-1.f, 0.f, 0.f); }
    static inline Vector3 right() { return Vector3(1.f, 0.f, 0.f); }

    inline std::string str() const {

        return "(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")";

    }

};

inline Vector3 operator*(float d, const Vector3& a) { return Vector3(a.x * d, a.y * d, a.z * d); }

namespace onyx::sdk {

    using Vector3 = ::Vector3;

}

#endif // ONYX_SDK_ENGINE_STRUCTURES_VECTOR3_HPP

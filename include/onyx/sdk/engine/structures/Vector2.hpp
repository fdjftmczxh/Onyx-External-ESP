// 作者: feng_xingzhe_cn_.86

#pragma once
#ifndef ONYX_SDK_ENGINE_STRUCTURES_VECTOR2_HPP
#define ONYX_SDK_ENGINE_STRUCTURES_VECTOR2_HPP

#include <cmath>

namespace onyx::sdk {

struct Vector2 {

    float x;
    float y;

    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float x, float y) : x(x), y(y) {}

    [[nodiscard]] float length() const {

        return std::sqrt(x * x + y * y);

    }

    [[nodiscard]] float distance(const Vector2& other) const {

        return Vector2(x - other.x, y - other.y).length();

    }

    Vector2 operator+(const Vector2& v) const { return {x + v.x, y + v.y}; }
    Vector2 operator-(const Vector2& v) const { return {x - v.x, y - v.y}; }
    Vector2 operator*(float f) const { return {x * f, y * f}; }
    Vector2 operator/(float f) const { return {x / f, y / f}; }

};

} // namespace onyx::sdk

#endif // ONYX_SDK_ENGINE_STRUCTURES_VECTOR2_HPP

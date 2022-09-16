#pragma once
#include <raylib-cpp.hpp>

namespace data {

    struct IVector2 {
        signed int x, y;
        IVector2(signed int x = 0, signed int y = 0) : x(x), y(y) {}
        IVector2(const Vector2& other) : x(other.x), y(other.y) {}
        IVector2& operator=(const Vector2& other) {
            x = static_cast<signed int>(other.x);
            y = static_cast<signed int>(other.y);
            return *this;
        }
        bool operator==(const IVector2& other) const {
            return x == other.x && y == other.y;
        }
        IVector2 diff(const IVector2& other) const { return *this - other; }
        IVector2 operator-(const IVector2& other) const {
            return IVector2(x - other.x, y - other.y);
        }
        operator Vector2() const {
            return Vector2(static_cast<float>(x), static_cast<float>(y));
        }
        Vector2 operator+(const IVector2& other) const {
            return IVector2(x + other.x, y + other.y);
        }
    };
} // namespace data
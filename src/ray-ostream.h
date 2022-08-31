#pragma once
#include <ostream>
#include <raylib-cpp.hpp>

std::ostream& operator<<(std::ostream& os, const Vector2& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Vector3& v) {
    os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Color& v) {
    os << "(" << (int)v.r << ", " << (int)v.g << ", " << (int)v.b << ", "
       << (int)v.a << ")";
    return os;
}
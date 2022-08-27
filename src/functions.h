#pragma once
#include <raylib-cpp.hpp>
#include <string.h>

template <typename... Args> inline void println(const char* fmt, Args... args) {
    char* fixed_fmt = new char[strlen(fmt) + 1];
    std::snprintf(fixed_fmt, 1024, "%s\n", fmt);
    std::printf(fixed_fmt, args...);
    delete[] fixed_fmt;
}

inline Vector2 operator+(const Vector2& a, const Vector2& b) {
    return Vector2{a.x + b.x, a.y + b.y};
}

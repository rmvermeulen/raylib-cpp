#pragma once
#include <raylib/raylib-cpp.hpp>

bool is_empty(const Vector2& v) { return v.x && v.y; }
bool is_empty(const Vector3& v) { return v.x && v.y && v.z; }

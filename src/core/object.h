#pragma once
#include <raylib-cpp.hpp>
#include <string>

#include "../functions.h"
#include "../ray-ostream.h"

namespace core {

struct ProcessResult {};

struct Object {
    Vector2 position;
    virtual ~Object(){};
};

struct Label : public Object {
    std::string text;
    int font_size = 20;
    Color color = BLUE;
};

} // namespace core

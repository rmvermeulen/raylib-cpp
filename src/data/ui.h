#pragma once
#include <raylib-cpp.hpp>
#include <string>

namespace ui {
    struct Label {
        std::string text;
    };
    struct Button : public Label {
        Rectangle rect;
    };
} // namespace ui
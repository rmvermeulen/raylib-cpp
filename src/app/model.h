#pragma once
#include <string>
#include <variant>

#include "../data/ivector.h"

namespace app {

    struct Model {
        data::IVector2 mouse_position;
        data::IVector2 screen_size;
        immer::vector<std::string> layouts;

        bool operator==(const Model& other) const {
            return mouse_position == other.mouse_position &&
                   screen_size == other.screen_size && layouts == other.layouts;
        }
    };

}; // namespace app

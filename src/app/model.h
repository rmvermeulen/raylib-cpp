#pragma once
#include <string>
#include <variant>

#include "../data/ivector.h"

namespace app {

    struct Model {
        data::IVector2 mouse_position;
        data::IVector2 screen_size;
        immer::vector<const std::string> layouts;

        bool operator==(const Model& other) const {
            auto eq = mouse_position == other.mouse_position &&
                      screen_size == other.screen_size;
            auto d1 = mouse_position.diff(other.mouse_position);
            auto d2 = screen_size.diff(other.screen_size);
            // fmt::print("comparing states... eq={} ({}, {}) & ({}, {})\n", eq,
            //            d1.x, d1.y, d2.x, d2.y);

            return eq;
        }
    };

}; // namespace app

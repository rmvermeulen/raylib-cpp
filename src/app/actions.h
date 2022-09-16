#pragma once

#include <variant>

#include "../data/ivector.h"

namespace app {
    namespace actions {
        struct SetMousePosition {
            data::IVector2 position;
        };
        struct SetScreenSize {
            data::IVector2 size;
        };
    } // namespace actions

    typedef std::variant<actions::SetMousePosition, actions::SetScreenSize>
        Action;

} // namespace app
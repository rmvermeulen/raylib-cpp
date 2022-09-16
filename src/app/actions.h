#pragma once

#include <variant>

#include "../data/ivector.h"

namespace app {
    namespace actions {
        struct Initialize {};
        struct SetMousePosition {
            data::IVector2 position;
        };
        struct SetScreenSize {
            data::IVector2 size;
        };
        struct SetLayouts {
            immer::box<immer::vector<std::string>> layouts;
        };
        typedef std::variant<Initialize, SetMousePosition, SetScreenSize,
                             SetLayouts>
            Action;
    } // namespace actions

    using actions::Action;
} // namespace app
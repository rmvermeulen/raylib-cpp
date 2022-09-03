#pragma once
#include <variant>

namespace state {
    namespace actions {
        struct count_frame {};
        struct count_click {};
    } // namespace actions

    using Action = std::variant<actions::count_frame, actions::count_click>;
} // namespace state
#pragma once
#include <variant>

#include "../game.h"
#include "./model.h"

namespace state {
    namespace actions {
        struct add_factory {
            Factory factory;
        };
        struct use_factory {
            int index;
        };
        struct close_game {};
    } // namespace actions

    using ActionType = std::variant<actions::add_factory, actions::use_factory,
                                    actions::close_game>;
} // namespace state
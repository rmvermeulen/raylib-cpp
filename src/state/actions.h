#pragma once
#include <variant>

#include "../game.h"
#include "./model.h"

namespace state {
    namespace actions {
        struct AddFactory {
            Factory factory;
        };
        struct UseFactory {
            int index;
        };
        struct CloseGame {};
    } // namespace actions

    using ActionType = std::variant<actions::AddFactory, actions::UseFactory,
                                    actions::CloseGame>;
} // namespace state
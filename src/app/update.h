#pragma once
#include <lager/util.hpp>

#include "./actions.h"
#include "./model.h"

namespace app {
    Model update(Model model, Action action) {
        return std::visit(
            lager::visitor{
                [&](actions::SetMousePosition& action) {
                    if (model.mouse_position == action.position)
                        return model;
                    auto diff = model.mouse_position.diff(action.position);
                    fmt::print("SetMousePos diff({}, {})\n", diff.x, diff.y);
                    model.mouse_position = action.position;
                    return model;
                },
                [&](actions::SetScreenSize& action) {
                    if (model.screen_size == action.size)
                        return model;
                    model.screen_size = action.size;
                    return model;
                },
            },
            action);
    }
} // namespace app

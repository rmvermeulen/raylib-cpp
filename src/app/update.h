#pragma once
#include <immer/vector.hpp>
#include <lager/util.hpp>

#include "./actions.h"
#include "./model.h"

namespace app {

    using result = std::pair<Model, lager::effect<Action>>;

    result update(Model model, Action action) {
        const auto just = [](Model model) -> result {
            return {model, lager::noop};
        };
        return std::visit(
            lager::visitor{
                [&](const actions::Initialize& action) -> result {
                    return {model, [](auto&& ctx) {
                                fmt::print("Initialize\n");
                                auto home = getenv("HOME");
                                auto layout_path =
                                    fmt::format("{}/.screenlayout", home);

                                immer::vector<std::string> layouts;
                                for (auto entry :
                                     std::filesystem::directory_iterator{
                                         layout_path}) {
                                    const auto name =
                                        entry.path().filename().string();
                                    if (name.length() < 1 || name[0] == '.') {
                                        fmt::print("skipping {}...\n", name);
                                        continue;
                                    }
                                    layouts = layouts.push_back(name);
                                }
                                ctx.dispatch(actions::SetLayouts{layouts});
                            }};
                },
                [&](const actions::SetMousePosition& action) -> result {
                    if (model.mouse_position == action.position)
                        return just(model);
                    // fmt::print("SetMousePosition\n");
                    model.mouse_position = action.position;
                    return just(model);
                },
                [&](const actions::SetScreenSize& action) -> result {
                    if (model.screen_size == action.size)
                        return just(model);
                    fmt::print("SetScreenSize\n");
                    model.screen_size = action.size;
                    return just(model);
                },
                [&](const actions::SetLayouts& action) -> result {
                    fmt::print("SetLayouts ({})\n",
                               action.layouts.get().size());
                    model.layouts = action.layouts.get();
                    return just(model);
                }},
            action);
    }
} // namespace app

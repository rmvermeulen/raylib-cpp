#pragma once
#include "./actions.h"
#include "./model.h"
#include <lager/util.hpp>

namespace state {

    Model update(Model model, Action action) {
        const auto new_model =
            std::visit(lager::visitor{
                           [&](actions::count_frame) {
                               model.frames = model.frames.update(add(1));
                               return model;
                           },
                           [&](actions::count_click) {
                               model.clicks = model.clicks.update(add(1));
                               return model;
                           },
                       },
                       action);

        // cereal::JSONOutputArchive archive(std::cout);
        // archive(CEREAL_NVP(new_model));
        return new_model;
    }
} // namespace state

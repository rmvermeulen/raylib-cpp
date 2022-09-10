#pragma once

#include <lager/util.hpp>

#include "./actions.h"
#include "./model.h"

namespace state {

    Model update(Model model, ActionType action) {
        const auto new_model = std::visit(
            lager::visitor{[&](actions::add_factory action) {
                               model.factories =
                                   model.factories.push_back(action.factory);
                               return model;
                           },
                           [&](actions::use_factory action) {
                               if (action.index >= model.factories.size())
                                   return model;
                               auto factory = model.factories[action.index];
                               model.current_game = factory();
                               return model;
                           },
                           [&](actions::close_game) {
                               model.current_game = {};
                               return model;
                           }},
            action);

        // cereal::JSONOutputArchive archive(std::cout);
        // archive(CEREAL_NVP(new_model));
        return new_model;
    }
} // namespace state

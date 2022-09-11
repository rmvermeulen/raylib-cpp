#include "./schmup.h"

#include <lager/util.hpp>

#include "../game.h"

namespace schmup {
    IGame::TickResult Schmup::tick() {
        // update state
        // render state
        // update the game
        return TickResult::Continue;
    }
    void Schmup::render() const {}

    state::Model Schmup::reduce(state::Model model, state::ActionType action) {
        using namespace state::actions;
        return std::visit(
            lager::visitor{[&](AddEntity action) {
                               model.entities =
                                   model.entities.push_back(action.entity);
                               return model;
                           },
                           [&](RemoveEntity action) {
                               immer::vector<Entity> entities;
                               for (const auto& entity : model.entities) {
                                   if (entity.id == action.id)
                                       continue;
                                   entities = entities.push_back(entity);
                               }
                               model.entities = entities;
                               return model;
                           },
                           [&](UpdateEntity action) { return model; }},
            action);
    }
} // namespace schmup
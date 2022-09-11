#pragma once
#include "../game.h"
#include "../store-base.h"
#include "./state/actions.h"
#include "./state/model.h"
namespace schmup {
    typedef ::StoreBase<state::Model, state::ActionType> Store;
    class Schmup : public IGame {
        Store store;

      public:
        static state::Model reduce(state::Model model,
                                   state::ActionType action) {
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

        Schmup() : store(reduce) {
            store.dispatch(state::actions::AddEntity{Entity{0, 0, 10}});
            store.dispatch(state::actions::AddEntity{Entity{1, 0, 10}});
            store.dispatch(state::actions::AddEntity{Entity{2, 0, 10}});
                }
        TickResult tick() override {
            // update state
            // render state
            // update the game
            return TickResult::Continue;
        }
        void serialize(cereal::JSONOutputArchive& archive) const override {
            const state::Model state = store.get_state();
            archive(cereal::make_nvp("name", std::string{"schmup"}),
                    cereal::make_nvp("state", state));
        };
    };
} // namespace schmup
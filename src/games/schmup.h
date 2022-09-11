#pragma once

#include <cereal/archives/json.hpp>
#include <cereal/cereal.hpp>

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
                                   state::ActionType action);

        Schmup() : store(reduce) {
            store.dispatch(state::actions::AddEntity{Entity{0, 0, 10}});
            store.dispatch(state::actions::AddEntity{Entity{1, 0, 10}});
            store.dispatch(state::actions::AddEntity{Entity{2, 0, 10}});
        }
        TickResult tick() override;
        void render() const override;
        void serialize(cereal::JSONOutputArchive& archive) const override {
            const state::Model state = store.get_state();
            archive(cereal::make_nvp("name", std::string{"schmup"}),
                    cereal::make_nvp("state", state));
        };
    };
} // namespace schmup
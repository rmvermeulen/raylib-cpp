#pragma once

#include "./actions.h"
#include "./model.h"
#include "./update.h"
namespace app {

    lager::store<Action, Model> initialize() {
        auto store = lager::make_store<app::Action>(
            app::Model{}, lager::with_manual_event_loop{});
        store.dispatch(actions::Initialize{});
        return store;
    }
} // namespace app
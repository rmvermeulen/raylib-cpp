#pragma once

#include "./actions.h"
#include "./model.h"
#include "./reducers.h"
#include "./store.h"

namespace state {
    using DefaultReducer = decltype(update);
    typedef Store<Model, ActionType> DefaultStore;
} // namespace state

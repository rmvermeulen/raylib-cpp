#pragma once

#include "./actions.h"
#include "./model.h"
#include "./reducers.h"
#include "./store.h"

namespace state {
    typedef Store<Model, typeof update, ActionType> DefaultStore;
} // namespace state

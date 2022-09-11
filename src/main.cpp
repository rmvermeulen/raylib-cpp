#define RAYGUI_IMPLEMENTATION
#define RUN_TESTS 0

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>

#include <cereal/archives/json.hpp>
#include <immer/box.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>
#include <raylib/raylib-cpp.hpp>

#include "./core/object.h"
#include "./functions.h"
#include "./state/actions.h"
#include "./state/model.h"
#include "./state/reducers.h"

int main() {

    auto initial_state = state::Model{};
    {
        cereal::JSONOutputArchive archive(std::cout);
        archive(CEREAL_NVP(initial_state));
    }

    auto store = lager::make_store<state::Action>(
        initial_state, lager::with_manual_event_loop());

    store.watch([&](auto state) {
        cereal::JSONOutputArchive archive(std::cout);
        archive(cereal::make_nvp("new state", state));
    });

    store.dispatch(state::actions::count_frame{});
    store.dispatch(state::actions::count_frame{});
    store.dispatch(state::actions::count_frame{});
    store.dispatch(state::actions::count_click{});

    cereal::JSONOutputArchive archive(std::cout);
    archive(cereal::make_nvp("final state", store.get()));

    return 0;
}

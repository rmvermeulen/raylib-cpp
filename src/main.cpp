#define RAYGUI_IMPLEMENTATION

#include <algorithm>
#include <chrono>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>

#include <cereal/archives/json.hpp>
#include <immer/box.hpp>
#include <lager/util.hpp>
#include <raylib/raylib-cpp.hpp>

#include "./game.h"
#include "./games/schmup.h"
#include "./state/actions.h"
#include "./state/model.h"
#include "./state/reducer.h"
#include "./utils/raylib-bool.h"
#include "./utils/raylib-ostream.h"

typedef std::function<void(std::stringstream&)> ss_operator;

std::string build_string(ss_operator builder) {
    std::stringstream ss;
    builder(ss);
    return ss.str();
}

template <typename T> ss_operator ss_append(const T& value) {
    return [=](std::stringstream& ss) { ss << value; };
}

ss_operator ss_sequence(const std::vector<ss_operator>& actions) {
    return [=](std::stringstream& ss) {
        for (auto& action : actions) {
            action(ss);
        }
    };
}

int main() {
    const size_t text_size = 20;

    StoreBase<state::Model, state::ActionType> store{
        [](auto s, auto a) { return state::reducer(s, a); }};

    store.dispatch(state::actions::AddFactory{
        [] { return std::make_shared<schmup::Schmup>(); }});
    store.dispatch(state::actions::UseFactory{0});

    raylib::Window window{};

    window.SetTargetFPS(60);

    std::vector<std::pair<std::string, int>> labels;

    int countdown = 0;

    while (!window.ShouldClose()) {

        // update labels
        for (auto& label : labels)
            ++label.second;
        std::remove_if(labels.begin(), labels.end(),
                       [](const auto& label) { return label.second > 200; });

        // update active game
        const auto state = store.get_state();
        if (state.current_game.has_value())
            state.current_game.value()->tick();

        BeginDrawing();
        ClearBackground(SKYBLUE);

        state.current_game.value()->render();

        auto fps = build_string([](auto& ss) { ss << "FPS: " << GetFPS(); });
        DrawText(fps.c_str(), 0, 0, text_size, BLACK);
#ifdef DUMP_STATE_JSON
        auto json = build_string([&](auto& ss) {
            cereal::JSONOutputArchive archive{ss};
            archive(cereal::make_nvp("state", store.get_state()));
        });
        DrawText(json.c_str(), 0, 20, text_size, BLACK);
#endif

        EndDrawing();
    }

    return 0;
}

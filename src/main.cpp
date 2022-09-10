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

#include "./state/default_store.h"
#include "./utils/raylib-bool.h"
#include "./utils/raylib-ostream.h"

#include "./game.h"
#include "./games/schmup.h"

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

    state::DefaultStore store{
        [](auto s, auto a) { return state::update(s, a); }};

    store.dispatch(
        state::actions::add_factory{[] { return std::make_shared<Schmup>(); }});
    store.dispatch(state::actions::use_factory{0});

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
        ClearBackground(WHITE);

        if (!countdown) {
            auto md = GetMouseDelta();
            if (is_empty(md)) {
                countdown = 10;
                auto smd = build_string(ss_append(md));
                labels.emplace_back(std::pair<std::string, int>{smd, 0});
            }
        } else {
            --countdown;
        }

        auto first_label = std::string("MouseDelta: ") + labels[0].first;
        DrawText(first_label.c_str(), 100, 100, 24, BLACK);
        bool first = true;
        for (const auto& label : labels) {
            if (first) {
                first = false;
                continue;
            }
            DrawText(label.first.c_str(), 252, 100 - label.second, 24,
                     {0, 0, 0,
                      static_cast<unsigned char>(
                          std::max(0, 255 - 5 * label.second))});
        }

        auto sft = build_string(
            [](auto& ss) { ss << "FrameTime: " << GetFrameTime(); });
        DrawText(sft.c_str(), 100, 150, 24, BLACK);

        auto fps = build_string([](auto& ss) { ss << "FPS: " << GetFPS(); });
        DrawText(fps.c_str(), 100, 200, 24, BLACK);

        auto json = build_string([&](auto& ss) {
            cereal::JSONOutputArchive archive{ss};
            archive(cereal::make_nvp("state", store.get_state()));
        });
        DrawText(json.c_str(), 100, 250, 24, BLACK);

        EndDrawing();
    }

    return 0;
}

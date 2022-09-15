#include <algorithm>
#include <chrono>
#include <fmt/format.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>

#include <cereal/archives/json.hpp>
#include <immer/box.hpp>
#include <immer/map.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>
#include <raygui.h>
#include <raylib-cpp.hpp>

struct IVector2 {
    signed int x, y;
    IVector2(signed int x = 0, signed int y = 0) : x(x), y(y) {}
    IVector2(const Vector2& other) : x(other.x), y(other.y) {}
    IVector2& operator=(const Vector2& other) {
        x = static_cast<signed int>(other.x);
        y = static_cast<signed int>(other.y);
        return *this;
    }
    bool operator==(const IVector2& other) const {
        return x == other.x && y == other.y;
    }
    IVector2 diff(const IVector2& other) const {
        return IVector2(x - other.x, y - other.y);
    }
};

namespace app {
    struct SetMousePosition {
        IVector2 position;
    };
    struct SetScreenSize {
        IVector2 size;
    };
    typedef std::variant<SetMousePosition, SetScreenSize> Action;

    struct Model {
        IVector2 mouse_position;
        IVector2 screen_size;
        bool operator==(const Model& other) const {
            auto eq = mouse_position == other.mouse_position &&
                      screen_size == other.screen_size;
            auto d1 = mouse_position.diff(other.mouse_position);
            auto d2 = screen_size.diff(other.screen_size);
            // fmt::print("comparing states... eq={} ({}, {}) & ({}, {})\n", eq,
            //            d1.x, d1.y, d2.x, d2.y);

            return eq;
        }
    };

    Model update(Model model, Action action) {
        return std::visit(
            lager::visitor{
                [&](SetMousePosition& action) {
                    if (model.mouse_position == action.position)
                        return model;
                    auto diff = model.mouse_position.diff(action.position);
                    fmt::print("SetMousePos diff({}, {})\n", diff.x, diff.y);
                    model.mouse_position = action.position;
                    return model;
                },
                [&](SetScreenSize& action) {
                    if (model.screen_size == action.size)
                        return model;
                    model.screen_size = action.size;
                    return model;
                },
            },
            action);
    }

}; // namespace app

int main() {
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window window(screenWidth, screenHeight,
                          "Raylib C++ Starter Kit Example", false);

    size_t frames_rendered = 0;

    auto store = lager::make_store<app::Action>(
        app::Model{}, lager::with_manual_event_loop{});

    auto previous_state = store.get();

    // window.SetTargetFPS(60);

    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        // update state
        store.dispatch(app::SetMousePosition{raylib::Mouse::GetPosition()});
        store.dispatch(app::SetScreenSize{window.GetSize()});

        // Draw
        if (store.get() != previous_state) {
            window.BeginDrawing().ClearBackground(raylib::Color::SkyBlue());

            previous_state = store.get();

            auto cursor_pos = store.get().mouse_position;
            raylib::DrawText(
                fmt::format("Mouse x={} y={}", cursor_pos.x, cursor_pos.y), 20,
                20, 20, raylib::Color::Black());

            ++frames_rendered;
            fmt::print("frame {} rendered\n", frames_rendered);
            window.EndDrawing();
            SwapScreenBuffer();
        }
        PollInputEvents();
    }
    return 0;
}

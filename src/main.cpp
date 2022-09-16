#define RAYGUI_IMPLEMENTATION

#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fmt/format.h>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <type_traits>

#include <boolinq/boolinq.h>
#include <cereal/archives/json.hpp>
#include <immer/box.hpp>
#include <immer/map.hpp>
#include <immer/vector.hpp>
#include <lager/event_loop/manual.hpp>
#include <lager/store.hpp>
#include <lager/util.hpp>
#include <raygui.h>
#include <raylib-cpp.hpp>

#include "app/app.hpp"
#include "data/ivector.h"

namespace layout {
    template <typename T>
    using render_fn =
        std::function<void(const data::IVector2& position, const T& item)>;

    using step_fn = std::function<data::IVector2(data::IVector2)>;

    template <typename T>
    void custom(const immer::vector<T>& items, data::IVector2 position,
                step_fn next, render_fn<T> display) {
        for (const auto& item : items) {
            display(position, item);
            position = next(position);
        }
    }

    template <typename T>
    void stepped(const immer::vector<T>& items, data::IVector2 position,
                 const data::IVector2& step, render_fn<T> display) {
        for (const auto& item : items) {
            display(position, item);
            position = position + step;
        }
    }
    template <typename T>
    void column(const immer::vector<T>& items, const data::IVector2& position,
                uint step, render_fn<T> display) {
        stepped(items, position, {0, step}, display);
    }
    template <typename T>
    void row(const immer::vector<T>& items, const data::IVector2& position,
             uint step, render_fn<T> display) {
        stepped(items, position, {step, 0}, display);
    }
} // namespace layout
int main() {
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window window(screenWidth, screenHeight,
                          "Raylib C++ Starter Kit Example", false);

    size_t frames_rendered = 0;

    auto store = app::initialize();
    auto previous_state = app::Model{};

    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        // update state
        store.dispatch(
            app::actions::SetMousePosition{raylib::Mouse::GetPosition()});
        store.dispatch(app::actions::SetScreenSize{window.GetSize()});

        // Draw
        if (store.get() != previous_state) {
            previous_state = store.get();

            window.BeginDrawing().ClearBackground(raylib::Color::SkyBlue());

            auto cursor_pos = store.get().mouse_position;
            raylib::DrawText(
                fmt::format("Mouse x={} y={}", cursor_pos.x, cursor_pos.y), 20,
                20, 20, raylib::Color::Black());

            auto screen_size = store.get().screen_size;
            raylib::Rectangle outline_rect(
                Vector2(10, 10), Vector2(screen_size - data::IVector2(20, 20)));
            outline_rect.DrawLines(raylib::Color::Yellow());

            raylib::Rectangle mouse_rect(cursor_pos - data::IVector2(20, 20),
                                         {40, 40});
            mouse_rect.DrawLines(raylib::Color::White());

            auto draw_string = [](const auto& pos, const auto& str) -> void {
                raylib::DrawText(str, pos.x, pos.y, 20, raylib::Color::Black());
            };

            layout::column<std::string>(store.get().layouts, {200, 150}, 20,
                                        draw_string);

            layout::stepped<std::string>(store.get().layouts, {200, 50},
                                         {50, 20}, draw_string);

            if (GuiButton(Rectangle{10, 10, 100, 100}, "some button!")) {
                fmt::print("pressed!");
            }

            window.EndDrawing();
            SwapScreenBuffer();

            ++frames_rendered;
            // fmt::print("frame {} rendered\n", frames_rendered);
        }
        PollInputEvents();
    }
    return 0;
}

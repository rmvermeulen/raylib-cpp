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

            int i = 0;
            for (const auto& layout : store.get().layouts) {

                raylib::DrawText(layout, 200, 50 + i * 20, 20,
                                 raylib::Color::Black());
                ++i;
            }

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

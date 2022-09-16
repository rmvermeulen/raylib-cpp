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

    auto store = lager::make_store<app::Action>(
        app::Model{}, lager::with_manual_event_loop{});

    auto previous_state = store.get();

    // window.SetTargetFPS(60);

    auto home = getenv("HOME");
    auto layout_path = fmt::format("{}/.screenlayout", home);

    std::vector<std::filesystem::directory_entry> entries;
    for (auto entry : std::filesystem::directory_iterator{layout_path}) {
        entries.push_back(std::move(entry));
    }

    auto layouts = boolinq::from(entries)
                       .where([](std::filesystem::directory_entry entry) {
                           return entry.is_regular_file();
                       })
                       .select([](std::filesystem::directory_entry entry) {
                           return entry.path().filename().string();
                       })
                       .where([](auto name) {
                           return name.length() > 1 && name[0] != '.';
                       })
                       .toStdVector();
    fmt::print("layout: {}\n", layouts.size());
    for (auto item : layouts) {
        fmt::print("item = {};\n", item);
    }

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
            for (auto layout : layouts) {

                raylib::DrawText(layout, 20, 50 + i * 20, 20,
                                 raylib::Color::Black());
                ++i;
            }

            if (GuiButton(Rectangle{10, 10, 100, 100}, "some button!")) {
                fmt::print("pressed!");
            }

            window.EndDrawing();
            SwapScreenBuffer();

            ++frames_rendered;
            fmt::print("frame {} rendered\n", frames_rendered);
        }
        PollInputEvents();
    }
    return 0;
}

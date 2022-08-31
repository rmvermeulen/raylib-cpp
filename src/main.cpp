#define RAYGUI_IMPLEMENTATION
#define RUN_TESTS 0

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>

#include <cereal/archives/json.hpp>
#include <immer/box.hpp>
#include <lager/util.hpp>
#include <raygui.h>
#include <raylib-cpp.hpp>

#include "./core/object.h"
#include "./functions.h"

namespace state {
    struct Model {
        int counter{0};
        immer::box<int> bCounter{0};
        template <class Archive> void serialize(Archive& archive) {
            archive(CEREAL_NVP(counter));
        }
    };
    namespace actions {
        struct increment {};
        struct decrement {};
        struct reset {
            int new_counter = 0;
        };
    } // namespace actions

    using Action =
        std::variant<actions::increment, actions::decrement, actions::reset>;

    Model update(Model model, Action action) {
        const auto new_model = std::visit(lager::visitor{
                                              [&](actions::increment) {
                                                  ++model.counter;
                                                  return model;
                                              },
                                              [&](actions::decrement) {
                                                  --model.counter;
                                                  return model;
                                              },
                                              [&](actions::reset action) {
                                                  model.counter =
                                                      action.new_counter;
                                                  return model;
                                              },
                                          },
                                          action);

        cereal::JSONOutputArchive archive(std::cout);
        archive(CEREAL_NVP(new_model.counter));
        return new_model;
    }
} // namespace state

void run_state_stuff() {
    const auto old_model = state::Model{0};
    auto new_model = old_model;
    for (int i = 0; i < 10; ++i)
        new_model = update(new_model, state::actions::increment{});
}

void run_game() {
    int wait;
    std::cin >> wait;
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window window(screenWidth, screenHeight,
                          "Raylib C++ Starter Kit Example", false);

    SetTargetFPS(60);

    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        // Update

        // TODO: Update your variables here

        // Draw
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        textColor.DrawText("Congrats! You created your first window!", 190, 200,
                           20);
        textColor.DrawText("And this is my own addition, even!", 190, 220, 20);

        if (GuiButton(Rectangle{10, 10, 100, 100},
                      "#05#Open Image")) { /* ACTION */
            println("click!");
        }

        EndDrawing();
    }
}

int main() {
    run_state_stuff();
#if RUN_TESTS
    println("main: running tests...");
    tests::run_all();
    println("main: tests done!");
#else
    println("main: starting game!");
    run_game();
#endif
    return 0;
}

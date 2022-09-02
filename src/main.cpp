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
#include <lager/util.hpp>
#include <raygui.h>
#include <raylib-cpp.hpp>

#include "./core/object.h"
#include "./functions.h"
#include "./store.h"

namespace State {

    int add(int a, int b) { return a + b; }
    std::function<int(int)> add(int a) {
        return [=](int b) { return add(a, b); };
    }

    struct Model {
        int counter{0};
        immer::box<int> bCounter{0};
        template <class Archive> void serialize(Archive& archive) {
            archive(CEREAL_NVP(counter), CEREAL_NVP(bCounter.get()));
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
        const auto new_model =
            std::visit(lager::visitor{
                           [&](actions::increment) {
                               ++model.counter;
                               model.bCounter = model.bCounter.update(add(1));
                               return model;
                           },
                           [&](actions::decrement) {
                               --model.counter;
                               return model;
                           },
                           [&](actions::reset action) {
                               model.counter = action.new_counter;
                               return model;
                           },
                       },
                       action);

        // cereal::JSONOutputArchive archive(std::cout);
        // archive(CEREAL_NVP(new_model));
        return new_model;
    }
} // namespace State

class Reducer {
  public:
    State::Model reduce(State::Model m, State::Action a) {
        return State::update(m, a);
    }
};

void run_game() {
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window window(screenWidth, screenHeight,
                          "Raylib C++ Starter Kit Example", false);

    SetTargetFPS(60);

    Store<State::Model,
          std::function<State::Model(State::Model, State::Action)>,
          State::Action>
        store{&State::update};

    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        cereal::JSONOutputArchive json_out(std::cout);
        // Update
        store.dispatch(State::actions::increment{});
        // TODO: Update your variables here
        // std::cout << '\n';
        // json_out(CEREAL_NVP(store.get_state().counter));

        // Draw
        BeginDrawing();
        ClearBackground(GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR)));
        textColor.DrawText("Congrats! You created your first window!", 190, 200,
                           20);
        {
            std::stringstream ss;
            ss << "And this is my own addition: ";
            cereal::JSONOutputArchive dump(ss);
            auto state = store.get_state();
            dump(CEREAL_NVP(state));
            // s += std::to_string(store.get_state().counter);
            textColor.DrawText(ss.str(), 190, 220, 20);
        }

        if (GuiButton(Rectangle{10, 10, 100, 100},
                      "#05#Open Image")) { /* ACTION */
            println("click!");
        }

        EndDrawing();
    }
}

int main() {
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

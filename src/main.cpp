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

bool EditableLabel(float x, bool is_confirmed, char* buf) {
    Rectangle rect{x + 210, 200, 100, 100};
    if (is_confirmed) {
        GuiLabel(rect, buf);
    } else {
        if (GuiTextBox(rect, buf, 28, true)) {
            println("confirmed: %s", buf);
            return true;
        }
    }
    return is_confirmed;
}

namespace State {

    int add(int a, int b) { return a + b; }
    std::function<int(int)> add(int a) {
        return [=](int b) { return add(a, b); };
    }

    struct Model {
        immer::box<int> frames{0};
        immer::box<int> clicks{0};
        template <class Archive> void serialize(Archive& archive) {
            archive(CEREAL_NVP(frames.get()), CEREAL_NVP(clicks.get()));
        }
    };
    namespace actions {
        struct count_frame {};
        struct count_click {};
    } // namespace actions

    using Action = std::variant<actions::count_frame, actions::count_click>;

    Model update(Model model, Action action) {
        const auto new_model =
            std::visit(lager::visitor{
                           [&](actions::count_frame) {
                               model.frames = model.frames.update(add(1));
                               return model;
                           },
                           [&](actions::count_click) {
                               model.clicks = model.clicks.update(add(1));
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

    char buffer1[1024];
    memset(buffer1, 0, 1024);
    bool confirm1 = false;

    char buffer2[1024];
    memset(buffer2, 0, 1024);
    bool confirm2 = false;

    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        cereal::JSONOutputArchive json_out(std::cout);
        // Update
        store.dispatch(State::actions::count_frame{});
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

        confirm1 = EditableLabel(0, confirm1, buffer1);
        confirm2 = EditableLabel(200, confirm2, buffer2);

        if (GuiButton(Rectangle{10, 10, 100, 100},
                      "#05#Open Image")) { /* ACTION */
            println("click!");
            store.dispatch(State::actions::count_click{});
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

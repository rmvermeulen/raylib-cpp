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
#include <raylib/raylib-cpp.hpp>

#include "./app.h"
#include "./core/object.h"
#include "./functions.h"

void run_game() {}

int main() {
#if RUN_TESTS
    println("main: running tests...");
    tests::run_all();
    println("main: tests done!");
#else
    println("main: starting app!");

    auto title = "Raylib C++ Starter Kit Example";
    if (false) {
        auto window = std::make_unique<raylib::Window>(800, 450, title, false);
        App app{std::move(window), 60};
        app.start();
    } else {
        App app{800, 450, 60, title, WHITE, GRAY};
        app.start();
    }
#endif
    return 0;
}

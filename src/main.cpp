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
#include <raylib-cpp.hpp>

#include "./app.h"
#include "./core/object.h"
#include "./functions.h"
#include "./store.h"

void run_game() {
    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window window(screenWidth, screenHeight,
                          "Raylib C++ Starter Kit Example", false);

    SetTargetFPS(120);

    App app{};

    app.start();
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

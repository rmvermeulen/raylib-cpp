#define RAYGUI_IMPLEMENTATION
#define RUN_TESTS 0

#include <chrono>
#include <iomanip>
#include <sstream>

#include <raygui.h>
#include <raylib-cpp.hpp>

#include "./core/object.h"
#include "./functions.h"
#include "./tests.h"

void run_game() {

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
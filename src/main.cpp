#define RAYGUI_IMPLEMENTATION
#define RUN_TESTS

#include <chrono>
#include <iomanip>
#include <sstream>

#include <raygui.h>
#include <raylib-cpp.hpp>

#include "./core/framework.h"
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

    core::Framework f;
    auto& console = f.console;
    console.Log("console initialized!");
    console.Log("more lines to log woohoo!");
    console.Log("EVEN MORE lines to log woohoo!");

    Vector2 console_pos{0, screenHeight - 160};
    Rectangle console_output_rect{console_pos.x, console_pos.y, screenWidth,
                                  160};

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

        GuiPanel(console_output_rect, "Console");
        console.Render(console_pos + Vector2{0, 25});

        EndDrawing();
    }
}

int main() {

#ifdef RUN_TESTS
    printf("main: running tests...\n");
    tests::run_all();
    printf("main: tests done!\n");
#else
    printf("main: starting game!\n");
    run_game();
#endif
    return 0;
}
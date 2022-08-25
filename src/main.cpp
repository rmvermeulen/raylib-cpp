#include <chrono>
#include <iomanip>
#include <raylib-cpp.hpp>
#include <sstream>

#include "framework.h"

int main() {

    // Initialization
    int screenWidth = 800;
    int screenHeight = 450;

    raylib::Color textColor(LIGHTGRAY);
    raylib::Window window(screenWidth, screenHeight,
                          "Raylib C++ Starter Kit Example");

    SetTargetFPS(60);

    Framework f;
    auto& console = f.console;
    console.Log("console initialized!");
    console.Log("more lines to log woohoo!");
    console.Log("EVEN MORE lines to log woohoo!");
    // Main game loop
    while (!window.ShouldClose()) // Detect window close button or ESC key
    {
        // Update

        // TODO: Update your variables here

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        textColor.DrawText("Congrats! You created your first window!", 190, 200,
                           20);
        textColor.DrawText("And this is my own addition, even!", 190, 220, 20);
        console.Render();
        EndDrawing();
    }

    return 0;
}
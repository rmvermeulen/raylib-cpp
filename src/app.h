#pragma once
#include <memory>
#include <raylib-cpp.hpp>

template <typename T> struct Vector2t {
    T x;
    T y;
};

struct Vector2i : public Vector2t<int> {
    int x;
    int y;
};

class App {
  private:
    std::unique_ptr<raylib::Window> window;
    raylib::Color background_color;
    raylib::Color text_color;

  public:
    App(std::unique_ptr<raylib::Window> a_window);
    App(

        int width = 800, int height = 450,
        const raylib::Color& a_text_color = LIGHTGRAY,
        const raylib::Color& a_background_color = GRAY,
        const char* a_title = "Window title", int a_fps = 60);
    ~App();
    inline const Color& get_background_color() const {
        return background_color;
    }
    inline const Color& get_text_color() const { return text_color; }

    void start();
};

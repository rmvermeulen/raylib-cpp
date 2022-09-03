#pragma once
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
    raylib::Window window;
    raylib::Color background_color;
    raylib::Color text_color;
    Vector2i screen_size;
    /* data */
  public:
    App(/* args */);
    ~App();
    inline const Color& get_background_color() const {
        return background_color;
    }
    inline const Color& get_text_color() const { return text_color; }
    inline const Vector2i& get_screen_size() const { return screen_size; }
    inline int get_screen_width() const { return screen_size.x; }
    inline int get_screen_height() const { return screen_size.y; }

    void start();
};

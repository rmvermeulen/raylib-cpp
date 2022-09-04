#include "./app.h"
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <immer/box.hpp>
#include <iostream>
#include <lager/util.hpp>
#include <string>
#include <vector>

#include "./functions.h"
#include "./state/actions.h"
#include "./state/model.h"
#include "./state/reducers.h"
#include "./state/store.h"

#include "./ui/node.h"
#include "./ui/render.h"

App::App(std::unique_ptr<raylib::Window> a_window, int a_fps,
         const raylib::Color& a_text_color,
         const raylib::Color& a_background_color)
    : window(std::move(a_window)), text_color(a_text_color),
      background_color(a_background_color) {
    SetTargetFPS(a_fps);
}

App::App(int a_width, int a_height, int a_fps, const std::string& a_title,
         const raylib::Color& a_text_color,
         const raylib::Color& a_background_color)
    : text_color(a_text_color), background_color(a_background_color) {

    window = std::make_unique<raylib::Window>(a_width, a_height, a_title, true);
    SetTargetFPS(a_fps);
    std::cout << "window created!\n"
              << window->GetWidth() << " vs " << a_width << '\n'
              << window->GetHeight() << " vs " << a_height << std::endl;
}

App::~App() {}

void App::start() {
    window->Init();

    Store<state::Model,
          std::function<state::Model(state::Model, state::Action)>,
          state::Action>
        store{&state::update};

    auto root =
        Node::create(Node::Column{},
                     std::vector<std::shared_ptr<Node>>{
                         Node::create(Node::Label{"This is a bunch of nodes!"}),
                         Node::create(),
                         Node::create(),
                         Node::create(),
                     });

    int safety = 10e4;

    while (!window->ShouldClose()) {
        if (!--safety)
            break;
        // update stuff
        cereal::JSONOutputArchive json_out(std::cout);

        store.dispatch(state::actions::count_frame{});
        // collect render stuff
        BeginDrawing();
        ClearBackground(GRAY);
        text_color.DrawText("Congrats! You created your first window!", 190,
                            200, 20);
        {
            std::stringstream ss;
            ss << "And this is my own addition: ";

            cereal::JSONOutputArchive json(ss);
            json(cereal::make_nvp("app state", store.get_state()));

            text_color.DrawText(ss.str(), 190, 220, 20);
        }

        // render root node
        render(root);

        EndDrawing();
    }
}
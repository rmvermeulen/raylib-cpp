#include "./app.h"
#include <cereal/archives/json.hpp>
#include <cereal/archives/xml.hpp>
#include <immer/box.hpp>
#include <iostream>
#include <lager/util.hpp>

#include "./store.h"

namespace State {

    int add(int a, int b) { return a + b; }
    std::function<int(int)> add(int a) {
        return [=](int b) { return add(a, b); };
    }

    struct Model {
        std::string some_text{"Is that a __nested__ \"quote\"?!"};
        immer::box<int> frames{0};
        immer::box<int> clicks{0};
        template <class Archive> void serialize(Archive& archive) {
            using cereal::make_nvp;
            archive(make_nvp("some_text", some_text),
                    make_nvp("fra:mes", frames.get()),
                    make_nvp("clicks", clicks.get()));
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

App::App(/* args */) {}

App::~App() {}

void App::start() {

    Store<State::Model,
          std::function<State::Model(State::Model, State::Action)>,
          State::Action>
        store{&State::update};

    while (!window.ShouldClose()) {
        // update stuff
        cereal::JSONOutputArchive json_out(std::cout);

        store.dispatch(State::actions::count_frame{});
        // collect render stuff
        BeginDrawing();
        ClearBackground(GRAY);
        text_color.DrawText("Congrats! You created your first window!", 190,
                            200, 20);
        {
            std::stringstream ss;
            ss << "And this is my own addition: ";
            {
                cereal::JSONOutputArchive json(ss);
                // json(CEREAL_NVP(store.get_state()));
                json(cereal::make_nvp("app state", store.get_state()));
            }

            text_color.DrawText(ss.str(), 190, 220, 20);
        }
        EndDrawing();
    }
}
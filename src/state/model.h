
#pragma once
#include <functional>
#include <optional>

#include <immer/box.hpp>
#include <immer/vector.hpp>

namespace state {
    using Factory = std::function<std::shared_ptr<IGame>(void)>;
    struct Model {
        immer::vector<Factory> factories;
        std::optional<std::shared_ptr<IGame>> current_game;

        template <class Archive> void serialize(Archive& archive) {

            if (current_game.has_value()) {
                const auto& game = *current_game.value();
                archive(cereal::make_nvp("factories", factories.size()),
                        cereal::make_nvp("current game", game));
            } else {
                archive(cereal::make_nvp("factories", factories.size()),
                        cereal::make_nvp("current game", "-"));
            }
        }
    };

} // namespace state

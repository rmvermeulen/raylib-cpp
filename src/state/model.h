
#pragma once
#include <functional>
#include <optional>

#include <immer/box.hpp>
#include <immer/vector.hpp>

#include <nameof.hpp>

namespace state {
    using Factory = std::function<std::shared_ptr<IGame>(void)>;
    struct Model {
        immer::vector<Factory> factories;
        std::optional<std::shared_ptr<IGame>> current_game;

        template <class Archive> void serialize(Archive& archive) {
            using cereal::make_nvp;
            archive(nameof("factories", factories.size()),
                    nameof("current game", current_game.has_value()));
        }
    };

} // namespace state

#pragma once
#include <cereal/cereal.hpp>
#include <immer/vector.hpp>

#include "../entity.h"

namespace schmup::state {
    struct Model {
        immer::vector<Entity> entities;

        template <class Archive> void serialize(Archive& archive) const {
            for (const auto& e : entities)
                archive(e);
        }
    };
} // namespace schmup::state
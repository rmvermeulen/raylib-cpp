
#pragma once
#include <immer/box.hpp>

namespace state {
    struct Model {
        immer::box<int> frames{0};
        immer::box<int> clicks{0};
        template <class Archive> void serialize(Archive& archive) {
            using cereal::make_nvp;
            archive(make_nvp("frames", frames.get()),
                    make_nvp("clicks", clicks.get()));
        }
    };
} // namespace state

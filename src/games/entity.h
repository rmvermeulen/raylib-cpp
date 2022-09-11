#pragma once
namespace schmup {

    struct Entity {
        int id;
        int type;
        int health = 100;
        template <class Archive> void serialize(Archive& archive) {
            archive(CEREAL_NVP(id), CEREAL_NVP(type), CEREAL_NVP(health));
        }
    };
} // namespace schmup
#pragma once
#include "../game.h"

struct Entity {
    int type;
    int health = 100;
};
class Schmup : public IGame {
    std::vector<Entity> entities;

  public:
    TickResult tick() override {
        // update the game
        return TickResult::Continue;
    }
    void serialize(cereal::JSONOutputArchive& archive) override {
        archive(cereal::make_nvp("name", std::string{"schmup"}),
                cereal::make_nvp("entities", entities.size()));
    };
};
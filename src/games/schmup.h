#pragma once

#include "../game.h"

class Schmup : public IGame {
  public:
    TickResult tick() {
        // update the game
        return TickResult::Continue;
    }
};
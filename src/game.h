
#pragma once

class IGame {
  public:
    enum TickResult { Stop, Continue };
    virtual ~IGame() {}
    virtual TickResult tick() = 0;
};
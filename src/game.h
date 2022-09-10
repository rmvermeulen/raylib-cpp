#pragma once

class IGame {
  public:
    enum TickResult { Stop, Continue };
    virtual ~IGame() {}
    virtual TickResult tick() = 0;
    virtual void serialize(cereal::JSONOutputArchive& archive) = 0;
};
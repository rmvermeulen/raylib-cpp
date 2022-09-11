#pragma once
#include <cereal/cereal.hpp>

class IGame {
  public:
    enum TickResult { Stop, Continue };
    virtual ~IGame() {}
    virtual TickResult tick() = 0;
    virtual void render() const = 0;
    virtual void serialize(cereal::JSONOutputArchive& archive) const = 0;
};
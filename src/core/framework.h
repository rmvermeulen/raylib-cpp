#pragma once
#include <chrono>
#include <iomanip>
#include <raylib-cpp.hpp>
#include <sstream>

#include "../ui/tree.h"

#include "console.h"

namespace core {

class Framework {
  public:
    Console console;
};
} // namespace core
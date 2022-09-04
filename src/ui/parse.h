#pragma once
#include "./node.h"
#include <memory>

namespace ui {
    shared_ptr<Node> parse(const std::string& path);

} // namespace ui
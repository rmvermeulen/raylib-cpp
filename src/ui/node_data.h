#pragma once
#include <vector>

#include "node_type.h"

namespace ui {

struct NodeData {
    NodeType type;
    std::vector<NodeData> children;
    NodeData(NodeType);
    NodeData(NodeType, std::vector<NodeData>);
    ~NodeData();

    size_t get_total_count() const;
};

} // namespace ui
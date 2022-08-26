#include <stddef.h>

#include "node_data.h"
namespace ui {

NodeData::NodeData(NodeType a_type) : type(a_type) {}
NodeData::NodeData(NodeType a_type, std::vector<NodeData> a_children)
    : type(a_type), children(a_children) {}

NodeData::~NodeData() {}

size_t NodeData::get_child_count() const {
    size_t count = children.size();
    for (const auto& child : children)
        count += child.get_child_count();
    return count;
}
} // namespace ui
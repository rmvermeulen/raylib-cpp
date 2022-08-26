#include <memory>

#include "node.h"
#include "tree.h"

namespace ui {

Node::Node(Tree& a_tree) : _tree(a_tree) {}

Node::~Node() {}

size_t Node::get_child_count() const { return _child_count; }

Node& Node::create_child() {
    return _tree.create_child_for(*this, ui::NodeType::Row);
}

const std::weak_ptr<Node>& Node::get_parent() const { return _parent; }

const NodeType& Node::get_type() const { return _type; }
void Node::set_type(const NodeType& a_type) { _type = a_type; }
} // namespace ui

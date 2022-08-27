#include <memory>

#include "node.h"
#include "tree.h"

namespace ui {

size_t Node::next_id = 0;

Node::Node(Tree& a_tree) : _tree(a_tree), _id(Node::next_id++) {}

Node::~Node() {}

Node& Node::create_child() {
    return _tree.create_child_for(*this, ui::NodeType::Row);
}

} // namespace ui

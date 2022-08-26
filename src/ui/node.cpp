#include "node.h"
#include "tree.h"
#include <memory>
namespace ui {

Node::Node(Tree& a_tree) : _tree(a_tree) {}

Node::~Node() {}

size_t Node::get_child_count() const { return _child_count; }

Node& Node::create_child() { return _tree.create_child_for(*this); }

const std::weak_ptr<Node>& Node::get_parent() const { return _parent; }
} // namespace ui

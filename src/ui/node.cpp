#include "node.h"
#include "tree.h"
#include <memory>
namespace ui {

node::node(Tree& a_tree) : _tree(a_tree) {}

node::~node() {}

size_t node::get_child_count() const { return _child_count; }

node& node::create_child() { return _tree.create_child_for(*this); }

} // namespace ui

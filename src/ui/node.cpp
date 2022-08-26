
#include "node.h"
#include <memory>
namespace ui
{

    node::node(Tree &a_tree) : _tree(a_tree) {}

    node::~node() {}

    size_t node::get_child_count() const { return _children.size(); }

    node &node::create_child() { return _tree.create_child(*this); }

    void node::add_child(node &child) { _children.push_back(&child); }

} // namespace ui

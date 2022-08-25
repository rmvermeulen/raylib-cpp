#include "tree.h"
#include "node.h"
namespace ui {
Tree::Tree()
    :

      _root(std::make_shared<node>(*this)) {
    _nodes.push_back(_root);
}

Tree::~Tree() {}

node& Tree::create_child(node& parent) {
    auto pNode = std::make_shared<node>(*this);
    parent.add_child(*pNode);
    _nodes.push_back(pNode);
    return *pNode;
}
node& Tree::get_root() const { return *_root; }

size_t Tree::get_node_count() const { return _nodes.size(); }
} // namespace ui

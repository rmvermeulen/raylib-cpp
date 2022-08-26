#include "tree.h"
#include "node.h"
namespace ui
{
  Tree::Tree() { _nodes.push_back(std::make_shared<node>(*this)); }

  Tree::~Tree() {}

  node &Tree::create_child(node &parent)
  {
    auto pNode = std::make_shared<node>(*this);
    parent.add_child(*pNode);
    _nodes.push_back(pNode);
    return *pNode;
  }
  node &Tree::get_root() const { return *_nodes[0]; }

  size_t Tree::get_node_count() const { return _nodes.size(); }
} // namespace ui

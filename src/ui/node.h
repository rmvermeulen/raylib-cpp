#pragma once
#include <memory>
#include <vector>

#include "node_type.h"

namespace ui {
class Tree;
class Node {
  private:
    friend class Tree;
    Tree& _tree;
    std::weak_ptr<Node> _parent;
    size_t _child_count = 0;
    NodeType _type = NodeType::Leaf;

  public:
    Node(Tree& a_tree);
    ~Node();
    size_t get_child_count() const;
    Node& create_child();

    const std::weak_ptr<Node>& get_parent() const;
    const NodeType& get_type() const;
    void set_type(const NodeType&);
};

} // namespace ui
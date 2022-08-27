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
    static size_t next_id;
    size_t _id;

  public:
    Node(Tree& a_tree);
    ~Node();
    Node& create_child();

    inline size_t get_child_count() const { return _child_count; };
    inline size_t get_id() const { return _id; };
    inline const NodeType& get_type() const { return _type; }
    inline const std::weak_ptr<Node>& get_parent() const { return _parent; }
    inline void set_type(const NodeType& a_type) { _type = a_type; }

    inline static void reset_static_id() { Node::next_id = 0; }
};

} // namespace ui
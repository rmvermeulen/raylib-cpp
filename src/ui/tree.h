#pragma once
#include <memory>
#include <vector>

#include "node_data.h"

namespace ui {
class Node;
class Tree {
  private:
    std::vector<std::shared_ptr<Node>> _nodes;
    void _init(NodeData);
    size_t _apply(Node&, NodeData, size_t);
    std::shared_ptr<Node> _create_child_for(Node& parent, const NodeType& type);

  public:
    Tree(/* args */);
    Tree(NodeData);
    ~Tree();

    // main stuff
    Node& get_root() const;
    size_t get_node_count() const;

    // parent stuff
    size_t get_node_index(const Node& node) const;
    Node& create_child_for(Node& parent, const NodeType& type);

    std::vector<std::shared_ptr<Node>>
    get_children_of(const Node& parent) const;

    std::vector<std::shared_ptr<Node>> get_parents_of(const Node&) const;
};

} // namespace ui

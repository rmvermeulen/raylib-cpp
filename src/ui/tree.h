#pragma once
#include <memory>
#include <vector>

namespace ui {
class Node;
class Tree {
  private:
    std::vector<std::shared_ptr<Node>> _nodes;

  public:
    Tree(/* args */);
    ~Tree();

    // main stuff
    Node& get_root() const;
    size_t get_node_count() const;

    // parent stuff
    size_t get_node_index(const Node& node) const;
    Node& create_child_for(Node& parent);

    std::vector<std::shared_ptr<Node>>
    get_children_of(const Node& parent) const;

    std::vector<std::shared_ptr<Node>> get_parents_of(const Node&) const;
};

} // namespace ui

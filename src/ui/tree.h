#pragma once
#include <memory>
#include <vector>

namespace ui {
class node;
class Tree {
  private:
    std::vector<std::shared_ptr<node>> _nodes;

  public:
    Tree(/* args */);
    ~Tree();

    // main stuff
    node& get_root() const;
    size_t get_node_count() const;

    // parent stuff
    size_t get_node_index(const node& node) const;
    node& create_child_for(node& parent);

    std::vector<std::shared_ptr<node>>
    get_children_of(const node& parent) const;

    std::vector<std::shared_ptr<node>> get_parents_of(const node&) const;
};

} // namespace ui

#pragma once
#include <memory>
#include <vector>
namespace ui
{
  class node;
  class Tree
  {
  private:
    std::vector<std::shared_ptr<node>> _nodes;

  public:
    Tree(/* args */);
    ~Tree();

    node &create_child(node &parent);
    node &get_root() const;
    size_t get_node_count() const;
  };

} // namespace ui

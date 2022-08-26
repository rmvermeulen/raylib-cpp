#pragma once
#include <memory>
#include <vector>
namespace ui {
class Tree;
class node {
  private:
    friend class Tree;
    Tree& _tree;
    std::weak_ptr<node> _parent;
    size_t _child_count = 0;

  public:
    node(Tree& a_tree);
    ~node();
    size_t get_child_count() const;
    node& create_child();
};

} // namespace ui
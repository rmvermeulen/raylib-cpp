#pragma once
#include "tree.h"
#include <memory>
#include <vector>
namespace ui {
class node {
  private:
    friend class Tree;
    Tree& _tree;
    node* _parent;
    std::vector<node*> _children;
    void init();
    void add_child(node&);

  public:
    node(Tree& a_tree);
    ~node();
    size_t get_child_count() const;
    node& create_child();
};

} // namespace ui
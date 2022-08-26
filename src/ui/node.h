#pragma once
#include <memory>
#include <vector>
namespace ui {
class Tree;
class Node {
  private:
    friend class Tree;
    Tree& _tree;
    std::weak_ptr<Node> _parent;
    size_t _child_count = 0;

  public:
    Node(Tree& a_tree);
    ~Node();
    size_t get_child_count() const;
    Node& create_child();

    const std::weak_ptr<Node>& get_parent() const;
};

} // namespace ui
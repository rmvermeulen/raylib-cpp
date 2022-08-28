#pragma once
#include <functional>
#include <memory>

namespace store {

template <typename T> class Tree {
    struct Node;
    std::shared_ptr<Node> root;

  public:
    Tree();
    Tree(std::unique_ptr<T> a_data);
    class Builder;
    friend std::unique_ptr<Tree<T>> Builder::build() const;
};

template <typename T> struct Tree<T>::Node {
    enum Type { Div, Label, List };
    std::unique_ptr<T> data;
    std::weak_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;
};
template <typename T> class Tree<T>::Builder {
  public:
    Builder();

    const Builder& add_child(const Node::Type&) const;
    const Builder& add_child(const Node::Type&,
                             std::function<void(const Builder&)>) const;

    std::unique_ptr<Tree<T>> build() const;
};

} // namespace store

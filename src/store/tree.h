#pragma once
#include <functional>
#include <memory>

namespace store {

enum NodeType { Div, Label, List };

template <typename T> class Tree {
    struct Node;
    std::shared_ptr<Node> root;

  public:
    Tree();
    Tree(std::unique_ptr<T> a_data);
    class Builder;
    friend std::unique_ptr<Tree<T>> Builder::build() const;

    size_t get_node_count() const;
    size_t count_nodes(const std::shared_ptr<Node>&) const;
};

template <typename T> struct Tree<T>::Node {
    NodeType type;
    std::unique_ptr<T> data;
    std::weak_ptr<Node> parent;
    std::vector<std::shared_ptr<Node>> children;
    class Builder {
      protected:
        std::shared_ptr<Node> _root;
        std::shared_ptr<Node> _add_child(const NodeType&);

      public:
        Builder(std::shared_ptr<Node> root);
        Builder();
        Builder& add_child(const NodeType&);
        Builder& add_child(const NodeType&, std::function<void(Builder&)>);
        std::shared_ptr<Node> build() const;
    };
};
template <typename T> class Tree<T>::Builder : public Tree<T>::Node::Builder {
  public:
    Builder();
    Builder& add_child(const NodeType&);
    Builder& add_child(const NodeType&,
                       std::function<void(typename Tree<T>::Node::Builder&)>);
    std::unique_ptr<Tree<T>> build() const;
};

} // namespace store

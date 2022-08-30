#include "tree.h"

#include <numeric>

#include "../functions.h"

namespace store {

template <typename T> Tree<T>::Tree() {}

template <typename T>
Tree<T>::Tree(std::unique_ptr<T> a_data)
    : root(std::make_unique<Node>(Node::Type::Div, a_data, nullptr, {})) {}

template <typename T> size_t Tree<T>::get_node_count() const {
    return root == nullptr ? 0 : 1 + count_nodes(root);
}
template <typename T>
size_t Tree<T>::count_nodes(const std::shared_ptr<Node>& a_node

) const {
    if (!a_node)
        return 0;
    const auto& children = a_node->children;
    return std::accumulate(children.begin(), children.end(), 1,
                           [](size_t acc, const std::shared_ptr<Node>& node) {
                               return acc + node->children.size();
                           });
}

/////////////////
// NODE BUILDER
/////////////////

template <typename T> Tree<T>::Node::Builder::Builder() {}

template <typename T>
Tree<T>::Node::Builder::Builder(std::shared_ptr<Node> a_root)

    : _root(a_root) {}

template <typename T>
std::shared_ptr<typename Tree<T>::Node>
Tree<T>::Node::Builder::_add_child(const NodeType& a_type) {
    return std::make_shared<Tree<T>::Node>(a_type);
}

template <typename T>
Tree<T>::Node::Builder&
Tree<T>::Node::Builder::add_child(const NodeType& a_type) {
    _add_child(a_type);
    return *this;
}

template <typename T>
Tree<T>::Node::Builder& Tree<T>::Node::Builder::add_child(
    const NodeType& a_type,
    std::function<void(Tree<T>::Node::Builder&)> a_configure) {
    // add node by type
    auto child = _add_child(a_type);
    // configure node
    Tree<T>::Node::Builder node_builder{child};
    a_configure(node_builder);

    return *this;
}

/////////////////
// TREE BUILDER
/////////////////

template <typename T> Tree<T>::Builder::Builder() {}

template <typename T>
Tree<T>::Builder& Tree<T>::Builder::add_child(const NodeType& a_type) {
    Tree<T>::Node::Builder::_add_child(a_type);
    return *this;
}

template <typename T>
Tree<T>::Builder& Tree<T>::Builder::add_child(
    const NodeType& a_type,
    std::function<void(typename Tree<T>::Node::Builder&)> a_configure) {
    // add node by type
    auto child = Tree<T>::Node::Builder::_add_child(a_type);
    // configure node
    typename Tree<T>::Node::Builder node_builder{child};
    a_configure(node_builder);

    return *this;
}

template <typename T> std::unique_ptr<Tree<T>> Tree<T>::Builder::build() const {
    // auto tree = std::make_unique<Tree<T>>();
    // tree->root = root;
    // return tree;
    return std::make_unique<Tree<T>>();
}

} // namespace store

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
// BUILDER
/////////////////

template <typename T> Tree<T>::Builder::Builder() {}

template <typename T>
const Tree<T>::Builder&
Tree<T>::Builder::add_child(const NodeType& a_type) const {
    // add node by type
    return *this;
}

template <typename T>
const Tree<T>::Builder& Tree<T>::Builder::add_child(
    const NodeType& a_type,
    std::function<void(const Tree<T>::Builder&)> a_configure) const {
    // add node by type
    // configure node
    return *this;
}

template <typename T> std::unique_ptr<Tree<T>> Tree<T>::Builder::build() const {
    return std::make_unique<Tree<T>>();
}

} // namespace store

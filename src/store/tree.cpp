#include "tree.h"

#include "../functions.h"

namespace store {

template <typename T> Tree<T>::Tree() {}

template <typename T>
Tree<T>::Tree(std::unique_ptr<T> a_data)
    : root(std::make_unique<Node>(Node::Type::Div, a_data, nullptr, {})) {}

template <typename T> Tree<T>::Builder::Builder() {
    println("creating builder...");
}

template <typename T>
const Tree<T>::Builder&
Tree<T>::Builder::add_child(const Node::Type& a_type) const {
    // add node by type
    return &this;
}

template <typename T>
const Tree<T>::Builder& Tree<T>::Builder::add_child(
    const Node::Type& a_type,
    std::function<void(const Tree<T>::Builder&)> a_configure) const {
    // add node by type
    // configure node
    return &this;
}

template <typename T> std::unique_ptr<Tree<T>> Tree<T>::Builder::build() const {
    return std::make_unique<Tree<T>>();
}

} // namespace store

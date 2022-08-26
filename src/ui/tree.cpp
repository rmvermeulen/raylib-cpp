#include <algorithm>
#include <memory>

#include "node.h"
#include "tree.h"

namespace ui {
Tree::Tree() { _nodes.push_back(std::make_shared<Node>(*this)); }

Tree::~Tree() {}

Node& Tree::create_child_for(Node& parent) {
    auto pNode = std::make_shared<Node>(*this);

    ++parent._child_count;
    pNode->_parent = *std::find_if(
        _nodes.begin(), _nodes.end(),
        [&](std::shared_ptr<Node> node) { return node.get() == &parent; });

    _nodes.push_back(pNode);
    return *pNode;
}
Node& Tree::get_root() const { return *_nodes[0]; }

size_t Tree::get_node_count() const { return _nodes.size(); }
size_t Tree::get_node_index(const Node& node) const {
    for (size_t i = 0; i < _nodes.size(); ++i) {
        if (_nodes[i].get() == &node)
            return i;
    }
    return -1;
}

std::vector<std::shared_ptr<Node>>
Tree::get_children_of(const Node& parent) const {
    if (parent._parent.expired()) {
        // parent is root_node
        auto cc = parent.get_child_count();
        std::vector<std::shared_ptr<Node>> children;
        children.reserve(cc);

        std::copy(_nodes.begin() + 1, _nodes.begin() + 1 + cc,
                  children.begin());
        return children;
    } else {
        auto p = parent._parent.lock();
        auto start = get_node_index(*p);
        auto end = start + parent.get_child_count();

        std::vector<std::shared_ptr<Node>> children;
        children.reserve(parent.get_child_count());

        std::copy(_nodes.begin() + start, _nodes.begin() + end,
                  children.begin());
        return children;
    }
}

std::vector<std::shared_ptr<Node>>
Tree::get_parents_of(const Node& parent) const {
    std::vector<std::shared_ptr<Node>> parents{};
    auto p = &parent;
    while (p != nullptr) {
        bool is_expired = p->_parent.expired();
        if (is_expired)
            break;

        auto next = p->_parent.lock();
        parents.push_back(next);
        p = next.get();
    }
    return parents;
}
} // namespace ui

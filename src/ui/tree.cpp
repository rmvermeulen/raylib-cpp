#include <algorithm>
#include <memory>

#include "../functions.h"
#include "node.h"
#include "tree.h"

namespace ui {
Tree::Tree() { _nodes.push_back(std::make_shared<Node>(*this)); }
Tree::Tree(NodeData a_data) { _init(a_data); }

Tree::~Tree() {}

Node& Tree::create_child_for(Node& a_parent, const NodeType& a_type) {
    return *_create_child_for(a_parent, a_type);
}

std::shared_ptr<Node> Tree::_create_child_for(Node& a_parent,
                                              const NodeType& type) {
    auto pNode = std::make_shared<Node>(*this);

    ++a_parent._child_count;
    pNode->_parent = *std::find_if(
        _nodes.begin(), _nodes.end(),
        [&](std::shared_ptr<Node> node) { return node.get() == &a_parent; });

    _nodes.push_back(pNode);
    return pNode;
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

void Tree::_init(NodeData a_data) {
    println("init: preparing for %d nodes", a_data.get_total_count());
    auto root = std::make_shared<Node>(*this);
    _nodes.push_back(root);
    auto total = _apply(*root, a_data, 0);
    println("init: done. %d/%d nodes created", _nodes.size(), total);
}

size_t Tree::_apply(Node& a_target, NodeData a_data, size_t a_depth) {
    println("apply[%d] with %d children", a_depth, a_data.children.size());
    a_target.set_type(a_data.type);
    std::vector<std::pair<std::shared_ptr<Node>, NodeData>> created_nodes;
    created_nodes.reserve(a_data.children.size());

    for (const auto& child_data : a_data.children) {
        const auto& node = _create_child_for(a_target, child_data.type);
        _nodes.push_back(node);
        created_nodes.push_back(std::pair(node, child_data));
    }
    size_t total = 1;
    for (auto& pair : created_nodes)
        total += _apply(*pair.first, pair.second, a_depth + 1);
    println("apply[%d] created: %d", a_depth, total);
    return total;
}

std::vector<std::shared_ptr<Node>>
Tree::get_children_of(const Node& a_parent) const {
    if (a_parent._parent.expired()) {
        // parent is root_node
        auto cc = a_parent.get_child_count();
        std::vector<std::shared_ptr<Node>> children;
        children.reserve(cc);

        std::copy(_nodes.begin() + 1, _nodes.begin() + 1 + cc,
                  children.begin());
        return children;
    } else {
        auto p = a_parent._parent.lock();
        auto start = get_node_index(*p);
        auto end = start + a_parent.get_child_count();

        std::vector<std::shared_ptr<Node>> children;
        children.reserve(a_parent.get_child_count());

        std::copy(_nodes.begin() + start, _nodes.begin() + end,
                  children.begin());
        return children;
    }
}

std::vector<std::shared_ptr<Node>>
Tree::get_parents_of(const Node& a_parent) const {
    std::vector<std::shared_ptr<Node>> parents{};
    auto p = &a_parent;
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

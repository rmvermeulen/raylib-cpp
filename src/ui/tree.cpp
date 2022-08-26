#include <algorithm>
#include <memory>

#include "node.h"
#include "tree.h"

namespace ui {
Tree::Tree() { _nodes.push_back(std::make_shared<node>(*this)); }

Tree::~Tree() {}

node& Tree::create_child_for(node& parent) {
    auto pNode = std::make_shared<node>(*this);
    printf("  first: node has %d child[ren]\n", parent._child_count);
    ++parent._child_count;
    printf("  second: node has %d child[ren]\n", parent._child_count);
    _nodes.push_back(pNode);
    return *pNode;
}
node& Tree::get_root() const { return *_nodes[0]; }

size_t Tree::get_node_count() const { return _nodes.size(); }
size_t Tree::get_node_index(const node& node) const {
    for (size_t i = 0; i < _nodes.size(); ++i) {
        if (_nodes[i].get() == &node)
            return i;
    }
    return -1;
}

std::vector<std::shared_ptr<node>>
Tree::get_children_of(const node& parent) const {
    if (parent._parent.expired()) {
        // parent is root_node
        auto cc = parent.get_child_count();
        std::vector<std::shared_ptr<node>> children;
        children.reserve(cc);

        std::copy(_nodes.begin() + 1, _nodes.begin() + 1 + cc,
                  children.begin());
        return children;
    } else {
        auto p = parent._parent.lock();
        auto start = get_node_index(*p);
        auto end = start + parent.get_child_count();

        std::vector<std::shared_ptr<node>> children;
        children.reserve(parent.get_child_count());

        std::copy(_nodes.begin() + start, _nodes.begin() + end,
                  children.begin());
        return children;
    }
}

std::vector<std::shared_ptr<node>>
Tree::get_parents_of(const node& parent) const {
    std::vector<std::shared_ptr<node>> parents{};
    auto p = &parent;
    while (p != nullptr) {
        if (p->_parent.expired()) {
            break;
        }
        auto next = p->_parent.lock();
        parents.push_back(next);
        p = next.get();
    }
    return parents;
}
} // namespace ui

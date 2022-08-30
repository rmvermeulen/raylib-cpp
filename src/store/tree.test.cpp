
#include "../testing.h"
#include <vector>

#include "tree.cpp"
#include "tree.h"

namespace store::tests {

void tree_test() {
    using namespace testing;
    using Tree = store::Tree<std::string>;

    describe("store::Tree", [](Setup& $) {
        $.it("it can be built", [] {
            Tree::Builder builder;
            const auto& tree = builder.build();
            expect(tree != nullptr, "Got a valid tree");
            expect(tree->get_node_count() == 0, "Got an empty tree");
        });
        $.it("it can add a node", [] {
            Tree::Builder builder;
            const auto& tree = builder.add_child(NodeType::Div).build();
            expect(tree != nullptr, "Got a valid tree");
            expect(tree->get_node_count() == 1, "Got a tree with a node");
        });
        $.it("it can configure a node", [] {
            Tree::Builder builder;
            const auto& tree =
                builder
                    .add_child(
                        NodeType::Div,
                        [](auto& builder) { builder.add_child(NodeType::Div); })
                    .build();
            expect(tree != nullptr, "Got a valid tree");
            expect(tree->get_node_count() == 2,
                   "Got a tree with a nested node");
        });
    });
}

} // namespace store::tests
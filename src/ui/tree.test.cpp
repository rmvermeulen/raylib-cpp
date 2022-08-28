
#include "../testing.h"
#include <vector>

#include "node.h"
#include "tree.h"

namespace ui::tests {

void tree_test() {
    using namespace testing;
    describe("tree", [](Setup& $) {
        $.before_each(&ui::Node::reset_static_id);

        $.it("creates nodes", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            root.create_child();
            expect(root.get_child_count() == 1, "root must have 1 child");
            expect(tree.get_node_count() == 2, "tree must have 2 children");
            root.create_child();
            expect(root.get_child_count() == 2);
            expect(tree.get_node_count() == 3);
        });
        $.it("gets the nodes' ids", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            root.create_child();
            root.create_child();

            std::vector<size_t> expected{0, 1, 2};
            const auto& ids = tree.get_node_ids();
            expect(ids.size() == expected.size(), "wrong number of ids");
            // for (int i = 0; i < ids.size(); ++i)
            //     println("%d\t got %d, expected %d", i, ids[i], expected[i]);
            expect(ids == expected, "ids must match expected list");
        });
        $.it("creates nested nodes", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            auto& child = root.create_child();
            expect(root.get_child_count() == 1, "root must have 1 child");
            expect(tree.get_node_count() == 2, "tree must have 2 children");
            child.create_child();
            expect(root.get_child_count() == 1, "root must still have 1 child");
            expect(child.get_child_count() == 1, "child must have 1 child");
            expect(tree.get_node_count() == 3, "tree now has 3 children");
        });
        $.it("has a direct api", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            auto& child = tree.create_child_for(root, ui::NodeType::Row);
            expect(root.get_child_count() == 1, "root must have 1 child");
            expect(tree.get_node_count() == 2, "tree must have 2 children");
            tree.create_child_for(child, ui::NodeType::Row);
            expect(root.get_child_count() == 1, "root must still have 1 child");
            expect(child.get_child_count() == 1, "child must have 1 child");
            expect(tree.get_node_count() == 3, "tree now has 3 children");
        });
        $.it("sets the parent on new nodes", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            auto& child = tree.create_child_for(root, ui::NodeType::Row);
            expect(child.get_parent().lock().get() == &root,
                   "child must have root as parent");
        });
        $.it("can get the parents from a child", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            auto& first = tree.create_child_for(root, ui::NodeType::Row);
            const auto& second =
                tree.create_child_for(first, ui::NodeType::Row);

            const auto& parents = tree.get_parents_of(second);
            expect(parents.size() > 0, "parents must not be empty");
            expect(parents[0].get() == &first);
            expect(parents[1].get() == &root);
        });
        $.it("can get the children from a parent", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            const auto& first = tree.create_child_for(root, ui::NodeType::Row);
            const auto& second = tree.create_child_for(root, ui::NodeType::Row);

            auto children = tree.get_children_of(root);
            expect(children[0].get() == &first, "[0] must be the first child");
            expect(children[1].get() == &second,
                   "[1] must be the second child");
        });

        $.it("can use node_data", [] {
            ui::NodeData data{
                ui::NodeType::Row,
                {
                    ui::NodeData{ui::NodeType::Column,
                                 {
                                     ui::NodeData{ui::NodeType::Leaf},
                                     ui::NodeData{ui::NodeType::Leaf},
                                 }},
                    ui::NodeData{ui::NodeType::Column,
                                 {
                                     ui::NodeData{ui::NodeType::Leaf},
                                     ui::NodeData{ui::NodeType::Leaf},
                                 }},

                }};
            ui::Tree tree{data};
            println("created = %d, expected = %d", tree.get_node_count(),
                    data.get_total_count());
            expect(tree.get_node_count() == 1 + data.get_total_count(),
                   "All nodes + root-node are created");
        });
    });
}

} // namespace ui::tests
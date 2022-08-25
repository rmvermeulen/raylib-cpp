#include "ui/node.h"
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

namespace tests {
class test {
  public:
    virtual const char* get_name() const = 0;
    virtual bool run() = 0;
};

void test(const char* name, void run()) {
    printf("TEST '%s'\n", name);
    try {
        run();
        printf("\tOK\n");
    } catch (const std::exception& ex) {
        // auto ex = std::current_exception();
        printf("\tFAILED: %s\n", ex.what());
    }
}

void expect(bool expectation) {
    if (!expectation) {
        throw std::logic_error("Expectation failed!");
    }
}
void expect(bool expectation, const char* message) {
    if (!expectation) {
        throw std::logic_error(message);
    }
}

void run_all() {
    test("tree: create nodes", [] {
        ui::Tree tree{};
        auto& root = tree.get_root();
        root.create_child();
        expect(root.get_child_count() == 1, "root must have 1 child");
        expect(tree.get_node_count() == 2, "tree must have 2 children");
        root.create_child();
        expect(root.get_child_count() == 2);
        expect(tree.get_node_count() == 3);
    });
    test("tree: create nested nodes", [] {
        ui::Tree tree{};
        auto& root = tree.get_root();
        auto child = root.create_child();
        expect(root.get_child_count() == 1, "root must have 1 child");
        expect(tree.get_node_count() == 2, "tree must have 2 children");
        child.create_child();
        expect(root.get_child_count() == 1, "root must still have 1 child");
        expect(child.get_child_count() == 1, "child must have 1 child");
        expect(tree.get_node_count() == 3, "tree now has 3 children");
    });
    test("tree: direct api", [] {
        ui::Tree tree{};
        auto& root = tree.get_root();
        auto& child = tree.create_child(root);
        expect(root.get_child_count() == 1, "root must have 1 child");
        expect(tree.get_node_count() == 2, "tree must have 2 children");
        tree.create_child(child);
        expect(root.get_child_count() == 1, "root must still have 1 child");
        expect(child.get_child_count() == 1, "child must have 1 child");
        expect(tree.get_node_count() == 3, "tree now has 3 children");
    });
}
} // namespace tests

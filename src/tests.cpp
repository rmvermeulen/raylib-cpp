#include <cstdio>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "functions.h"
#include "ui/node.h"
#include "ui/node_data.h"
#include "ui/tree.h"
namespace tests {
class test {
  public:
    virtual const char* get_name() const = 0;
    virtual bool run() = 0;
};

using run_fn = std::function<void()>;
using it_fn = std::function<void(const char*, const run_fn& _case)>;
using setup_fn = std::function<void(it_fn)>;

void test(const char* name, const run_fn& run) {
    println("TEST '%s'", name);
    try {
        run();
        println("\tOK");
    } catch (const std::exception& ex) {
        // auto ex = std::current_exception();
        println("\tFAILED: %s", ex.what());
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

class suite {
    const std::string _name;
    std::map<std::string, run_fn> _cases;

  public:
    suite(const char* a_name) : _name(a_name) {}
    ~suite() {}
    void add(const char* a_scenario, const run_fn& fn) {
        std::stringstream ss;
        ss << _name << ": " << a_scenario;
        _cases.insert(std::make_pair(ss.str(), fn));
    }
    void run() {
        for (const auto& pair : _cases) {
            test(pair.first.c_str(), pair.second);
        }
    }
};

void describe(const char* topic, const setup_fn& setup) {
    suite s{topic};
    setup([&s](const char* scenario, const run_fn& run) {
        s.add(scenario, run);
    });
    s.run();
}

void run_all() {
    describe("NodeData", [](const it_fn& it) {
        it("counts total nodes", [] {
            {
                ui::NodeData data{ui::NodeType::Row};
                expect(data.get_total_count() == 1, "must be 1 total");
            }
            {
                ui::NodeData data{ui::NodeType::Row,
                                  {ui::NodeData{ui::NodeType::Row}}};

                expect(data.get_total_count() == 2, "must be 2 total");
            }
            {
                ui::NodeData data{ui::NodeType::Row,
                                  {ui::NodeData{ui::NodeType::Row},
                                   ui::NodeData{ui::NodeType::Row}}};

                expect(data.get_total_count() == 3, "must be 3 total");
            }
            {
                ui::NodeData data{
                    ui::NodeType::Row,
                    {ui::NodeData{ui::NodeType::Column,
                                  {ui::NodeData{ui::NodeType::Row},
                                   ui::NodeData{ui::NodeType::Row}}},
                     ui::NodeData{ui::NodeType::Column,
                                  {ui::NodeData{ui::NodeType::Row},
                                   ui::NodeData{ui::NodeType::Row}}}}};

                expect(data.get_total_count() == 7, "must be 7 total");
            }
        });
    });
    describe("tree", [](const it_fn& it) {
        it("creates nodes", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            root.create_child();
            expect(root.get_child_count() == 1, "root must have 1 child");
            expect(tree.get_node_count() == 2, "tree must have 2 children");
            root.create_child();
            expect(root.get_child_count() == 2);
            expect(tree.get_node_count() == 3);
        });
        it("creates nested nodes", [] {
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
        it("has a direct api", [] {
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
        it("sets the parent on new nodes", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            auto& child = tree.create_child_for(root, ui::NodeType::Row);
            expect(child.get_parent().lock().get() == &root,
                   "child must have root as parent");
        });
        it("can get the parents from a child", [] {
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
        it("can get the children from a parent", [] {
            ui::Tree tree{};
            auto& root = tree.get_root();
            const auto& first = tree.create_child_for(root, ui::NodeType::Row);
            const auto& second = tree.create_child_for(root, ui::NodeType::Row);

            auto children = tree.get_children_of(root);
            expect(children[0].get() == &first, "[0] must be the first child");
            expect(children[1].get() == &second,
                   "[1] must be the second child");
        });

        it("can use node_data", [] {
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
} // namespace tests


#include "../testing.h"
#include <vector>

#include "tree.cpp"
#include "tree.h"

namespace store::tests {

void tree_test() {
    using namespace testing;
    describe("tree store", [](Setup& $) {
        $.it("it can be build", [] {
            store::Tree<std::string>::Builder builder;
            const auto& tree = builder.build();
            expect(tree != nullptr, "Got a valid tree");
        });
    });
}

} // namespace store::tests
#include <cstdio>
#include <functional>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "functions.h"
#include "testing.h"

#include "ui/node.h"
#include "ui/node_data.h"
#include "ui/tree.h"

#include "store/tree.h"

namespace ui::tests {

void node_data_test();
void tree_test();
} // namespace ui::tests
namespace store::tests {
void tree_test();
}

namespace tests {
void run_all() {
    using namespace testing;
    ui::tests::node_data_test();
    ui::tests::tree_test();
    store::tests::tree_test();
}
} // namespace tests

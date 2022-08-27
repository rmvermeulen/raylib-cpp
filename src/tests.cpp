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

namespace tests {

void node_data_test();
void tree_test();

void run_all() {
    using namespace testing;
    node_data_test();
    tree_test();
}
} // namespace tests

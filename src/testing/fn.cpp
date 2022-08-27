#include "fn.h"

#include <functional>

#include "../functions.h"
#include "suite.h"

namespace testing {

void run_test(const char* name, const run_fn& run) {
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

void describe(const char* topic, const suite_setup_fn& setup_test_cases) {
    Suite suite{topic};
    setup_test_cases(static_cast<Setup&>(suite));
    // if (verbose) suite.describe();
    suite.run();
}

} // namespace testing
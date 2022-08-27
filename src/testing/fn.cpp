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

void describe(const char* topic, const ctx_fn& setup_test_cases) {
    Suite suite{topic};
    // todo: add more hooked fns to the context
    Context ctx{
        [&suite](const char* scenario, const run_fn& run) {
            suite.add(scenario, run);
        },
        [&suite](run_fn fn) { suite.before_each(fn); },
        [&suite](run_fn fn) { suite.after_each(fn); },
        [&suite](run_fn fn) { suite.before_all(fn); },
        [&suite](run_fn fn) { suite.after_all(fn); },

    };
    setup_test_cases(ctx);
    // if (verbose)
    // suite.describe();
    suite.run();
}

} // namespace testing
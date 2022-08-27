#pragma once
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

void describe(const char* topic, const setup_test_cases_fn& setup_test_cases) {
    Suite suite{topic};
    setup_test_cases([&suite](const char* scenario, const run_fn& run) {
        suite.add(scenario, run);
    });
    suite.run();
}

} // namespace testing
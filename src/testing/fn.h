#pragma once
#include <functional>
namespace testing {

class Setup;

using run_fn = std::function<void()>;

using it_fn = std::function<void(const char*, const run_fn& _case)>;

using hook_fn = std::function<void(run_fn)>;

using setup_test_cases_fn = std::function<void(it_fn)>;

using suite_setup_fn = std::function<void(Setup&)>;

void run_test(const char* name, const run_fn& run);

void expect(bool expectation);

void expect(bool expectation, const char* message);

void describe(const char* topic, const suite_setup_fn& setup_test_cases);

} // namespace testing
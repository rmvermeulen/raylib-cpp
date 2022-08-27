#pragma once
#include <functional>
namespace testing {

using run_fn = std::function<void()>;

using it_fn = std::function<void(const char*, const run_fn& _case)>;

using setup_test_cases_fn = std::function<void(it_fn)>;
struct Context {
    it_fn it;
};

using setup_test_cases_ctx_fn = std::function<void(Context)>;

void run_test(const char* name, const run_fn& run);

void expect(bool expectation);

void expect(bool expectation, const char* message);

void describe(const char* topic, const setup_test_cases_fn& setup_test_cases);
void describe(const char* topic,
              const setup_test_cases_ctx_fn& setup_test_cases);

} // namespace testing
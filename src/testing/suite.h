#pragma once

#include <map>
#include <string>
#include <vector>

#include "fn.h"
#include "setup.h"
#include "test_case.h"

namespace testing {

class Suite : public Setup {
    const std::string _name;
    std::vector<std::pair<std::string, run_fn>> _cases;

    std::vector<run_fn> _before_each;
    std::vector<run_fn> _after_each;
    std::vector<run_fn> _before_all;
    std::vector<run_fn> _after_all;

    inline void _run_hooks(const std::vector<run_fn>& a_hooks) {
        for (const auto& hook : a_hooks)
            hook();
    }

    friend void describe(const char*, const suite_setup_fn&);
    Suite(const char* a_name);
    ~Suite();

  public:
    void add_test(const char* a_scenario, const run_fn& fn);
    void run();
    // impl Setup
    void it(const char* name, const run_fn& fn) override;
    void before_each(const run_fn& fn) override;
    void after_each(const run_fn& fn) override;
    void before_all(const run_fn& fn) override;
    void after_all(const run_fn& fn) override;

    void describe();
};
} // namespace testing

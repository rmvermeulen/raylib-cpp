#pragma once
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "fn.h"
#include "test_case.h"

namespace testing {

class Suite {
    const std::string _name;
    std::map<std::string, run_fn> _cases;

    std::vector<run_fn> _before_each;
    std::vector<run_fn> _after_each;
    std::vector<run_fn> _before_all;
    std::vector<run_fn> _after_all;

    inline void _run_hooks(const std::vector<run_fn>& a_hooks) {
        for (const auto& hook : a_hooks)
            hook();
    }

  public:
    Suite(const char* a_name) : _name(a_name) {}
    ~Suite() {}
    void add(const char* a_scenario, const run_fn& fn) {
        std::stringstream ss;
        ss << _name << ": " << a_scenario;
        _cases.insert(std::make_pair(ss.str(), fn));
    }
    void run() {
        _run_hooks(_before_all);
        for (const auto& pair : _cases) {
            _run_hooks(_before_each);
            run_test(pair.first.c_str(), pair.second);
            _run_hooks(_after_each);
        }
        _run_hooks(_after_all);
    }
    inline void before_each(run_fn fn) { _before_each.push_back(fn); }
    inline void after_each(run_fn fn) { _after_each.push_back(fn); }
    inline void before_all(run_fn fn) { _before_all.push_back(fn); }
    inline void after_all(run_fn fn) { _after_all.push_back(fn); }

    inline void describe() {
        println("suite[%s]", _name.c_str());
        println("\thooks:before_each %d", _before_each.size());
        println("\thooks:after_each %d", _after_each.size());
        println("\thooks:before_all %d", _before_all.size());
        println("\thooks:after_all %d", _after_all.size());
    }
};
} // namespace testing

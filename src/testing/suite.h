#pragma once
#include <map>
#include <sstream>
#include <string>

#include "fn.h"
#include "test_case.h"

namespace testing {

class Suite {
    const std::string _name;
    std::map<std::string, run_fn> _cases;

  public:
    Suite(const char* a_name) : _name(a_name) {}
    ~Suite() {}
    void add(const char* a_scenario, const run_fn& fn) {
        std::stringstream ss;
        ss << _name << ": " << a_scenario;
        _cases.insert(std::make_pair(ss.str(), fn));
    }
    void run() {
        for (const auto& pair : _cases) {
            run_test(pair.first.c_str(), pair.second);
        }
    }
};
} // namespace testing

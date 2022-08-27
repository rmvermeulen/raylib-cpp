#include "suite.h"

#include <exception>

#include "../functions.h"
#include "fn.h"

namespace testing {

Suite::Suite(const char* a_name) : _name(a_name) {}
Suite::~Suite() {}

void Suite::add_test(const char* a_scenario, const run_fn& fn) {
    std::string full_name = _name + ": " + a_scenario;

    if (std::any_of(_cases.begin(), _cases.end(),
                    [&full_name](std::pair<std::string, run_fn> pair) {
                        return full_name == pair.first;
                    })) {
        const auto& message = "Duplicate test case name: '" + full_name + "'";
        throw std::logic_error(message);
    }
    _cases.push_back(std::make_pair(full_name, fn));
}

void Suite::run() {
    _run_hooks(_before_all);
    for (const auto& pair : _cases) {
        _run_hooks(_before_each);
        run_test(pair.first.c_str(), pair.second);
        _run_hooks(_after_each);
    }
    _run_hooks(_after_all);
}

void Suite::describe() {
    println("suite[%s]", _name.c_str());
    println("\thooks:before_each %d", _before_each.size());
    println("\thooks:after_each %d", _after_each.size());
    println("\thooks:before_all %d", _before_all.size());
    println("\thooks:after_all %d", _after_all.size());
}

void Suite::it(const char* name, const run_fn& fn) { add_test(name, fn); }

void Suite::before_each(const run_fn& fn) { _before_each.push_back(fn); }

void Suite::after_each(const run_fn& fn) { _after_each.push_back(fn); }

void Suite::before_all(const run_fn& fn) { _before_all.push_back(fn); }

void Suite::after_all(const run_fn& fn) { _after_all.push_back(fn); }

} // namespace testing
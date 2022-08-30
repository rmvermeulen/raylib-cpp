#include "suite.h"

#include <exception>

#include "../functions.h"
#include "fn.h"

namespace testing {

Suite::Suite(const char* a_name) : _name(a_name) {}
Suite::~Suite() {}

void Suite::add_test(const char* a_scenario, const run_fn& fn) {
    std::string scenario{a_scenario};
    if (std::any_of(_cases.begin(), _cases.end(),
                    [&scenario](std::pair<std::string, run_fn> pair) {
                        return scenario == pair.first;
                    })) {
        const auto& message = "Duplicate test case name: '" + scenario + "'";
        throw std::logic_error(message);
    }
    _cases.push_back(std::make_pair(scenario, fn));
}

void Suite::run() {
    _run_hooks(_before_all);
    for (const auto& pair : _cases) {
        _run_hooks(_before_each);
        std::string full_name = _name + ": " + pair.first;
        run_test(full_name.c_str(), pair.second);
        _run_hooks(_after_each);
    }
    _run_hooks(_after_all);
}

void Suite::describe() {
    println("");
    println("suite \"%s\", %d test(s)", _name.c_str(), _cases.size());
    if (_has_hooks) {
        println("hooks:");
        if (_before_each.size())
            println("\tbefore_each(%d)", _before_each.size());
        if (_after_each.size())
            println("\tafter_each(%d)", _after_each.size());
        if (_before_all.size())
            println("\tbefore_all(%d)", _before_all.size());
        if (_after_all.size())
            println("\tafter_all(%d)", _after_all.size());
    }

    for (auto& _case : _cases) {
        println("- %s", _case.first.c_str());
    }
    println("");
}

void Suite::it(const char* name, const run_fn& fn) { add_test(name, fn); }

void Suite::before_each(const run_fn& fn) {
    _before_each.push_back(fn);
    _has_hooks = true;
}

void Suite::after_each(const run_fn& fn) {
    _after_each.push_back(fn);
    _has_hooks = true;
}

void Suite::before_all(const run_fn& fn) {
    _before_all.push_back(fn);
    _has_hooks = true;
}

void Suite::after_all(const run_fn& fn) {
    _after_all.push_back(fn);
    _has_hooks = true;
}

} // namespace testing
#pragma once
#include "fn.h"

namespace testing {

class Setup {
  public:
    virtual void it(const char*, const run_fn&) = 0;
    virtual void before_each(const run_fn&) = 0;
    virtual void after_each(const run_fn&) = 0;
    virtual void before_all(const run_fn&) = 0;
    virtual void after_all(const run_fn&) = 0;
};

} // namespace testing
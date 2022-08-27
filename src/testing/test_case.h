#pragma once
namespace testing {

class TestCase {
  public:
    virtual const char* get_name() const = 0;
    virtual bool run() = 0;
};

} // namespace testing
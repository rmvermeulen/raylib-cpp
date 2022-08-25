#include <chrono>
#include <iomanip>
#include <raylib-cpp.hpp>
#include <sstream>

class LogLine {
  public:
    const std::chrono::system_clock::time_point date;
    const std::string text;
    LogLine(const std::string& a_text)
        : date(std::chrono::system_clock::now()), text(a_text) {}
    LogLine(std::chrono::system_clock::time_point a_date,
            const std::string& a_text)
        : date(a_date), text(a_text) {}
};
class Console;
class MyStringBuf : public std::stringbuf {
    std::vector<LogLine> lines;
    friend class Console;

  public:
    virtual int sync() {
        // read buffer into line object
        lines.push_back(LogLine{str()});
        // clear buffer
        str("");
    }
};
class Console {
    MyStringBuf buf;

    std::ostream out;

  public:
    Console() : buf(), out(&buf) {}
    void Log(const char* str) { out << str << std::endl; }
    void Render() {

        int i = 0;
        for (auto line : buf.lines) {
            auto t = std::chrono::system_clock::to_time_t(line.date);
            std::stringstream ss;
            ss << i << ": [" << std::put_time(std::localtime(&t), "%T") << "] "
               << line.text;
            DrawText(ss.str().c_str(), 100, 100 + i * 20, 20, LIGHTGRAY);
            ++i;
        }
    }
};

class Framework {
  public:
    Console console;
};
#include "log_line.h"
#include <algorithm>
#include <sstream>
namespace core {

class Console {
    class LogLineBuf : public std::stringbuf {
        std::vector<LogLine> _lines;

      public:
        virtual int sync() {
            // read buffer into line object
            _lines.push_back(LogLine{str()});
            // clear buffer and leave
            str("");
            return 0;
        }

        const std::vector<LogLine>& get_lines() const { return _lines; }
    };

    LogLineBuf buf;

    std::ostream out;

  public:
    Console() : buf(), out(&buf) {}
    void Log(const char* str) { out << str << std::endl; }
    void Render(const Vector2& offset) {

        int i = 0;
        for (auto line : buf.get_lines()) {
            auto t = std::chrono::system_clock::to_time_t(line.date);
            std::stringstream ss;
            ss << i << ": [" << std::put_time(std::localtime(&t), "%T") << "] "
               << line.text;
            DrawText(ss.str().c_str(), offset.x, offset.y + i * 20, 20,
                     LIGHTGRAY);
            ++i;
        }
    }
};

} // namespace core
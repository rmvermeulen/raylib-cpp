#include <chrono>
#include <string>

namespace core
{
    class LogLine
    {
    public:
        const std::chrono::system_clock::time_point date;
        const std::string text;
        LogLine(const std::string &a_text)
            : date(std::chrono::system_clock::now()), text(a_text) {}
        LogLine(std::chrono::system_clock::time_point a_date,
                const std::string &a_text)
            : date(a_date), text(a_text) {}
    };

} // namespace core
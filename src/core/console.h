#include "my_stringbuf.h"
namespace core
{

    class Console
    {
        MyStringBuf buf;

        std::ostream out;

    public:
        Console() : buf(), out(&buf) {}
        void Log(const char *str) { out << str << std::endl; }
        void Render()
        {

            int i = 0;
            for (auto line : buf.lines)
            {
                auto t = std::chrono::system_clock::to_time_t(line.date);
                std::stringstream ss;
                ss << i << ": [" << std::put_time(std::localtime(&t), "%T") << "] "
                   << line.text;
                DrawText(ss.str().c_str(), 100, 100 + i * 20, 20, LIGHTGRAY);
                ++i;
            }
        }
    };

} // namespace core
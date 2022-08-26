#include <sstream>
#include "log_line.h"

namespace core
{
    class MyStringBuf : public std::stringbuf
    {
        std::vector<LogLine> lines;
        friend class Console;

    public:
        virtual int sync()
        {
            // read buffer into line object
            lines.push_back(LogLine{str()});
            // clear buffer
            str("");
            return 0;
        }
    };

} // namespace core
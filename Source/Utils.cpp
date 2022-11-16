module Utils;

import std.core;

namespace CLR
{
    // std::cerr is not buffered
    template<bool USE_FILE>
    void LOG_ERROR(const char* error)
    {
        if (!USE_FILE)
        {
            std::cerr << error << std::endl;
        }
        else
        {
            std::clog << error << std::endl;
        }
    }
}
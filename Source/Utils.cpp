module Utils;

import <cassert>;
import <iostream>;

namespace CLR
{
    void ASSERT(bool exp, char const* msg)
    {
        assert(((void)msg, exp));
    }

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
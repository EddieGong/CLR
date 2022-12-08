module CLR.Utils;

import <cassert>;
import <iostream>;

import CLR.Settings;

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

    // TODO: it doesn't work with message. Rewrite the function.
    void ASSERT(bool exp, const char* msg)
    {
        if (Settings::Debug::AllowAssert)
        {
            if (exp == false)
            {
                throw std::exception(msg);
                //LOG_ERROR<false>(msg);
            }
        }
    }
}
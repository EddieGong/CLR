#include "pch.h"

#include <Utils.h>

import std.core;

namespace CLR
{
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
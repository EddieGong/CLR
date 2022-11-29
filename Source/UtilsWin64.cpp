module;

#include "Headers.h"

module Utils;

namespace CLR
{
    void LOG_DEBUG_INFO(char const* str)
    {
        OutputDebugStringA(str);
    }
}
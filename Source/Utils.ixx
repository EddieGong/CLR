module;

#include <cstdio>

export module CLR.Utils;

namespace CLR
{
    using HRESULT = long;

    export template<typename... Args>
    constexpr void UNUSED_PARAMS(const Args& ...args)
    {
        (args, ...);
    }

    void LogInternal(char const* str);

    export template<bool USE_FILE>
    void LOG_ERROR(const char* error);

    export template<typename... Args>
    void LOG_INFO(char const* format, const Args& ...args)
    {
        char str[256] = {};
        sprintf_s(str, format, args...);
        LogInternal(str);
    }

    export template<typename... Args>
    void LOG_DEBUG_INFO(char const* format, const Args& ...args)
    {
#ifdef _DEBUG
        LOG_INFO(format, args...);
#endif
    }

    export void ThrowIfFailed(HRESULT hr);

    // TONOTE: equivalent to std::size, use it if there is no proper std header included
    export template<typename T, size_t N>
    constexpr size_t GetArrayLength(T const (&array)[N]) noexcept
    {
        return sizeof(array) / sizeof(T);
    }
}
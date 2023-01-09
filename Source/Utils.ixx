module;

#include <format>

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
        // TODO: read https://en.cppreference.com/w/cpp/utility/format/format
        // As of P2216R3, it is an error if the format string is not a constant expression.std::vformat can be used in this case.
        std::string str = std::vformat(format, std::make_format_args(args...));
        str += '\n';
        LogInternal(str.c_str());
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
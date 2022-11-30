
export module Utils;

namespace CLR
{
    using HRESULT = long;

    export void ASSERT(bool exp, char const* msg);

    export template<typename... Args>
    void UNUSED_PARAMS(const Args& ...arg)
    {
        (arg, ...);
    }

    template<bool USE_FILE>
    void LOG_ERROR(const char* error);

    export void LOG_DEBUG_INFO(char const* warning);

    export void ThrowIfFailed(HRESULT hr);
}
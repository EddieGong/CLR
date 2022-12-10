
export module CLR.Utils;

export namespace CLR
{
    using HRESULT = long;

    template<typename... Args>
    constexpr void UNUSED_PARAMS(const Args& ...arg)
    {
        (arg, ...);
    }

    template<bool USE_FILE>
    void LOG_ERROR(const char* error);

    void LOG_INFO(char const* str);
    void LOG_DEBUG_INFO(char const* str);
    
    void ThrowIfFailed(HRESULT hr);

    // TONOTE: equivalent to std::size, use it if there is no proper std header included
    template<typename T, size_t N>
    constexpr size_t GetArrayLength(T const (&array)[N]) noexcept
    {
        return sizeof(array) / sizeof(T);
    }
}
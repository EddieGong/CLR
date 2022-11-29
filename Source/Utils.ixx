
export module Utils;

namespace CLR
{
    export void ASSERT(bool exp, char const* msg);

    export template<typename... Args>
    void UNUSED_PARAMS(const Args& ...arg)
    {
        (arg, ...);
    }

    template<bool USE_FILE>
    void LOG_ERROR(const char* error);

    export void LOG_DEBUG_INFO(char const* warning);

    //inline std::string HrToString(HRESULT hr)
    //{
    //    char s_str[64] = {};
    //    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<uint32>(hr));
    //    return std::string(s_str);
    //}

    //class HrException : public std::runtime_error
    //{
    //public:
    //    HrException(HRESULT hr) : 
    //        std::runtime_error(HrToString(hr)), m_hr(hr) 
    //    {}
    //    HRESULT Error() const { return mHResult; }

    //private:
    //    const HRESULT mHResult;
    //};


    //inline void ThrowIfFailed(HRESULT hr)
    //{
    //    if (FAILED(hr))
    //    {
    //        throw HrException(hr);
    //    }
    //}
}
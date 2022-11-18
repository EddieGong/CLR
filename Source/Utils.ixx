#pragma warning(disable : 5050)

export module Utils;

import std.core;
import <cassert>;

namespace CLR
{
    export void ASSERT(bool exp, char const* msg) 
    { 
        assert(((void)msg, exp)); 
    }

    export template<typename... Args>
    void UNUSED_PARAMS(const Args& ...arg)
    {
        (arg, ...);
    }

    template<bool USE_FILE>
    void LOG_ERROR(const char* error);

    //inline std::string HrToString(HRESULT hr)
    //{
    //    char s_str[64] = {};
    //    sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<uint32>(hr));
    //    return std::string(s_str);
    //}

    //class HrException : public std::runtime_error
    //{
    //public:
    //    HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
    //    HRESULT Error() const { return m_hr; }
    //private:
    //    const HRESULT m_hr;
    //};


    //inline void ThrowIfFailed(HRESULT hr)
    //{
    //    if (FAILED(hr))
    //    {
    //        throw HrException(hr);
    //    }
    //}
}
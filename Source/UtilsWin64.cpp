module;

#include "Headers.h"

module CLR.Utils;

namespace CLR
{
    void LogInternal(char const* str)
    {
        OutputDebugStringA(str);
    }

    // Helper class for COM exceptions
    class com_exception : public std::exception
    {
    public:
        com_exception(HRESULT hr) noexcept : result(hr) {}

        const char* what() const noexcept override
        {
            static char s_str[64] = {};
            sprintf_s(s_str, "Failure with HRESULT of %08X", static_cast<unsigned int>(result));
            return s_str;
        }

    private:
        HRESULT result;
    };

    // TODO: Is exception necessary? Is it handled somewhere?
    // Helper utility converts D3D API failures into exceptions.
    void ThrowIfFailed(HRESULT hr)
    {
        if (FAILED(hr))
        {
            // Set a breakpoint on this line to catch DirectX API errors
            throw com_exception(hr);
        }
    }
}
#pragma once

namespace CLR
{
    class CLRMain : public winrt::implements<CLRMain, winrt::Windows::Foundation::IInspectable>
    {
    public:
        CLRMain() = default;
        ~CLRMain() = default;

    };
}
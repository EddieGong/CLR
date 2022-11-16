#pragma once

#include <Headers.h>

import Utils;

namespace CLR
{
    class CLRMain : public winrt::implements<CLRMain, winrt::Windows::Foundation::IInspectable>
    {
    public:
        CLRMain() = default;
        ~CLRMain() = default;

        void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window) { UNUSED_PARAMS(window); }

        void Init()     {}
        void Load()     {}
        void Run();//      {}
        void Suspend()  {}
        void Resume()   {}
        void Close()    {}

    private:
        bool windowClosed {false};
        bool visible      {false};
    };
}
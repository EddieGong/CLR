#pragma once

#include <Headers.h>

import Utils;

namespace CLR
{
    class Framework : public winrt::implements<Framework, winrt::Windows::Foundation::IInspectable>
    {
    public:
        Framework() = default;
        ~Framework() = default;

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
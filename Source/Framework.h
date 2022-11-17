#pragma once

#include <Headers.h>

import Utils;

namespace CLR
{
    class Renderer;

    class Framework : public winrt::implements<Framework, winrt::Windows::Foundation::IInspectable>
    {
    public:
        // TONOTE: Can't use "= defualt" because of unique_ptr
        Framework();
        ~Framework();

        void SetWindow(winrt::Windows::UI::Core::CoreWindow const& window) { UNUSED_PARAMS(window); }

        void Init();
        void Load()     {}
        void Run();
        void Suspend()  {}
        void Resume()   {}
        void Close();

    private:
        bool windowClosed {false};
        bool visible      {false};

        std::unique_ptr<Renderer> renderer;
    };
}
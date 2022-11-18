#pragma once

#include <Headers.h>

import Utils;

using namespace winrt;

namespace CLR
{
    class Renderer;

    class Framework : public implements<Framework, Windows::Foundation::IInspectable>
    {
    public:
        // TONOTE: Can't use "= defualt" because of unique_ptr
        Framework();
        ~Framework();

        void SetWindow(Windows::UI::Core::CoreWindow const& window) { UNUSED_PARAMS(window); }

        void Init();
        void Load()     {}
        void Run();
        void Suspend()  {}
        void Resume()   {}
        void Close();

        void SetVisibility(bool /*visible*/) {}
        void WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState /*state*/) {}

    private:
        bool m_WindowClosed {false};
        bool m_Visible      {false};

        std::unique_ptr<Renderer> m_Renderer;
    };
}
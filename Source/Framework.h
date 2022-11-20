#pragma once

#include <Headers.h>
#include <BasicTypes.h>

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

        void SetWindow(Size const& windowSize);

        void Init();
        void Load()     {}
        void Run();
        void Suspend()  {}
        void Resume()   {}
        void Close();

        void Visibility(bool /*visible*/) {}
        void WindowActivationChanged(Windows::UI::Core::CoreWindowActivationState /*state*/) {}
        void WindowSizeChanged(Size const& /*size*/) {}
        void DpiChanged(float /*dpi*/) {}
        void ValidateGraphicsDevice() {}

    private:
        bool m_WindowClosed {false};
        bool m_Visible      {false};

        std::unique_ptr<Renderer> m_Renderer;
    };
}
#include <Framework.h>
#include <Renderer.h>

import Utils;

using namespace winrt::Windows::UI::Core;

namespace CLR
{
    Framework::Framework()
    {}
    Framework::~Framework()
    {}

    void Framework::Init()
    {
        m_Renderer = std::make_unique<Renderer>();
        m_Renderer->Init();
    }

    void Framework::Close()
    {
        m_Renderer.reset();
    }

    void Framework::Run()
    {
        while (!m_WindowClosed)
        {
            if (m_Visible)
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);
                //Update();
                //m_renderer->Render();
                //m_deviceResources->Present();
            }
            else
            {
                CoreWindow::GetForCurrentThread().Dispatcher().ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
            }
        }
    }

    void Framework::SetWindow(Size const& /*windowSize*/)
    {
    
    }
}
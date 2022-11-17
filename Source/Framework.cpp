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
        renderer = std::make_unique<Renderer>();
    }

    void Framework::Close()
    {
        renderer.reset();
    }

    void Framework::Run()
    {
        while (!windowClosed)
        {
            if (visible)
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
}
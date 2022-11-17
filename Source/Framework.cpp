#include <Framework.h>

import Utils;

using namespace winrt::Windows::UI::Core;

namespace CLR
{
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
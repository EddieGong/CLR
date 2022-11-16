#include <CLRMain.h>

import Utils;
import Module;

using namespace winrt::Windows::UI::Core;

namespace CLR
{
    void CLRMain::Run() 
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
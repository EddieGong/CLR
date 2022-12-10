
#include <Renderer.h>

import CLR.Graphics.Core;
import CLR.CmdLineArg;

namespace CLR
{
    Renderer::Renderer()
    {
        CreateDeviceIndependentResources();
        CreateDeviceResources();
    }
    Renderer::~Renderer()
    {
        Graphics::Core::DestroyDevice(mDevice);
    }

    void Renderer::CreateDeviceIndependentResources()
    {
        // Direct2D, DirectWrite
    }

    void Renderer::CreateDeviceResources()
    {
        Graphics::Core::DeviceCreateParameters param;
#if _DEBUG
        if (bool debugLayerEnabled = false; CmdLineArg::GetValue(L"gfx-debug", debugLayerEnabled))
        {
            param.debugLayerEnabled = debugLayerEnabled;
        }
#endif
        mDevice = Graphics::Core::CreateDevice(param);
    }
}
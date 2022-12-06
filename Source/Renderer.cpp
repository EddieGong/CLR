
#include <Renderer.h>

import CLR.Graphics.Core;

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
        // TODO: Use command arguments
        param.debugLayerEnabled = true;
#endif
        mDevice = Graphics::Core::CreateDevice(param);
    }
}
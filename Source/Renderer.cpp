
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
        Graphics::DestroyDevice(mDevice);
    }

    void Renderer::CreateDeviceIndependentResources()
    {
        // Direct2D, DirectWrite
    }

    void Renderer::CreateDeviceResources()
    {
        Graphics::DeviceCreateParameters param;
        mDevice = Graphics::CreateDevice(param);
    }
}
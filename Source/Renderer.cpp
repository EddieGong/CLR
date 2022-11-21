
#include <Renderer.h>
#include <GraphicsCore.h>

//import GraphicsDX12;

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
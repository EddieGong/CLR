
#include <Renderer.h>
#include <GraphicsCore.h>

//import GraphicsDX12;

namespace CLR
{
    Renderer::Renderer()
    {}
    Renderer::~Renderer()
    {}

    void Renderer::Init()
    {
        CreateDeviceIndependentResources();
        CreateDeviceResources();
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
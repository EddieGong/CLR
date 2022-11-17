
#include <Renderer.h>
#include <GraphicsCore.h>

//import GraphicsDX12;

namespace CLR
{
    Renderer::Renderer()
    {
    }
    Renderer::~Renderer()
    {}

    void Renderer::Create()
    {
        Graphics::DeviceCreateParameters param;
        device = Graphics::CreateDevice(param);
    }
}
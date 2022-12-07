#pragma once

#include <BasicTypes.h>
#include <GraphicsCoreTypes.h>

namespace CLR
{
    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

    private:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

    private:
        Graphics::Core::HDevice mDevice { nullptr };

        // Display device resolution
        Size mDisplaySize;
        // Max scaled render target resolution
        Size mRenderTargetSize;
    };
}
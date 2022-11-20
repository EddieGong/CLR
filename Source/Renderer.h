#pragma once

#include <BasicTypes.h>
#include <GraphicsTypes.h>

namespace CLR
{
    class Renderer
    {
    public:

        Renderer();
        ~Renderer();

        void Init();

    private:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

    private:
        Graphics::HDevice mDevice { nullptr };

        // Display device resolution
        Size mDisplaySize;
        // Max scaled render target resolution
        Size mRenderTargetSize;
    };
}
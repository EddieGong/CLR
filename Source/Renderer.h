#pragma once

#include <BasicTypes.h>
#include <GraphicsCoreTypes.h>

namespace CLR
{
    namespace GCore = Graphics::Core;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void OnDeviceLost()
        {}
        void OnDeviceRestored()
        {}

    private:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

        void DestroyDeviceResources();

    private:
        GCore::HDevice  mDevice     { 0 };
        GCore::HDisplay mDisplay    { 0 };

        // Display device resolution
        Size mDisplaySize;
        // Max scaled render target resolution
        Size mRenderTargetSize;
    };
}
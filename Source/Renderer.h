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

    private:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

        void DestroyDeviceResources();

    private:
        GCore::HDevice mDevice                      { 0 };
        GCore::HCommandQueue mGraphicsCommandQueue  { 0 };
        GCore::HCommandQueue mComputeCommandQueue   { 0 };
        GCore::HCommandQueue mCopyCommandQueue      { 0 };

        // Display device resolution
        Size mDisplaySize;
        // Max scaled render target resolution
        Size mRenderTargetSize;
    };
}
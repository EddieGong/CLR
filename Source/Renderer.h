#pragma once

#include <any>

#include "BasicTypes.h"
#include "GraphicsCoreTypes.h"

namespace CLR
{
    namespace GCore = Graphics::Core;

    class Renderer
    {
    public:
        Renderer() noexcept(false);
        ~Renderer();

        Renderer(Renderer const&)             = delete;
        Renderer& operator= (Renderer const&) = delete;

    public:
        void Init(std::any window) noexcept(false);

        void Draw();

        void WaitForGpu();

    private:
        void CreateDeviceIndependentResources();
        void CreateDeviceResources();

        void DestroyDeviceResources();

        void OnDeviceLost();
        void OnDeviceRestored();

    private:
        GCore::HDevice  mDevice     { 0 };
        GCore::HDisplay mDisplay    { 0 };

        std::any mWindow;

        // Display device resolution
        Size mDisplaySize;
        // Max scaled render target resolution
        Size mRenderTargetSize;
    };
}
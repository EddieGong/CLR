
#include "CLRAssert.h"
#include "Renderer.h"

import CLR.Graphics.Core;
import CLR.CmdLineArg;

namespace CLR
{
    namespace GCore = Graphics::Core;

    Renderer::Renderer() noexcept(false)
    {
        CreateDeviceIndependentResources();
    }

    Renderer::~Renderer()
    {
        DestroyDeviceResources();
    }

    void Renderer::Init(std::any window) noexcept(false)
    {
        mWindow = window;

        CreateDeviceResources();
    }

    void Renderer::Draw()
    {
    }

    void Renderer::WaitForGpu()
    {
        GCore::WaitForGpuToFinish();
    }

    void Renderer::CreateDeviceIndependentResources()
    {
        // Direct2D, DirectWrite
    }

    void Renderer::CreateDeviceResources()
    {
        {
            GCore::DeviceCreateParameters param;
#if _DEBUG
            if (bool debugLayerEnabled = false; CmdLineArg::GetValue(L"gfx-debug", debugLayerEnabled))
            {
                param.DebugLayerEnabled = debugLayerEnabled;
            }
#endif
            mDevice = GCore::CreateDevice(param);

            GCore::RegisterDeviceLostCallbackFunc    (mDevice, [this]() { this->OnDeviceLost(); });
            GCore::RegisterDeviceRestoredCallbackFunc(mDevice, [this]() { this->OnDeviceRestored(); });
        }

        {
            GCore::DisplayCreateParameters params{};
            params.WindowHandle = mWindow;
            mDisplay = GCore::CreateDisplay(mDevice, params);
        }
    }

    void Renderer::DestroyDeviceResources()
    {
        GCore::DestroyDisplay(mDisplay);

        GCore::DestroyDevice(mDevice);
    }

    void Renderer::OnDeviceLost()
    {
        CLR_ASSERT_MSG(false, "Not implemented");
    }

    void Renderer::OnDeviceRestored()
    {
        CLR_ASSERT_MSG(false, "Not implemented");
    }
}
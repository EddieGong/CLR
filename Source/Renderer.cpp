
#include <Renderer.h>

import CLR.Graphics.Core;
import CLR.CmdLineArg;

namespace CLR
{
    namespace GCore = Graphics::Core;

    Renderer::Renderer()
    {
        CreateDeviceIndependentResources();
        CreateDeviceResources();
    }
    Renderer::~Renderer()
    {
        DestroyDeviceResources();
    }

    void Renderer::CreateDeviceIndependentResources()
    {
        // Direct2D, DirectWrite
    }

    void Renderer::CreateDeviceResources()
    {
        GCore::DeviceCreateParameters param;
#if _DEBUG
        if (bool debugLayerEnabled = false; CmdLineArg::GetValue(L"gfx-debug", debugLayerEnabled))
        {
            param.DebugLayerEnabled = debugLayerEnabled;
        }
#endif
        mDevice = GCore::CreateDevice(param);

        mGraphicsCommandQueue = GCore::CreateCommandQueue(mDevice, GCore::CommandQueueType::Graphics);
        mComputeCommandQueue  = GCore::CreateCommandQueue(mDevice, GCore::CommandQueueType::Compute);
        mCopyCommandQueue     = GCore::CreateCommandQueue(mDevice, GCore::CommandQueueType::Copy);
    }

    void Renderer::DestroyDeviceResources()
    {
        GCore::DestroyCommandQueue(mGraphicsCommandQueue);
        GCore::DestroyCommandQueue(mComputeCommandQueue);
        GCore::DestroyCommandQueue(mCopyCommandQueue);

        GCore::DestroyDevice(mDevice);
    }
}
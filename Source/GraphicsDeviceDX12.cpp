module;

#include "Headers.h"
#include "GraphicsDX12.h"

module CLR.Graphics.Core;

import Utils;

using Microsoft::WRL::ComPtr;

namespace CLR::Graphics
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
        Device* device = new Device();

#if defined(_DEBUG)
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        //
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        if (createParams.debugLayerEnabled)
        {
            ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
            {
                debugController->EnableDebugLayer();
            }
            else
            {
                LOG_DEBUG_INFO("WARNING: Direct3D Debug Device is not available\n");
            }

            ComPtr<IDXGIInfoQueue> dxgiInfoQueue;
            if (SUCCEEDED(DXGIGetDebugInterface1(0, IID_PPV_ARGS(dxgiInfoQueue.GetAddressOf()))))
            {
                device->DXGIFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif

        return device;
    }

    void DestroyDevice(HDevice device)
    {
        device;
    }
}
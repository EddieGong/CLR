module;

#include "Headers.h"
#include "GraphicsDX12.h"

module CLR.Graphics.Core;

import Utils;

using Microsoft::WRL::ComPtr;

namespace CLR::Graphics
{
    void EnableDebugLayer(Device* device, bool debugLayerEnabled)
    {
#if defined(_DEBUG)
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        //
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        if (debugLayerEnabled)
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

                dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
                dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

                // TODO: filters?
            }
        }
#endif
    }

    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
        Device* device = new Device();

        EnableDebugLayer(device, createParams.debugLayerEnabled);

        ComPtr<IDXGIFactory6> dxgiFactory;
        ThrowIfFailed(CreateDXGIFactory2(device->DXGIFactoryFlags, IID_PPV_ARGS(dxgiFactory.GetAddressOf())));
        device->DXGIFactory = dxgiFactory.Get();

        return device;
    }

    void DestroyDevice(HDevice device)
    {
        device;
    }
}
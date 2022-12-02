module;

#include "Headers.h"
#include "BasicTypes.h"
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
                LOG_INFO("WARNING: Direct3D Debug Device is not available\n");
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

    void GetAdapter(IDXGIFactory6* dxgiFactory, IDXGIAdapter1*& pAdapter, bool highPerf, D3D_FEATURE_LEVEL featureLevel)
    {
        pAdapter = nullptr;

        DXGI_GPU_PREFERENCE gpuPref = highPerf ? DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE : DXGI_GPU_PREFERENCE_MINIMUM_POWER;

        ComPtr<IDXGIAdapter1> adapter;
        for (uint32 adapterIndex = 0; SUCCEEDED(dxgiFactory->EnumAdapterByGpuPreference(adapterIndex, gpuPref, IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))); ++adapterIndex)
        {
            DXGI_ADAPTER_DESC1 desc;
            ThrowIfFailed(adapter->GetDesc1(&desc));

            if (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)
            {
                // Don't select the Basic Render Driver adapter.
                continue;
            }

            // Check but don't create the actuall device
            if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), featureLevel, _uuidof(ID3D12Device), nullptr)))
            {
#ifdef _DEBUG
                wchar_t buff[256]{};
                swprintf_s(buff, L"Direct3D Adapter #%u: VenderID: %04X, DeviceID: %04X - %ls\n",
                    adapterIndex, desc.VendorId, desc.DeviceId, desc.Description);
                OutputDebugStringW(buff);
#endif
                break;
            }
        }
    }

    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
        Device* device = new Device();

        EnableDebugLayer(device, createParams.debugLayerEnabled);

        ComPtr<IDXGIFactory6> dxgiFactory;
        ThrowIfFailed(CreateDXGIFactory2(device->DXGIFactoryFlags, IID_PPV_ARGS(dxgiFactory.GetAddressOf())));
        device->DXGIFactory = dxgiFactory.Get();

        // Ref: Variable refresh-rate displays https://walbourn.github.io/care-and-feeding-of-modern-swap-chains-3/
        if (device->Options & (uint32_t)Option::AllowTearing)
        {
            bool allowTearing = false;
            HRESULT hr = device->DXGIFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
            if (FAILED(hr) || !allowTearing)
            {
                device->Options &= ~((uint32_t)Option::AllowTearing);
                LOG_DEBUG_INFO("WARNING: Variable refresh rate displays not supported");
            }
        }

        ComPtr<IDXGIAdapter1> adapter;
        GetAdapter(device->DXGIFactory, *adapter.GetAddressOf(), true, device->MinFeatureLevel);

        return device;
    }

    void DestroyDevice(HDevice device)
    {
        device;
    }
}
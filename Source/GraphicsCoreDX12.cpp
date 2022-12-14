module;

#include "Headers.h"
#include "BasicTypes.h"
#include "GraphicsCoreDX12.h"

#include "CLRAssert.h"

module CLR.Graphics.Core;

import CLR.Math.Vector;
import CLR.Utils;

using Microsoft::WRL::ComPtr;

namespace CLR::Graphics::Core
{
    static const uint32 sBackBufferCount = 3;

    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
        Device* device = new Device();

        EnableDebugLayer(device, createParams.DebugLayerEnabled);

        ThrowIfFailed(CreateDXGIFactory2(device->DXGIFactoryFlags, IID_PPV_ARGS(device->DXGIFactory.ReleaseAndGetAddressOf())));
        CheckVariableRefreshRateSupport(device->DXGIFactory.Get(), device->Options);

        ComPtr<IDXGIAdapter1> dxgiAdapter;
        GetAdapter(device->DXGIFactory.Get(), *dxgiAdapter.GetAddressOf(), device->MinFeatureLevel);

        HRESULT hr = D3D12CreateDevice(dxgiAdapter.Get(), device->MinFeatureLevel, IID_PPV_ARGS(device->D3DDevice.ReleaseAndGetAddressOf()));
        ThrowIfFailed(hr);
        device->D3DDevice->SetName(L"D3D12 Device (CLR)");

        // TODO: Use ID3D12InfoQueue to configure debug devie?

        device->D3DFeatureLevel = GetMaxSupportedFeatureLevel(device->D3DDevice.Get(), device->MinFeatureLevel);

        return device;
    }

    void DestroyDevice(HDevice device)
    {
        delete device;
    }
    
    // Command Queue
    HCommandQueue CreateCommandQueue(HDevice device, CommandQueueType type)
    {
        CommandQueue* queue = new CommandQueue();

        D3D12_COMMAND_QUEUE_DESC queueDesc {};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type  = GetInternalCommandQueueType(type);

        ThrowIfFailed(device->D3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(queue->D3DCommandQueue.ReleaseAndGetAddressOf())));
        return queue;
    }

    void DestroyCommandQueue(HCommandQueue queue)
    {
        delete queue;
    }

    // Display
    HDisplay CreateDisplay(HDevice device)
    {
        Display* display = new Display();

        D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc {};
        rtvDescriptorHeapDesc.NumDescriptors = sBackBufferCount;
        rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        ThrowIfFailed(device->D3DDevice->CreateDescriptorHeap(&rtvDescriptorHeapDesc, IID_PPV_ARGS(display->DescriptorHeap.ReleaseAndGetAddressOf())));

        return display;
    }

    void DestroyDisplay(HDisplay display)
    {
        delete display;
    }

    // Internal functions

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

    void GetAdapter(IDXGIFactoryX* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, D3D_FEATURE_LEVEL featureLevel, bool highPerf)
    {
        dxgiAdapter = nullptr;

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

        // TODO: try warp12 instead

        if (adapter == nullptr)
        {
            throw std::runtime_error("No Direct3D 12 device found");
        }

        dxgiAdapter = adapter.Detach();
    }

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32_t& options)
    {
        // Ref: Variable refresh-rate displays https://walbourn.github.io/care-and-feeding-of-modern-swap-chains-3/
        if (options & (uint32_t)Option::AllowTearing)
        {
            // TONOTE: can't use bool because CheckFeatureSupport needs a 32 bits type
            BOOL allowTearing = false;
            HRESULT hr = dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
            if (FAILED(hr) || !allowTearing)
            {
                options &= ~((uint32_t)Option::AllowTearing);
                LOG_DEBUG_INFO("WARNING: Variable refresh rate displays not supported");
            }
        }
    }

    D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel(ID3D12Device* d3dDevice, D3D_FEATURE_LEVEL minFeatureLevel)
    {
        static const D3D_FEATURE_LEVEL sFeatureLevels[] =
        {
 #ifdef USING_D3D12_AGILITY_SDK
            D3D_FEATURE_LEVEL_12_2,
 #endif
            D3D_FEATURE_LEVEL_12_1,
            D3D_FEATURE_LEVEL_12_0,
        };

        CLR_ASSERT_MSG(minFeatureLevel == sFeatureLevels[GetArrayLength(sFeatureLevels) - 1], "Min feature level is not matched in the code");
        D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevels{ static_cast<UINT>(std::size(sFeatureLevels)), sFeatureLevels, minFeatureLevel };

        HRESULT hr = d3dDevice->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevels, sizeof(featureLevels));
        return SUCCEEDED(hr) ? featureLevels.MaxSupportedFeatureLevel : minFeatureLevel;
    }

    D3D12_COMMAND_LIST_TYPE GetInternalCommandQueueType(CommandQueueType type)
    {
        D3D12_COMMAND_LIST_TYPE internalType = D3D12_COMMAND_LIST_TYPE_NONE;
        switch (type)
        {
        case CLR::Graphics::Core::CommandQueueType::Graphics:
            internalType = D3D12_COMMAND_LIST_TYPE_DIRECT;
            break;
        case CLR::Graphics::Core::CommandQueueType::Compute:
            internalType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            break;
        case CLR::Graphics::Core::CommandQueueType::Copy:
            internalType = D3D12_COMMAND_LIST_TYPE_COPY;
            break;
        }

        CLR_ASSERT_MSG(internalType != D3D12_COMMAND_LIST_TYPE_NONE, "Can't find correct D3D12 command queue type");
        return internalType;
    }
}
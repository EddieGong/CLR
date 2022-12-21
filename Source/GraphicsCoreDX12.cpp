module;

// TONOTE: Can't use "import <array>;" becasue of warnings. The modules implementation is still experimental, hopefully, it will be fixed in the future.
#include <array>

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
    std::unique_ptr<Device>       sDevice;
    std::unique_ptr<CommandQueue> sCommandQueues[int32(CommandListType::Count)];
    std::unique_ptr<Display>      sDisplay;

    template<typename T>
    void SetName(T obj, wchar_t* name)
    {
        obj->SetName(name == nullptr ? L"NULL" : name);
    }


    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
        sDevice = std::make_unique<Device>();
        Device* device = sDevice.get();

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

        std::array<CommandListType, size_t(CommandListType::Count)> commandListTypes = { CommandListType::Graphics, CommandListType::Compute, CommandListType::Copy };
        for (auto type : commandListTypes)
        {
            sCommandQueues[size_t(type)] = std::make_unique<CommandQueue>();
            CreateCommandQueue(device, sCommandQueues[size_t(type)].get(), type);
        }

        return device;
    }

    void DestroyDevice(HDevice device)
    {
        CLR_ASSERT(device == sDevice.get());
        sDevice.reset();
    }
    

    // Display
    HDisplay CreateDisplay(HDevice device, DisplayCreateParameters const& createParams)
    {
        sDisplay = std::make_unique<Display>();
        Display* display = sDisplay.get();

        D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescRTV {};
        descriptorHeapDescRTV.NumDescriptors = sBackBufferCount;
        descriptorHeapDescRTV.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

        ThrowIfFailed(device->D3DDevice->CreateDescriptorHeap(&descriptorHeapDescRTV, IID_PPV_ARGS(display->DescriptorHeapRTV.ReleaseAndGetAddressOf())));
        display->DescriptorHeapRTV->SetName(L"Display Descriptor Heap RTV");
        display->DescriptorSize = device->D3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

        if (createParams.DepthBufferFormat != SurfaceFormat::UNKNOWN)
        {
            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescDSV {};
            descriptorHeapDescDSV.NumDescriptors = 1;
            descriptorHeapDescDSV.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

            ThrowIfFailed(device->D3DDevice->CreateDescriptorHeap(&descriptorHeapDescDSV, IID_PPV_ARGS(display->DescriptorHeapDSV.ReleaseAndGetAddressOf())));
            display->DescriptorHeapDSV->SetName(L"Display Descriptor Heap DSV");
        }

        return display;
    }

    void DestroyDisplay(HDisplay display)
    {
        CLR_ASSERT(display == sDisplay.get());
        sDisplay.reset();
    }


    // Command
    void CreateCommandQueue(HDevice device, HCommandQueue queue, CommandListType type)
    {
        D3D12_COMMAND_QUEUE_DESC queueDesc{};
        queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
        queueDesc.Type = GetInternalCommandListType(type);

        ThrowIfFailed(device->D3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(queue->D3DCommandQueue.ReleaseAndGetAddressOf())));
    }

    void DestroyCommandQueue(HCommandQueue queue)
    {
        delete queue;
    }

    HCommandList CreateCommandList(HDevice device, CommandListCreateParameters const& createParams)
    {
        CommandList* commandList = new CommandList;

        const D3D12_COMMAND_LIST_TYPE listType = GetInternalCommandListType(createParams.Type);

        for (uint32 i = 0; i < sBackBufferCount; ++i)
        {
            ThrowIfFailed(device->D3DDevice->CreateCommandAllocator(listType, IID_PPV_ARGS(commandList->Allocators[i].ReleaseAndGetAddressOf())));
            //SetName(commandList->Allocator[i], createParams.Name);
        }

        ThrowIfFailed(device->D3DDevice->CreateCommandList(0, listType, commandList->Allocators[0].Get(), nullptr, IID_PPV_ARGS(commandList->List.ReleaseAndGetAddressOf())));
        ThrowIfFailed(commandList->List->Close());

        SetName(commandList->List, createParams.Name);

        return commandList;
    }

    void DestroyCommandList(HCommandList commandList)
    {
        // TODO: RAII?
        delete commandList;
    }


    // Fence
    HFence CreateFence(HDevice device)
    {
        Fence* fence = new Fence();

        ThrowIfFailed(device->D3DDevice->CreateFence(fence->Value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence->D3DFence.ReleaseAndGetAddressOf())));
        // TODO: SetName
    }

    void DestroyFence(HFence fence)
    {
    
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

    D3D12_COMMAND_LIST_TYPE GetInternalCommandListType(CommandListType type)
    {
        D3D12_COMMAND_LIST_TYPE internalType = D3D12_COMMAND_LIST_TYPE_NONE;
        switch (type)
        {
        case CLR::Graphics::Core::CommandListType::Graphics:
            internalType = D3D12_COMMAND_LIST_TYPE_DIRECT;
            break;
        case CLR::Graphics::Core::CommandListType::Compute:
            internalType = D3D12_COMMAND_LIST_TYPE_COMPUTE;
            break;
        case CLR::Graphics::Core::CommandListType::Copy:
            internalType = D3D12_COMMAND_LIST_TYPE_COPY;
            break;
        }

        CLR_ASSERT_MSG(internalType != D3D12_COMMAND_LIST_TYPE_NONE, "Can't find correct D3D12 command queue type");
        return internalType;
    }
}
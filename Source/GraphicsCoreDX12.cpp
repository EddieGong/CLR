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
import CLR.EnumUtils;

using Microsoft::WRL::ComPtr;
using EventWrapper = Microsoft::WRL::Wrappers::Event;

namespace CLR::Graphics::Core
{
    namespace
    {
        std::unique_ptr<Device>         sDevice;
        std::unique_ptr<CommandQueue>   sCommandQueues[int32(CommandListType::Count)];
        std::unique_ptr<Display>        sDisplay;

        EventWrapper                    sFenceEvent;
        std::unique_ptr<Fence>          sFence;
        
        uint32                          sBackBufferIndex{ 0 };

        uint32                          sDescriptorSizes[D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES];
    }

    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
        sDevice = std::make_unique<Device>();
        Device* device = sDevice.get();

        const bool debugLayerEnabled = EnableDebugLayer(createParams.DebugLayerEnabled);
        if (debugLayerEnabled)
        {
            device->DXGIFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
        }
        ThrowIfFailed(CreateDXGIFactory2(device->DXGIFactoryFlags, IID_PPV_ARGS(device->DXGIFactory.ReleaseAndGetAddressOf())));

        IDXGIFactoryX* dxgiFactory = device->DXGIFactory.Get();
        CheckVariableRefreshRateSupport(dxgiFactory, device->Options);

        ComPtr<IDXGIAdapter1> dxgiAdapter;
        GetAdapter(dxgiFactory, *dxgiAdapter.GetAddressOf(), device->MinFeatureLevel);

        HRESULT hr = D3D12CreateDevice(dxgiAdapter.Get(), device->MinFeatureLevel, IID_PPV_ARGS(device->D3DDevice.ReleaseAndGetAddressOf()));
        ThrowIfFailed(hr);
        device->D3DDevice->SetName(L"D3D12 Device (CLR)");
        ID3D12DeviceX* d3dDevice = device->D3DDevice.Get();

        if (debugLayerEnabled)
        {
            UpdateDebugDeviceConfig(device->D3DDevice);
        }

        device->D3DFeatureLevel = GetMaxSupportedFeatureLevel(d3dDevice, device->MinFeatureLevel);


        // Command Queue creation. It might be better to tigger the code outside of the create device function.
        std::array<CommandListType, size_t(CommandListType::Count)> commandListTypes = { CommandListType::Graphics, CommandListType::Compute, CommandListType::Copy };
        for (auto type : commandListTypes)
        {
            sCommandQueues[size_t(type)] = std::make_unique<CommandQueue>();
            CreateCommandQueue(device, sCommandQueues[size_t(type)].get(), type);
        }

        // ED: Continue

        for (auto type = 0; type < D3D12_DESCRIPTOR_HEAP_TYPE_NUM_TYPES; ++type)
        {
            sDescriptorSizes[type] = device->D3DDevice->GetDescriptorHandleIncrementSize(static_cast<D3D12_DESCRIPTOR_HEAP_TYPE>(type));
            // TODO: convert enum to str
            LOG_INFO("Descriptor Type : {}, Size : {}", type, sDescriptorSizes[type]);
        }


        // TODO: Move it out of CreateDevice function
        sFence = std::make_unique<Fence>();
        ThrowIfFailed(d3dDevice->CreateFence(sFence->Value, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(sFence->D3DFence.ReleaseAndGetAddressOf())));
        sFence->Value++;

        sFenceEvent.Attach(CreateEventEx(nullptr, nullptr, 0, EVENT_MODIFY_STATE | SYNCHRONIZE));
        CLR_ASSERT_MSG(sFenceEvent.IsValid(), "CreateEventEx fails");

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
        descriptorHeapDescRTV.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        ThrowIfFailed(device->D3DDevice->CreateDescriptorHeap(&descriptorHeapDescRTV, IID_PPV_ARGS(display->DescriptorHeapRTV.ReleaseAndGetAddressOf())));

        display->DescriptorHeapRTV->SetName(L"Display Descriptor Heap RTV");

        if (createParams.DepthBufferFormat != SurfaceFormat::UNKNOWN)
        {
            D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDescDSV {};
            descriptorHeapDescDSV.NumDescriptors = 1;
            descriptorHeapDescDSV.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

            ThrowIfFailed(device->D3DDevice->CreateDescriptorHeap(&descriptorHeapDescDSV, IID_PPV_ARGS(display->DescriptorHeapDSV.ReleaseAndGetAddressOf())));
            display->DescriptorHeapDSV->SetName(L"Display Descriptor Heap DSV");
        }

        display->WindowHandle = std::any_cast<HWND>(createParams.WindowHandle);
        if (!display->WindowHandle)
        {
            throw std::logic_error("Set DisplayCreateParameters with a valid window handle");
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

        return fence;
    }

    void DestroyFence(HFence fence)
    {
        delete fence;
    }

    void WaitForGpuToFinish() noexcept
    {
        CommandQueue* graphicsQueue = sCommandQueues[int(CommandListType::Graphics)].get();
        CLR_ASSERT(sFence && graphicsQueue);

        const uint64_t value = sFence->Value;
        if (SUCCEEDED(graphicsQueue->D3DCommandQueue->Signal(sFence->D3DFence.Get(), value)))
        {
            if (SUCCEEDED(sFence->D3DFence->SetEventOnCompletion(value, sFenceEvent.Get())))
            {
                std::ignore = WaitForSingleObjectEx(sFenceEvent.Get(), INFINITE, FALSE);
                sFence->Value++;
            }
        }
    }

    void RegisterDeviceLostCallbackFunc(HDevice device, std::function<void()> func)
    {
        device->DeviceLostCallbackFunc = func;
    }

    void RegisterDeviceRestoredCallbackFunc(HDevice device, std::function<void()> func)
    {
        device->DeviceRestoredCallbackFunc = func;
    }


    // Internal functions

    bool EnableDebugLayer(bool debugLayerEnabled)
    {
#if defined(_DEBUG)
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
                dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true);
                dxgiInfoQueue->SetBreakOnSeverity(DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true);

                // TODO: AddStorageFilterEntries
                return true;
            }
        }
#endif
        return false;
    }

    void UpdateDebugDeviceConfig(ComPtr<ID3D12DeviceX> d3dDevice)
    {
#if defined(_DEBUG)
        ComPtr<ID3D12InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDevice.As(&d3dInfoQueue)))
        {
            d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

            // TODO: AddStorageFilterEntries
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

        if (adapter == nullptr)
        {
            LOG_DEBUG_INFO("WARNING: It is using WARP12");

            // Try WARP12 instead
            if (FAILED(dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(adapter.ReleaseAndGetAddressOf()))))
            {
                throw std::runtime_error("WARP12 not available. Enable the 'Graphics Tools' optional feature");
            }
        }

        if (adapter == nullptr)
        {
            throw std::runtime_error("No Direct3D 12 device found");
        }

        dxgiAdapter = adapter.Detach();
    }

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32& options)
    {
        // Ref: Variable refresh-rate displays https://walbourn.github.io/care-and-feeding-of-modern-swap-chains-3/
        if (options & uint32(Option::AllowTearing))
        {
            // TONOTE: can't use bool because CheckFeatureSupport needs a 32 bits type
            BOOL allowTearing = false;
            HRESULT hr = dxgiFactory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing));
            if (FAILED(hr) || !allowTearing)
            {
                options &= ~(uint32(Option::AllowTearing));
                LOG_DEBUG_INFO("WARNING: Variable refresh rate displays not supported");
            }
        }
    }

    D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel(ID3D12DeviceX* d3dDevice, D3D_FEATURE_LEVEL minFeatureLevel)
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

    DXGI_FORMAT GetInternalFormat(SurfaceFormat format)
    {
        DXGI_FORMAT internal_format = DXGI_FORMAT_UNKNOWN;
        switch (format)
        {
        // Unsigned
        case SurfaceFormat::BGRA8:
            internal_format = DXGI_FORMAT_B8G8R8A8_UNORM;
            break;

        // Depth
        case SurfaceFormat::D16:
            internal_format = DXGI_FORMAT_D16_UNORM;
            break;
        case SurfaceFormat::D24_S8:
            internal_format = DXGI_FORMAT_D24_UNORM_S8_UINT;
            break;
        case SurfaceFormat::D32F:
            internal_format = DXGI_FORMAT_D32_FLOAT;
            break;
        case SurfaceFormat::D32F_S8:
            internal_format = DXGI_FORMAT_D32_FLOAT_S8X24_UINT;
            break;
        }

        CLR_ASSERT_MSG(internal_format != DXGI_FORMAT_UNKNOWN, "Unsupported format!");
        return internal_format;       
    }
}
#pragma once

// From Agility SDK
#include <d3d12.h>

#include <functional>

#include "GraphicsCoreTypes.h"

namespace CLR::Graphics::Core
{
    using Microsoft::WRL::ComPtr;

    using IDXGIFactoryX              = IDXGIFactory6;
    using ID3D12GraphicsCommandListX = ID3D12GraphicsCommandList5;
    using IDXGUISwapChainX           = IDXGISwapChain4;
    using ID3D12FenceX               = ID3D12Fence1;

    static const uint32 sBackBufferCount = 3;


    // Device
    struct Device
    {
		ComPtr<ID3D12Device>    D3DDevice                                   { 0 };

        ComPtr<IDXGIFactoryX>   DXGIFactory                                 { 0 };

        DeviceCreateParameters  CreateParams;

        D3D_FEATURE_LEVEL       MinFeatureLevel                             { D3D_FEATURE_LEVEL_12_0 };

        // Cached device properties
		DWORD                   DXGIFactoryFlags                            { 0 };
        D3D_FEATURE_LEVEL       D3DFeatureLevel                             { MinFeatureLevel };

        uint32                  Options                                     { (uint32)Option::All };

        std::function<void()>   DeviceLostCallbackFunc;
        std::function<void()>   DeviceRestoredCallbackFunc;
    };


    // Display
    struct Display
    {
        ComPtr<IDXGUISwapChainX>        SwapChain           { 0 };
        ComPtr<ID3D12DescriptorHeap>    DescriptorHeapRTV   { 0 };
        ComPtr<ID3D12DescriptorHeap>    DescriptorHeapDSV   { 0 };
        uint32                          DescriptorSize      { 0 };
        DXGI_FORMAT                     BackBufferFormat    { DXGI_FORMAT_UNKNOWN };
        DXGI_FORMAT                     DepthBufferFormat   { DXGI_FORMAT_UNKNOWN };
    };


    // Command
    struct CommandQueue
    {
        ComPtr<ID3D12CommandQueue>  D3DCommandQueue { 0 };
        D3D12_COMMAND_LIST_TYPE     Type            { D3D12_COMMAND_LIST_TYPE_NONE };
    };

    struct CommandList
    {
        ComPtr<ID3D12CommandAllocator>      Allocators[sBackBufferCount]    { 0, 0, 0 };
        // TODO: Check if there is another d3d12 command list type or if it is more beneficial to using the last command list type
        ComPtr<ID3D12GraphicsCommandListX>  List                            { 0 };
        D3D12_COMMAND_LIST_TYPE             Type                            { D3D12_COMMAND_LIST_TYPE_NONE };
    };


    // Fence
    struct Fence
    {
        ComPtr<ID3D12FenceX> D3DFence   { 0 };
        uint64               Value      { 0 };
    };

    // Internal functions
    void EnableDebugLayer(Device* device, bool debugLayerEnabled);

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32& options);
    
    void GetAdapter(IDXGIFactoryX* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, D3D_FEATURE_LEVEL featureLevel, bool highPerf = true);

    D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel(ID3D12Device* d3dDevice, D3D_FEATURE_LEVEL minFeatureLevel);

    D3D12_COMMAND_LIST_TYPE GetInternalCommandListType(CommandListType type);

    template<typename T>
    void SetName(T obj, wchar_t* name)
    {
        obj->SetName(name == nullptr ? L"NULL" : name);
    }

    DXGI_FORMAT GetInternalFormat(SurfaceFormat format);
}

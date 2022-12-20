#pragma once

// From Agility SDK
#include <d3d12.h>

#include "GraphicsCoreTypes.h"

namespace CLR::Graphics::Core
{
    using Microsoft::WRL::ComPtr;

    using IDXGIFactoryX = IDXGIFactory6;

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

        uint32_t                Options                                     { (uint32_t)Option::All };
    };


    // Display
    struct Display
    {
        ComPtr<ID3D12DescriptorHeap>    DescriptorHeapRTV   { 0 };
        ComPtr<ID3D12DescriptorHeap>    DescriptorHeapDSV   { 0 };
        uint32_t                        DescriptorSize      { 0 };
        DXGI_FORMAT                     BackBufferFormat    { DXGI_FORMAT_UNKNOWN };
        DXGI_FORMAT                     DepthBufferFormat   { DXGI_FORMAT_UNKNOWN };
    };


    // Command
    struct CommandQueue
    {
        ComPtr<ID3D12CommandQueue>  D3DCommandQueue{ 0 };
        D3D12_COMMAND_LIST_TYPE     Type{ D3D12_COMMAND_LIST_TYPE_NONE };
    };

    struct CommandList
    {
        ComPtr<ID3D12CommandAllocator>      Allocators[sBackBufferCount]    { 0, 0, 0 };
        // TODO: Check if there is another d3d12 command list type or if it is more beneficial to using the last command list type
        ComPtr<ID3D12GraphicsCommandList>   List                            { 0 };
        D3D12_COMMAND_LIST_TYPE             Type                            { D3D12_COMMAND_LIST_TYPE_NONE };
    };


    // Internal functions
    void EnableDebugLayer(Device* device, bool debugLayerEnabled);

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32_t& options);
    
    void GetAdapter(IDXGIFactoryX* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, D3D_FEATURE_LEVEL featureLevel, bool highPerf = true);

    D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel(ID3D12Device* d3dDevice, D3D_FEATURE_LEVEL minFeatureLevel);

    D3D12_COMMAND_LIST_TYPE GetInternalCommandListType(CommandListType type);
}

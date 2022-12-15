#pragma once

// From Agility SDK
#include <d3d12.h>

#include "GraphicsCoreTypes.h"

namespace CLR::Graphics::Core
{
    using Microsoft::WRL::ComPtr;

    using IDXGIFactoryX = IDXGIFactory6;

    struct Device
    {
		ComPtr<ID3D12Device>    D3DDevice           { nullptr };

        ComPtr<IDXGIFactoryX>   DXGIFactory         { nullptr };

        DeviceCreateParameters  CreateParams;

        D3D_FEATURE_LEVEL       MinFeatureLevel     { D3D_FEATURE_LEVEL_12_0 };

        // Cached device properties
		DWORD                   DXGIFactoryFlags    { 0 };
        D3D_FEATURE_LEVEL       D3DFeatureLevel     { MinFeatureLevel };

        uint32_t                Options             { (uint32_t)Option::All };
    };

    struct CommandQueue
    {
        ComPtr<ID3D12CommandQueue>  D3DCommandQueue     { nullptr };
        D3D12_COMMAND_LIST_TYPE     Type                { D3D12_COMMAND_LIST_TYPE_NONE };
    };

    struct Display
    {
        ComPtr<ID3D12DescriptorHeap>    DescriptorHeapRTV   { nullptr };
        ComPtr<ID3D12DescriptorHeap>    DescriptorHeapDSV   { nullptr };
        uint32_t                        DescriptorSize      { 0 };
        DXGI_FORMAT                     BackBufferFormat    { DXGI_FORMAT_UNKNOWN };
        DXGI_FORMAT                     DepthBufferFormat   { DXGI_FORMAT_UNKNOWN };
    };


    // Internal functions
    void EnableDebugLayer(Device* device, bool debugLayerEnabled);

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32_t& options);
    
    void GetAdapter(IDXGIFactoryX* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, D3D_FEATURE_LEVEL featureLevel, bool highPerf = true);

    D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel(ID3D12Device* d3dDevice, D3D_FEATURE_LEVEL minFeatureLevel);

    D3D12_COMMAND_LIST_TYPE GetInternalCommandQueueType(CommandQueueType type);
}

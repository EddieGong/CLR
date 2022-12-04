#pragma once

// From Agility SDK
#include <d3d12.h>

#include "GraphicsTypes.h"

namespace CLR::Graphics
{
    using IDXGIFactoryX = IDXGIFactory6;

    enum class Option
    {
        AllowTearing    = 1 << 0,
        EnableHDR       = 1 << 1,
        ReverseDepth    = 1 << 2,
        All             = AllowTearing | EnableHDR | ReverseDepth,
    };

    struct Device
    {
		ID3D12Device*           D3DDevice           { nullptr };

        IDXGIFactoryX*          DXGIFactory         { nullptr };

        DeviceCreateParameters  CreateParams;

        D3D_FEATURE_LEVEL       MinFeatureLevel     { D3D_FEATURE_LEVEL_12_0 };

        // Cached device properties
		DWORD                   DXGIFactoryFlags    { 0 };
        D3D_FEATURE_LEVEL       D3DFeatureLevel     { MinFeatureLevel };

        uint32_t                Options             { (uint32_t)Option::All };
    };

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32_t& options);
    
    void GetAdapter(IDXGIFactoryX* dxgiFactory, IDXGIAdapter1*& dxgiAdapter, D3D_FEATURE_LEVEL featureLevel, bool highPerf = true);

    D3D_FEATURE_LEVEL GetMaxSupportedFeatureLevel(ID3D12Device* d3dDevice, D3D_FEATURE_LEVEL minFeatureLevel);
}

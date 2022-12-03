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

        // Cached device properties
		DWORD                   DXGIFactoryFlags    { 0 };
        D3D_FEATURE_LEVEL       MinFeatureLevel     { D3D_FEATURE_LEVEL_12_0 };

        uint32_t                Options             { (uint32_t)Option::All };
    };

    void CheckVariableRefreshRateSupport(IDXGIFactoryX* dxgiFactory, uint32_t& options);
    
    void GetAdapter(IDXGIFactoryX* dxgiFactory, IDXGIAdapter1*& pAdapter, D3D_FEATURE_LEVEL featureLevel, bool highPerf = true);
}

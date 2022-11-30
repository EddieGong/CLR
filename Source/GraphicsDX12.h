#pragma once

// From Agility SDK
#include <d3d12.h>

#include "GraphicsTypes.h"

namespace CLR::Graphics
{
    enum class Option
    {
        AllowTearing    = 1 >> 0,
        EnableHDR       = 1 >> 1,
        ReverseDepth    = 1 >> 2,
    };


    struct Device
    {
		ID3D12Device*           D3DDevice           { nullptr };

        IDXGIFactory6*          DXGIFactory         { nullptr };

        DeviceCreateParameters  CreateParams;

        // Cached device properties
		DWORD                   DXGIFactoryFlags    { 0 };

        uint32_t                Options             { 0 };
    };
}

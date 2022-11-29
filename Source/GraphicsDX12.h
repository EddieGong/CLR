#pragma once

// From Agility SDK
#include <d3d12.h>

#include "GraphicsTypes.h"

namespace CLR::Graphics
{
    struct Device
    {
		ID3D12Device*           D3dDevice           { nullptr };

        DeviceCreateParameters  CreateParams;

        // Cached device properties
		DWORD                   DXGIFactoryFlags    { 0 };
    };
}

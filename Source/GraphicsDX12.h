#pragma once

// From Agility SDK
#include <d3d12.h>

#include "GraphicsTypes.h"

namespace CLR::Graphics
{
    struct Device
    {
        ID3D12Device* mD3dDevice {nullptr};

        DeviceCreateParameters mCreateParams;
    };
}

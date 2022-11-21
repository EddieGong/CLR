#pragma once

#include <winrt/base.h>

// From Agility SDK
#include <d3d12.h>

#include <dxgi1_6.h>

namespace CLR::Graphics
{
    struct Device
    {
        ID3D12Device* mD3dDevice {nullptr};

        DeviceCreateParameters mCreateParams;
    };
}

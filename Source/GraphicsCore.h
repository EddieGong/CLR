#pragma once

#include <Headers.h>

using winrt::com_ptr;

namespace CLR::GraphicsCore
{
    struct Device
    {
        com_ptr<ID3D12Device> device;
    };

    struct DeviceCreateParameters
    {
        bool debugLayerEnabled  { false };
        bool useWarpDevice      { false };
    };

    using HDevice = Device*;
    HDevice CreateDevice(const DeviceCreateParameters& createParams);
}
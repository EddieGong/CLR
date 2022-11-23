#pragma once

namespace CLR::Graphics
{
    struct Device;

    struct DeviceCreateParameters
    {
        bool debugLayerEnabled{ false };
        bool useWarpDevice{ false };
    };

    using HDevice = Device*;
}
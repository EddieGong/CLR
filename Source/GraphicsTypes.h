#pragma once

namespace CLR::Graphics::Core
{
    struct Device;

    struct DeviceCreateParameters
    {
        bool debugLayerEnabled  { false };
        bool useWarpDevice      { false };
    };

    using HDevice = Device*;

    enum class Option
    {
        AllowTearing    = 1 << 0,
        EnableHDR       = 1 << 1,
        ReverseDepth    = 1 << 2,
        All             = AllowTearing | EnableHDR | ReverseDepth,
    };
}
#pragma once

//#include <Headers.h>

//using winrt::com_ptr;

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
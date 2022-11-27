#pragma once

#ifndef CLR_DX12
#define CLR_DX12    1
#endif

#include "GraphicsTypes.h"


// TODO: change it to module!!!

namespace CLR::Graphics
{
    enum class Format
    {

    };

    HDevice CreateDevice(DeviceCreateParameters const& createParams);
    void DestroyDevice(HDevice device);
}
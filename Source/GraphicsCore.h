#pragma once

#ifndef CLR_DX12
#define CLR_DX12    1
#endif

#include "GraphicsTypes.h"

namespace CLR::Graphics
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams);
    void DestroyDevice(HDevice device);
}
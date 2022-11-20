#pragma once

#ifndef CLR_DX12
#define CLR_DX12    1
#endif

#include <GraphicsTypes.h>

namespace CLR::Graphics
{
    HDevice CreateDevice(const DeviceCreateParameters& createParams);
}
export module CLR.Graphics.Core;

import "GraphicsCoreTypes.h";

export namespace CLR::Graphics::Core
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams);
    void DestroyDevice(HDevice device);

    HCommandQueue CreateCommandQueue(HDevice device, CommandQueueType type);
    void DestroyCommandQueue(HCommandQueue queue);

    HDisplay CreateDisplay(HDevice device);
    void DestroyDisplay(HDisplay display);
}

export module CLR.Graphics.Core;

import "GraphicsCoreTypes.h";

export namespace CLR::Graphics::Core
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams);
    void DestroyDevice(HDevice device);

    HCommandQueue CreateCommandQueue(CommandQueueType type);
    void DestroyCommandQueue(HCommandQueue queue);
}

export module CLR.Graphics.Core;

import "GraphicsCoreTypes.h";

export namespace CLR::Graphics::Core
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams);
    void DestroyDevice(HDevice device);

    HDisplay CreateDisplay(HDevice device, DisplayCreateParameters const& createParams);
    void DestroyDisplay(HDisplay display);

    HCommandQueue CreateCommandQueue(HDevice device, CommandListType type);
    void DestroyCommandQueue(HCommandQueue queue);

    HCommandList CreateCommandList(HDevice device, CommandListType type);
    void DestroyCommandList(HCommandList commandList);
}

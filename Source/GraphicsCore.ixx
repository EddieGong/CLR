export module CLR.Graphics.Core;

import "GraphicsCoreTypes.h";

namespace CLR::Graphics::Core
{
    export HDevice CreateDevice(DeviceCreateParameters const& createParams);
    export void DestroyDevice(HDevice device);

    export HDisplay CreateDisplay(HDevice device, DisplayCreateParameters const& createParams);
    export void DestroyDisplay(HDisplay display);

    export HCommandList CreateCommandList(HDevice device, CommandListCreateParameters const& createParams);
    export void DestroyCommandList(HCommandList commandList);

    // TODO: 
    void CreateCommandQueue(HDevice device, HCommandQueue queue, CommandListType type);
    void DestroyCommandQueue(HCommandQueue queue);
}

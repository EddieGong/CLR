export module CLR.Graphics.Core;

import "GraphicsTypes.h";

export namespace CLR::Graphics::Core
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams);

    void DestroyDevice(HDevice device);
}

module;

#include "Headers.h"
#include "GraphicsDX12.h"

module CLR.Graphics.Core;

using Microsoft::WRL::ComPtr;

namespace CLR::Graphics
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {       
#if defined(_DEBUG)
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        //
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        if (createParams.debugLayerEnabled)
        {
            ComPtr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(debugController.GetAddressOf()))))
            {
            }
        }
#endif

        Device* device = new Device();
        return device;
    }

    void DestroyDevice(HDevice device)
    {
        device;
    }
}
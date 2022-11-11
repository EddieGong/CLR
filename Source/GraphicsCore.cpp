#include "pch.h"

#include <GraphicsCore.h>

// TODO: empty help function
#define ThrowIfFailed(x) x

namespace CLR::GraphicsCore
{
    class HandleAllocator
    {};

    HDevice CreateDevice(const DeviceCreateParameters& createParams)
    {
        uint32_t dxgiFactoryFlags = 0;

#if defined(_DEBUG)
        if (createParams.debugLayerEnabled)
        {
            com_ptr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            {
                debugController->EnableDebugLayer();
                // Enable additional debug layers.
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif
        com_ptr<IDXGIFactory4> factory;
        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

        com_ptr<ID3D12Device> device;
        if (createParams.useWarpDevice)
        {
            com_ptr<IDXGIAdapter> warpAdapter;
            ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
            // TODO: set feature level by a parameter
            ThrowIfFailed(D3D12CreateDevice(warpAdapter.get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
        }
        else
        {
            com_ptr<IDXGIAdapter4> hardwareAdapter;
            //GetHardwareAdapter(factory.Get(), &hardwareAdapter);

            ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device)));
        }


        return HDevice();
    }
}
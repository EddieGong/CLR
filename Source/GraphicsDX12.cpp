#include <GraphicsTypes.h>

#include <GraphicsDX12.h>

#define CLR_NEW new
#define CLR_DELETE delete

#define ThrowIfFailed(func) func


namespace CLR::Graphics
{
    HDevice CreateDevice(DeviceCreateParameters const& createParams)
    {
        UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
        // Enable the debug layer (requires the Graphics Tools "optional feature").
        // NOTE: Enabling the debug layer after device creation will invalidate the active device.
        if (createParams.debugLayerEnabled)
        {
            winrt::com_ptr<ID3D12Debug> debugController;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
            {
                debugController->EnableDebugLayer();

                // Enable additional debug layers.
                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
            }
        }
#endif

        winrt::com_ptr<IDXGIFactory4> factory;
        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));

        winrt::com_ptr<ID3D12Device> d3dDevice;
        if (createParams.useWarpDevice)
        {
            winrt::com_ptr<IDXGIAdapter> warpAdapter;
            ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

            ThrowIfFailed(D3D12CreateDevice(warpAdapter.get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3dDevice)));
        }
        else
        {
            winrt::com_ptr<IDXGIAdapter1> hardwareAdapter;
            //GetHardwareAdapter(factory.get(), &hardwareAdapter);

            ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&d3dDevice)));
        }


        Device* device = CLR_NEW Device();
        device->mD3dDevice = d3dDevice.detach();
        return device;
    }

    void DestroyDevice(HDevice device)
    {
        CLR_DELETE(device);
    }
}
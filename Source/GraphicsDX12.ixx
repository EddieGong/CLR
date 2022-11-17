export module GraphicsDX12;

//#include <GraphicsCore.h>;
//import <windows.h>;
//import <winrt/Windows.Foundation.h>;

// From Agility SDK
import <d3d12.h>;

import <dxgi1_6.h>;

import <GraphicsTypes.h>;

//using winrt::com_ptr;

namespace CLR::Graphics
{

    struct Device
    {
        //com_ptr<ID3D12Device> device;
        ID3D12Device* device;
    };

    HDevice CreateDevice(const DeviceCreateParameters& createParams)
    {
//        uint32_t dxgiFactoryFlags = 0;
//
//#if defined(_DEBUG)
//        if (createParams.debugLayerEnabled)
//        {
//            com_ptr<ID3D12Debug> debugController;
//            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
//            {
//                debugController->EnableDebugLayer();
//                // Enable additional debug layers.
//                dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
//            }
//        }
//#endif
//        com_ptr<IDXGIFactory4> factory;
//        ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory)));
//
//        com_ptr<ID3D12Device> device;
//        if (createParams.useWarpDevice)
//        {
//            com_ptr<IDXGIAdapter> warpAdapter;
//            ThrowIfFailed(factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));
//            // TODO: set feature level by a parameter
//            ThrowIfFailed(D3D12CreateDevice(warpAdapter.get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device)));
//        }
//        else
//        {
//            com_ptr<IDXGIAdapter4> hardwareAdapter;
//            //GetHardwareAdapter(factory.Get(), &hardwareAdapter);
//            IDXGIFactory2* factory1 = factory.get();
//
//            com_ptr<IDXGIFactory6> factory6;
//            if (SUCCEEDED(factory1->QueryInterface(IID_PPV_ARGS(&factory6))))
//            {
//
//            }
//
//            ThrowIfFailed(D3D12CreateDevice(hardwareAdapter.get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&device)));
//        }
//
//
        return HDevice();
    }
}
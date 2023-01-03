#pragma once

#include <any>
#include "BasicTypes.h"

namespace CLR::Graphics::Core
{
    // Misc 
    enum class Option
    {
        AllowTearing    = 1 << 0,
        EnableHDR       = 1 << 1,
        ReverseDepth    = 1 << 2,
        All             = AllowTearing | EnableHDR | ReverseDepth,
    };

    enum class CommandListType
    {
        Graphics,
        Compute,
        Copy,
        Count
    };

    enum class SurfaceFormat
    {
        UNKNOWN = 0,

        // Unsigned
        BGRA8,

        // Depth
        D16,
        D24_S8,
        D32F,
        D32F_S8,
    };


    // Device
    struct DeviceCreateParameters
    {
        bool DebugLayerEnabled  { false };
        bool UseWarpDevice      { false };
    };

    struct Device;
    using HDevice = Device*;


    // Command
    struct CommandListCreateParameters
    {
        CommandListType Type { CommandListType::Count };
        wchar_t*        Name { nullptr };
    };

    struct CommandQueue;
    using HCommandQueue = CommandQueue*;

    struct CommandList;
    using HCommandList = CommandList*;

    // Display
    struct DisplayCreateParameters
    {
        SurfaceFormat BackBufferFormat  { SurfaceFormat::UNKNOWN };
        SurfaceFormat DepthBufferFormat { SurfaceFormat::UNKNOWN };

        std::any WindowHandle;
    };

    struct Display;
    using HDisplay = Display*;

    // Fence
    struct Fence;
    using HFence = Fence*;
}
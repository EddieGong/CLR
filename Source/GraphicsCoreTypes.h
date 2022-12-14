#pragma once

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

    enum class CommandQueueType
    {
        Graphics,
        Compute,
        Copy
    };

    enum class SurfaceFormat
    {

    };

    // Device
    struct DeviceCreateParameters
    {
        bool DebugLayerEnabled  { false };
        bool UseWarpDevice      { false };
    };

    struct Device;
    using HDevice = Device*;

    // Command Queue
    struct CommandQueue;
    using HCommandQueue = CommandQueue*;

    // Display
    struct DisplayCreateParameters
    {
        SurfaceFormat BackBufferFormat;
        SurfaceFormat DepthBufferFormat;
    };

    struct Display;
    using HDisplay = Display*;
}
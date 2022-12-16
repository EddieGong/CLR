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
        Copy,
        Count
    };

    enum class SurfaceFormat
    {
        UNKNOWN = 0
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
    struct CommandQueue;
    using HCommandQueue = CommandQueue*;

    struct CommandList;
    using HCommandList = CommandList*;


    // Display
    struct DisplayCreateParameters
    {
        SurfaceFormat BackBufferFormat { SurfaceFormat::UNKNOWN };
        SurfaceFormat DepthBufferFormat{ SurfaceFormat::UNKNOWN };
    };

    struct Display;
    using HDisplay = Display*;
}
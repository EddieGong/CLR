#pragma once

#include <Headers.h>
#include <GraphicsTypes.h>

namespace CLR
{
    class Renderer
    {
    public:

        Renderer();
        ~Renderer();

        void Create();

    private:
        Graphics::HDevice device { nullptr };
    };
}
export module CLR.Settings;

#include "BasicTypes.h"

namespace CLR::Settings
{
    export class Display
    {
    public:
        Display() = default;

        float GetWidth() const              { return mResolution.Width; }
        float GetHeight() const             { return mResolution.Height; }

        void SetFullScreen(bool enabled)    { mFullScreen = enabled; }
        bool IsFullScreen() const           { return mFullScreen; }

    private:
        Size mResolution{ 2560.f, 1440.f };

        bool mFullScreen{ false };
    };

    export namespace Debug
    {
        bool AllowAssert    { true };
    };
}
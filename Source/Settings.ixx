export module Settings;

#include "BasicTypes.h"

namespace CLR
{
    export class DisplaySettings
    {
    public:
        DisplaySettings() = default;

        float GetWidth() const              { return mResolution.Width; }
        float GetHeight() const             { return mResolution.Height; }

        void SetFullScreen(bool enabled)    { mFullScreen = enabled; }
        bool IsFullScreen() const           { return mFullScreen; }

    private:
        Size mResolution{ 2560.f, 1440.f };

        bool mFullScreen{ false };
    };
}
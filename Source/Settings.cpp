module;

#include <fstream>
#include "inipp.h"

module CLR.Settings;

namespace CLR::Settings
{
    Display::Display()
    {
        inipp::Ini<char> ini;
        std::ifstream is("../Resource/Configs/Graphics.ini");
        ini.parse(is);
        ini.default_section(ini.sections["Default"]);
        ini.interpolate();
        
        int width{ 0 };
        inipp::get_value(ini.sections["Default"], "DisplayResolutionWidth", width);
        int height{ 0 };
        inipp::get_value(ini.sections["Default"], "DisplayResolutionHeight", height);

        mResolution.Width  = static_cast<float>(width);
        mResolution.Height = static_cast<float>(height);
    }
}
export module CLR.CmdLineArg;

import <string_view>;
import <unordered_map>;

export namespace CLR::CmdLineArg
{
    export void Init(int argc, wchar_t** argv);

    extern std::unordered_map<std::wstring_view, std::wstring_view> sArgumentMap;

    template<typename T>
    T GetValue(wchar_t const* key)
    {
        if (sArgumentMap.contains(key))
        {
            return static_cast<T>(0);
        }
        else
        {
            return static_cast<T>(0);
        }
    }
}


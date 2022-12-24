export module CLR.CmdLineArg;

import <cassert>;
import <string>;
import <string_view>;
import <unordered_map>;

export namespace CLR::CmdLineArg
{
    extern std::unordered_map<std::wstring_view, std::wstring_view> sArgumentMap;

    void Init(int argc, wchar_t** argv);

    template<typename T>
    bool GetValue(wchar_t const* key, T &value)
    {
        if (sArgumentMap.contains(key))
        {
            wchar_t const* data = sArgumentMap[key].data();
            if constexpr (std::is_same_v<T, int>)
            {
                value = std::stoi(data);
            }
            else if (std::is_same_v<T, float>)
            {
                value = std::stof(data);
            }
            else if (std::is_same_v<T, bool>)
            {
                std::wstring_view sv_true{ L"true" };
                value = (sArgumentMap[key].compare(sv_true) == 0) ? true : false;
            }
            else if (std::is_same_v<T, std::wstring_view>)
            {
                //value = sArgumentMap[key];
            }
            else
            {
                assert(false && "The value's data type is not supported");
            }

            return true;
        }
        else
        {
            return false;
        }
    }
}


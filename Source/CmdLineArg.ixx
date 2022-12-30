module;

#include "CLRAssert.h"

export module CLR.CmdLineArg;

import <string>;
import <string_view>;
import <unordered_map>;

namespace CLR::CmdLineArg
{
    std::unordered_map<std::wstring_view, std::wstring_view> sArgumentMap;

    export void Init(int argc, wchar_t** argv);

    export template<typename T>
    bool GetValue(wchar_t const* key, T &value)
    {
        if (sArgumentMap.contains(key))
        {
            bool valid = true;

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
                CLR_ASSERT_MSG(false, "Not enabled. Waiting for a real case for testing");
                valid = false;
            }
            else
            {
                CLR_ASSERT_MSG(false, "The value's data type is not supported");
                valid = false;
            }

            return valid;
        }
            
        return false;
    }
}


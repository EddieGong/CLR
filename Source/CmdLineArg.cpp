module CLR.CmdLineArg;

#include "CLRAssert.h"

namespace CLR::CmdLineArg
{
    using namespace std;

    unordered_map<wstring_view, wstring_view> sArgumentMap;

    void Init(int argc, wchar_t** argv)
    {
        if (argc > 1)
        {
            for (int i = 1; i < argc; ++i)
            {
                wstring_view cmdLineArg{ argv[i] };
                if (cmdLineArg[0] == L'-')
                {
                    const size_t koffset = 1;
                    const size_t voffset = cmdLineArg.find(L"=");
                    //ASSERT(voffset != std::npos);
                    wstring_view key   = cmdLineArg.substr(koffset,     voffset - koffset);
                    wstring_view value = cmdLineArg.substr(voffset + 1);
                    sArgumentMap[key] = value;
                }
            }
        }

    }
}

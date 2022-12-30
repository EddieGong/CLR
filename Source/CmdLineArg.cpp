module;

#include "CLRAssert.h"

module CLR.CmdLineArg;

namespace CLR::CmdLineArg
{
    using namespace std;

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
                    CLR_ASSERT_MSG(voffset != wstring_view::npos, "key=value, symbol '=' is missing");
                    wstring_view key   = cmdLineArg.substr(koffset,     voffset - koffset);
                    wstring_view value = cmdLineArg.substr(voffset + 1);
                    sArgumentMap[key] = value;
                }
            }
        }
    }
}

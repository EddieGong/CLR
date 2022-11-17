#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers.
#endif 

#include <unknwn.h>

#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h> // TOREMOVE
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>
#include <winrt/Windows.UI.Composition.h> // TOREMOVE
#include <winrt/Windows.UI.Input.h>
#include <winrt/Windows.Graphics.Display.h> // TOREMOVE

// From Agility SDK
#include <d3d12.h>

#include <dxgi1_6.h>

#include <Types.h>

#pragma warning(default : 4061 4062 4191 4242 4263 4264 4265 4266 4289 4365 4746 4826 4841 4986 4987 5029 5038 5042)

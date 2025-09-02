/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Precompiled Header File                       * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#define NOMINMAX
#define STRICT
#define WIN32_LEAN_AND_MEAN


// * System headers * //
#include <Windows.h> // WIN32_LEAN_AND_MEAN must precede Windows.h
#include <commdlg.h>
#include <mmsystem.h> // When WIN32_LEAN_AND_MEAN macro is used, mmsystem.h is needed for timeGetTime in pfc/timers.h
#include <ShObjIdl.h>

// * Standard C++ libraries * //
#include <algorithm>
#include <comdef.h>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

// * External libraries * //
#include <dwmapi.h>
#include <gdiplus.h>
#include <MinHook.h>

// * foobar2000 SDK * //
#ifdef __cplusplus
#include <helpers/foobar2000+atl.h>
#endif
#include <helpers/advconfig_impl.h>
#include <helpers/atl-misc.h>
#include <helpers/window_placement_helper.h>
#include <SDK/cfg_var.h>
#include <SDK/coreDarkMode.h>

// * macOS-specific includes * //
#ifdef __OBJC__
#include <Cocoa/Cocoa.h>
#endif

// * Linking directives //
#pragma comment(lib, "Comdlg32.lib")
#pragma comment(lib, "dwmapi.lib")
#pragma comment(lib, "Gdiplus.lib")

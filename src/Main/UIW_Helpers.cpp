/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Helpers Source File                           * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.3                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    08-11-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "resource.h"
#include "UIW_Helpers.h"
#include "UIW_Settings.h"
#include "UIW.h"


/////////////////////
// * COM HELPERS * //
/////////////////////
#pragma region COM Helpers
namespace UIWHCOM {
	HRESULT LogError(HRESULT errorCode, const std::wstring& source, const std::wstring& description, bool setErrorInfo) {
		pfc::string8 logMessage;

		logMessage << pfc::stringcvt::string_utf8_from_wide(source.c_str())
			<< ": " << pfc::stringcvt::string_utf8_from_wide(description.c_str())
			<< ", HRESULT: 0x" << pfc::format_hex(errorCode);

		FB2K_console_formatter() << logMessage;

		if (setErrorInfo && FAILED(errorCode)) {
			CComPtr<ICreateErrorInfo> createErrorInfo;

			if (SUCCEEDED(CreateErrorInfo(&createErrorInfo))) {
				createErrorInfo->SetSource(CComBSTR(source.c_str()));
				createErrorInfo->SetDescription(CComBSTR(description.c_str()));

				CComPtr<IErrorInfo> errorInfo;
				if (SUCCEEDED(createErrorInfo->QueryInterface(IID_IErrorInfo, reinterpret_cast<void**>(&errorInfo)))) {
					SetErrorInfo(0, errorInfo);
				}
			}
		}

		return errorCode;
	}

	void CreateCallback(VARIANT& targetCallback, const VARIANT* newCallback, const char* callbackName) {
		if (!newCallback) {
			FB2K_console_formatter() << "Audio Wizard => AudioWizardMain: Set" << callbackName << "Callback failed, callback is null";
			return;
		}

		if (targetCallback.vt == VT_DISPATCH && targetCallback.pdispVal != nullptr) {
			targetCallback.pdispVal->Release();
		}

		HRESULT hr = VariantCopy(&targetCallback, newCallback);
		if (FAILED(hr)) {
			FB2K_console_formatter() << "Audio Wizard => AudioWizardMain: VariantCopy failed for " << callbackName << " callback, HRESULT=" << hr;
			return;
		}

		if (targetCallback.vt == VT_DISPATCH && targetCallback.pdispVal != nullptr) {
			targetCallback.pdispVal->AddRef();
		}
		else {
			FB2K_console_formatter() << "Audio Wizard => AudioWizardMain: " << callbackName << " callback is invalid or empty";
		}
	}

	void FireCallback(const VARIANT& callback, const std::function<void()>& postAction) {
		if (callback.vt == VT_DISPATCH && callback.pdispVal != nullptr) {
			fb2k::inMainThread([callback, postAction] {
				DISPPARAMS params = { nullptr, nullptr, 0, 0 };
				HRESULT hr = callback.pdispVal->Invoke(DISPID_VALUE, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &params, nullptr, nullptr, nullptr);

				if (FAILED(hr)) {
					FB2K_console_formatter() << "Audio Wizard => ExecuteCallback: Callback invocation failed, HRESULT: " << hr;
				}

				if (postAction) postAction();
				});
		}
		else if (postAction) {
			fb2k::inMainThread(postAction);
		}
	}

	HRESULT GetOptionalLong(const VARIANT* variant, LONG& output) {
		output = 0;

		if (variant != nullptr) {
			if (variant->vt == VT_I4) {
				output = variant->lVal;
			}
			else if (variant->vt != VT_ERROR || variant->scode != DISP_E_PARAMNOTFOUND) {
				FB2K_console_formatter() << "Audio Wizard => GetOptionalLong: Invalid variant type or error code";
				return E_INVALIDARG;
			}
		}

		return S_OK;
	}

	SafeArrayAccess::SafeArrayAccess(SAFEARRAY* inputPsa) : psa(inputPsa) {
		if (!psa) {
			hr = E_INVALIDARG;
			FB2K_console_formatter() << "Audio Wizard => SafeArrayAccess: Invalid SAFEARRAY";
			return;
		}

		hr = SafeArrayAccessData(psa, reinterpret_cast<void**>(&data));

		if (FAILED(hr)) {
			FB2K_console_formatter() << "Audio Wizard => SafeArrayAccessData failed with HRESULT: " << hr;
			data = nullptr;
		}
	}

	SafeArrayAccess::~SafeArrayAccess() {
		if (data && psa) SafeArrayUnaccessData(psa);
	}

	SAFEARRAY* CreateSafeArrayFromData(size_t size, float defaultValue, const char* context) {
		if (static_cast<ULONG>(size) > std::numeric_limits<ULONG>::max()) {
			FB2K_console_formatter() << "Audio Wizard => " << context << ": Invalid size (" << size << ")";
			return nullptr;
		}

		SAFEARRAY* psa = SafeArrayCreateVector(VT_R4, 0, static_cast<ULONG>(size));
		if (!psa) {
			FB2K_console_formatter() << "Audio Wizard => " << context << ": Failed to create SAFEARRAY";
			return nullptr;
		}

		SafeArrayAccess access(psa);
		if (FAILED(access.getHr())) {
			SafeArrayDestroy(psa);
			FB2K_console_formatter() << "Audio Wizard => " << context << ": SafeArrayAccessData failed: " << access.getHr();
			return nullptr;
		}

		auto* pData = access.getData();
		std::fill(pData, pData + size, defaultValue);

		return psa;
	}
}
#pragma endregion


////////////////////////////
// * CONVERSION HELPERS * //
////////////////////////////
#pragma region Conversion Helpers
namespace UIWHConvert {
	bool BOOLFromVARIANT(const VARIANT& var, bool defaultValue) {
		return (var.vt == VT_BOOL) ? (var.boolVal == VARIANT_TRUE) : defaultValue;
	}

	long INTFromVARIANT(const VARIANT& var, long defaultValue) {
		return (var.vt == VT_I4) ? var.intVal : defaultValue;
	}

	std::wstring STRINGFromVARIANT(const VARIANT& var, const std::wstring& defaultValue) {
		return (var.vt == VT_BSTR) ? std::wstring(var.bstrVal) : defaultValue;
	}

	double DOUBLEFromVARIANT(const VARIANT& var, double defaultValue) {
		return (var.vt == VT_R8) ? var.dblVal : defaultValue;
	}

	CSize DialogUnitsToPixel(HWND hWnd, int dluX, int dluY) {
		CRect rect(0, 0, dluX, dluY);
		::MapDialogRect(hWnd, &rect);
		return CSize(rect.Width(), rect.Height());
	}

	int PercentToPixels(double percent, int dimension) {
		return static_cast<int>(percent * dimension);
	}

	double PixelsToPercent(int pixels, int dimension) {
		return (dimension > 0) ? (static_cast<double>(pixels) / dimension) : 0.0;
	}
}
#pragma endregion


///////////////////////////
// * DARK MODE HELPERS * //
///////////////////////////
#pragma region Dark Mode Helpers
namespace UIWHDarkMode {
	namespace {
		std::shared_ptr<fb2k::CCoreDarkModeHooks>& GetHooks() {
			static std::shared_ptr<fb2k::CCoreDarkModeHooks> hooks;
			hooks = std::make_shared<fb2k::CCoreDarkModeHooks>();
			return hooks;
		}
		bool isDark = false;
	}

	void AddControls(HWND wnd) {
		auto hooks = GetHooks();
		if (hooks) hooks->AddControls(wnd);
	}

	void AddControlsWithExclude(HWND parent, HWND exclude) {
		auto hooks = GetHooks();
		if (!hooks) return;

		auto hWndChild = GetWindow(parent, GW_CHILD);
		while (hWndChild) {
			if (hWndChild != exclude) {
				AddCtrlAuto(hWndChild);
			}
			hWndChild = ::GetNextWindow(hWndChild, GW_HWNDNEXT);
		}
	}

	void AddCtrlAuto(HWND wnd) {
		auto hooks = GetHooks();
		if (hooks) hooks->AddCtrlAuto(wnd);
	}

	void AddDialog(HWND wnd) {
		auto hooks = GetHooks();
		if (hooks) hooks->AddDialog(wnd);
	}

	void AddDialogWithControls(HWND wnd) {
		auto hooks = GetHooks();
		if (hooks) hooks->AddDialogWithControls(wnd);
	}

	void Cleanup() {
		GetHooks().reset();
	}

	bool IsDark() {
		auto hooks = GetHooks();
		if (!hooks) return isDark;

		isDark = hooks->operator bool();
		return isDark;
	}

	void SetDark(bool dark) {
		auto hooks = GetHooks();

		if (hooks) {
			hooks->SetDark(dark);
			isDark = dark;
		}
	}
}
#pragma endregion


////////////////////////
// * DIALOG HELPERS * //
////////////////////////
#pragma region Dialog Helpers
namespace UIWHDialog {
	bool CreateCustomFont(HWND hWnd, CFont& font, int pointSize, int weight, const TCHAR* faceName) {
		LOGFONT lf = { 0 };

		CSize fontSize = UIWHConvert::DialogUnitsToPixel(hWnd, 0, pointSize);
		lf.lfHeight = -fontSize.cy;
		lf.lfWeight = weight;
		_tcscpy_s(lf.lfFaceName, LF_FACESIZE, faceName);

		if (!font.CreateFontIndirect(&lf)) { // Fallback to system font
			_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("MS Shell Dlg"));

			fontSize = UIWHConvert::DialogUnitsToPixel(hWnd, 0, 8);
			lf.lfHeight = -fontSize.cy;
			lf.lfWeight = FW_NORMAL;

			return font.CreateFontIndirect(&lf);
		}

		return true;
	}

	bool GetCheckBoxState(HWND hWnd, int id) {
		return IsDlgButtonChecked(hWnd, id) == BST_CHECKED;
	}

	void SetCheckBox(HWND hWnd, int id, bool checked) {
		CheckDlgButton(hWnd, id, checked ? BST_CHECKED : BST_UNCHECKED);
	}

	void SetControlEnableState(HWND hWnd, const std::vector<int>& controlIDs, bool enabled) {
		for (int id : controlIDs) {
			::EnableWindow(GetDlgItem(hWnd, id), enabled);
		}
	}

	int GetDropDownIndex(HWND hWnd, int id) {
		LRESULT selectedIndex = SendDlgItemMessage(hWnd, id, CB_GETCURSEL, 0, 0);
		return selectedIndex == CB_ERR ? -1 : static_cast<int>(selectedIndex);
	}

	void SetDropDownMenu(HWND hWnd, int id, const std::vector<std::wstring>& items, int selectedItem) {
		SendDlgItemMessage(hWnd, id, CB_RESETCONTENT, 0, 0);

		for (const auto& item : items) {
			SendDlgItemMessage(hWnd, id, CB_ADDSTRING, 0, (LPARAM)item.c_str());
		}

		SendDlgItemMessage(hWnd, id, CB_SETCURSEL, (WPARAM)selectedItem, 0);
	}

	int GetInputFieldNumber(HWND hWnd, int id, BOOL* pSuccess, BOOL signedValue) {
		return GetDlgItemInt(hWnd, id, pSuccess, signedValue);
	}

	void SetInputFieldNumber(HWND hWnd, int id, int value, BOOL signedValue) {
		SetDlgItemInt(hWnd, id, static_cast<UINT>(value), signedValue);
	}

	pfc::string8 GetInputFieldText(HWND hWnd, int id) {
		int length = ::GetWindowTextLength(GetDlgItem(hWnd, id));
		if (length <= 0) return pfc::string8();

		std::wstring tempStr(static_cast<size_t>(length) + 1, L'\0');
		GetDlgItemTextW(hWnd, id, &tempStr[0], length + 1);
		tempStr.resize(length);

		return pfc::stringcvt::string_utf8_from_wide(tempStr.c_str()).get_ptr();
	}

	void SetInputFieldText(HWND hWnd, int id, const std::wstring& text) {
		SetDlgItemText(hWnd, id, text.c_str());
	}

	void SetSpinControlRange(HWND hWnd, int id, int minVal, int maxVal) {
		SendDlgItemMessage(hWnd, id, UDM_SETRANGE32, static_cast<WPARAM>(minVal), static_cast<LPARAM>(maxVal));
	}

	LRESULT CALLBACK SpinControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
		switch (message) { // Only need to subclass the spin controls because of flicker issues, do double buffering here
			case WM_PAINT: {
				PAINTSTRUCT ps;
				HDC hdc = ::BeginPaint(hWnd, &ps);

				HDC hdcMem = CreateCompatibleDC(hdc);
				HBITMAP hbm = CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
				HBITMAP hOldBmp = (HBITMAP)SelectObject(hdcMem, hbm);

				DefSubclassProc(hWnd, WM_ERASEBKGND, (WPARAM)hdcMem, 0);
				DefSubclassProc(hWnd, WM_PRINTCLIENT, (WPARAM)hdcMem, PRF_CLIENT);

				BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcMem, 0, 0, SRCCOPY);

				SelectObject(hdcMem, hOldBmp);
				DeleteObject(hbm);
				DeleteDC(hdcMem);

				::EndPaint(hWnd, &ps);
				return 0;
			}

			case WM_ERASEBKGND: {
				return 1;
			}

			case WM_NCDESTROY: {
				RemoveWindowSubclass(hWnd, SpinControlProc, uIdSubclass);
				break;
			}

			default: {
				return DefSubclassProc(hWnd, message, wParam, lParam);
			}
		}

		return DefSubclassProc(hWnd, message, wParam, lParam);
	}

	void SpinControlSubclass(HWND hWnd) {
		SetWindowSubclass(hWnd, SpinControlProc, 0, 0);
	}
}
#pragma endregion


/////////////////////////
// * DISPLAY HELPERS * //
/////////////////////////
#pragma region Display Helpers
namespace UIWHDisplay {
	int GetDisplayDPI() {
		HDC screen = GetDC(nullptr);
		if (!screen) return 96;

		int dpi = GetDeviceCaps(screen, LOGPIXELSX);
		ReleaseDC(nullptr, screen);

		return dpi;
	}

	float GetDisplayDPIScaleFactor() {
		HDC hdc = GetDC(nullptr);
		if (!hdc) return 1.0f;

		int dpiX = GetDeviceCaps(hdc, LOGPIXELSX);
		ReleaseDC(nullptr, hdc);

		return static_cast<float>(dpiX) / 96.0f;
	}

	const char* GetDisplayResolutionMode() {
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		if (width >= 7680 && height >= 4320) {
			return "8K";
		}
		else if (width >= 6016 && height >= 3384) {
			return "6K";
		}
		else if (width >= 5120 && height >= 2880) {
			return "5K";
		}
		else if (width >= 3840 && height >= 2160) {
			return "4K";
		}
		else if (width >= 2560 && height >= 1440) {
			return "QHD";
		}
		else {
			return "HD";
		}
	}

	std::string GetDisplayResolution() {
		int width = GetSystemMetrics(SM_CXSCREEN);
		int height = GetSystemMetrics(SM_CYSCREEN);

		std::ostringstream oss;
		oss << R"({"width":)" << width << R"(, "height":)" << height << "}";
		return oss.str();
	}

	std::string GetDisplayResolutionMultiMonitors() {
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();

		if (!GetWindowRect(hDesktop, &desktop)) {
			return R"({"Error": "failed to get display resolution."})";
		}

		int width = desktop.right;
		int height = desktop.bottom;

		std::ostringstream oss;
		oss << R"({"width":)" << width << R"(, "height":)" << height << "}";
		return oss.str();
	}

	MONITORINFO GetMonitorMetrics(HWND hWnd) {
		MONITORINFO mi = { sizeof(MONITORINFO) };
		HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);

		if (!::GetMonitorInfo(hMonitor, &mi)) {
			return {};
		}

		return mi;
	}

	bool IsTaskbarAutoHiding() {
		APPBARDATA appBarData = { sizeof(appBarData) };
		UINT_PTR taskbarState = SHAppBarMessage(ABM_GETSTATE, &appBarData);
		return (taskbarState & ABS_AUTOHIDE) != 0;
	}

	bool IsTaskbarVisible() {
		HWND taskbarWnd = FindWindow(L"Shell_TrayWnd", nullptr);

		if (taskbarWnd) {
			return IsWindowVisible(taskbarWnd) != FALSE;
		}

		return false;
	}
}
#pragma endregion


//////////////////////////
// * GRAPHICS HELPERS * //
//////////////////////////
#pragma region Graphics Helpers
namespace UIWHGraphics {
	HICON GetCustomIcon(const std::string& path) {
		if (path.empty()) {
			return nullptr;
		}

		pfc::stringcvt::string_wide_from_utf8 widePath(path.c_str());

		return static_cast<HICON>(LoadImage(
			nullptr, widePath.get_ptr(), IMAGE_ICON,
			GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON),
			LR_LOADFROMFILE | LR_SHARED
		));
	}

	HICON GetDefaultIcon() {
		DWORD copied = 0;
		DWORD bufferSize = MAX_PATH;
		std::vector<wchar_t> fb2kExePath(bufferSize, L'\0');

		while (true) {
			copied = GetModuleFileNameW(nullptr, fb2kExePath.data(), bufferSize);
			if (copied < bufferSize - 1 || copied == 0) break;
			bufferSize *= 2;
			fb2kExePath.resize(bufferSize);
		}

		if (copied == 0) return nullptr;

		HICON hIcon = nullptr;
		if (ExtractIconExW(fb2kExePath.data(), 0, nullptr, &hIcon, 1) > 0 && hIcon) {
			return hIcon;
		}

		return nullptr;
	}

	void SetIcon(HWND hWndMain, HWND hWndDialog, bool customIcon, const std::string& customIconPath) {
		if (!hWndMain && !hWndDialog) return;

		HICON hIcon = customIcon && !customIconPath.empty() ? GetCustomIcon(customIconPath) : GetDefaultIcon();

		if (hWndMain) {
			SetClassLongPtr(hWndMain, GCLP_HICON, reinterpret_cast<LONG_PTR>(hIcon));
		}

		if (hWndDialog) {
			SendDlgItemMessage(hWndDialog, IDC_CUSTOM_ICON_IMG, STM_SETIMAGE, IMAGE_ICON, (LPARAM)hIcon);
			pfc::stringcvt::string_wide_from_utf8 widePath(customIconPath.c_str());
			SetDlgItemText(hWndDialog, IDC_CUSTOM_ICON_PATH, widePath.get_ptr());
		}
	}

	void WindowRepaint(HWND hWnd) {
		RedrawWindow(hWnd, nullptr, nullptr, RDW_INVALIDATE | RDW_FRAME | RDW_UPDATENOW | RDW_ALLCHILDREN);
	}
}
#pragma endregion


////////////////////////
// * WINDOW HELPERS * //
////////////////////////
#pragma region Window Helpers
namespace UIWHWindow {
	bool IsWindows11() {
		OSVERSIONINFOEXW osvi = { 0 };
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEXW);
		osvi.dwMajorVersion = 10;
		osvi.dwMinorVersion = 0;
		osvi.dwBuildNumber = 22000; // Windows 11 threshold

		DWORDLONG conditionMask = 0;
		VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
		VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
		VER_SET_CONDITION(conditionMask, VER_BUILDNUMBER, VER_GREATER_EQUAL);

		return VerifyVersionInfoW(&osvi, VER_MAJORVERSION | VER_MINORVERSION | VER_BUILDNUMBER, conditionMask) != FALSE;
	}

	bool IsFrameStyle(std::string_view style) {
		int expectedState = 0;

		if (style == "Default") expectedState = 0;
		else if (style == "SmallCaption") expectedState = 1;
		else if (style == "NoCaption") expectedState = 2;
		else if (style == "NoBorder") expectedState = 3;
		else expectedState = 0;

		return UIWizardSettings::frameStyle == expectedState;
	}

	void SetFrameStyle(std::string_view style) {
		if (style == "Default") UIWizardSettings::frameStyle = 0;
		else if (style == "SmallCaption") UIWizardSettings::frameStyle = 1;
		else if (style == "NoCaption") UIWizardSettings::frameStyle = 2;
		else if (style == "NoBorder") UIWizardSettings::frameStyle = 3;
		else UIWizardSettings::frameStyle = 0;
	}

	bool IsAeroEffect(std::string_view effect) {
		int expectedState = 0;

		if (effect == "Default") expectedState = 0;
		else if (effect == "Disabled") expectedState = 1;
		else if (effect == "GlassFrame") expectedState = 2;
		else if (effect == "SheetOfGlass") expectedState = 3;
		else expectedState = 0;

		return UIWizardSettings::aeroEffect == expectedState;
	}

	void SetAeroEffect(std::string_view effect) {
		if (effect == "Default") UIWizardSettings::aeroEffect = 0;
		else if (effect == "Disabled") UIWizardSettings::aeroEffect = 1;
		else if (effect == "GlassFrame") UIWizardSettings::aeroEffect = 2;
		else if (effect == "SheetOfGlass") UIWizardSettings::aeroEffect = 3;
		else UIWizardSettings::aeroEffect = 0;
	}

	bool IsWindowState(std::string_view state) {
		int expectedState = 0;

		if (state == "Normal") expectedState = 0;
		else if (state == "Maximized") expectedState = 1;
		else if (state == "Fullscreen") expectedState = 2;
		else expectedState = 0;

		return UIWizardSettings::windowState == expectedState;
	}

	std::string_view GetWindowState(bool previousState) {
		int state = previousState
			? UIWizardSettings::windowStatePrevious.get_value()
			: UIWizardSettings::windowState.get_value();

		if (state == 0) return "Normal";
		else if (state == 1) return "Maximized";
		else if (state == 2) return "Fullscreen";
		else return "Normal";
	}

	void SetWindowState(std::string_view state) {
		if (state == "Normal") UIWizardSettings::windowState = 0;
		else if (state == "Maximized") UIWizardSettings::windowState = 1;
		else if (state == "Fullscreen") UIWizardSettings::windowState = 2;
		else UIWizardSettings::windowState = 0;
	}

	int GetWindowX(HWND hWnd) {
		RECT rect;
		return GetWindowRect(hWnd, &rect) ? rect.left : -1;
	}

	int GetWindowY(HWND hWnd) {
		RECT rect;
		return GetWindowRect(hWnd, &rect) ? rect.top : -1;
	}

	int GetWindowWidth(HWND hWnd) {
		RECT rect;
		return GetWindowRect(hWnd, &rect) ? rect.right - rect.left : -1;
	}

	int GetWindowHeight(HWND hWnd) {
		RECT rect;
		return GetWindowRect(hWnd, &rect) ? rect.bottom - rect.top : -1;
	}

	int GetWindowTitlebarHeight(HWND hWnd) {
		LONG_PTR style = GetWindowLongPtr(hWnd, GWL_STYLE);
		LONG_PTR exstyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);

		// Get DPI-aware metrics
		int caption_height = 0;
		int frame_top = 0;

		if (IsFrameStyle("Default")) {
			caption_height = GetSystemMetrics(SM_CYCAPTION);  // Standard titlebar
			if (style & WS_THICKFRAME) {
				frame_top = GetSystemMetrics(SM_CYFRAME);  // Resizable top frame
			}
		}
		else if (IsFrameStyle("SmallCaption")) {
			caption_height = GetSystemMetrics(SM_CYSMCAPTION);  // Small titlebar
			if (style & WS_THICKFRAME) {
				frame_top = GetSystemMetrics(SM_CYFRAME);
			}
		}
		else if (IsFrameStyle("NoCaption")) {
			caption_height = 0;  // No titlebar
			if (style & WS_THICKFRAME) {
				frame_top = GetSystemMetrics(SM_CYFRAME);
			}
		}
		else if (IsFrameStyle("NoBorder")) {
			caption_height = 0;
			frame_top = 0;  // No non-client area
		}

		// Total top non-client height (caption + top frame)
		return caption_height + frame_top;
	}

	void SetWindowPosition(HWND hWnd, int x, int y) {
		SetWindowPos(hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void SetWindowPositionInGrid(HWND hWnd, const std::string& position) {
		MONITORINFO mi = UIWHDisplay::GetMonitorMetrics(hWnd);
		RECT rect;
		GetWindowRect(hWnd, &rect);

		int screenWidth = mi.rcWork.right - mi.rcWork.left;
		int screenHeight = mi.rcWork.bottom - mi.rcWork.top;
		int windowWidth = rect.right - rect.left;
		int windowHeight = rect.bottom - rect.top;

		std::map<std::string, std::function<void(int&, int&)>> gridPosition = {
			{"top-left", [&mi](int& x, int& y) {
				x = mi.rcWork.left;
				y = mi.rcWork.top;
			}},
			{"top-center", [&mi, &screenWidth, &windowWidth](int& x, int& y) {
				x = mi.rcWork.left + (screenWidth - windowWidth) / 2;
				y = mi.rcWork.top;
			}},
			{"top-right", [&mi, &windowWidth](int& x, int& y) {
				x = mi.rcWork.right - windowWidth;
				y = mi.rcWork.top;
			}},

			{"center-left", [&mi, &screenHeight, &windowHeight](int& x, int& y) {
				x = mi.rcWork.left;
				y = mi.rcWork.top + (screenHeight - windowHeight) / 2;
			}},
			{"center", [&mi, &screenWidth, &windowWidth, &screenHeight, &windowHeight](int& x, int& y) {
				x = mi.rcWork.left + (screenWidth - windowWidth) / 2;
				y = mi.rcWork.top + (screenHeight - windowHeight) / 2;
			}},
			{"center-right", [&mi, &windowWidth, &screenHeight, &windowHeight](int& x, int& y) {
				x = mi.rcWork.right - windowWidth;
				y = mi.rcWork.top + (screenHeight - windowHeight) / 2;
			}},

			{"bottom-left", [&mi, &windowHeight](int& x, int& y) {
				x = mi.rcWork.left;
				y = mi.rcWork.bottom - windowHeight;
			}},
			{"bottom-center", [&mi, &screenWidth, &windowWidth, &windowHeight](int& x, int& y) {
				x = mi.rcWork.left + (screenWidth - windowWidth) / 2;
				y = mi.rcWork.bottom - windowHeight;
			}},
			{"bottom-right", [&mi, &windowWidth, &windowHeight](int& x, int& y) {
				x = mi.rcWork.right - windowWidth;
				y = mi.rcWork.bottom - windowHeight;
			}}
		};

		int x = 0;
		int y = 0;
		auto it = gridPosition.find(position);

		if (it != gridPosition.end()) {
			it->second(x, y);
		}

		SetWindowPos(hWnd, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void ValidateWindowPosition(HWND hWnd, const std::string& resetPosition, int bufferPx) {
		MONITORINFO mi = UIWHDisplay::GetMonitorMetrics(hWnd);
		RECT windowRect;
		GetWindowRect(hWnd, &windowRect);

		LONG leftBound = mi.rcMonitor.left + bufferPx;
		LONG topBound = mi.rcMonitor.top + bufferPx;
		LONG rightBound = mi.rcMonitor.right - bufferPx;
		LONG bottomBound = mi.rcMonitor.bottom - bufferPx;

		if (windowRect.left < leftBound || windowRect.top < topBound ||
			windowRect.right > rightBound || windowRect.bottom > bottomBound) {
			UIWHWindow::SetWindowPositionInGrid(hWnd, resetPosition);
		}
	}

	void SetWindowSize(HWND hWnd, int width, int height) {
		SetWindowPos(hWnd, nullptr, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);
	}

	void ValidateWindowSize(HWND hWnd) {
		RECT rect;
		GetWindowRect(hWnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		if (width < 1 || height < 1) {
			MONITORINFO mi = UIWHDisplay::GetMonitorMetrics(hWnd);
			int screenWidth = mi.rcWork.right - mi.rcWork.left;
			int screenHeight = mi.rcWork.bottom - mi.rcWork.top;

			auto newWidth = static_cast<int>(screenWidth * 0.66);
			auto newHeight = static_cast<int>(screenHeight * 0.66);

			UIWHWindow::SetWindowSize(hWnd, newWidth, newHeight);
		}
	}

	void ValidateWindowSizeConstraints(cfg_int& minWidth, cfg_int& minHeight, cfg_int& maxWidth, cfg_int& maxHeight) {
		// Validate and correct the settings for minimum dimensions
		auto windowMinWidth = static_cast<int>(minWidth);
		auto windowMinHeight = static_cast<int>(minHeight);
		minWidth  = std::max(windowMinWidth, 1);
		minHeight = std::max(windowMinHeight, 1);

		// Validate and correct the settings for maximum dimensions
		auto windowMaxWidth  = static_cast<int>(maxWidth)  > static_cast<int>(minWidth)  ? static_cast<int>(maxWidth)  : GetSystemMetrics(SM_CXSCREEN);
		auto windowMaxHeight = static_cast<int>(maxHeight) > static_cast<int>(minHeight) ? static_cast<int>(maxHeight) : GetSystemMetrics(SM_CYSCREEN);
		maxWidth  = std::max(windowMaxWidth, static_cast<int>(minWidth));
		maxHeight = std::max(windowMaxHeight, static_cast<int>(minHeight));
	}

	void SetWindowTransparency(HWND hWnd, int transparency, bool enableTransparency) {
		int currentAlpha = 255;
		static std::optional<int> lastAlpha;

		if (enableTransparency) {
			currentAlpha = std::clamp(transparency, 0, 255);
		}

		if (!enableTransparency && lastAlpha == 255 || lastAlpha == currentAlpha) {
			return;
		}

		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, 0, static_cast<BYTE>(currentAlpha), LWA_ALPHA);

		lastAlpha = currentAlpha;
	}
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Helpers Header File                           * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once


/////////////////////
// * COM HELPERS * //
/////////////////////
#pragma region COM Helpers
namespace UIWHCOM {
	HRESULT LogError(HRESULT errorCode, const std::wstring& source, const std::wstring& description, bool setErrorInfo = false);

	void CreateCallback(VARIANT& targetCallback, const VARIANT* newCallback, const char* callbackName);
	void FireCallback(const VARIANT& callback, const std::function<void()>& postAction = nullptr);

	HRESULT GetOptionalLong(const VARIANT* variant, LONG& output);

	class SafeArrayAccess {
	public:
		explicit SafeArrayAccess(SAFEARRAY* psa);
		~SafeArrayAccess();
		float* getData() const { return data; }
		HRESULT getHr() const { return hr; }

	private:
		SAFEARRAY* psa = nullptr;
		float* data = nullptr;
		HRESULT hr = S_OK;
		SafeArrayAccess(const SafeArrayAccess&) = delete;
		SafeArrayAccess& operator=(const SafeArrayAccess&) = delete;
	};

	template<typename InputIt>
	SAFEARRAY* CreateSafeArrayFromData(InputIt begin, InputIt end, const char* context = "CreateSafeArrayFromData") {
		using value_type = typename std::iterator_traits<InputIt>::value_type;
		static_assert(std::is_arithmetic_v<value_type>, "Input iterator must yield arithmetic values");

		auto size = std::distance(begin, end);
		if (size < 0 || static_cast<ULONG>(size) > std::numeric_limits<ULONG>::max()) {
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
		for (auto it = begin; it != end; ++it) {
			*pData++ = static_cast<float>(*it);
		}

		return psa;
	}

	SAFEARRAY* CreateSafeArrayFromData(size_t size, float defaultValue, const char* context = "CreateSafeArrayFromData");
}
#pragma endregion


////////////////////////////
// * CONVERSION HELPERS * //
////////////////////////////
#pragma region Conversion Helpers
namespace UIWHConvert {
	bool BOOLFromVARIANT(const VARIANT& var, bool defaultValue);
	double DOUBLEFromVARIANT(const VARIANT& var, double defaultValue);
	long INTFromVARIANT(const VARIANT& var, long defaultValue);
	std::wstring STRINGFromVARIANT(const VARIANT & var, const std::wstring & defaultValue = L"");
}
#pragma endregion


///////////////////////////
// * DARK MODE HELPERS * //
///////////////////////////
#pragma region Dark Mode Helpers
namespace UIWHDarkMode {
	void AddControls(HWND wnd);
	void AddControlsWithExclude(HWND parent, HWND exclude);
	void AddCtrlAuto(HWND wnd);
	void AddDialog(HWND wnd);
	void AddDialogWithControls(HWND wnd);
	void Cleanup();
	bool IsDark();
	void SetDark(bool dark);
}
#pragma endregion


////////////////////////
// * DIALOG HELPERS * //
////////////////////////
#pragma region Dialog Helpers
namespace UIWHDialog {
	bool CreateCustomFont(CFont& font, int height, int weight, const TCHAR* faceName = _T("Tahoma"));

	bool GetCheckBoxState(HWND hWnd, int id);
	void SetCheckBox(HWND hWnd, int id, bool checked);

	void SetControlEnableState(HWND hWnd, const std::vector<int>& controlIDs, bool enabled);

	int GetDropDownIndex(HWND hWnd, int id);
	void SetDropDownMenu(HWND hWnd, int id, const std::vector<std::wstring>& items, int selectedItem);

	int GetInputFieldNumber(HWND hWnd, int id, BOOL* pSuccess = nullptr, BOOL signedValue = false);
	void SetInputFieldNumber(HWND hWnd, int id, int value, BOOL signedValue = false);

	pfc::string8 GetInputFieldText(HWND hWnd, int id);
	void SetInputFieldText(HWND hWnd, int id, const std::wstring& text);

	void SetSpinControlRange(HWND hWnd, int id, int minVal, int maxVal);
	LRESULT CALLBACK SpinControlProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData);
	void SpinControlSubclass(HWND hWnd);
};
#pragma endregion


/////////////////////////
// * DISPLAY HELPERS * //
/////////////////////////
#pragma region Display Helpers
namespace UIWHDisplay {
	int GetDisplayDPI();
	float GetDisplayDPIScaleFactor();

	const char* GetDisplayResolutionMode();
	std::string GetDisplayResolution();
	std::string GetDisplayResolutionMultiMonitors();

	MONITORINFO GetMonitorMetrics(HWND hWnd);

	bool IsTaskbarAutoHiding();
	bool IsTaskbarVisible();
};
#pragma endregion


//////////////////////////
// * GRAPHICS HELPERS * //
//////////////////////////
#pragma region Graphics Helpers
namespace UIWHGraphics {
	HICON GetCustomIcon(const std::string& path);
	HICON GetDefaultIcon();
	void SetIcon(HWND hWndMain, HWND hWndDialog, bool customIcon, const std::string& customIconPath);
	void WindowRepaint(HWND hWnd);
};
#pragma endregion

////////////////////////
// * WINDOW HELPERS * //
////////////////////////
#pragma region Window Helpers
namespace UIWHWindow {
	int GetWindowX(HWND hWnd);
	int GetWindowY(HWND hWnd);
	int GetWindowWidth(HWND hWnd);
	int GetWindowHeight(HWND hWnd);

	void SetWindowPosition(HWND hWnd, int x, int y);
	void SetWindowPositionInGrid(HWND hWnd, const std::string& position);
	void ValidateWindowPosition(HWND hWnd, const std::string& resetPosition = "center");

	void SetWindowSize(HWND hWnd, int width, int height);
	void ValidateWindowSize(HWND hWnd);
	void ValidateWindowSizeConstraints(cfg_int& minWidth, cfg_int& minHeight, cfg_int& maxWidth, cfg_int& maxHeight);

	void SetWindowTransparency(HWND hWnd, int transparency, bool enableTransparency);
};
#pragma endregion

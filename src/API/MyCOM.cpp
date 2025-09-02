/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    MyCOM Source File                                       * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "MyCOM.h"
#include "UIW.h"
#include "UIW_Settings.h"
#include "UIW_Helpers.h"


//////////////////////////////////////////
// * MyCOM - CONSTRUCTOR & DESTRUCTOR * //
//////////////////////////////////////////
#pragma region MyCOM - Constructor & Destructor
MyCOM::MyCOM() {
	InterlockedIncrement(&activeObjects);
}

MyCOM::~MyCOM() {
	InterlockedDecrement(&activeObjects);
}
#pragma endregion


///////////////////////////////////////
// * MyCOM - PUBLIC STATIC METHODS * //
///////////////////////////////////////
#pragma region MyCOM - Public Static Methods
HRESULT MyCOM::InitMyCOM() {
	HMODULE hModule = nullptr;
	HRESULT hr = OleInitialize(nullptr);

	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::InitMyCOM", L"OleInitialize failed", true);
	}

	if (!GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, reinterpret_cast<LPCTSTR>(static_cast<void*>(&InitMyCOM)), &hModule)) {
		OleUninitialize();
		return UIWHCOM::LogError(E_FAIL, L"UI Wizard => MyCOM::InitMyCOM", L"Failed to get module handle", true);
	}

	hr = InitTypeLibrary(hModule);
	if (FAILED(hr)) {
		OleUninitialize();
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::InitMyCOM", L"InitTypeLibrary failed", true);
	}

	hr = RegisterMyCOM();
	if (FAILED(hr)) {
		OleUninitialize();
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::InitMyCOM", L"RegisterMyCOM failed", true);
	}

	return S_OK;
}

HRESULT MyCOM::InitTypeLibrary(HMODULE hModule) {
	std::wstring module_path(MAX_PATH, L'\0');

	DWORD len = GetModuleFileNameW(hModule, &module_path[0], MAX_PATH);
	if (len == 0 || len >= MAX_PATH) {
		return UIWHCOM::LogError(E_FAIL, L"UI Wizard => MyCOM::InitTypeLibrary", L"Failed to retrieve module path", true);
	}

	module_path.resize(len);

	HRESULT hr = LoadTypeLibEx(module_path.c_str(), REGKIND_NONE, &MyCOM::typeLib);
	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::InitTypeLibrary", L"LoadTypeLibEx failed", true);
	}

	return S_OK;
}

HRESULT MyCOM::RegisterMyCOM(std::wstring_view regMethod) {
	auto hr = S_OK;
	CComPtr<MyCOMClassFactory> pClassFactory = new MyCOMClassFactory();

	if (!pClassFactory) {
		return UIWHCOM::LogError(E_OUTOFMEMORY, L"UI Wizard => MyCOM::RegisterMyCOM", L"Failed to create class factory", true);
	}

	hr = CoRegisterClassObject(MyCOM_CLSID, pClassFactory, CLSCTX_INPROC_SERVER, REGCLS_MULTIPLEUSE, &dwRegister);
	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterMyCOM", L"CoRegisterClassObject failed", true);
	}

	if (regMethod == L"RegEntry") {
		hr = RegisterCLSID();
		if (FAILED(hr)) {
			CoRevokeClassObject(dwRegister);
			return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterMyCOM", L"RegisterCLSID failed", true);
		}
	}
	else {
		hr = HookCLSIDFromProgID();
		if (FAILED(hr)) {
			CoRevokeClassObject(dwRegister);
			return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterMyCOM", L"HookCLSIDFromProgID failed", true);
		}
	}

	return S_OK;
}

HRESULT MyCOM::UnregisterMyCOM(std::wstring_view regMethod) {
	if (dwRegister != 0) {
		HRESULT hr = CoRevokeClassObject(dwRegister);

		if (FAILED(hr)) {
			return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::UnregisterMyCOM", L"CoRevokeClassObject failed", true);
		}

		if (regMethod == L"RegFree" && original_CLSIDFromProgID) {
			MH_DisableHook(&CLSIDFromProgID);
			MH_Uninitialize();
			original_CLSIDFromProgID = nullptr;
		}
		else if (regMethod == L"RegEntry") {
			UnregisterCLSID();
		}

		dwRegister = 0;
	}

	return S_OK;
}

HRESULT MyCOM::QuitMyCOM(std::wstring_view regMethod) {
	HRESULT hr = UnregisterMyCOM(regMethod);
	OleUninitialize();
	return hr;
}
#pragma endregion


////////////////////////////////////////
// * MyCOM - PRIVATE STATIC METHODS * //
////////////////////////////////////////
#pragma region MyCOM - COM Registration-Free with MinHook
HRESULT WINAPI MyCOM::MyCLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID lpclsid) {
	static std::map<std::wstring, CLSID> progidToClsidMap = {
		{ MyCOM_PROGID, MyCOM_CLSID }
	};

	auto it = progidToClsidMap.find(lpszProgID);
	if (it != progidToClsidMap.end()) {
		*lpclsid = it->second;
		return S_OK;
	}

	// Call the original function for other ProgIDs
	if (original_CLSIDFromProgID != nullptr) {
		return original_CLSIDFromProgID(lpszProgID, lpclsid);
	}

	return UIWHCOM::LogError(REGDB_E_CLASSNOTREG, L"UI Wizard => MyCOM::MyCLSIDFromProgID", L"Class not registered", true);
}

HRESULT MyCOM::HookCLSIDFromProgID() {
	if (MH_Initialize() != MH_OK) {
		return UIWHCOM::LogError(E_FAIL, L"UI Wizard => MyCOM::HookCLSIDFromProgID", L"MinHook initialization failed", true);
	}

	if (MH_CreateHook(&CLSIDFromProgID, &MyCLSIDFromProgID, reinterpret_cast<void**>(&original_CLSIDFromProgID)) != MH_OK) {
		MH_Uninitialize();
		return UIWHCOM::LogError(E_FAIL, L"UI Wizard => MyCOM::HookCLSIDFromProgID", L"MinHook creation failed", true);
	}

	if (MH_EnableHook(&CLSIDFromProgID) != MH_OK) {
		MH_Uninitialize();
		return UIWHCOM::LogError(E_FAIL, L"UI Wizard => MyCOM::HookCLSIDFromProgID", L"MinHook enable failed", true);
	}

	return S_OK;
}
#pragma endregion

#pragma region MyCOM - COM Registration with Reg Entry
HRESULT MyCOM::RegisterCLSID() {
	HRESULT hr;
	LPOLESTR clsidString = nullptr;
	hr = StringFromCLSID(MyCOM_CLSID, &clsidString);

	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterCLSID", L"StringFromCLSID failed", true);
	}

	CComBSTR clsidBSTR(clsidString);
	CoTaskMemFree(clsidString);

	// Register ProgID
	hr = SetRegistry(HKEY_CLASSES_ROOT, L"UIWizard\\CLSID", nullptr, clsidBSTR);
	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterCLSID", L"Failed to set registry for ProgID", true);
	}

	// Register CLSID key
	std::wstring clsidKeyPath = L"CLSID\\" + std::wstring(clsidBSTR);
	std::wstring modulePath(MAX_PATH, L'\0');
	DWORD len = GetModuleFileNameW(core_api::get_my_instance(), &modulePath[0], MAX_PATH);

	if (len == 0) {
		return UIWHCOM::LogError(E_FAIL, L"UI Wizard => MyCOM::RegisterCLSID", L"GetModuleFileNameW failed", true);
	}
	modulePath.resize(len);

	// Set InProcServer32
	hr = SetRegistry(HKEY_CLASSES_ROOT, clsidKeyPath.c_str(), L"InProcServer32", modulePath.c_str());
	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterCLSID", L"Failed to set InProcServer32 registry", true);
	}

	// Set ThreadingModel
	hr = SetRegistry(HKEY_CLASSES_ROOT, clsidKeyPath.c_str(), L"ThreadingModel", L"Apartment");
	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::RegisterCLSID", L"Failed to set ThreadingModel registry", true);
	}

	return S_OK;
}

HRESULT MyCOM::UnregisterCLSID() {
	LPOLESTR clsidString = nullptr;
	HRESULT hr = StringFromCLSID(MyCOM_CLSID, &clsidString);

	if (FAILED(hr)) {
		return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::UnregisterCLSID", L"StringFromCLSID failed");
	}

	std::wstring clsidKeyPath = L"CLSID\\" + std::wstring(clsidString);
	CoTaskMemFree(clsidString);

	LONG result = RegDeleteTree(HKEY_CLASSES_ROOT, clsidKeyPath.c_str());
	if (result != ERROR_SUCCESS) {
		UIWHCOM::LogError(HRESULT_FROM_WIN32(result), L"UI Wizard => MyCOM::UnregisterCLSID", L"Failed to delete CLSID registry key");
	}

	result = RegDeleteTree(HKEY_CLASSES_ROOT, L"UIWizard");
	if (result != ERROR_SUCCESS) {
		UIWHCOM::LogError(HRESULT_FROM_WIN32(result), L"UI Wizard => MyCOM::UnregisterCLSID", L"Failed to delete UIWizard registry key");
	}

	return S_OK;
}

HRESULT MyCOM::SetRegistry(HKEY rootKey, const wchar_t* subKey, const wchar_t* valueName, const wchar_t* value) {
	CRegKey hKey;
	LONG lResult = hKey.Create(rootKey, subKey);

	if (lResult != ERROR_SUCCESS) {
		return UIWHCOM::LogError(HRESULT_FROM_WIN32(lResult), L"UI Wizard => MyCOM::SetRegistry", L"Failed to create/open registry key", true);
	}

	lResult = hKey.SetStringValue(valueName, value);

	if (lResult != ERROR_SUCCESS) {
		return UIWHCOM::LogError(HRESULT_FROM_WIN32(lResult), L"UI Wizard => MyCOM::SetRegistry", L"Failed to set registry value", true);
	}

	return S_OK;
}
#pragma endregion


/////////////////////////////////////////
// * MyCOM - IUNKNOWN PUBLIC METHODS * //
/////////////////////////////////////////
#pragma region MyCOM - IUnknown Public Methods
STDMETHODIMP MyCOM::QueryInterface(REFIID riid, void** ppvObject) {
	if (riid == IID_IUnknown || riid == IID_IDispatch || riid == MyCOMAPI_IID) {
		*ppvObject = static_cast<IDispatch*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return UIWHCOM::LogError(E_NOINTERFACE, L"UI Wizard => MyCOM::QueryInterface", L"Interface not supported", true);
}

STDMETHODIMP_(ULONG) MyCOM::AddRef() {
	return InterlockedIncrement(&refCount);
}

STDMETHODIMP_(ULONG) MyCOM::Release() {
	LONG count = InterlockedDecrement(&refCount);

	if (count == 0) {
		delete this;
	}

	return count;
}
#pragma endregion


///////////////////////////////////////////
// * MyCOM - IDISPATCH PRIVATE METHODS * //
///////////////////////////////////////////
#pragma region MyCOM - IDispatch Private Methods
HRESULT MyCOM::InitTypeInfo() {
	if (MyCOM::typeInfo == nullptr && typeLib != nullptr) {
		HRESULT hr = typeLib->GetTypeInfoOfGuid(MyCOMAPI_IID, &MyCOM::typeInfo);

		if (FAILED(hr)) {
			return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::InitTypeInfo", L"GetTypeInfoOfGuid failed", true);
		}
	}

	return S_OK;
}

STDMETHODIMP MyCOM::GetTypeInfoCount(UINT* pctinfo) {
	*pctinfo = (typeInfo != nullptr) ? 1 : 0;
	return S_OK;
}

STDMETHODIMP MyCOM::GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) {
	if (iTInfo != 0) {
		return UIWHCOM::LogError(DISP_E_BADINDEX, L"UI Wizard => MyCOM::GetTypeInfo", L"Invalid type info index", true);
	}

	if (typeInfo == nullptr) {
		HRESULT hr = InitTypeInfo();
		if (FAILED(hr)) {
			return UIWHCOM::LogError(hr, L"UI Wizard => MyCOM::GetTypeInfo", L"InitTypeInfo failed", true);
		}
	}

	*ppTInfo = typeInfo;
	if (*ppTInfo) {
		(*ppTInfo)->AddRef();
	}

	return S_OK;
}

STDMETHODIMP MyCOM::GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) {
	if (typeInfo == nullptr) {
		return UIWHCOM::LogError(E_NOTIMPL, L"UI Wizard => MyCOM::GetIDsOfNames", L"Type info not initialized", true);
	}

	return typeInfo->GetIDsOfNames(rgszNames, cNames, rgDispId);
}

STDMETHODIMP MyCOM::Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) {
	if (typeInfo == nullptr) {
		return UIWHCOM::LogError(E_NOTIMPL, L"UI Wizard => MyCOM::Invoke", L"Type info not initialized", true);
	}

	return typeInfo->Invoke(static_cast<IDispatch*>(this), dispIdMember, wFlags, pDispParams, pVarResult, pExcepInfo, puArgErr);
}
#pragma endregion


/////////////////////////////////////////////////
// * MyCOM - PUBLIC API - DISPLAY PROPERTIES * //
/////////////////////////////////////////////////
#pragma region MyCOM - Public API - Display Properties
STDMETHODIMP MyCOM::get_DisplayDPI(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DisplayDPI", L"Invalid pointer");
	}

	*pValue = UIWizardSettings::displayDPI;
	return S_OK;
}

STDMETHODIMP MyCOM::get_DisplayResolutionMode(BSTR* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DisplayResolutionMode", L"Invalid pointer");
	}

	const char* mode = UIWHDisplay::GetDisplayResolutionMode();
	CComBSTR bstrMode(mode);
	*pValue = bstrMode.Detach();

	if (!*pValue) {
		return UIWHCOM::LogError(E_OUTOFMEMORY, L"UI Wizard => MyCOM::get_DisplayResolutionMode", L"Failed to allocate BSTR");
	}

	return S_OK;
}

STDMETHODIMP MyCOM::get_DisplayResolution(BSTR* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DisplayResolution", L"Invalid pointer");
	}

	std::string resolutionJson = UIWHDisplay::GetDisplayResolution();
	CComBSTR bstrResolution(resolutionJson.c_str());
	*pValue = bstrResolution.Detach();

	if (!*pValue) {
		return UIWHCOM::LogError(E_OUTOFMEMORY, L"UI Wizard => MyCOM::get_DisplayResolution", L"Failed to allocate BSTR");
	}

	return S_OK;
}

STDMETHODIMP MyCOM::get_DisplayResolutionMultiMonitors(BSTR* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DisplayResolutionMultiMonitors", L"Invalid pointer");
	}

	std::string resolutionJson = UIWHDisplay::GetDisplayResolutionMultiMonitors();
	CComBSTR bstrResolutionMultiMon(resolutionJson.c_str());
	*pValue = bstrResolutionMultiMon.Detach();

	if (!*pValue) {
		return UIWHCOM::LogError(E_OUTOFMEMORY, L"UI Wizard => MyCOM::get_DisplayResolutionMultiMonitors", L"Failed to allocate BSTR");
	}

	return S_OK;
}
#pragma endregion


/////////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW GEOMETRY PROPERTIES * //
/////////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window Geometry Properties
STDMETHODIMP MyCOM::get_WindowX(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_WindowX", L"Invalid pointer");
	}
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::get_WindowX", L"Window not initialized");
	}

	*pValue = UIWHWindow::GetWindowX(UIWizard::Window()->mainHwnd);
	return S_OK;
}

STDMETHODIMP MyCOM::get_WindowY(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_WindowY", L"Invalid pointer");
	}
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::get_WindowY", L"Window not initialized");
	}

	*pValue = UIWHWindow::GetWindowY(UIWizard::Window()->mainHwnd);
	return S_OK;
}

STDMETHODIMP MyCOM::get_WindowWidth(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_WindowWidth", L"Invalid pointer");
	}
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::get_WindowWidth", L"Window not initialized");
	}

	*pValue = UIWHWindow::GetWindowWidth(UIWizard::Window()->mainHwnd);
	return S_OK;
}

STDMETHODIMP MyCOM::get_WindowHeight(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_WindowHeight", L"Invalid pointer");
	}
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::get_WindowHeight", L"Window not initialized");
	}

	*pValue = UIWHWindow::GetWindowHeight(UIWizard::Window()->mainHwnd);
	return S_OK;
}
#pragma endregion


/////////////////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW SIZE CONSTRAINTS PROPERTIES * //
/////////////////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window Size Constraints Properties
STDMETHODIMP MyCOM::put_WindowMinSize(const VARIANT& newState) {
	bool convertedState = UIWHConvert::BOOLFromVARIANT(newState, UIWizardSettings::windowMinSizeDefault);
	UIWizardSettings::windowMinSize = convertedState;
	return S_OK;
}

STDMETHODIMP MyCOM::put_WindowMinWidth(const VARIANT& newValue) {
	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::windowMinWidthDefault);
	UIWizardSettings::windowMinWidth = convertedValue;
	return S_OK;
}

STDMETHODIMP MyCOM::put_WindowMinHeight(const VARIANT& newValue) {
	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::windowMinHeightDefault);
	UIWizardSettings::windowMinHeight = convertedValue;
	return S_OK;
}

STDMETHODIMP MyCOM::put_WindowMaxSize(const VARIANT& newState) {
	bool convertedState = UIWHConvert::BOOLFromVARIANT(newState, UIWizardSettings::windowMaxSizeDefault);
	UIWizardSettings::windowMaxSize = convertedState;
	return S_OK;
}

STDMETHODIMP MyCOM::put_WindowMaxWidth(const VARIANT& newValue) {
	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::windowMaxWidthDefault);
	UIWizardSettings::windowMaxWidth = convertedValue;
	return S_OK;
}

STDMETHODIMP MyCOM::put_WindowMaxHeight(const VARIANT& newValue) {
	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::windowMaxHeightDefault);
	UIWizardSettings::windowMaxHeight = convertedValue;
	return S_OK;
}
#pragma endregion


///////////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW APPEARANCE PROPERTIES * //
///////////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window Appearance Properties
STDMETHODIMP MyCOM::put_WindowBgColor(const VARIANT& newValue) {
	static bool firstBgColorInit = true;
	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::windowBgColorDefault);
	UIWizardSettings::windowBgColor = convertedValue;

	if (firstBgColorInit && UIWizard::Window()) {
		UIWizard::Window()->SetBgColor(static_cast<COLORREF>(convertedValue));
		RedrawWindow(UIWizard::MainWindow(), nullptr, nullptr, RDW_ERASE | RDW_INVALIDATE | RDW_UPDATENOW);
		firstBgColorInit = false;
	}

	return S_OK;
}

STDMETHODIMP MyCOM::get_DarkMode(VARIANT_BOOL* pState) {
	if (!pState) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DarkMode", L"Invalid pointer");
	}

	*pState = UIWHDarkMode::IsDark() ? VARIANT_TRUE : VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP MyCOM::put_DarkMode(const VARIANT& newState) {
	bool convertedState = UIWHConvert::BOOLFromVARIANT(newState, UIWHDarkMode::IsDark());
	UIWHDarkMode::SetDark(convertedState);
	return S_OK;
}

STDMETHODIMP MyCOM::get_FrameStyle(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_FrameStyle", L"Invalid pointer");
	}

	*pValue = UIWizardSettings::frameStyle;
	return S_OK;
}

STDMETHODIMP MyCOM::put_FrameStyle(const VARIANT& newValue) {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::put_FrameStyle", L"Window not initialized");
	}

	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::frameStyleDefault);

	if (UIWizardSettings::frameStyle != convertedValue) {
		UIWizardSettings::frameStyle = convertedValue;
		UIWizard::Window()->Initialize();
		return S_OK;
	}

	return S_FALSE;
}
#pragma endregion


/////////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW BEHAVIOR PROPERTIES * //
/////////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window Behavior Properties
STDMETHODIMP MyCOM::get_MoveStyle(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_MoveStyle", L"Invalid pointer");
	}

	*pValue = UIWizardSettings::moveStyle;
	return S_OK;
}

STDMETHODIMP MyCOM::put_MoveStyle(const VARIANT& newValue) {
	int convertedValue = UIWHConvert::INTFromVARIANT(newValue, UIWizardSettings::moveStyleDefault);
	UIWizardSettings::moveStyle = convertedValue;
	return S_OK;
}

STDMETHODIMP MyCOM::get_DisableWindowMaximizing(VARIANT_BOOL* pState) {
	if (!pState) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DisableWindowMaximizing", L"Invalid pointer");
	}

	*pState = (UIWizardSettings::disableWindowMaximizing ? VARIANT_TRUE : VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP MyCOM::put_DisableWindowMaximizing(const VARIANT& newState) {
	bool convertedState = UIWHConvert::BOOLFromVARIANT(newState, UIWizardSettings::disableWindowMaximizingDefault);
	UIWizardSettings::disableWindowMaximizing = convertedState;
	return S_OK;
}

STDMETHODIMP MyCOM::get_DisableWindowSizing(VARIANT_BOOL* pState) {
	if (!pState) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_DisableWindowSizing", L"Invalid pointer");
	}

	*pState = (UIWizardSettings::disableWindowSizing ? VARIANT_TRUE : VARIANT_FALSE);
	return S_OK;
}

STDMETHODIMP MyCOM::put_DisableWindowSizing(const VARIANT& newState) {
	bool convertedState = UIWHConvert::BOOLFromVARIANT(newState, UIWizardSettings::windowMinSizeDefault);
	UIWizardSettings::disableWindowSizing = convertedState;
	return S_OK;
}
#pragma endregion


//////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW STATE PROPERTIES * //
//////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window State Properties
STDMETHODIMP MyCOM::get_WindowState(long* pValue) {
	if (!pValue) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::get_WindowState", L"Invalid pointer");
	}

	*pValue = UIWizardSettings::windowState;
	return S_OK;
}
#pragma endregion


//////////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW MANIPULATION METHODS * //
//////////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window Manipulation Methods
STDMETHODIMP MyCOM::SetCaptionAreaSize(int x, int y, int w, int h) {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::SetCaptionAreaSize", L"Window not initialized");
	}

	UIWizard::Window()->SetCaptionAreaSize(x, y, w, h);
	return S_OK;
}

STDMETHODIMP MyCOM::SetWindowPosition(int x, int y) {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::SetWindowPosition", L"Window not initialized");
	}

	UIWHWindow::SetWindowPosition(UIWizard::Window()->mainHwnd, x, y);
	return S_OK;
}

STDMETHODIMP MyCOM::SetWindowPositionInGrid(BSTR positionBSTR) {
	if (!positionBSTR) {
		return UIWHCOM::LogError(E_POINTER, L"UI Wizard => MyCOM::SetWindowPositionInGrid", L"Invalid pointer");
	}
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::SetWindowPositionInGrid", L"Window not initialized");
	}

	std::wstring positionWS(positionBSTR, SysStringLen(positionBSTR));
	int utf8Size = WideCharToMultiByte(CP_UTF8, 0, positionWS.c_str(), -1, nullptr, 0, nullptr, nullptr);
	std::vector<char> utf8Str(utf8Size);
	WideCharToMultiByte(CP_UTF8, 0, positionWS.c_str(), -1, utf8Str.data(), utf8Size, nullptr, nullptr);
	std::string position(utf8Str.begin(), utf8Str.end() - 1);

	UIWHWindow::SetWindowPositionInGrid(UIWizard::Window()->mainHwnd, position);
	return S_OK;
}

STDMETHODIMP MyCOM::SetWindowSize(int width, int height) {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::SetWindowSize", L"Window not initialized");
	}

	if (width <= 0) {
		width = UIWHWindow::GetWindowWidth(UIWizard::Window()->mainHwnd);
	}

	if (height <= 0) {
		height = UIWHWindow::GetWindowHeight(UIWizard::Window()->mainHwnd);
	}

	UIWHWindow::SetWindowSize(UIWizard::Window()->mainHwnd, width, height);
	UIWizard::Window()->UpdateWindowSize();

	return S_OK;
}

STDMETHODIMP MyCOM::SetWindowSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) {
	if (minWidth < 0 || minHeight < 0 || maxWidth < 0 || maxHeight < 0) {
		minWidth = UIWizardSettings::windowMinWidthDefault;
		minHeight = UIWizardSettings::windowMinHeightDefault;
		maxWidth = UIWizardSettings::windowMaxWidthDefault;
		maxHeight = UIWizardSettings::windowMaxHeightDefault;
	}

	UIWizardSettings::windowMinWidth = minWidth;
	UIWizardSettings::windowMinHeight = minHeight;
	UIWizardSettings::windowMaxWidth = maxWidth;
	UIWizardSettings::windowMaxHeight = maxHeight;

	return S_OK;
}
#pragma endregion


/////////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW METRICS METHODS * //
/////////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window Metrics Methods
STDMETHODIMP MyCOM::LoadWindowMetrics() {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::LoadWindowMetrics", L"Window not initialized");
	}

	UIWizard::Window()->LoadWindowMetrics();
	return S_OK;
}

STDMETHODIMP MyCOM::SaveWindowMetrics() {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::SaveWindowMetrics", L"Window not initialized");
	}

	UIWizard::Window()->SaveWindowMetrics();
	return S_OK;
}
#pragma endregion


///////////////////////////////////////////////////
// * MyCOM - PUBLIC API - WINDOW STATE METHODS * //
///////////////////////////////////////////////////
#pragma region MyCOM - Public API - Window State Methods
STDMETHODIMP MyCOM::ToggleFullscreen() {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::ToggleFullscreen", L"Window not initialized");
	}

	UIWizard::Window()->ToggleFullscreen();
	return S_OK;
}

STDMETHODIMP MyCOM::ExitFullscreen() {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::ExitFullscreen", L"Window not initialized");
	}

	UIWizard::Window()->ToggleFullscreen(true);
	return S_OK;
}

STDMETHODIMP MyCOM::ToggleMaximize() {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::ToggleMaximize", L"Window not initialized");
	}

	UIWizard::Window()->ToggleMaximize();
	return S_OK;
}

STDMETHODIMP MyCOM::ExitMaximize() {
	if (!UIWizard::Window()) {
		return UIWHCOM::LogError(E_UNEXPECTED, L"UI Wizard => MyCOM::ExitMaximize", L"Window not initialized");
	}

	UIWizard::Window()->ToggleMaximize(true);
	return S_OK;
}
#pragma endregion


///////////////////////////////////////////////////////
// * MyCOM CLASS FACTORY - IUNKNOWN PUBLIC METHODS * //
///////////////////////////////////////////////////////
#pragma region MyCOM Class Factory - IUnknown Public Methods
STDMETHODIMP MyCOMClassFactory::QueryInterface(REFIID riid, void** ppvObject) {
	if (riid == IID_IUnknown || riid == IID_IClassFactory) {
		*ppvObject = static_cast<IClassFactory*>(this);
		AddRef();
		return S_OK;
	}

	*ppvObject = nullptr;
	return UIWHCOM::LogError(E_NOINTERFACE, L"UI Wizard => MyCOMClassFactory::QueryInterface", L"Interface not supported", true);
}

STDMETHODIMP_(ULONG) MyCOMClassFactory::AddRef() {
	return InterlockedIncrement(&refCount);
}

STDMETHODIMP_(ULONG) MyCOMClassFactory::Release() {
	LONG count = InterlockedDecrement(&refCount);

	if (count == 0) {
		delete this;
	}

	return count;
}
#pragma endregion


/////////////////////////////////////////////////////////////
// * MyCOM CLASS FACTORY - ICLASSFACTORY PRIVATE METHODS * //
/////////////////////////////////////////////////////////////
#pragma region MyCOM Class Factory - IClassFactory Private Methods
STDMETHODIMP MyCOMClassFactory::CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) {
	if (pUnkOuter != nullptr) {
		return UIWHCOM::LogError(CLASS_E_NOAGGREGATION, L"UI Wizard => MyCOMClassFactory::CreateInstance", L"Aggregation not supported", true);
	}

	CComPtr<MyCOM> pMyCOM = new MyCOM();
	return pMyCOM->QueryInterface(riid, ppvObject);
}

STDMETHODIMP MyCOMClassFactory::LockServer(BOOL fLock) {
	fLock ? InterlockedIncrement(&MyCOM::serverLocks) :
		InterlockedDecrement(&MyCOM::serverLocks);

	return S_OK;
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    MyCOM Header File                                       * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    16-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once


///////////////
// * MyCOM * //
///////////////
#pragma region MyCOM
class MyCOM : public IDispatch {
public:
	MyCOM();
	~MyCOM();

	using CLSIDFromProgID_t = HRESULT(WINAPI*)(LPCOLESTR, LPCLSID);

	// * STATIC MEMBERS * //
	static inline IID MyCOMAPI_IID = { 0xE4F5C6B7, 0xD890, 0x41A2, { 0x91, 0x45, 0x78, 0x90, 0xBC, 0xDE, 0xF2, 0x13 } };
	static inline IID MyCOMLIB_IID = { 0xB2A3C4D5, 0xF6E7, 0x4891, { 0x92, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x23 } };
	static inline CLSID MyCOM_CLSID = { 0xd9fdd234, 0x4553, 0x40b4, { 0x89, 0x62, 0xe9, 0xd9, 0xf1, 0xb8, 0xee, 0x99 } };
	static inline LPCOLESTR MyCOM_PROGID = OLESTR("UIWizard");

	static inline LONG activeObjects = 0;
	static inline DWORD dwRegister = 0;
	static inline LONG serverLocks = 0;
	static inline CLSIDFromProgID_t original_CLSIDFromProgID = nullptr;
	static inline CComPtr<ITypeLib> typeLib = nullptr;
	static inline CComPtr<ITypeInfo> typeInfo = nullptr;

	// * STATIC METHODS * //
	static HRESULT InitMyCOM();
	static HRESULT InitTypeLibrary(HMODULE hModule);
	static HRESULT RegisterMyCOM(std::wstring_view regMethod = L"RegFree"); // "RegFree" or "RegEntry"
	static HRESULT UnregisterMyCOM(std::wstring_view regMethod = L"RegFree"); // "RegFree" or "RegEntry"
	static HRESULT QuitMyCOM(std::wstring_view regMethod = L"RegFree"); // "RegFree" or "RegEntry"

	// * IUNKNOWN METHODS * //
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
	STDMETHODIMP_(ULONG) AddRef() override;
	STDMETHODIMP_(ULONG) Release() override;

	// * PUBLIC API - DISPLAY PROPERTIES * //
	STDMETHOD(get_DisplayDPI)(long* pValue);
	STDMETHOD(get_DisplayResolutionMode)(BSTR* pValue);
	STDMETHOD(get_DisplayResolution)(BSTR* pValue);
	STDMETHOD(get_DisplayResolutionMultiMonitors)(BSTR* pValue);

	// * PUBLIC API - WINDOW GEOMETRY PROPERTIES * //
	STDMETHOD(get_WindowX)(long* pValue);
	STDMETHOD(get_WindowY)(long* pValue);
	STDMETHOD(get_WindowWidth)(long* pValue);
	STDMETHOD(get_WindowHeight)(long* pValue);

	// * PUBLIC API - WINDOW SIZE CONSTRAINTS PROPERTIES * //
	STDMETHOD(put_WindowMinSize)(const VARIANT& newState);
	STDMETHOD(put_WindowMinWidth)(const VARIANT& newValue);
	STDMETHOD(put_WindowMinHeight)(const VARIANT& newValue);
	STDMETHOD(put_WindowMaxSize)(const VARIANT& newState);
	STDMETHOD(put_WindowMaxWidth)(const VARIANT& newValue);
	STDMETHOD(put_WindowMaxHeight)(const VARIANT& newValue);

	// * PUBLIC API - WINDOW APPEARANCE PROPERTIES * //
	STDMETHOD(put_WindowBgColor)(const VARIANT& newValue);
	STDMETHOD(get_DarkMode)(VARIANT_BOOL* pState);
	STDMETHOD(put_DarkMode)(const VARIANT& newState);
	STDMETHOD(get_FrameStyle)(long* pValue);
	STDMETHOD(put_FrameStyle)(const VARIANT& newValue);

	// * PUBLIC API - WINDOW BEHAVIOR PROPERTIES * //
	STDMETHOD(get_MoveStyle)(long* pValue);
	STDMETHOD(put_MoveStyle)(const VARIANT& newValue);
	STDMETHOD(get_DisableWindowMaximizing)(VARIANT_BOOL* pState);
	STDMETHOD(put_DisableWindowMaximizing)(const VARIANT& newState);
	STDMETHOD(get_DisableWindowSizing)(VARIANT_BOOL* pState);
	STDMETHOD(put_DisableWindowSizing)(const VARIANT& newState);

	// * PUBLIC API - WINDOW STATE PROPERTIES * //
	STDMETHOD(get_WindowState)(long* pValue);

	// * PUBLIC API - WINDOW MANIPULATION METHODS * //
	STDMETHOD(SetCaptionAreaSize)(int x, int y, int w, int h);
	STDMETHOD(SetWindowPosition)(int x, int y);
	STDMETHOD(SetWindowPositionInGrid)(BSTR positionBSTR);
	STDMETHOD(SetWindowSize)(int width, int height);
	STDMETHOD(SetWindowSizeLimits)(int minWidth, int minHeight, int maxWidth, int maxHeight);

	// * PUBLIC API - WINDOW METRICS METHODS * //
	STDMETHOD(LoadWindowMetrics)();
	STDMETHOD(SaveWindowMetrics)();

	// * PUBLIC API - WINDOW STATE METHODS * //
	STDMETHOD(ToggleFullscreen)();
	STDMETHOD(ExitFullscreen)();
	STDMETHOD(ToggleMaximize)();
	STDMETHOD(ExitMaximize)();
	STDMETHOD(WindowMinimize)();
	STDMETHOD(WindowRestore)();

private:
	LONG refCount = 0;

	// * STATIC METHODS * //
	static HRESULT WINAPI MyCLSIDFromProgID(LPCOLESTR lpszProgID, LPCLSID lpclsid); // "RegFree" with MinHook method
	static HRESULT HookCLSIDFromProgID(); // "RegFree" with MinHook method
	static HRESULT RegisterCLSID(); // "RegEntry" method
	static HRESULT UnregisterCLSID(); // "RegEntry" method
	static HRESULT SetRegistry(HKEY rootKey, const wchar_t* subKey, const wchar_t* valueName, const wchar_t* value); // "RegEntry" method

	// * IDISPATCH METHODS * //
	static HRESULT InitTypeInfo();
	STDMETHODIMP GetTypeInfoCount(UINT* pctinfo) override;
	STDMETHODIMP GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo) override;
	STDMETHODIMP GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId) override;
	STDMETHODIMP Invoke(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags, DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo, UINT* puArgErr) override;
};
#pragma endregion


/////////////////////////////
// * MyCOM CLASS FACTORY * //
/////////////////////////////
#pragma region MyCOM Class Factory
class MyCOMClassFactory : public IClassFactory {
public:
	MyCOMClassFactory() = default;
	~MyCOMClassFactory() = default;

	// * IUNKNOWN METHODS * //
	STDMETHODIMP QueryInterface(REFIID riid, void** ppvObject) override;
	STDMETHODIMP_(ULONG) AddRef() override;
	STDMETHODIMP_(ULONG) Release() override;

private:
	LONG refCount = 0;

	// * ICLASSFACTORY METHODS * //
	STDMETHODIMP CreateInstance(IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override;
	STDMETHODIMP LockServer(BOOL fLock) override;
};
#pragma endregion

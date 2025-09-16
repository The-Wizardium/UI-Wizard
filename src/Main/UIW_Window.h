/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Window Header File                            * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    16-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once


////////////////
// * WINDOW * //
////////////////
#pragma region Window
class UIWizardWindow {
public:
	// * ENUMS * //
	enum class AeroEffect : int {
		Default = 0,
		Disabled = 1,
		GlassFrame = 2,
		SheetOfGlass = 3
	};

	enum class ESCKeyAction : int {
		None = 0,
		Hide = 1,
		Exit = 2
	};

	enum class FrameStyle : int {
		Default = 0,
		SmallCaption = 1,
		NoCaption = 2,
		NoBorder = 3
	};

	enum class MainMenuState : int {
		Show = 0,
		Hide = 1,
		Auto = 2
	};

	enum class MoveStyle : int {
		Default = 0,
		Middle = 1,
		Left = 2,
		Both = 3
	};

	enum class WindowState : int {
		Normal = 0,
		Minimized = 1,
		Maximized = 2,
		Fullscreen = 3
	};

	// * INITIALIZATION * //
	explicit UIWizardWindow(HWND hWnd);
	virtual ~UIWizardWindow();

	void Initialize();
	void InitWindowMessageLoop() const;

	HWND mainHwnd = nullptr;
	FrameStyle savedFrameStyle = FrameStyle::Default;

	std::unique_ptr<std::remove_pointer_t<HBRUSH>, decltype(&DeleteObject)> bgBrush {
		nullptr, &DeleteObject
	};
	COLORREF bgColor = RGB(240, 240, 240);

	bool mouseInCaption = false;
	bool mouseIsDragging = false;
	POINT mouseDragStart = { 0, 0 };
	POINT windowDragStart = { 0, 0 };
	bool windowMinimized = false;

	static const UINT HIDE_WINDOW_INACTIVITY_TIMER_ID = 30430;
	ULONGLONG hideWindowLastActivityTime = 0;
	ULONGLONG hideWindowLastInputTime = 0;

	// * WINDOW APPEARANCE * //
	void SetFrameStyle(FrameStyle style, bool forceUpdate = false);
	void SetAeroEffect(AeroEffect effect, bool forceUpdate = false);
	bool HandleAeroEffectDisabled(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	bool CreateWindowBgBrush();
	COLORREF GetWindowBgColor() const;
	void SetWindowBgColor(WPARAM wParam);
	void SetBgColor(COLORREF color);
	LRESULT HandleWindowFrame(UINT message, WPARAM wParam, LPARAM lParam);
	void SetCustomIcon() const;
	bool SetTaskbarIconVisibility(bool force = false) const;
	pfc::string8 GetWindowTitle() const;
	void SetWindowTitle() const;
	void SetWindows11RoundCorners() const;

	// * WINDOW CONTROLS * //
	bool CaptionArea(const POINT& pt) const;
	void CaptionAreaIndicator();
	void CaptionAreaIndicatorTimer(UINT_PTR nIDEvent);
	void SetCaptionAreaSize(int x, int y, int w, int h) const;
	bool DragStyle(MoveStyle moveStyle) const;
	void DragStart(LPARAM lParam, bool isClientCoords);
	void DragSnap(int& x, int& y);
	void DragMove();
	void DragEnd();
	void HandleESCKey();
	int HandleWindowHitTest(LPARAM lParam);

	// * WINDOW POSITION & SIZE * //
	void LoadWindowMetrics() const;
	void SaveWindowMetrics() const;
	void HandleWindowSizeConstraints(WPARAM wParam, LPARAM lParam) const;
	void UpdateWindowSize() const;

	// * WINDOW STATE * //
	void HandleWindowInactivity(bool fromTimer = false);
	void SetWindowHideInactivity();
	void HandleWindowMinimize();
	void HandleWindowRestore(WPARAM wParam);
	void HandleWindowState(bool wasMaximized, bool wasFullscreen, bool restoreState = false);
	void SetFullscreenSize();
	void ToggleFullscreen(bool forceExitFullscreen = false);
	void ToggleMaximize(bool forceExitMaximize = false);
	void ToggleMaximizeButtonState();
	bool WindowIsFullscreen() const;
	bool WindowIsMaximized() const;
};
#pragma endregion


/////////////////////
// * MAIN WINDOW * //
/////////////////////
#pragma region Main Window
class UIWizardMainWindow : public UIWizardWindow {
public:
	explicit UIWizardMainWindow(HWND hWnd);
	~UIWizardMainWindow() override;

	void MainWindowSubclass(HWND hwndParent) const;
	void MainWindowUnsubclass(HWND hwndParent) const;

protected:
	static LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
};
#pragma endregion


//////////////////////
// * CHILD WINDOW * //
//////////////////////
#pragma region Child Window
class UIWizardChildWindow : public UIWizardWindow {
public:
	explicit UIWizardChildWindow(HWND hWnd);
	~UIWizardChildWindow() override;

	void ChildWindowSubclass(HWND hwndParent) const;
	void ChildWindowUnsubclass(HWND hwndParent) const;
	int ChildWindowTotalCount(HWND hWndParent) const;

protected:
	static LRESULT CALLBACK ChildWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR);
	static BOOL CALLBACK ChildWindowEnumProc(HWND hwndChild, LPARAM lParam);
};
#pragma endregion


///////////////////////
// * SHADOW WINDOW * //
///////////////////////
#pragma region Shadow Window
class UIWizardShadowWindow : public UIWizardWindow {
public:
	explicit UIWizardShadowWindow(HWND hWnd);
	~UIWizardShadowWindow() override;

	HWND shadowHwnd = nullptr;

	HWND ShadowWindow();
	void ShadowWindowActiveState(WPARAM wParam, bool forceActive = false) const;
	void ShadowWindowDisplay() const;
	void ShadowWindowUpdate() const;

protected:
	static LRESULT CALLBACK ShadowWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};
#pragma endregion

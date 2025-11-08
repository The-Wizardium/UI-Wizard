/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Window Source File                            * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.3                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    08-11-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "UIW.h"
#include "UIW_Helpers.h"
#include "UIW_Menu.h"
#include "UIW_Settings.h"
#include "UIW_Window.h"


////////////////////////
// * INITIALIZATION * //
////////////////////////
#pragma region Initialization
UIWizardWindow::UIWizardWindow(HWND hWnd) : mainHwnd(hWnd) {
}

UIWizardWindow::~UIWizardWindow() {
}

void UIWizardWindow::Initialize() {
	// * INIT DISPLAY SETTINGS * //
	UIWizardSettings::displayDPI = UIWHDisplay::GetDisplayDPI();
	UIWizardSettings::displayResolutionMode = UIWHDisplay::GetDisplayResolutionMode();
	UIWizardSettings::displayResolution = UIWHDisplay::GetDisplayResolution().c_str();
	UIWizardSettings::displayResolutionMultiMonitors = UIWHDisplay::GetDisplayResolutionMultiMonitors().c_str();
	UIWizardSettings::windowState = UIWizardSettings::windowState.get_value();

	// * INIT SAFETY CHECKS * //
	if (!WindowIsMaximized() && !WindowIsFullscreen()) {
		UIWHWindow::ValidateWindowPosition(mainHwnd);
		UIWHWindow::ValidateWindowSize(mainHwnd);
		UIWHWindow::ValidateWindowSizeConstraints(
			UIWizardSettings::windowMinWidth, UIWizardSettings::windowMinHeight,
			UIWizardSettings::windowMaxWidth, UIWizardSettings::windowMaxHeight
		);
	}

	// * INIT FRAME STYLE AND AERO EFFECT * //
	SetFrameStyle(static_cast<FrameStyle>(UIWizardSettings::frameStyle.get_value()));
	SetAeroEffect(static_cast<AeroEffect>(UIWizardSettings::aeroEffect.get_value()));

	if (WindowIsFullscreen()) {
		SetFullscreenSize();
	}

	// * INIT AND CREATE SHADOW WINDOW IF BORDERLESS * //
	if (!UIWHWindow::IsWindows11()) {
		UIWizard::Shadow()->ShadowWindow();
		UIWizard::Shadow()->ShadowWindowDisplay();
		UIWizard::Shadow()->ShadowWindowUpdate();
	}

	// * INIT WINDOWS 11 ROUND CORNERS * //
	SetWindows11RoundCorners();

	// * INIT OTHER REMAINING WINDOW SETTINGS * //
	UIWHWindow::SetWindowTransparency(mainHwnd, UIWizardSettings::windowTransparency, UIWizardSettings::enableWindowTransparency);
	SetWindowTitle();
	SetCustomIcon();
	SetTaskbarIconVisibility();
	SetWindowHideInactivity();
	ToggleMaximizeButtonState();

	ShowWindow(mainHwnd, SW_SHOW);
}

void UIWizardWindow::InitWindowMessageLoop() const {
	if (GetModuleHandle(L"foo_ui_columns.dll")) {
		return; // Exit if foo_ui_columns is active, as it manages the WM_message loop itself.
	}

	MSG msg;

	while (GetMessage(&msg, nullptr, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
#pragma endregion


///////////////////////////
// * WINDOW APPEARANCE * //
///////////////////////////
#pragma region Window Appearance
void UIWizardWindow::SetFrameStyle(FrameStyle style, bool forceUpdate) {
	static FrameStyle frameStylePrevious = FrameStyle::Default;
	if (frameStylePrevious == style && !forceUpdate) return;

	// Capture states before style changes to preserve maximized/fullscreen and frameStyle status
	std::string_view windowStateCurrent = UIWHWindow::GetWindowState();
	bool frameStyleChanged = UIWizardSettings::frameStylePrevious != static_cast<int>(style);

	// Exit maximized/fullscreen before changing styles to avoid DWM sizing bugs
	if (frameStyleChanged) HandleWindowState(windowStateCurrent);

	if (!UIWHWindow::IsFrameStyle("Default") && UIWHWindow::IsAeroEffect("Disabled")) {
		UIWHWindow::SetFrameStyle("Default");
		style = FrameStyle::Default;
	}

	UIWizardSettings::frameStylePrevious = static_cast<int>(style);
	frameStylePrevious = style;

	LONG_PTR styleValue = GetWindowLongPtr(mainHwnd, GWL_STYLE);
	LONG_PTR exStyleValue = GetWindowLongPtr(mainHwnd, GWL_EXSTYLE) & ~WS_EX_TOOLWINDOW;

	switch (style) {
		case FrameStyle::Default: {
			styleValue |= WS_OVERLAPPEDWINDOW;
			break;
		}
		case FrameStyle::SmallCaption: {
			styleValue |= WS_OVERLAPPEDWINDOW;
			exStyleValue = WS_EX_TOOLWINDOW;
			break;
		}
		case FrameStyle::NoCaption: {
			styleValue = WS_POPUP | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
			break;
		}
		case FrameStyle::NoBorder: {
			styleValue = WS_POPUP | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;
			break;
		}
	}

	// Apply the frame style changes, WS_CLIPCHILDREN prevents window flicker so we do not need  return 1 in WM_ERASEBKGND
	SetWindowLongPtr(mainHwnd, GWL_STYLE, styleValue | WS_CLIPCHILDREN);
	SetWindowLongPtr(mainHwnd, GWL_EXSTYLE, exStyleValue);

	// Restore original maximized/fullscreen state if no restoration was needed
	if (frameStyleChanged) HandleWindowState(windowStateCurrent, true);

	UpdateWindowSize();
}

void UIWizardWindow::SetAeroEffect(AeroEffect effect, bool forceUpdate) {
	static AeroEffect lastAeroEffect = AeroEffect::Default;
	if (lastAeroEffect == effect && !forceUpdate) return;
	lastAeroEffect = effect;

	// Reset to the default state
	DWORD dwPolicy = DWMNCRP_ENABLED;
	DwmSetWindowAttribute(mainHwnd, DWMWA_NCRENDERING_POLICY, &dwPolicy, sizeof(dwPolicy));

	MARGINS margins = { 0, 0, 0, 0 };
	DwmExtendFrameIntoClientArea(mainHwnd, &margins);

	DWM_BLURBEHIND bb = { 0 };
	bb.dwFlags = DWM_BB_ENABLE;
	bb.fEnable = FALSE; // Disable blur behind the window by default
	DwmEnableBlurBehindWindow(mainHwnd, &bb);

	// Then apply the requested Aero effect
	switch (effect) {
		case AeroEffect::Default: {
			// Default case already handled above
			break;
		}
		case AeroEffect::Disabled: {
			UIWHWindow::SetFrameStyle("Default");
			SetFrameStyle(FrameStyle::Default);
			dwPolicy = DWMNCRP_DISABLED;
			DwmSetWindowAttribute(mainHwnd, DWMWA_NCRENDERING_POLICY, &dwPolicy, sizeof(dwPolicy));
			break;
		}
		case AeroEffect::GlassFrame: {
			margins = {
				UIWizardSettings::glassFrameLeft,
				UIWizardSettings::glassFrameRight,
				UIWizardSettings::glassFrameTop,
				UIWizardSettings::glassFrameBottom
			};
			DwmExtendFrameIntoClientArea(mainHwnd, &margins);
			break;
		}
		case AeroEffect::SheetOfGlass: {
			bb.fEnable = TRUE;
			bb.hRgnBlur = nullptr; // Apply a "sheet of glass" effect
			DwmEnableBlurBehindWindow(mainHwnd, &bb);
			break;
		}
	}
}

bool UIWizardWindow::HandleAeroEffectDisabled(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
	if (!UIWHWindow::IsAeroEffect("Disabled")) {
		return false; // Aero is not disabled, nothing to do
	}

	if (CreateWindowBgBrush()) {
		SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, reinterpret_cast<LONG_PTR>(bgBrush.get()));
	}

	return DwmDefWindowProc(hWnd, message, wParam, lParam, pResult);
}

LRESULT UIWizardWindow::HandleWindowFrame(UINT message, WPARAM wParam, LPARAM lParam) {
	if (UIWHWindow::IsFrameStyle("NoBorder")) {
		return 0;
	}

	return DefSubclassProc(mainHwnd, message, wParam, lParam);
}

bool UIWizardWindow::CreateWindowBgBrush() {
	COLORREF customColor = GetWindowBgColor();
	COLORREF defaultColor = UIWHDarkMode::IsDark() ? RGB(51, 51, 51) : RGB(240, 240, 240);

	if (customColor == CLR_INVALID) customColor = defaultColor;
	if (customColor == bgColor && bgBrush) return false;

	HBRUSH newBrush = CreateSolidBrush(customColor);

	if (!newBrush && customColor != defaultColor) {
		customColor = defaultColor;
		newBrush = CreateSolidBrush(customColor);
		if (!newBrush) return false;
	}

	bgBrush.reset(newBrush);
	bgColor = customColor;
	return true;
}

COLORREF UIWizardWindow::GetWindowBgColor() const {
	auto colorValue = static_cast<unsigned int>(UIWizardSettings::windowBgColor);

	if (colorValue == UIWizardSettings::windowBgColorDefault) {
		return CLR_INVALID; // Special value to indicate "no custom color"
	}

	// CreateSolidBrush expects a COLORREF so mask out the alpha channel if it's not needed.
	// This ensures the high-order byte is zero, complying with COLORREF's format.
	colorValue &= 0x00FFFFFF;

	return RGB((colorValue >> 16) & 0xFF, (colorValue >> 8) & 0xFF, colorValue & 0xFF);
}

void UIWizardWindow::SetWindowBgColor(WPARAM wParam) {
	RECT rect;
	GetClientRect(mainHwnd, &rect);
	FillRect((HDC)wParam, &rect, bgBrush.get());
}

void UIWizardWindow::SetBgColor(COLORREF color) {
	bgColor = color;
	CreateWindowBgBrush();
}

void UIWizardWindow::SetCustomIcon() const {
	if (!UIWizardSettings::customIcon) return;

	UIWHGraphics::SetIcon(mainHwnd, nullptr,
		UIWizardSettings::customIcon, UIWizardSettings::customIconPath.c_str()
	);
}

bool UIWizardWindow::SetTaskbarIconVisibility(bool force) const {
	_com_ptr_t<_com_IIID<ITaskbarList, &IID_ITaskbarList>> pTaskbar;

	HRESULT hr = CoCreateInstance(CLSID_TaskbarList, nullptr,
		CLSCTX_ALL, IID_ITaskbarList, reinterpret_cast<void**>(&pTaskbar));

	if (SUCCEEDED(hr) && SUCCEEDED(pTaskbar->HrInit())) {
		const BOOL hideTaskbar = UIWizardSettings::hideTaskbarIcon || force;
		return SUCCEEDED(hideTaskbar ? pTaskbar->DeleteTab(mainHwnd) : pTaskbar->AddTab(mainHwnd));
	}

	return false;
}

pfc::string8 UIWizardWindow::GetWindowTitle() const {
	metadb_handle_ptr track;
	const auto play_api = play_control::get();
	play_api->get_now_playing(track);

	pfc::string8 windowTitleStr;

	// Determine the base title: custom title name or default title name
	if (UIWizardSettings::customTitle && !UIWizardSettings::customTitleString.empty()) {
		windowTitleStr = UIWizardSettings::customTitleString.get_ptr();
	}
	else {
		auto core_info = core_version_info_v2::get();
		windowTitleStr = core_info->get_name();
		windowTitleStr += " ";
		windowTitleStr += core_info->get_version_as_text();
	}

	// If a track is currently playing, append its artist and title
	if (track.is_valid()) {
		pfc::string8 trackTitle;
		service_ptr_t<titleformat_object> to_wtitle;

		titleformat_compiler::get()->compile_safe(to_wtitle, " - %artist% - %title%");
		play_api->playback_format_title_ex(track, nullptr, trackTitle, to_wtitle, nullptr, play_control::display_level_all);

		windowTitleStr += trackTitle;
	}

	return windowTitleStr;
}

void UIWizardWindow::SetWindowTitle() const {
	const int windowTitleLen = GetWindowTextLength(mainHwnd);
	pfc::array_t<wchar_t> windowCurrentTitle;

	if (windowTitleLen > 0) {
		windowCurrentTitle.set_size(static_cast<size_t>(windowTitleLen) + 1);
		GetWindowText(mainHwnd, windowCurrentTitle.get_ptr(), static_cast<int>(windowCurrentTitle.get_size()));
	}

	pfc::string8 newTitle = GetWindowTitle();
	pfc::stringcvt::string_wide_from_utf8_fast wideTitle(newTitle);

	if (windowTitleLen == 0 || wcscmp(wideTitle.get_ptr(), windowCurrentTitle.get_ptr()) != 0) {
		SetWindowText(mainHwnd, wideTitle.get_ptr());
	}
}

void UIWizardWindow::SetWindows11RoundCorners() const {
	if (!UIWHWindow::IsWindows11()) return;

	constexpr DWORD DWMWCP_DONOTROUND = 1;
	constexpr DWORD DWMWCP_ROUND = 2;
	constexpr DWORD DWMWA_WINDOW_CORNER_PREFERENCE = 33;

	// Disable rounding for fullscreen/maximized to eliminate edge gaps, enable for normal window state
	bool windowNotNormal = (WindowIsFullscreen() || WindowIsMaximized());
	DWORD cornerPref = windowNotNormal ? DWMWCP_DONOTROUND : DWMWCP_ROUND;

	DwmSetWindowAttribute(mainHwnd, DWMWA_WINDOW_CORNER_PREFERENCE, &cornerPref, sizeof(cornerPref));
}
#pragma endregion


/////////////////////////
// * WINDOW CONTROLS * //
/////////////////////////
#pragma region Window Controls
bool UIWizardWindow::CaptionArea(const POINT& pt) const {
	RECT captionArea = {
		UIWizardSettings::captionLeft,
		UIWizardSettings::captionTop,
		UIWizardSettings::captionLeft + UIWizardSettings::captionWidth,
		UIWizardSettings::captionTop  + UIWizardSettings::captionHeight
	};

	return PtInRect(&captionArea, pt);
}

void UIWizardWindow::CaptionAreaIndicator() {
	if (!mainHwnd) return;

	// Temporarily disable WS_CLIPCHILDREN to allow drawing over children
	LONG_PTR style = GetWindowLongPtr(mainHwnd, GWL_STYLE);
	SetWindowLongPtr(mainHwnd, GWL_STYLE, style & ~WS_CLIPCHILDREN);

	// Clear last indicator
	UIWHGraphics::WindowRepaint(mainHwnd);

	CClientDC dc(mainHwnd);
	Gdiplus::Graphics graphics(dc);

	Gdiplus::Rect rect(
		UIWizardSettings::captionLeft,
		UIWizardSettings::captionTop,
		UIWizardSettings::captionWidth,
		UIWizardSettings::captionHeight
	);

	// Draw semi-transparent fill
	Gdiplus::SolidBrush brush(Gdiplus::Color(50, 255, 0, 0));
	graphics.FillRectangle(&brush, rect);

	// Draw border
	Gdiplus::Pen pen(Gdiplus::Color(255, 255, 0, 0), 2);
	graphics.DrawRectangle(&pen, rect);

	SetTimer(mainHwnd, 123, 3000, nullptr);
}

void UIWizardWindow::CaptionAreaIndicatorTimer(UINT_PTR nIDEvent) {
	if (nIDEvent != 123) return;

	KillTimer(mainHwnd, 123);

	// Restore WS_CLIPCHILDREN
	LONG_PTR style = GetWindowLongPtr(mainHwnd, GWL_STYLE);
	SetWindowLongPtr(mainHwnd, GWL_STYLE, style | WS_CLIPCHILDREN);

	UIWHGraphics::WindowRepaint(mainHwnd);
}

void UIWizardWindow::SetCaptionAreaSize(int x, int y, int w, int h) const {
	UIWizardSettings::captionLeft = x;
	UIWizardSettings::captionTop = y;
	UIWizardSettings::captionWidth = w;
	UIWizardSettings::captionHeight = h;
}

bool UIWizardWindow::DragStyle(MoveStyle moveStyle) const {
	static constexpr int KEY_PRESSED = 0x8000;
	bool ALT_KEY = GetKeyState(VK_MENU) & KEY_PRESSED;
	bool CTRL_KEY = GetKeyState(VK_CONTROL) & KEY_PRESSED;
	bool MOUSE_BTN_LEFT = GetKeyState(VK_LBUTTON) & KEY_PRESSED;
	bool MOUSE_BTN_MIDDLE = GetKeyState(VK_MBUTTON) & KEY_PRESSED;

	auto moveStyleCurrent = static_cast<MoveStyle>(
		UIWizardSettings::moveStyle.get_value()
	);

	if (moveStyleCurrent != moveStyle) {
		return false;
	}

	switch (moveStyle) {
		case MoveStyle::Default: {
			return MOUSE_BTN_LEFT && mouseInCaption;
		}
		case MoveStyle::Middle: {
			return MOUSE_BTN_MIDDLE;
		}
		case MoveStyle::Left: {
			return MOUSE_BTN_LEFT && CTRL_KEY && ALT_KEY;
		}
		case MoveStyle::Both: {
			return MOUSE_BTN_LEFT && mouseInCaption
				|| MOUSE_BTN_MIDDLE
				|| MOUSE_BTN_LEFT && CTRL_KEY && ALT_KEY;
		}
		default: return false;
	}
}

void UIWizardWindow::DragStart(LPARAM lParam, bool isClientCoords) {
	if (WindowIsFullscreen()) {
		return;
	}

	RECT rect;
	GetWindowRect(mainHwnd, &rect);

	mouseDragStart.x = GET_X_LPARAM(lParam);
	mouseDragStart.y = GET_Y_LPARAM(lParam);

	if (isClientCoords) {
		ClientToScreen(mainHwnd, &mouseDragStart);
	}

	windowDragStart.x = rect.left;
	windowDragStart.y = rect.top;

	mouseIsDragging = true;
	SetCapture(mainHwnd);
}

void UIWizardWindow::DragSnap(int& x, int& y) {
	HMONITOR hmon = MonitorFromWindow(mainHwnd, MONITOR_DEFAULTTOPRIMARY);
	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(hmon, &mi);

	RECT windowRect;
	GetWindowRect(mainHwnd, &windowRect);
	RECT clientRect;
	GetClientRect(mainHwnd, &clientRect);
	POINT clientTopLeft = { 0, 0 };
	ClientToScreen(mainHwnd, &clientTopLeft);

	int leftBorder = clientTopLeft.x - windowRect.left;
	int topBorder = clientTopLeft.y - windowRect.top;
	int rightBorder = windowRect.right - (clientTopLeft.x + clientRect.right);
	int bottomBorder = windowRect.bottom - (clientTopLeft.y + clientRect.bottom);

	// Adjust for borderless frame style
	if (UIWHWindow::IsFrameStyle("NoBorder")) {
		leftBorder = rightBorder = topBorder = bottomBorder = 0;
	}

	int windowWidth = windowRect.right - windowRect.left;
	int windowHeight = windowRect.bottom - windowRect.top;
	int clientWidth = windowWidth - leftBorder - rightBorder;
	int clientHeight = windowHeight - topBorder - bottomBorder;
	int taskbarHeight = UIWHDisplay::IsTaskbarVisible() ? mi.rcMonitor.bottom - mi.rcWork.bottom : 0;

	auto snapLogic = [&](int pos, int size, int edgeStart, int edgeEnd) {
		int distToStart = pos - edgeStart;
		int distToEnd = edgeEnd - (pos + size);
		const int snapSize = UIWizardSettings::snapSize;
		const int unsnapSize = UIWizardSettings::unsnapSize;

		if (distToStart >= 0 && distToStart <= snapSize) {
			return edgeStart;
		}
		if (distToStart > snapSize && distToStart < unsnapSize) {
			return edgeStart + unsnapSize;
		}
		if (distToEnd >= 0 && distToEnd <= snapSize) {
			return edgeEnd - size;
		}
		if (distToEnd > snapSize && distToEnd < unsnapSize) {
			return edgeEnd - size - unsnapSize;
		}
		return pos;
	};

	x = snapLogic(x + leftBorder, clientWidth, mi.rcMonitor.left, mi.rcMonitor.right) - leftBorder;
	y = snapLogic(y + topBorder, clientHeight, mi.rcMonitor.top, mi.rcMonitor.bottom - taskbarHeight) - topBorder;
}

void UIWizardWindow::DragMove() {
	if (!mouseIsDragging) return;

	POINT cursorPos;
	GetCursorPos(&cursorPos);

	int dx = cursorPos.x - mouseDragStart.x;
	int dy = cursorPos.y - mouseDragStart.y;
	int x = windowDragStart.x + dx;
	int y = windowDragStart.y + dy;

	const int moveThreshold = 10;
	bool noticeableMovement = (abs(dx) > moveThreshold || abs(dy) > moveThreshold);

	// Exit maximized window state after noticeable mouse movement (10px) drag
	// to mimic Windows titlebar restore and avoid double-click maximize conflicts
	if (mouseInCaption && WindowIsMaximized() && noticeableMovement) {
		// Capture old (maximized) rect before restore
		RECT oldRect;
		GetWindowRect(mainHwnd, &oldRect);
		double oldWidth = static_cast<double>(oldRect.right) - oldRect.left;
		double oldHeight = static_cast<double>(oldRect.bottom) - oldRect.top;

		// Calculate proportional relative click position
		double relX = (static_cast<double>(mouseDragStart.x) - oldRect.left) / oldWidth;
		double relY = (static_cast<double>(mouseDragStart.y) - oldRect.top) / oldHeight;

		ToggleMaximize();

		// Get new (restored) rect after toggle
		RECT newRect;
		GetWindowRect(mainHwnd, &newRect);
		int newWidth = newRect.right - newRect.left;
		int newHeight = newRect.bottom - newRect.top;

		// Recompute absolute relative position based on new dimensions
		auto newX = std::clamp(static_cast<int>(relX * newWidth + 0.5), 0, newWidth);
		auto newY = std::clamp(static_cast<int>(relY * newHeight + 0.5), 0, newHeight);

		// Reposition window to keep cursor at proportional relative position post-restore
		int correctX = cursorPos.x - newX;
		int correctY = cursorPos.y - newY;
		SetWindowPos(mainHwnd, nullptr, correctX, correctY, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);

		// Update drag reference points to the post-restore state
		windowDragStart.x = correctX;
		windowDragStart.y = correctY;
		mouseDragStart = cursorPos;

		// Recalculate dx, dy, x, y with updated references
		dx = cursorPos.x - mouseDragStart.x;
		dy = cursorPos.y - mouseDragStart.y;
		x = windowDragStart.x + dx;
		y = windowDragStart.y + dy;
	}

	if (UIWizardSettings::snapToEdge) {
		DragSnap(x, y);
	}

	SetWindowPos(mainHwnd, nullptr, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER);
}

void UIWizardWindow::DragEnd() {
	if (!mouseIsDragging) return;

	mouseIsDragging = false;
	ReleaseCapture();
}

void UIWizardWindow::HandleESCKey() {
	int cfgValue = UIWizardSettings::ESCKey;
	auto action = static_cast<ESCKeyAction>(cfgValue);

	if (UIWHWindow::IsWindowState("Fullscreen")) {
		UIWizard::Window()->ToggleFullscreen();
		return;
	}

	switch (action) {
		case ESCKeyAction::None: {
			// Do nothing
			break;
		}
		case ESCKeyAction::Hide: {
			ShowWindow(mainHwnd, SW_HIDE);
			break;
		}
		case ESCKeyAction::Exit: {
			PostMessage(mainHwnd, WM_CLOSE, 0, 0);
			break;
		}
	}
}

int UIWizardWindow::HandleWindowHitTest(LPARAM lParam) {
	if (UIWizardSettings::disableWindowSizing || WindowIsFullscreen()) {
		return HTCLIENT;
	}

	RECT rect;
	GetWindowRect(mainHwnd, &rect);
	POINT cursor = {
		GET_X_LPARAM(lParam) - rect.left,
		GET_Y_LPARAM(lParam) - rect.top
	};

	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	const int borderSize = 8;
	bool borderTop = cursor.y < borderSize;
	bool borderBottom = cursor.y > windowHeight - borderSize;
	bool borderLeft = cursor.x < borderSize;
	bool borderRight = cursor.x > windowWidth - borderSize;

	// Corners
	if (borderTop && borderLeft) return HTTOPLEFT;
	if (borderTop && borderRight) return HTTOPRIGHT;
	if (borderBottom && borderRight) return HTBOTTOMRIGHT;
	if (borderBottom && borderLeft) return HTBOTTOMLEFT;
	// Borders
	if (borderTop) return HTTOP;
	if (borderBottom) return HTBOTTOM;
	if (borderLeft) return HTLEFT;
	if (borderRight) return HTRIGHT;

	return HTCLIENT;
}
#pragma endregion


///////////////////////////////
// * WINDOW POSITON & SIZE * //
///////////////////////////////
#pragma region Window Size
void UIWizardWindow::LoadWindowMetrics() const {
	if (!UIWHWindow::IsWindowState("Normal")) return;
	UIWizardSettings::windowPosition.apply_to_window(mainHwnd, true);
	UIWizardSettings::windowSize.apply_to_window(mainHwnd);
}

void UIWizardWindow::SaveWindowMetrics() const {
	if (!UIWHWindow::IsWindowState("Normal")) return;
	UIWizardSettings::windowPosition.read_from_window(mainHwnd);
	UIWizardSettings::windowSize.read_from_window(mainHwnd);
}

void UIWizardWindow::HandleWindowSizeConstraints(WPARAM wParam, LPARAM lParam) const {
	auto* pRect = reinterpret_cast<RECT*>(lParam);

	const bool isRightEdge = wParam == WMSZ_RIGHT || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_TOPRIGHT;
	const bool isBottomEdge = wParam == WMSZ_BOTTOM || wParam == WMSZ_BOTTOMRIGHT || wParam == WMSZ_BOTTOMLEFT;
	const bool resizeWidth = wParam == WMSZ_LEFT || isRightEdge || wParam == WMSZ_TOPLEFT || wParam == WMSZ_BOTTOMLEFT;
	const bool resizeHeight = wParam == WMSZ_TOP || isBottomEdge || wParam == WMSZ_TOPLEFT || wParam == WMSZ_BOTTOMLEFT;

	// Enforce window size constraints if settings are true, otherwise full resize freedom
	int minWidth = UIWizardSettings::windowMinSize ? UIWizardSettings::windowMinWidth : 1;
	int minHeight = UIWizardSettings::windowMinSize ? UIWizardSettings::windowMinHeight : 1;
	int maxWidth = UIWizardSettings::windowMaxSize ? UIWizardSettings::windowMaxWidth : GetSystemMetrics(SM_CXSCREEN);
	int maxHeight = UIWizardSettings::windowMaxSize ? UIWizardSettings::windowMaxHeight : GetSystemMetrics(SM_CYSCREEN);

	if (resizeWidth) {
		int newWidth = std::clamp(static_cast<int>(pRect->right - pRect->left), minWidth, maxWidth);
		if (isRightEdge) {
			pRect->right = pRect->left + newWidth;
		}
		else {
			pRect->left = pRect->right - newWidth;
		}
	}

	if (resizeHeight) {
		int newHeight = std::clamp(static_cast<int>(pRect->bottom - pRect->top), minHeight, maxHeight);
		if (isBottomEdge) {
			pRect->bottom = pRect->top + newHeight;
		}
		else {
			pRect->top = pRect->bottom - newHeight;
		}
	}
}

void UIWizardWindow::UpdateWindowSize() const {
	// Resize the main window
	RECT rc;
	MONITORINFO mi = UIWHDisplay::GetMonitorMetrics(mainHwnd);

	if (WindowIsMaximized()) {
		rc = mi.rcWork;
		int borderWidth = GetSystemMetrics(SM_CXFRAME);  // Width of resizable border (left/right)
		int borderHeight = GetSystemMetrics(SM_CYFRAME); // Height of resizable border (top/bottom)
		int fixedBorderWidth = GetSystemMetrics(SM_CXBORDER);  // Width of non-resizable border
		int fixedBorderHeight = GetSystemMetrics(SM_CYBORDER); // Height of non-resizable border

		if (UIWHWindow::IsFrameStyle("SmallCaption") || UIWHWindow::IsFrameStyle("NoCaption")) {
			SetWindowPos(mainHwnd, nullptr,
				rc.left - (borderWidth * 2 + fixedBorderWidth), rc.top,
				rc.right - rc.left + (borderWidth * 4 + fixedBorderWidth * 2),
				rc.bottom - rc.top + (borderHeight * 2 + fixedBorderHeight),
				SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER
			);
		}
		else {
			SetWindowPos(mainHwnd, nullptr,
				rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
				SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER
			);
		}
	}
	else {
		GetWindowRect(mainHwnd, &rc);
		SetWindowPos(mainHwnd, nullptr,
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE | SWP_NOZORDER
		);
	}

	// Resize all child windows
	int childCount = UIWizard::Child()->ChildWindowTotalCount(mainHwnd);
	HDWP hdwp = BeginDeferWindowPos(childCount);
	if (!hdwp) return;

	RECT rcClient;
	GetClientRect(mainHwnd, &rcClient);
	HWND hwndChild = GetWindow(mainHwnd, GW_CHILD);

	while (hwndChild) {
		DeferWindowPos(hdwp, hwndChild, nullptr, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		hwndChild = GetWindow(hwndChild, GW_HWNDNEXT);
	}

	EndDeferWindowPos(hdwp);

	// Force an additional minimal resize to trigger proper redraw when using Columns UI
	if (GetModuleHandle(L"foo_ui_columns.dll")) {
		GetWindowRect(mainHwnd, &rc);

		SetWindowPos(mainHwnd, nullptr,
			rc.left, rc.top, rc.right - rc.left + 1, rc.bottom - rc.top,
			SWP_FRAMECHANGED | SWP_NOZORDER
		);
		SetWindowPos(mainHwnd, nullptr,
			rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
			SWP_FRAMECHANGED | SWP_NOZORDER
		);
	}

	// Force layout recalculation to update all sizes
	GetClientRect(mainHwnd, &rcClient);
	SendMessage(mainHwnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(rcClient.right, rcClient.bottom));
}
#pragma endregion


//////////////////////
// * WINDOW STATE * //
//////////////////////
#pragma region Window State
void UIWizardWindow::HandleWindowInactivity(bool fromTimer) {
	if (!UIWizardSettings::hideWindowInactivity) {
		KillTimer(mainHwnd, HIDE_WINDOW_INACTIVITY_TIMER_ID);
		return;
	}

	const ULONGLONG currentTime = GetTickCount64();
	const ULONGLONG inactivityThreshold = static_cast<ULONGLONG>(UIWizardSettings::hideWindowInactivitySec) * 1000;

	if (!fromTimer) {
		// Throttle: Update only if >500ms since last input
		if (currentTime - hideWindowLastInputTime > 500) {
			hideWindowLastActivityTime = currentTime;
			hideWindowLastInputTime = currentTime;
			// Reset the timer with the current threshold
			SetTimer(mainHwnd, HIDE_WINDOW_INACTIVITY_TIMER_ID, static_cast<UINT>(inactivityThreshold), nullptr);
		}
		return;
	}

	// Check inactivity only when the timer expires
	if (currentTime - hideWindowLastActivityTime >= inactivityThreshold) {
		PostMessage(UIWizard::Shadow()->shadowHwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		PostMessage(mainHwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
		KillTimer(mainHwnd, HIDE_WINDOW_INACTIVITY_TIMER_ID);
	}
}

void UIWizardWindow::SetWindowHideInactivity() {
	if (!UIWizardSettings::hideWindowInactivity) return;

	// Initialize activity times to current time
	ULONGLONG currentTime = GetTickCount64();
	hideWindowLastActivityTime = currentTime;
	hideWindowLastInputTime = currentTime;

	// Set timer with the inactivity threshold
	UINT inactivityThreshold = static_cast<UINT>(UIWizardSettings::hideWindowInactivitySec) * 1000;
	SetTimer(mainHwnd, HIDE_WINDOW_INACTIVITY_TIMER_ID, inactivityThreshold, nullptr);
}

void UIWizardWindow::HandleWindowFullscreenActiveState(WPARAM wParam) {
	if (!UIWHWindow::IsWindowState("Fullscreen")) return;

	LONG_PTR exStyle = GetWindowLongPtr(mainHwnd, GWL_EXSTYLE);

	if (LOWORD(wParam) != WA_INACTIVE && (exStyle & WS_EX_TOPMOST) == 0) {
		SetWindowLongPtr(mainHwnd, GWL_EXSTYLE, exStyle | WS_EX_TOPMOST);
		SetWindowPos(mainHwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
	}
	else if (exStyle & WS_EX_TOPMOST) {
		SetWindowLongPtr(mainHwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TOPMOST);
		SetWindowPos(mainHwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOOWNERZORDER);
	}
}

void UIWizardWindow::HandleWindowMinimize() {
	UIWizardSettings::windowStatePrevious = UIWizardSettings::windowState;
	UIWHWindow::SetWindowState("Minimized");

	ShowWindow(UIWizard::Shadow()->shadowHwnd, SW_HIDE);
	ShowWindow(mainHwnd, SW_MINIMIZE);

	if (UIWizardSettings::hideWindowInactivity) {
		KillTimer(mainHwnd, HIDE_WINDOW_INACTIVITY_TIMER_ID);
	}
}

void UIWizardWindow::HandleWindowMaximize(LPARAM lParam) const {
	// Handle WM_GETMINMAXINFO for NoBorder (WS_POPUP) frame style to prevent
	// maximization to the full monitor rect (including taskbar), which causes a visible glitch.
	// Instead, constrain to the work area (excluding taskbar) to mimic WS_OVERLAPPEDWINDOW behavior.
	if (!UIWHWindow::IsFrameStyle("NoBorder")) return;

	auto pmmi = reinterpret_cast<LPMINMAXINFO>(lParam);
	MONITORINFO mi = UIWHDisplay::GetMonitorMetrics(mainHwnd);
	RECT rcWork = mi.rcWork;

	// Set max position to work area top-left and max size to work area dimensions.
	pmmi->ptMaxPosition.x = rcWork.left;
	pmmi->ptMaxPosition.y = rcWork.top;
	pmmi->ptMaxSize.x = rcWork.right - rcWork.left;
	pmmi->ptMaxSize.y = rcWork.bottom - rcWork.top;

	// Prevent dragging beyond work area if resizable.
	pmmi->ptMaxTrackSize.x = pmmi->ptMaxSize.x;
	pmmi->ptMaxTrackSize.y = pmmi->ptMaxSize.y;
}

void UIWizardWindow::HandleWindowRestore(WPARAM wParam) {
	UIWHWindow::SetWindowState(UIWHWindow::GetWindowState(true));

	ShowWindow(UIWizard::Shadow()->shadowHwnd, SW_RESTORE);
	UIWizard::Shadow()->ShadowWindowActiveState(wParam);
	ShowWindow(mainHwnd, SW_RESTORE);

	if (UIWizardSettings::hideWindowInactivity) {
		ULONGLONG currentTime = GetTickCount64();
		hideWindowLastActivityTime = currentTime;
		hideWindowLastInputTime = currentTime;
		SetWindowHideInactivity();
	}
}

void UIWizardWindow::HandleWindowState(std::string_view windowState, bool enterState) {
	if (windowState == "Fullscreen") {
		ToggleFullscreen(enterState);
	}
	else if (windowState == "Maximized") {
		ToggleMaximize(enterState);
	}
	else if (!enterState) {
		UIWHWindow::SetWindowState("Normal");
		LoadWindowMetrics();
		ShowWindow(mainHwnd, SW_RESTORE);
	}
}

void UIWizardWindow::SetFullscreenSize() {
	LONG_PTR savedExStyle = GetWindowLongPtr(mainHwnd, GWL_EXSTYLE);
	SetWindowLongPtr(mainHwnd, GWL_EXSTYLE, savedExStyle | WS_EX_TOPMOST);

	// Get current styles for AdjustWindowRectEx
	auto style = static_cast<DWORD>(GetWindowLongPtr(mainHwnd, GWL_STYLE));
	auto exstyle = static_cast<DWORD>(GetWindowLongPtr(mainHwnd, GWL_EXSTYLE));

	MONITORINFO mi = UIWHDisplay::GetMonitorMetrics(mainHwnd);
	int monitorWidth = mi.rcMonitor.right - mi.rcMonitor.left;
	int monitorHeight = mi.rcMonitor.bottom - mi.rcMonitor.top;
	RECT windowRect = { 0, 0, monitorWidth, monitorHeight };
	AdjustWindowRectEx(&windowRect, style, FALSE, exstyle);

	SetWindowPos(mainHwnd, HWND_TOPMOST,
		mi.rcMonitor.left + windowRect.left,
		mi.rcMonitor.top + windowRect.top,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		SWP_NOSENDCHANGING
		// Workaround hack bypassing WM_GETMINMAXINFO to allow resizing larger than screen res
		// because of hidden titlebar at the top
	);
}

void UIWizardWindow::ToggleFullscreen(bool forceEnterFullscreen, bool forceExitFullscreen) {
	static LONG_PTR savedExStyle = 0; // To restore topmost state

	// Enter fullscreen
	if (forceEnterFullscreen || !forceExitFullscreen && !UIWHWindow::IsWindowState("Fullscreen")) {
		SaveWindowMetrics();

		if (UIWHWindow::IsWindowState("Maximized")) {
			// Remove WS_MAXIMIZE flag to treat as "normal" (but keep current large pos/size)
			LONG_PTR style = GetWindowLongPtr(mainHwnd, GWL_STYLE);
			style &= ~WS_MAXIMIZE;
			SetWindowLongPtr(mainHwnd, GWL_STYLE, style);

			// Update placement showCmd to SW_NORMAL (ensures state checks see it as non-max)
			WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
			GetWindowPlacement(mainHwnd, &wp);
			wp.showCmd = SW_NORMAL;
			SetWindowPlacement(mainHwnd, &wp);
		}

		UIWHWindow::SetWindowState("Fullscreen");

		// Make topmost to cover taskbar
		savedExStyle = GetWindowLongPtr(mainHwnd, GWL_EXSTYLE);
		SetWindows11RoundCorners();
		SetFullscreenSize();
	}
	// Exit fullscreen
	else {
		UIWHWindow::SetWindowState("Normal");

		SetWindowLongPtr(mainHwnd, GWL_EXSTYLE, savedExStyle); // Restore original exstyle (remove topmost)
		SetWindowPos(mainHwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOOWNERZORDER
		);
		LoadWindowMetrics();
		SetWindows11RoundCorners();

		ShowWindow(mainHwnd, SW_SHOW); // Needed when changing from maximize to fullscreen to reset and correct windows state
		ShowWindow(mainHwnd, SW_RESTORE);
	}
}

void UIWizardWindow::ToggleMaximize(bool forceEnterMaximize, bool forceExitMaximize) {
	if (UIWizardSettings::disableWindowMaximizing) {
		return;
	}

	// Enter maximized
	if (forceEnterMaximize || !forceExitMaximize && !UIWHWindow::IsWindowState("Maximized")) {
		SaveWindowMetrics();

		if (UIWHWindow::IsWindowState("Fullscreen")) {
			// Remove WS_EX_TOPMOST to treat as "normal"
			LONG_PTR exStyle = GetWindowLongPtr(mainHwnd, GWL_EXSTYLE);
			SetWindowLongPtr(mainHwnd, GWL_EXSTYLE, exStyle & ~WS_EX_TOPMOST);
			SetWindowPos(mainHwnd, HWND_NOTOPMOST, 0, 0, 0, 0,
				SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED | SWP_NOOWNERZORDER
			);
		}

		UIWHWindow::SetWindowState("Maximized");
		SetWindows11RoundCorners();
		ShowWindow(mainHwnd, SW_MAXIMIZE);
		UpdateWindowSize();
	}
	// Exit maximized
	else {
		UIWHWindow::SetWindowState("Normal");
		LoadWindowMetrics();
		SetWindows11RoundCorners();
		ShowWindow(mainHwnd, SW_RESTORE);
	}
}

void UIWizardWindow::ToggleMaximizeButtonState() {
	LONG_PTR style = GetWindowLongPtr(mainHwnd, GWL_STYLE);
	bool maximizeEnabled = (style & WS_MAXIMIZEBOX) != 0;

	if (UIWizardSettings::disableWindowMaximizing == !maximizeEnabled) {
		return;
	}

	style = UIWizardSettings::disableWindowMaximizing ? (style & ~WS_MAXIMIZEBOX) : (style | WS_MAXIMIZEBOX);
	SetWindowLongPtr(mainHwnd, GWL_STYLE, style);
	SetWindowPos(mainHwnd, nullptr, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

bool UIWizardWindow::WindowIsFullscreen() const {
	RECT windowRect;
	GetWindowRect(mainHwnd, &windowRect);

	MONITORINFO mi = { sizeof(mi) };
	GetMonitorInfo(MonitorFromWindow(mainHwnd, MONITOR_DEFAULTTONEAREST), &mi);
	RECT screenRect = mi.rcMonitor;

	bool isMaximized = (GetWindowLongPtr(mainHwnd, GWL_STYLE) & WS_MAXIMIZE) != 0;
	bool fullscreen = !isMaximized && EqualRect(&windowRect, &screenRect)
		|| UIWHWindow::IsWindowState("Fullscreen");

	if (fullscreen) UIWHWindow::SetWindowState("Fullscreen");

	return fullscreen;
}

bool UIWizardWindow::WindowIsMaximized() const {
	WINDOWPLACEMENT wp = { sizeof(WINDOWPLACEMENT) };
	GetWindowPlacement(mainHwnd, &wp);

	bool maximized = (wp.showCmd == SW_MAXIMIZE
		|| (GetWindowLong(mainHwnd, GWL_STYLE) & WS_MAXIMIZE))
		|| UIWHWindow::IsWindowState("Maximized");

	if (maximized) UIWHWindow::SetWindowState("Maximized");

	return maximized;
}
#pragma endregion


/////////////////////
// * MAIN WINDOW * //
/////////////////////
#pragma region Main Window
UIWizardMainWindow::UIWizardMainWindow(HWND hWnd) : UIWizardWindow(hWnd) {
}

UIWizardMainWindow::~UIWizardMainWindow() {
	MainWindowUnsubclass(mainHwnd);
}

LRESULT CALLBACK UIWizardMainWindow::MainWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
	// Handle AeroEffect::Disabled
	if (LRESULT dwmResult = 0; UIWizard::Main()->HandleAeroEffectDisabled(hWnd, message, wParam, lParam, &dwmResult)) {
		return dwmResult; // Early return if DWM handled the message
	}

	switch (message) {
		// Window Appearance
		case WM_ACTIVATE: {
			UIWizard::Shadow()->ShadowWindowActiveState(wParam);
			UIWizard::Window()->HandleWindowFullscreenActiveState(wParam);
			break;
		}
		case WM_ERASEBKGND: {
			UIWizard::Window()->CreateWindowBgBrush();
			UIWizard::Window()->SetWindowBgColor(wParam);
			return 1;
		}
		case WM_NCACTIVATE: {
			if (UIWHWindow::IsFrameStyle("NoBorder") || UIWHWindow::IsWindowState("Fullscreen")) {
				return TRUE;
			}
			break;
		}

		// Window Mouse Events
		case WM_LBUTTONDBLCLK: {
			if (UIWizard::Window()->mouseInCaption) {
				UIWizard::Window()->ToggleMaximize();
			}
			break;
		}
		case WM_LBUTTONDOWN:
		case WM_MBUTTONDOWN: {
			UIWizard::Window()->DragStart(lParam, true);
			UIWizard::Window()->HandleWindowInactivity();
			break;
		}
		case WM_NCLBUTTONDOWN: {
			if (wParam == HTCAPTION) {
				UIWizard::Window()->DragStart(lParam, false);
				SetForegroundWindow(hWnd);
				return 0;
			}
			break;
		}
		case WM_LBUTTONUP:
		case WM_MBUTTONUP: {
			UIWizard::Window()->DragEnd();
			break;
		}
		case WM_MOUSEMOVE: {
			UIWizard::Window()->DragMove();
			UIWizard::Window()->HandleWindowInactivity();
			break;
		}
		case WM_NCHITTEST: {
			if (!UIWHWindow::IsAeroEffect("Disabled")) {
				return UIWizard::Window()->HandleWindowHitTest(lParam);
			}
		}
		case WM_SYSCOMMAND: {
			switch (wParam & 0xFFF0) {
				case SC_MOUSEMENU: {
					UIWizardMainMenu::HandleMainMenuSystemMenu();
					break;
				}
				case SC_MAXIMIZE: {
					UIWizard::Window()->ToggleMaximize(true);
					break;
				}
				case SC_MINIMIZE: {
					UIWizard::Window()->HandleWindowMinimize();
					return 0;
				}
				case SC_RESTORE: {
					UIWizard::Window()->HandleWindowRestore(wParam);
					UIWizard::Window()->HandleWindowInactivity();
					return 0;
				}
			}
			break;
		}

		// Window Keyboard Events
		case WM_KEYDOWN: {
			UIWizard::Window()->HandleESCKey();
			UIWizard::Window()->HandleWindowInactivity();
			break;
		}

		// Window Position and Sizing
		case WM_GETMINMAXINFO: {
			UIWizard::Window()->HandleWindowMaximize(lParam);
			return 0;
		}
		case WM_NCCALCSIZE: {
			return UIWizard::Window()->HandleWindowFrame(message, wParam, lParam);
		}
		case WM_SIZE: {
			UIWizard::Window()->windowMinimized = wParam == SIZE_MINIMIZED;
			UIWizard::Shadow()->ShadowWindowUpdate();
			break;
		}
		case WM_SIZING: {
			UIWizard::Window()->HandleWindowSizeConstraints(wParam, lParam);
			UIWizard::Shadow()->ShadowWindowUpdate();
			break;
		}
		case WM_WINDOWPOSCHANGED: {
			UIWizard::Shadow()->ShadowWindowUpdate();
			break;
		}
		case WM_WINDOWPOSCHANGING: {
			if (!UIWizard::Window()->windowMinimized && UIWHWindow::IsWindowState("Normal")) {
				UIWizard::Shadow()->ShadowWindowUpdate();
			}
			break;
		}

		// Window Timer
		case WM_TIMER: {
			UIWizard::Window()->CaptionAreaIndicatorTimer(wParam);
			UIWizard::Window()->HandleWindowInactivity(true);
			break;
		}

		default: {
			return DefSubclassProc(hWnd, message, wParam, lParam);
		}
	}

	return DefSubclassProc(hWnd, message, wParam, lParam);
}

void UIWizardMainWindow::MainWindowSubclass(HWND hwndParent) const {
	SetWindowSubclass(hwndParent, MainWindowProc, 0, 0);
}

void UIWizardMainWindow::MainWindowUnsubclass(HWND hwndParent) const {
	RemoveWindowSubclass(hwndParent, MainWindowProc, 0);
}
#pragma endregion


//////////////////////
// * CHILD WINDOW * //
//////////////////////
#pragma region Child Window
UIWizardChildWindow::UIWizardChildWindow(HWND hWnd) : UIWizardWindow(hWnd) {
}

UIWizardChildWindow::~UIWizardChildWindow() {
	ChildWindowUnsubclass(mainHwnd);
}

LRESULT CALLBACK UIWizardChildWindow::ChildWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam, UINT_PTR, DWORD_PTR) {
	static POINT lastCursorPos = { 0, 0 };

	switch (message) {
		case WM_LBUTTONDBLCLK: {
			PostMessage(UIWizard::Window()->mainHwnd, message, (WPARAM)hWnd, lParam);
			break;
		}
		case WM_LBUTTONDOWN: {
			if (UIWizard::Window()->DragStyle(MoveStyle::Default) ||
				UIWizard::Window()->DragStyle(MoveStyle::Left) ||
				UIWizard::Window()->DragStyle(MoveStyle::Both)) {
				PostMessage(UIWizard::Window()->mainHwnd, message, (WPARAM)hWnd, lParam);
			}
			break;
		}
		case WM_MBUTTONDOWN: {
			if (UIWizard::Window()->DragStyle(MoveStyle::Middle) ||
				UIWizard::Window()->DragStyle(MoveStyle::Both)) {
				PostMessage(UIWizard::Window()->mainHwnd, message, (WPARAM)hWnd, lParam);
			}
			break;
		}
		case WM_KEYDOWN: {
			if (wParam == VK_ESCAPE) {
				PostMessage(UIWizard::Window()->mainHwnd, message, (WPARAM)hWnd, lParam);
			}
			break;
		}
		case WM_NCHITTEST: {
			UIWizard::Window()->HandleWindowInactivity();

			POINT cursorPos;
			GetCursorPos(&cursorPos);
			ScreenToClient(UIWizard::Window()->mainHwnd, &cursorPos);

			RECT clientRect;
			GetClientRect(UIWizard::Window()->mainHwnd, &clientRect);
			lastCursorPos = cursorPos;

			UIWizard::Window()->mouseInCaption = UIWizard::Window()->CaptionArea(lastCursorPos);

			constexpr int borderSize = 5;
			bool borderTop = cursorPos.y <= borderSize;
			bool borderBottom = cursorPos.y >= clientRect.bottom - borderSize;
			bool borderLeft = cursorPos.x <= borderSize;
			bool borderRight = cursorPos.x >= clientRect.right - borderSize;

			if (borderTop || borderBottom || borderLeft || borderRight) {
				return UIWizard::Window()->WindowIsMaximized() ? HTCLIENT : HTTRANSPARENT;
			}
			break;
		}

		default: {
			return DefSubclassProc(hWnd, message, wParam, lParam);
		}
	}

	return DefSubclassProc(hWnd, message, wParam, lParam);
}

BOOL CALLBACK UIWizardChildWindow::ChildWindowEnumProc(HWND hwndChild, LPARAM) {
	SetWindowSubclass(hwndChild, ChildWindowProc, 1, 0);
	return TRUE; // Continue enumeration
}

void UIWizardChildWindow::ChildWindowSubclass(HWND hwndParent) const {
	EnumChildWindows(hwndParent, ChildWindowEnumProc, 0);
}

void UIWizardChildWindow::ChildWindowUnsubclass(HWND hwndParent) const {
	EnumChildWindows(hwndParent, [](HWND hwndChild, LPARAM) {
		RemoveWindowSubclass(hwndChild, ChildWindowProc, 1);
		return TRUE; // Continue enumeration
	}, 0);
}

int UIWizardChildWindow::ChildWindowTotalCount(HWND hWndParent) const {
	int count = 0;
	HWND hwndChild = GetWindow(hWndParent, GW_CHILD);

	while (hwndChild) {
		++count;
		hwndChild = GetWindow(hwndChild, GW_HWNDNEXT);
	}

	return count;
}
#pragma endregion


///////////////////////
// * SHADOW WINDOW * //
///////////////////////
#pragma region Shadow Window
UIWizardShadowWindow::UIWizardShadowWindow(HWND hWnd) : UIWizardWindow(hWnd) {
}

UIWizardShadowWindow::~UIWizardShadowWindow() {
	if (IsWindow(shadowHwnd)) {
		ShowWindow(shadowHwnd, SW_HIDE);
		DestroyWindow(shadowHwnd);
	}

	shadowHwnd = nullptr;
}

LRESULT CALLBACK UIWizardShadowWindow::ShadowWindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		// Window Appearance
		case WM_ERASEBKGND: {
			return 1; // Prevents flicker
		}
		case WM_PAINT: {
			PAINTSTRUCT ps;
			BeginPaint(hWnd, &ps);
			EndPaint(hWnd, &ps);
			return 0; // Validates the region but draw nothing
		}

		// Window Mouse Events
		case WM_NCHITTEST: {
			return HTTRANSPARENT; // Disables any window control
		}

		// Window Sizing and Position
		case WM_NCCALCSIZE: {
			return 0;
		}

		default: {
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND UIWizardShadowWindow::ShadowWindow() {
	if (shadowHwnd != nullptr) {
		return shadowHwnd; // Do not create a new window if there already is one!
	}

	const wchar_t* className = L"UI Wizard Borderless Window Shadow";
	WNDCLASSW wc = {};

	if (!GetClassInfoW(GetModuleHandle(nullptr), className, &wc)) {
		wc.lpfnWndProc = ShadowWindowProc;
		wc.lpszClassName = className;
		wc.hInstance = core_api::get_my_instance();
		wc.style = CS_DBLCLKS;
		RegisterClassW(&wc);
	}

	RECT parentRect;
	GetWindowRect(UIWizard::Window()->mainHwnd, &parentRect);

	shadowHwnd = CreateWindowExW(
		WS_EX_NOACTIVATE | WS_EX_COMPOSITED | WS_EX_TRANSPARENT | WS_EX_TOOLWINDOW,
		className, L"",
		WS_THICKFRAME,
		parentRect.left + 1, parentRect.top + 2,
		parentRect.right - parentRect.left - 4, parentRect.bottom - parentRect.top - 4,
		nullptr, nullptr, core_api::get_my_instance(), nullptr
	);

	if (!shadowHwnd) return nullptr;

	ShadowWindowDisplay();

	BOOL dwmTransitionsDisabled = TRUE;
	DwmSetWindowAttribute(shadowHwnd,
		DWMWA_TRANSITIONS_FORCEDISABLED, &dwmTransitionsDisabled, sizeof(dwmTransitionsDisabled)
	);

	SetClassLongPtr(shadowHwnd, GCLP_HBRBACKGROUND, (LONG_PTR)GetStockObject(HOLLOW_BRUSH));

	ShowWindow(shadowHwnd, SW_SHOW);

	// Set the shadow window activation state immediately after creation for full shadow effect
	ShadowWindowActiveState(0, true);

	// Explicitly activate and focus the main window
	SetForegroundWindow(UIWizard::Window()->mainHwnd);
	SetFocus(UIWizard::Window()->mainHwnd);

	return shadowHwnd;
}

void UIWizardShadowWindow::ShadowWindowActiveState(WPARAM wParam, bool forceActive) const {
	if (!shadowHwnd) return;

	BOOL isActive =
		forceActive || (!UIWizardSettings::enableWindowTransparency &&
		!UIWHWindow::IsAeroEffect("SheetOfGlass") && LOWORD(wParam) != WA_INACTIVE);

	// Forward activation state to the shadow window for active/non-active shadow effect and then to main window
	PostMessage(shadowHwnd, WM_NCACTIVATE, isActive, 0);
	PostMessage(UIWizard::Window()->mainHwnd, WM_NCACTIVATE, isActive, 0);
}

void UIWizardShadowWindow::ShadowWindowDisplay() const {
	const auto currentFrameStyle = static_cast<FrameStyle>(
		UIWizardSettings::frameStyle.get_value()
	);
	static FrameStyle lastFrameStyle = FrameStyle::Default;
	const bool currentWindowShadow = UIWizardSettings::hideWindowShadow;
	static bool lastWindowShadow = UIWizardSettings::hideWindowShadow;

	if (currentFrameStyle == lastFrameStyle &&
		currentWindowShadow == lastWindowShadow) {
		return;
	}

	lastFrameStyle = currentFrameStyle;
	lastWindowShadow = currentWindowShadow;

	const bool hideShadow = currentFrameStyle != FrameStyle::NoBorder || UIWizardSettings::hideWindowShadow;
	static const MARGINS shadowMargins = { 1, 1, 1, 1 };
	static const MARGINS noShadowMargins = { 0, 0, 0, 0 };
	const MARGINS& margins = hideShadow ? noShadowMargins : shadowMargins;

	DwmExtendFrameIntoClientArea(shadowHwnd, &margins);
}

void UIWizardShadowWindow::ShadowWindowUpdate() const {
	if (!shadowHwnd) return;

	RECT parentRect;
	GetWindowRect(UIWizard::Window()->mainHwnd, &parentRect);

	SetWindowPos(shadowHwnd, UIWizard::Window()->mainHwnd,
		parentRect.left + 1, parentRect.top + 2,
		parentRect.right - parentRect.left - 4, parentRect.bottom - parentRect.top - 4,
		SWP_NOACTIVATE | SWP_NOCOPYBITS | SWP_NOREDRAW
	);

	DwmFlush();
}
#pragma endregion

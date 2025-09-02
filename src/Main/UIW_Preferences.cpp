/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Preferences Source File                       * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "UIW.h"
#include "UIW_Helpers.h"
#include "UIW_Preferences.h"
#include "UIW_Settings.h"


/////////////////////
// * CONSTRUCTOR * //
/////////////////////
#pragma region Constructor
UIWizardPreferences::UIWizardPreferences(preferences_page_callback::ptr callback)
	: prefCallback(callback) {
}
#pragma endregion


////////////////////////
// * INITIALIZATION * //
////////////////////////
#pragma region Initialization
void UIWizardPreferences::InitFonts() {
	if (fontGroupbox.IsNull()) {
		UIWHDialog::CreateCustomFont(fontGroupbox, 26, FW_BOLD);
	}
	CWindow(GetDlgItem(IDC_APPEARANCE_GROUP)).SetFont(fontGroupbox, FALSE);
	CWindow(GetDlgItem(IDC_CONTROLS_GROUP)).SetFont(fontGroupbox, FALSE);

	if (fontSmall.IsNull()) {
		UIWHDialog::CreateCustomFont(fontSmall, 14, FW_SEMIBOLD);
	}
	CWindow(GetDlgItem(IDC_CAPTION_AREA_TEXT)).SetFont(fontSmall, FALSE);
	CWindow(GetDlgItem(IDC_GLASS_FRAME_AREA_TEXT)).SetFont(fontSmall, FALSE);

	CWindow(GetDlgItem(IDC_SNAP_SIZE_TEXT)).SetFont(fontSmall, FALSE);
	CWindow(GetDlgItem(IDC_UNSNAP_SIZE_TEXT)).SetFont(fontSmall, FALSE);
}

void UIWizardPreferences::InitFrameStyle() {
	UIWHDialog::SetDropDownMenu(m_hWnd, IDC_FRAME_STYLE, {
		L"Default",
		L"Small Caption",
		L"No Caption",
		L"No Border"
	}, UIWizardSettings::frameStyle);
}

void UIWizardPreferences::InitAeroEffects() {
	UIWHDialog::SetDropDownMenu(m_hWnd, IDC_AERO_EFFECT, {
		L"Default",
		L"Disabled",
		L"Glass frame",
		L"Sheet of glass",
	}, UIWizardSettings::aeroEffect);

	UIWHDialog::SetCheckBox(m_hWnd, IDC_HIDE_WINDOW_SHADOW, UIWizardSettings::hideWindowShadow);
	UIWHDialog::SetCheckBox(m_hWnd, IDC_ENABLE_WINDOW_TRANSPARENCY, UIWizardSettings::enableWindowTransparency);
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_TRANSPARENCY, UIWizardSettings::windowTransparency);
}

void UIWizardPreferences::InitGlassFrameArea() {
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_LEFT, UIWizardSettings::glassFrameLeft);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_GLASS_FRAME_LEFT_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_GLASS_FRAME_LEFT_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_TOP, UIWizardSettings::glassFrameTop);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_GLASS_FRAME_TOP_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_GLASS_FRAME_TOP_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_RIGHT, UIWizardSettings::glassFrameRight);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_GLASS_FRAME_RIGHT_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_GLASS_FRAME_RIGHT_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_BOTTOM, UIWizardSettings::glassFrameBottom);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_GLASS_FRAME_BOTTOM_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_GLASS_FRAME_BOTTOM_SPIN));
}

void UIWizardPreferences::InitMenuSettings() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_HIDDEN_MENU, UIWizardSettings::showHiddenMenu);
	UIWHDialog::SetCheckBox(m_hWnd, IDC_MAIN_MENU_SYSTEM_MENU, UIWizardSettings::mainMenuSystemMenu);
}

void UIWizardPreferences::InitCustomTitleName() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_CUSTOM_TITLE, UIWizardSettings::customTitle);
	auto customTitleString = std::wstring(pfc::stringcvt::string_wide_from_utf8(UIWizardSettings::customTitleString.c_str()).get_ptr());
	UIWHDialog::SetInputFieldText(m_hWnd, IDC_CUSTOM_TITLE_STRING, customTitleString);
}

void UIWizardPreferences::InitCustomIcon() {
	UIWHGraphics::SetIcon(UIWizard::MainWindow(), m_hWnd, UIWizardSettings::customIcon, UIWizardSettings::customIconPath.c_str());
	UIWHDialog::SetCheckBox(m_hWnd, IDC_CUSTOM_ICON, UIWizardSettings::customIcon);

	auto customIconPath = std::wstring(pfc::stringcvt::string_wide_from_utf8(UIWizardSettings::customIconPath.c_str()).get_ptr());
	UIWHDialog::SetInputFieldText(m_hWnd, IDC_CUSTOM_ICON_PATH, customIconPath);

	UIWHDialog::SetCheckBox(m_hWnd, IDC_TASKBAR_ICON, UIWizardSettings::hideTaskbarIcon);
}

void UIWizardPreferences::InitMoveStyle() {
	UIWHDialog::SetDropDownMenu(m_hWnd, IDC_MOVE_STYLE, {
		L"Caption only",
		L"Middle mouse button",
		L"Ctrl + Alt + Left mouse button",
		L"Any method"
	}, UIWizardSettings::moveStyle);
}

void UIWizardPreferences::InitSnapToEdges() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_SNAP_EDGE, UIWizardSettings::snapToEdge);

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_SNAP_SIZE, UIWizardSettings::snapSize);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_SNAP_SIZE_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_SNAP_SIZE_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_UNSNAP_SIZE, UIWizardSettings::unsnapSize);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_UNSNAP_SIZE_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_UNSNAP_SIZE_SPIN));
}

void UIWizardPreferences::InitEscKey() {
	UIWHDialog::SetDropDownMenu(m_hWnd, IDC_ESC_KEY, {
		L"None",
		L"Hide",
		L"Exit"
	}, UIWizardSettings::ESCKey);
}

void UIWizardPreferences::InitCaptionArea() {
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_CAPTION_LEFT, UIWizardSettings::captionLeft);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_CAPTION_LEFT_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_CAPTION_LEFT_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_CAPTION_TOP, UIWizardSettings::captionTop);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_CAPTION_TOP_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_CAPTION_TOP_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_CAPTION_WIDTH, UIWizardSettings::captionWidth);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_CAPTION_WIDTH_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_CAPTION_WIDTH_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_CAPTION_HEIGHT, UIWizardSettings::captionHeight);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_CAPTION_HEIGHT_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_CAPTION_HEIGHT_SPIN));
}

void UIWizardPreferences::InitWindowConstraints() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_DISABLE_WINDOW_MAXIMIZING, UIWizardSettings::disableWindowMaximizing);
	UIWHDialog::SetCheckBox(m_hWnd, IDC_DISABLE_WINDOW_SIZING, UIWizardSettings::disableWindowSizing);
}

void UIWizardPreferences::InitMinSizeConstraints() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_WINDOW_MIN_SIZE, UIWizardSettings::windowMinSize);

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MIN_WIDTH, UIWizardSettings::windowMinWidth);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_WINDOW_MIN_WIDTH_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_WINDOW_MIN_WIDTH_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MIN_HEIGHT, UIWizardSettings::windowMinHeight);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_WINDOW_MIN_HEIGHT_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_WINDOW_MIN_HEIGHT_SPIN));
}

void UIWizardPreferences::InitMaxSizeConstraints() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_WINDOW_MAX_SIZE, UIWizardSettings::windowMaxSize);

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MAX_WIDTH, UIWizardSettings::windowMaxWidth);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_WINDOW_MAX_WIDTH_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_WINDOW_MAX_WIDTH_SPIN));

	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MAX_HEIGHT, UIWizardSettings::windowMaxHeight);
	UIWHDialog::SetSpinControlRange(m_hWnd, IDC_WINDOW_MAX_HEIGHT_SPIN, 0, 9999);
	UIWHDialog::SpinControlSubclass(GetDlgItem(IDC_WINDOW_MAX_HEIGHT_SPIN));
}

void UIWizardPreferences::InitHideWindowInactivity() {
	UIWHDialog::SetCheckBox(m_hWnd, IDC_HIDE_WINDOW_INACTIVITY, UIWizardSettings::hideWindowInactivity);
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_HIDE_WINDOW_INACTIVITY_SEC, UIWizardSettings::hideWindowInactivitySec);
}

void UIWizardPreferences::InitAllControlEnableState() {
	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_GLASS_FRAME_AREA_TEXT,
		IDC_GLASS_FRAME_LEFT, IDC_GLASS_FRAME_LEFT_SPIN,
		IDC_GLASS_FRAME_TOP, IDC_GLASS_FRAME_TOP_SPIN,
		IDC_GLASS_FRAME_RIGHT, IDC_GLASS_FRAME_RIGHT_SPIN,
		IDC_GLASS_FRAME_BOTTOM, IDC_GLASS_FRAME_BOTTOM_SPIN
	}, UIWHDialog::GetDropDownIndex(m_hWnd, IDC_AERO_EFFECT) == 2);

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_HIDE_WINDOW_SHADOW
	}, UIWHDialog::GetDropDownIndex(m_hWnd, IDC_FRAME_STYLE) == 3);

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_WINDOW_TRANSPARENCY
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_ENABLE_WINDOW_TRANSPARENCY));

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_MAIN_MENU_SYSTEM_MENU
	}, UIWHDialog::GetDropDownIndex(m_hWnd, IDC_FRAME_STYLE) == 0);

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_CUSTOM_TITLE_STRING
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_CUSTOM_TITLE));

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_CUSTOM_ICON_PATH, IDC_CUSTOM_ICON_BUTTON, IDC_CUSTOM_ICON_IMG
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_CUSTOM_ICON));

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_CAPTION_AREA_TEXT,
		IDC_CAPTION_LEFT, IDC_CAPTION_LEFT_SPIN,
		IDC_CAPTION_TOP, IDC_CAPTION_TOP_SPIN,
		IDC_CAPTION_WIDTH, IDC_CAPTION_WIDTH_SPIN,
		IDC_CAPTION_HEIGHT, IDC_CAPTION_HEIGHT_SPIN
	}, UIWHDialog::GetDropDownIndex(m_hWnd, IDC_FRAME_STYLE) == 2 || UIWHDialog::GetDropDownIndex(m_hWnd, IDC_FRAME_STYLE) == 3);

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_SNAP_SIZE_TEXT, IDC_SNAP_SIZE,
		IDC_UNSNAP_SIZE_TEXT, IDC_UNSNAP_SIZE
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_SNAP_EDGE));

	UIWHDialog::SetControlEnableState(m_hWnd, { 
		IDC_WINDOW_MIN_WIDTH, IDC_WINDOW_MIN_WIDTH_SPIN, IDC_WINDOW_MIN_WIDTH_BUTTON,
		IDC_WINDOW_MIN_HEIGHT, IDC_WINDOW_MIN_HEIGHT_SPIN, IDC_WINDOW_MIN_HEIGHT_BUTTON
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_WINDOW_MIN_SIZE));

	UIWHDialog::SetControlEnableState(m_hWnd, { 
		IDC_WINDOW_MAX_WIDTH, IDC_WINDOW_MAX_WIDTH_SPIN, IDC_WINDOW_MAX_WIDTH_BUTTON,
		IDC_WINDOW_MAX_HEIGHT, IDC_WINDOW_MAX_HEIGHT_SPIN, IDC_WINDOW_MAX_HEIGHT_BUTTON
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_WINDOW_MAX_SIZE));

	UIWHDialog::SetControlEnableState(m_hWnd, {
		IDC_HIDE_WINDOW_INACTIVITY_SEC
	}, UIWHDialog::GetCheckBoxState(m_hWnd, IDC_HIDE_WINDOW_INACTIVITY));
}
#pragma endregion


/////////////////////////
// * EVENT LISTENERS * //
/////////////////////////
#pragma region Event Listeners
BOOL UIWizardPreferences::OnInitDialog(CWindow, LPARAM) {
	initDialog = true;
	UIWHDarkMode::AddDialogWithControls(*this);

	InitFonts();
	InitFrameStyle();
	InitAeroEffects();
	InitGlassFrameArea();
	InitMenuSettings();
	InitCustomIcon();
	InitCustomTitleName();
	InitMoveStyle();
	InitEscKey();
	InitCaptionArea();
	InitSnapToEdges();
	InitWindowConstraints();
	InitMinSizeConstraints();
	InitMaxSizeConstraints();
	InitHideWindowInactivity();
	InitAllControlEnableState();

	initDialog = false;
	return TRUE;
}

void UIWizardPreferences::OnSettingChanged(UINT, int, CWindow) {
	hasChanged = true;
	prefCallback->on_state_changed();
	InitAllControlEnableState();
}

void UIWizardPreferences::OnGlassFrameEdit(UINT, int, CWindow) {
	if (initDialog) return;

	hasChanged = true;
	prefCallback->on_state_changed();

	UIWizardSettings::glassFrameLeft = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_LEFT);
	UIWizardSettings::glassFrameTop = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_TOP);
	UIWizardSettings::glassFrameRight = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_RIGHT);
	UIWizardSettings::glassFrameBottom = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_BOTTOM);

	UIWizard::Window()->SetAeroEffect(UIWizardWindow::AeroEffect::GlassFrame, true);
	UIWHGraphics::WindowRepaint(m_hWnd);
}

void UIWizardPreferences::OnCustomTitleEdit(UINT, int, CWindow) {
	hasChanged = true;
	prefCallback->on_state_changed();
	UIWizard::Window()->SetWindowTitle();
}

void UIWizardPreferences::OnCustomIconButtonClick(UINT, int, CWindow) {
	OPENFILENAME ofn{};
	std::wstring szFile(2048, L'\0');

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = UIWizard::MainWindow();
	ofn.lpstrFile = szFile.data();
	ofn.nMaxFile = static_cast<DWORD>(szFile.size());
	ofn.lpstrFilter = L"ICO file format (*.ico)\0*.ico\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = nullptr;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
		pfc::stringcvt::string_utf8_from_wide narrowPath(szFile.c_str());
		UIWizardSettings::customIconPath = narrowPath.get_ptr();
		UIWHGraphics::SetIcon(UIWizard::MainWindow(), m_hWnd, UIWizardSettings::customIcon, UIWizardSettings::customIconPath.c_str());
	}
}

void UIWizardPreferences::OnCaptionEdit(UINT, int, CWindow) {
	if (initDialog) return;

	hasChanged = true;
	prefCallback->on_state_changed();

	UIWizardSettings::captionLeft = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_LEFT);
	UIWizardSettings::captionTop = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_TOP);
	UIWizardSettings::captionWidth = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_WIDTH);
	UIWizardSettings::captionHeight = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_HEIGHT);

	UIWizard::Window()->CaptionAreaIndicator();
}

void UIWizardPreferences::OnMinWidthButtonClick(UINT, int, CWindow) {
	int currentWidth = UIWHWindow::GetWindowWidth(UIWizard::Window()->mainHwnd);
	if (currentWidth == -1) return;

	UIWizardSettings::windowMinWidth = (currentWidth == UIWizardSettings::windowMaxWidth) ? currentWidth - 1 : currentWidth;
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MIN_WIDTH, UIWizardSettings::windowMinWidth);
}

void UIWizardPreferences::OnMinHeightButtonClick(UINT, int, CWindow) {
	int currentHeight = UIWHWindow::GetWindowHeight(UIWizard::Window()->mainHwnd);
	if (currentHeight == -1) return;

	UIWizardSettings::windowMinHeight = (currentHeight == UIWizardSettings::windowMaxHeight) ? currentHeight - 1 : currentHeight;
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MIN_HEIGHT, UIWizardSettings::windowMinHeight);
}

void UIWizardPreferences::OnMaxWidthButtonClick(UINT, int, CWindow) {
	int currentWidth = UIWHWindow::GetWindowWidth(UIWizard::Window()->mainHwnd);
	if (currentWidth == -1) return;

	UIWizardSettings::windowMaxWidth = (currentWidth == UIWizardSettings::windowMinWidth) ? currentWidth + 1 : currentWidth;
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MAX_WIDTH, UIWizardSettings::windowMaxWidth);
}

void UIWizardPreferences::OnMaxHeightButtonClick(UINT, int, CWindow) {
	int currentHeight = UIWHWindow::GetWindowHeight(UIWizard::Window()->mainHwnd);
	if (currentHeight == -1) return;

	UIWizardSettings::windowMaxHeight = (currentHeight == UIWizardSettings::windowMinHeight) ? currentHeight + 1 : currentHeight;
	UIWHDialog::SetInputFieldNumber(m_hWnd, IDC_WINDOW_MAX_HEIGHT, UIWizardSettings::windowMaxHeight);
}
#pragma endregion


//////////////////////////
// * STATE MANAGEMENT * //
//////////////////////////
#pragma region State Managmenet
t_uint32 UIWizardPreferences::get_state() {
	t_uint32 state = preferences_state::resettable | preferences_state::dark_mode_supported;

	if (hasChanged) {
		state |= preferences_state::changed;
	}

	return state;
}

void UIWizardPreferences::apply() {
	UIWizardSettings::frameStyle = UIWHDialog::GetDropDownIndex(m_hWnd, IDC_FRAME_STYLE);
	UIWizardSettings::aeroEffect = UIWHDialog::GetDropDownIndex(m_hWnd, IDC_AERO_EFFECT);

	UIWizardSettings::glassFrameLeft = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_LEFT);
	UIWizardSettings::glassFrameTop = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_TOP);
	UIWizardSettings::glassFrameRight = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_RIGHT);
	UIWizardSettings::glassFrameBottom = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_GLASS_FRAME_BOTTOM);

	UIWizardSettings::hideWindowShadow = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_HIDE_WINDOW_SHADOW);
	UIWizardSettings::enableWindowTransparency = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_ENABLE_WINDOW_TRANSPARENCY);
	UIWizardSettings::windowTransparency = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_WINDOW_TRANSPARENCY);

	UIWizardSettings::showHiddenMenu = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_HIDDEN_MENU);
	UIWizardSettings::mainMenuSystemMenu = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_MAIN_MENU_SYSTEM_MENU);

	UIWizardSettings::customIcon = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_CUSTOM_ICON);
	UIWHGraphics::SetIcon(UIWizard::MainWindow(), m_hWnd, UIWizardSettings::customIcon, UIWizardSettings::customIconPath.c_str());
	UIWizardSettings::customTitle = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_CUSTOM_TITLE);

	pfc::string8 customTitleString = UIWHDialog::GetInputFieldText(m_hWnd, IDC_CUSTOM_TITLE_STRING);
	UIWizardSettings::customTitleString.set(customTitleString.c_str());

	UIWizardSettings::hideTaskbarIcon = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_TASKBAR_ICON);

	UIWizardSettings::moveStyle = UIWHDialog::GetDropDownIndex(m_hWnd, IDC_MOVE_STYLE);
	UIWizardSettings::ESCKey = UIWHDialog::GetDropDownIndex(m_hWnd, IDC_ESC_KEY);

	UIWizardSettings::captionLeft = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_LEFT);
	UIWizardSettings::captionTop = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_TOP);
	UIWizardSettings::captionWidth = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_WIDTH);
	UIWizardSettings::captionHeight = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_CAPTION_HEIGHT);

	UIWizardSettings::snapToEdge = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_SNAP_EDGE);
	UIWizardSettings::snapSize = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_SNAP_SIZE);
	UIWizardSettings::unsnapSize = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_UNSNAP_SIZE);

	UIWizardSettings::disableWindowMaximizing = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_DISABLE_WINDOW_MAXIMIZING);
	UIWizardSettings::disableWindowSizing = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_DISABLE_WINDOW_SIZING);

	UIWizardSettings::windowMinSize = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_WINDOW_MIN_SIZE);
	UIWizardSettings::windowMinWidth = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_WINDOW_MIN_WIDTH);
	UIWizardSettings::windowMinHeight = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_WINDOW_MIN_HEIGHT);

	UIWizardSettings::windowMaxSize = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_WINDOW_MAX_SIZE);
	UIWizardSettings::windowMaxWidth = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_WINDOW_MAX_WIDTH);
	UIWizardSettings::windowMaxHeight = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_WINDOW_MAX_HEIGHT);

	UIWizardSettings::hideWindowInactivity = UIWHDialog::GetCheckBoxState(m_hWnd, IDC_HIDE_WINDOW_INACTIVITY);
	UIWizardSettings::hideWindowInactivitySec = UIWHDialog::GetInputFieldNumber(m_hWnd, IDC_HIDE_WINDOW_INACTIVITY_SEC);

	UIWizard::Window()->Initialize();
}

void UIWizardPreferences::reset() {
	UIWizardSettings::ResetSettings();
	OnInitDialog(nullptr, 0);
}
#pragma endregion

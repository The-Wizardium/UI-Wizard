/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Preferences Header File                       * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1                                                     * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    22-12-2024                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "resource.h"


/////////////////////
// * PREFERENCES * //
/////////////////////
#pragma region Preferences
class UIWizardPreferences : public CDialogImpl<UIWizardPreferences>, public preferences_page_instance {
public:
	explicit UIWizardPreferences(preferences_page_callback::ptr callback);

	enum { IDD = IDD_PREFERENCES };

	BEGIN_MSG_MAP(UIWizardPreferences)
		MSG_WM_INITDIALOG(OnInitDialog)
		COMMAND_HANDLER_EX(IDC_FRAME_STYLE, CBN_SELCHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_AERO_EFFECT, CBN_SELCHANGE, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_GLASS_FRAME_LEFT, EN_CHANGE, OnGlassFrameEdit)
		COMMAND_HANDLER_EX(IDC_GLASS_FRAME_TOP, EN_CHANGE, OnGlassFrameEdit)
		COMMAND_HANDLER_EX(IDC_GLASS_FRAME_RIGHT, EN_CHANGE, OnGlassFrameEdit)
		COMMAND_HANDLER_EX(IDC_GLASS_FRAME_BOTTOM, EN_CHANGE, OnGlassFrameEdit)

		COMMAND_HANDLER_EX(IDC_HIDE_WINDOW_SHADOW, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_ENABLE_WINDOW_TRANSPARENCY, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_TRANSPARENCY, EN_CHANGE, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_HIDDEN_MENU, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_MAIN_MENU_SYSTEM_MENU, BN_CLICKED, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_CUSTOM_TITLE, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_CUSTOM_TITLE_STRING, EN_CHANGE, OnCustomTitleEdit)

		COMMAND_HANDLER_EX(IDC_CUSTOM_ICON, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_CUSTOM_ICON_BUTTON, BN_CLICKED, OnCustomIconButtonClick)
		COMMAND_HANDLER_EX(IDC_TASKBAR_ICON, BN_CLICKED, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_MOVE_STYLE, CBN_SELCHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_ESC_KEY, CBN_SELCHANGE, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_CAPTION_LEFT, EN_CHANGE, OnCaptionEdit)
		COMMAND_HANDLER_EX(IDC_CAPTION_TOP, EN_CHANGE, OnCaptionEdit)
		COMMAND_HANDLER_EX(IDC_CAPTION_WIDTH, EN_CHANGE, OnCaptionEdit)
		COMMAND_HANDLER_EX(IDC_CAPTION_HEIGHT, EN_CHANGE, OnCaptionEdit)

		COMMAND_HANDLER_EX(IDC_SNAP_EDGE, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_SNAP_SIZE, EN_CHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_UNSNAP_SIZE, EN_CHANGE, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_DISABLE_WINDOW_MAXIMIZING, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_DISABLE_WINDOW_SIZING, BN_CLICKED, OnSettingChanged)

		COMMAND_HANDLER_EX(IDC_WINDOW_MIN_SIZE, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_MIN_WIDTH, EN_CHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_MIN_WIDTH_BUTTON, BN_CLICKED, OnMinWidthButtonClick)
		COMMAND_HANDLER_EX(IDC_WINDOW_MIN_HEIGHT, EN_CHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_MIN_HEIGHT_BUTTON, BN_CLICKED, OnMinHeightButtonClick)

		COMMAND_HANDLER_EX(IDC_WINDOW_MAX_SIZE, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_MAX_WIDTH, EN_CHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_MAX_WIDTH_BUTTON, BN_CLICKED, OnMaxWidthButtonClick)
		COMMAND_HANDLER_EX(IDC_WINDOW_MAX_HEIGHT, EN_CHANGE, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_WINDOW_MAX_HEIGHT_BUTTON, BN_CLICKED, OnMaxHeightButtonClick)

		COMMAND_HANDLER_EX(IDC_HIDE_WINDOW_INACTIVITY, BN_CLICKED, OnSettingChanged)
		COMMAND_HANDLER_EX(IDC_HIDE_WINDOW_INACTIVITY_SEC, EN_CHANGE, OnSettingChanged)
	END_MSG_MAP()

	t_uint32 get_state() override;
	void apply() override;
	void reset() override;

private:
	const preferences_page_callback::ptr prefCallback;

	CFont fontGroupbox = nullptr;
	CFont fontSmall = nullptr;
	bool hasChanged = false;
	bool initDialog = true;

	// * INITIALIZATION * //
	void InitFonts();
	void InitFrameStyle();
	void InitAeroEffects();
	void InitGlassFrameArea();
	void InitMenuSettings();
	void InitCustomIcon();
	void InitCustomTitleName();
	void InitMoveStyle();
	void InitEscKey();
	void InitCaptionArea();
	void InitSnapToEdges();
	void InitWindowConstraints();
	void InitMinSizeConstraints();
	void InitMaxSizeConstraints();
	void InitHideWindowInactivity();
	void InitAllControlEnableState();

	// * EVENT LISTENERS * //
	BOOL OnInitDialog(CWindow, LPARAM);
	void OnSettingChanged(UINT, int, CWindow);
	void OnGlassFrameEdit(UINT, int, CWindow);
	void OnCustomTitleEdit(UINT, int, CWindow);
	void OnCustomIconButtonClick(UINT, int, CWindow);
	void OnCaptionEdit(UINT, int, CWindow);
	void OnMinWidthButtonClick(UINT, int, CWindow);
	void OnMinHeightButtonClick(UINT, int, CWindow);
	void OnMaxWidthButtonClick(UINT, int, CWindow);
	void OnMaxHeightButtonClick(UINT, int, CWindow);
};
#pragma endregion


//////////////////////////
// * PREFERENCES IMPL * //
//////////////////////////
#pragma region Preferences Impl
class UIWizardPreferencesImpl : public preferences_page_impl<UIWizardPreferences> {
public:
	virtual ~UIWizardPreferencesImpl() = default;

	const char* get_name() override { 
		return "UI Wizard"; 
	}

	GUID get_guid() override {
		return GUID{ 0x213bd677, 0x5ddc, 0x4534, { 0x90, 0x65, 0xc8, 0x6, 0xc5, 0x4e, 0xff, 0xc9 } };
	}

	GUID get_parent_guid() override { 
		return guid_display; 
	}
};

FB2K_SERVICE_FACTORY(UIWizardPreferencesImpl);
#pragma endregion

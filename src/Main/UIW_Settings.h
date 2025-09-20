/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Settings Header File                          * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.2                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    19-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once


//////////////////
// * SETTINGS * //
//////////////////
#pragma region Settings
class UIWizardSettings {
public:
	// * GUID DECLARATIONS * //
	static constexpr GUID guid_displayDPI = { 0x28ea854d, 0xf67d, 0x4fd6, { 0xb0, 0x61, 0x96, 0x34, 0x1c, 0x8d, 0x18, 0xef } };
	static constexpr GUID guid_displayResolutionMode = { 0xe44809e9, 0xfb80, 0x4c49, { 0xb0, 0xc3, 0x20, 0x75, 0x7f, 0x3d, 0xb3, 0x9d } };
	static constexpr GUID guid_displayResolution = { 0x2456f6a2, 0x8ea, 0x4d4a, { 0x91, 0xac, 0x99, 0xff, 0xba, 0xfe, 0x49, 0x26 } };
	static constexpr GUID guid_displayResolutionMultiMonitors = { 0xbe3e4a01, 0x76a4, 0x48c4, { 0x82, 0x31, 0xd9, 0xc6, 0xdc, 0x6e, 0xe3, 0xbe } };

	static constexpr GUID guid_windowBgColor = { 0xaaac3ed5, 0xc1c6, 0x4302, { 0x95, 0xc6, 0xb6, 0x30, 0xd, 0x35, 0xf8, 0xfa } };
	static constexpr GUID guid_windowPosition = { 0xb14cf675, 0xc98c, 0x435e, { 0x8d, 0x2, 0xce, 0xb0, 0xf2, 0x77, 0xe9, 0x2a } };
	static constexpr GUID guid_windowSize = { 0xae506149, 0x18fe, 0x4752, { 0xa8, 0xf0, 0xdd, 0x86, 0xed, 0x2c, 0xf1, 0xbb } };
	static constexpr GUID guid_windowState = { 0x9d95b8e2, 0x380d, 0x4a7f, { 0xbc, 0xc0, 0x63, 0x6f, 0xac, 0xfa, 0x1c, 0xc5 } };
	static constexpr GUID guid_windowStatePrevious = { 0x866daf88, 0x47a, 0x4660, { 0xbf, 0x43, 0x18, 0xa5, 0x77, 0x6e, 0xdf, 0x55 } };

	static constexpr GUID guid_frameStylePrevious = { 0x91ef356f, 0x8a16, 0x405e, { 0x9b, 0xd7, 0x8e, 0x15, 0xa3, 0x53, 0x91, 0x2e } };
	static constexpr GUID guid_frameStyle = { 0x1a2b3c4d, 0x5e6f, 0x7a8b, { 0x9c, 0xd1, 0xe2, 0xf3, 0x45, 0x67, 0x89, 0xab } };
	static constexpr GUID guid_aeroEffect = { 0xc4b39208, 0x2c13, 0x478f, { 0xb2, 0x5b, 0xd0, 0x35, 0x7c, 0xb, 0xf9, 0x2c } };

	static constexpr GUID guid_glassFrameLeft = { 0x9aa4afc0, 0xef18, 0x46bb, { 0x85, 0x4f, 0x7e, 0x33, 0x93, 0x1, 0xa4, 0xf8 } };
	static constexpr GUID guid_glassFrameTop = { 0x39c4cd49, 0x6e0c, 0x43a4, { 0x97, 0x1f, 0x4b, 0x4b, 0x84, 0xff, 0x71, 0x26 } };
	static constexpr GUID guid_glassFrameRight = { 0x7230f3da, 0xc647, 0x4dbd, { 0x81, 0xd1, 0x40, 0x7d, 0x9d, 0x73, 0x17, 0x9a } };
	static constexpr GUID guid_glassFrameBottom = { 0x463ea273, 0x3215, 0x4d10, { 0x9f, 0x70, 0x27, 0xc, 0x3e, 0x82, 0x3, 0xf5 } };

	static constexpr GUID guid_hideWindowShadow = { 0x4a11e310, 0xe37c, 0x46f7, { 0x80, 0xf5, 0xd0, 0x85, 0x90, 0x20, 0x4a, 0x23 } };
	static constexpr GUID guid_enableWindowTransparency = { 0xc6552515, 0x5b9a, 0x422b, { 0xb8, 0xb7, 0x65, 0x50, 0xe6, 0xce, 0x33, 0xe5 } };
	static constexpr GUID guid_windowTransparency = { 0x3ae979c8, 0xe377, 0x4f4e, { 0xb4, 0x8, 0x19, 0x28, 0x3e, 0xe1, 0x65, 0x91 } };

	static constexpr GUID guid_showHiddenMenu = { 0x3c4d5e6f, 0x7a8b, 0x9cd1, { 0xe2, 0xf3, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef } };
	static constexpr GUID guid_mainMenuSystemMenu = { 0xe2925592, 0x6275, 0x4e04, { 0x9f, 0x7, 0x8, 0x47, 0x4c, 0xa9, 0x95, 0x77 } };

	static constexpr GUID guid_customIcon = { 0xb55c4659, 0xfd69, 0x4771, { 0xb7, 0xce, 0x91, 0xa8, 0x71, 0x96, 0xca, 0x99 } };
	static constexpr GUID guid_customIconPath = { 0x3fcd5fac, 0xa06c, 0x4e48, { 0x80, 0xe4, 0x4c, 0xf8, 0xec, 0xd5, 0x6f, 0x5b } };

	static constexpr GUID guid_customTitle = { 0x5654262, 0xcbb3, 0x49df, { 0x90, 0x98, 0xd, 0xea, 0x4c, 0x3, 0x9b, 0x8 } };
	static constexpr GUID guid_customTitleString = { 0xe328de52, 0xbe55, 0x45f0, { 0xbd, 0x8e, 0x8c, 0xf7, 0xfc, 0xfe, 0xf6, 0xbf } };

	static constexpr GUID guid_hideTaskbarIcon = { 0x4d5e6f7a, 0x8b9c, 0xd1e2, { 0xf3, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0x01 } };

	static constexpr GUID guid_moveStyle = { 0x2b3c4d5e, 0x6f7a, 0x8b9c, { 0xd1, 0xe2, 0xf3, 0x45, 0x67, 0x89, 0xab, 0xcd } };
	static constexpr GUID guid_ESCKey = { 0x16f62a50, 0x44f4, 0x4528, { 0xa7, 0x8, 0x5d, 0x40, 0x6a, 0x3c, 0x19, 0xda } };

	static constexpr GUID guid_captionLeft = { 0x3a4b5c6d, 0x7e8f, 0x9a0b, { 0xc1, 0xd2, 0xe3, 0xf4, 0x56, 0x78, 0x9a, 0xbc } };
	static constexpr GUID guid_captionTop = { 0x4b5c6d7e, 0x8f9a, 0x0b1c, { 0xd2, 0xe3, 0xf4, 0x56, 0x78, 0x9a, 0xbc, 0xde } };
	static constexpr GUID guid_captionWidth = { 0x5c6d7e8f, 0x9a0b, 0x1c2d, { 0xe3, 0xf4, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xef } };
	static constexpr GUID guid_captionHeight = { 0x6d7e8f9a, 0x0b1c, 0x2d3e, { 0xf4, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xef, 0xf1 } };

	static constexpr GUID guid_snapToEdge = { 0xf4f99f66, 0x1a83, 0x4b82, { 0xbb, 0xad, 0x6a, 0x64, 0x31, 0x36, 0xd9, 0x70 } };
	static constexpr GUID guid_snapSize = { 0x3a9d4111, 0x1ab, 0x4f6b, { 0x8d, 0x26, 0x5d, 0x4d, 0x69, 0xd0, 0xc7, 0xe9 } };
	static constexpr GUID guid_unsnapSize = { 0x29c174a7, 0x29bf, 0x4081, { 0x81, 0x2a, 0x95, 0x6b, 0x5, 0x5e, 0xb8, 0x89 } };

	static constexpr GUID guid_disableWindowMaximizing = { 0xfcf3a0f3, 0x769a, 0x4356, { 0x82, 0x10, 0xde, 0xa0, 0x72, 0x62, 0x5d, 0xbf } };
	static constexpr GUID guid_disableWindowSizing = { 0xa711f46b, 0xf3ae, 0x4bef, { 0x95, 0x93, 0xa9, 0x9e, 0xb5, 0x92, 0x4, 0xeb } };

	static constexpr GUID guid_windowMinSize = { 0x81441c86, 0x6878, 0x4d41, { 0xb8, 0x5, 0x36, 0xd, 0x6e, 0x8b, 0xb4, 0x2a } };
	static constexpr GUID guid_windowMinWidth = { 0xe60667f5, 0x4e3f, 0x424b, { 0x86, 0xa2, 0xce, 0xc5, 0x43, 0xd8, 0x82, 0x16 } };
	static constexpr GUID guid_windowMinHeight = { 0x77d68a71, 0x66b0, 0x47a1, { 0xa8, 0x3b, 0xa4, 0xa4, 0xa7, 0x9e, 0x11, 0x41 } };

	static constexpr GUID guid_windowMaxSize = { 0x56557119, 0x7ff, 0x4bb5, { 0xb0, 0xaf, 0x4, 0x61, 0x57, 0xf7, 0x6a, 0xe1 } };
	static constexpr GUID guid_windowMaxWidth = { 0xbda3f8ee, 0x3f62, 0x4dd3, { 0xbd, 0x48, 0x0, 0xdc, 0xfe, 0x2b, 0x5c, 0xbf } };
	static constexpr GUID guid_windowMaxHeight = { 0x3fe506a2, 0xb7a8, 0x47da, { 0xad, 0xd6, 0x3b, 0x5b, 0x8f, 0x2b, 0x74, 0x49 } };

	static constexpr GUID guid_hideWindowInactivity = { 0xc02a29f4, 0xdfef, 0x48ba, { 0x89, 0x86, 0xdf, 0x19, 0x2c, 0xb6, 0xa1, 0x15 } };
	static constexpr GUID guid_hideWindowInactivitySec = { 0xce8ef08c, 0x48ef, 0x434c, { 0x8c, 0x84, 0x64, 0xef, 0x15, 0xe8, 0x30, 0x5c } };

	// * STATIC MEMBER DEFAULTS * //
	static constexpr int displayDPIDefault = 96;
	static constexpr const char* displayResolutionModeDefault = "HD";
	static constexpr const char* displayResolutionDefault = "";
	static constexpr const char* displayResolutionMultiMonitorsDefault = "";

	static constexpr int windowBgColorDefault = -1;
	static constexpr int windowStateDefault = 0;
	static constexpr int windowStatePreviousDefault = 0;

	static constexpr int frameStylePreviousDefault = 0;
	static constexpr int frameStyleDefault = 0;
	static constexpr int aeroEffectDefault = 0;

	static constexpr int glassFrameLeftDefault = 0;
	static constexpr int glassFrameTopDefault = 0;
	static constexpr int glassFrameRightDefault = 0;
	static constexpr int glassFrameBottomDefault = 0;

	static constexpr bool hideWindowShadowDefault = false;
	static constexpr bool enableWindowTransparencyDefault = false;
	static constexpr int windowTransparencyDefault = 255;

	static constexpr bool showHiddenMenuDefault = false;
	static constexpr bool mainMenuSystemMenuDefault = false;

	static constexpr bool customIconDefault = false;
	static constexpr const char* customIconPathDefault = "";

	static constexpr bool customTitleDefault = false;
	static constexpr const char* customTitleStringDefault = "foobar2000";

	static constexpr bool hideTaskbarIconDefault = false;

	static constexpr int moveStyleDefault = 0;
	static constexpr int ESCKeyDefault = 0;

	static constexpr int captionLeftDefault = 0;
	static constexpr int captionTopDefault = 0;
	static constexpr int captionWidthDefault = 9999;
	static constexpr int captionHeightDefault = 5;

	static constexpr bool snapToEdgeDefault = false;
	static constexpr int snapSizeDefault = 20;
	static constexpr int unsnapSizeDefault = 40;

	static constexpr bool disableWindowMaximizingDefault = false;
	static constexpr bool disableWindowSizingDefault = false;

	static constexpr bool windowMinSizeDefault = false;
	static constexpr int windowMinWidthDefault = 1;
	static constexpr int windowMinHeightDefault = 1;

	static constexpr bool windowMaxSizeDefault = false;
	static constexpr int windowMaxWidthDefault = 9999;
	static constexpr int windowMaxHeightDefault = 9999;

	static constexpr bool hideWindowInactivityDefault = false;
	static constexpr int hideWindowInactivitySecDefault = 15;

	// * STATIC MEMBERS * //
	static inline cfg_int displayDPI{ guid_displayDPI, displayDPIDefault };
	static inline cfg_string displayResolutionMode{ guid_displayResolutionMode, displayResolutionModeDefault };
	static inline cfg_string displayResolution{ guid_displayResolution, displayResolutionDefault };
	static inline cfg_string displayResolutionMultiMonitors{ guid_displayResolutionMultiMonitors, displayResolutionMultiMonitorsDefault };

	static inline cfg_int windowBgColor{ guid_windowBgColor, windowBgColorDefault };
	static inline cfg_window_placement windowPosition{ guid_windowPosition };
	static inline cfg_window_size windowSize{ guid_windowSize };
	static inline cfg_int windowState{ guid_windowState, windowStateDefault };
	static inline cfg_int windowStatePrevious{ guid_windowStatePrevious, windowStatePreviousDefault };

	static inline cfg_int frameStylePrevious{ guid_frameStylePrevious, frameStylePreviousDefault };
	static inline cfg_int frameStyle{ guid_frameStyle, frameStyleDefault };
	static inline cfg_int aeroEffect{ guid_aeroEffect, aeroEffectDefault };

	static inline cfg_int glassFrameLeft{ guid_glassFrameLeft, glassFrameLeftDefault };
	static inline cfg_int glassFrameTop{ guid_glassFrameTop, glassFrameTopDefault };
	static inline cfg_int glassFrameRight{ guid_glassFrameRight, glassFrameRightDefault };
	static inline cfg_int glassFrameBottom{ guid_glassFrameBottom, glassFrameBottomDefault };

	static inline cfg_bool hideWindowShadow{ guid_hideWindowShadow, hideWindowShadowDefault };
	static inline cfg_bool enableWindowTransparency{ guid_enableWindowTransparency, enableWindowTransparencyDefault };
	static inline cfg_int windowTransparency{ guid_windowTransparency, windowTransparencyDefault };

	static inline cfg_bool showHiddenMenu{ guid_showHiddenMenu, showHiddenMenuDefault };
	static inline cfg_bool mainMenuSystemMenu{ guid_mainMenuSystemMenu, mainMenuSystemMenuDefault };

	static inline cfg_bool customIcon{ guid_customIcon, customIconDefault };
	static inline cfg_string customIconPath{ guid_customIconPath, customIconPathDefault };

	static inline cfg_bool customTitle{ guid_customTitle, customTitleDefault };
	static inline cfg_string customTitleString{ guid_customTitleString, customTitleStringDefault };

	static inline cfg_bool hideTaskbarIcon{ guid_hideTaskbarIcon, hideTaskbarIconDefault };

	static inline cfg_int moveStyle{ guid_moveStyle, moveStyleDefault };
	static inline cfg_int ESCKey{ guid_ESCKey, ESCKeyDefault };

	static inline cfg_int captionLeft{ guid_captionLeft, captionLeftDefault };
	static inline cfg_int captionTop{ guid_captionTop, captionTopDefault };
	static inline cfg_int captionWidth{ guid_captionWidth, captionWidthDefault };
	static inline cfg_int captionHeight{ guid_captionHeight, captionHeightDefault };

	static inline cfg_bool snapToEdge{ guid_snapToEdge, snapToEdgeDefault };
	static inline cfg_int snapSize{ guid_snapSize, snapSizeDefault };
	static inline cfg_int unsnapSize{ guid_unsnapSize, unsnapSizeDefault };

	static inline cfg_bool disableWindowMaximizing{ guid_disableWindowMaximizing, disableWindowMaximizingDefault };
	static inline cfg_bool disableWindowSizing{ guid_disableWindowSizing, disableWindowSizingDefault };

	static inline cfg_bool windowMinSize{ guid_windowMinSize, windowMinSizeDefault };
	static inline cfg_int windowMinWidth{ guid_windowMinWidth, windowMinWidthDefault };
	static inline cfg_int windowMinHeight{ guid_windowMinHeight, windowMinHeightDefault };

	static inline cfg_bool windowMaxSize{ guid_windowMaxSize, windowMaxSizeDefault };
	static inline cfg_int windowMaxWidth{ guid_windowMaxWidth, windowMaxWidthDefault };
	static inline cfg_int windowMaxHeight{ guid_windowMaxHeight, windowMaxHeightDefault };

	static inline cfg_bool hideWindowInactivity{ guid_hideWindowInactivity, hideWindowInactivityDefault };
	static inline cfg_int hideWindowInactivitySec{ guid_hideWindowInactivitySec, hideWindowInactivitySecDefault };

	// * STATIC METHODS * //
	static void ResetSettings();
};
#pragma endregion

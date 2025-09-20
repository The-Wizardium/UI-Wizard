/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Settings Source File                          * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.2                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    19-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "UIW_Settings.h"


////////////////////////
// * STATIC METHODS * //
////////////////////////
#pragma region Static Methods
void UIWizardSettings::ResetSettings() {
	displayDPI = displayDPIDefault;
	displayResolutionMode = displayResolutionModeDefault;
	displayResolution = displayResolutionDefault;
	displayResolutionMultiMonitors = displayResolutionMultiMonitorsDefault;

	windowBgColor = windowBgColorDefault;
	windowState = windowStateDefault;
	windowStatePrevious = windowStatePreviousDefault;

	frameStylePrevious = frameStylePreviousDefault;
	frameStyle = frameStyleDefault;
	aeroEffect = aeroEffectDefault;

	glassFrameLeft = glassFrameLeftDefault;
	glassFrameTop = glassFrameTopDefault;
	glassFrameRight = glassFrameRightDefault;
	glassFrameBottom = glassFrameBottomDefault;

	hideWindowShadow = hideWindowShadowDefault;
	enableWindowTransparency = enableWindowTransparencyDefault;
	windowTransparency = windowTransparencyDefault;

	showHiddenMenu = showHiddenMenuDefault;
	mainMenuSystemMenu = mainMenuSystemMenuDefault;

	customIcon = customIconDefault;
	customIconPath = customIconPathDefault;

	customTitle = customTitleDefault;
	customTitleString = customTitleStringDefault;

	hideTaskbarIcon = hideTaskbarIconDefault;

	moveStyle = moveStyleDefault;
	ESCKey = ESCKeyDefault;

	captionLeft = captionLeftDefault;
	captionTop = captionTopDefault;
	captionWidth = captionWidthDefault;
	captionHeight = captionHeightDefault;

	snapToEdge = snapToEdgeDefault;
	snapSize = snapSizeDefault;
	unsnapSize = unsnapSizeDefault;

	disableWindowMaximizing = disableWindowMaximizingDefault;
	disableWindowSizing = disableWindowSizingDefault;

	windowMinSize = windowMinSizeDefault;
	windowMinWidth = windowMinWidthDefault;
	windowMinHeight = windowMinHeightDefault;

	windowMaxSize = windowMaxSizeDefault;
	windowMaxWidth = windowMaxWidthDefault;
	windowMaxHeight = windowMaxHeightDefault;

	hideWindowInactivity = hideWindowInactivityDefault;
	hideWindowInactivitySec = hideWindowInactivitySecDefault;
}
#pragma endregion

/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Header File                                   * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.1                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once
#include "UIW_Window.h"


///////////////////
// * UI Wizard * //
///////////////////
#pragma region UI Wizard
class UIWizard {
public:
	UIWizard() = default;
	~UIWizard() = default;

	// * PUBLIC METHODS * //
	static void InitUIWizard();
	static void QuitUIWizard();

	// * PUBLIC GETTERS - SINGLETON ACCESSORS * //
	static HWND MainWindow() { return mainHwnd; }
	static UIWizardWindow* Window() { return uiwWindow.get(); }
	static UIWizardMainWindow* Main() { return uiwMainWindow.get(); }
	static UIWizardChildWindow* Child() { return uiwChildWindow.get(); }
	static UIWizardShadowWindow* Shadow() { return uiwShadowWindow.get(); }

private:
	static inline HWND mainHwnd = nullptr;
	static inline std::unique_ptr<UIWizardWindow> uiwWindow = nullptr;
	static inline std::unique_ptr<UIWizardMainWindow> uiwMainWindow = nullptr;
	static inline std::unique_ptr<UIWizardChildWindow> uiwChildWindow = nullptr;
	static inline std::unique_ptr<UIWizardShadowWindow> uiwShadowWindow = nullptr;
};
#pragma endregion

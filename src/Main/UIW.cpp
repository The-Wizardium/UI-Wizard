/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Source File                                   * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1                                                     * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    22-12-2024                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "MyCOM.h"
#include "UIW.h"
#include <UIW_Helpers.h>
#include "UIW_Menu.h"


/////////////////////////
// * COMPONENT SETUP * //
/////////////////////////
#pragma region Component Setup
DECLARE_COMPONENT_VERSION(
	"UI Wizard", "0.1",
	"UI Wizard \n"
	"The Azure Spell of Interface Enchantment \n"
	"A Sacred Chapter of The Wizardium \n"
	"https://github.com/The-Wizardium \n\n"
	"Woven from the azure mists of the Holy Foobar Land, UI Wizard bends foobar2000's interface to your will.\n"
	"This spellbinding art conjures window styles, Aero effects, and scripting sorcery via COM/ActiveX.\n"
	"Craft enchanted layouts with mischievous elegance and etch your legend in The Wizardium's grimoire."
);

DEFINE_GUID(UIWizard, 0xf3a2b1c4, 0x8d05, 0x4c21, 0x9f, 0x3d, 0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc);
VALIDATE_COMPONENT_FILENAME("foo_ui_wizard.dll");
FOOBAR2000_IMPLEMENT_CFG_VAR_DOWNGRADE;
#pragma endregion


/////////////////////////////
// * MAIN INITIALIZATION * //
/////////////////////////////
#pragma region Main Initialization
void UIWizard::InitUIWizard() {
	mainHwnd = core_api::get_main_window();

	if (!mainHwnd) return;

	uiwWindow = std::make_unique<UIWizardWindow>(mainHwnd);
	uiwMainWindow = std::make_unique<UIWizardMainWindow>(mainHwnd);
	uiwChildWindow = std::make_unique<UIWizardChildWindow>(mainHwnd);
	uiwShadowWindow = std::make_unique<UIWizardShadowWindow>(mainHwnd);

	UIWizardMenu::Init();

	uiwMainWindow->MainWindowSubclass(mainHwnd);
	uiwChildWindow->ChildWindowSubclass(mainHwnd);

	//// * INIT AND CREATE SHADOW WINDOW IF BORDERLESS * //
	//UIWizard::Shadow()->ShadowWindow();
	//UIWizard::Shadow()->ShadowWindowDisplay();
	//UIWizard::Shadow()->ShadowWindowUpdate();

	uiwWindow->Initialize();
	uiwWindow->InitWindowMessageLoop();

	ShowWindow(mainHwnd, SW_SHOW);
}

void UIWizard::QuitUIWizard() {
	UIWHDarkMode::Cleanup();

	if (uiwMainWindow) {
		uiwMainWindow->MainWindowUnsubclass(mainHwnd);
	}
	if (uiwChildWindow) {
		uiwChildWindow->ChildWindowUnsubclass(mainHwnd);
	}

	uiwWindow.reset();
	uiwMainWindow.reset();
	uiwChildWindow.reset();
	uiwShadowWindow.reset();

	UIWizardMenu::Quit();
	MyCOM::QuitMyCOM();
}

namespace {
	FB2K_ON_INIT_STAGE(MyCOM::InitMyCOM, init_stages::after_config_read);
	FB2K_ON_INIT_STAGE(UIWizard::InitUIWizard, init_stages::after_ui_init);
	FB2K_RUN_ON_QUIT(UIWizard::QuitUIWizard);
}
#pragma endregion

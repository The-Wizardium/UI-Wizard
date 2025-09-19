/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Source File                                   * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.1                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    19-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "MyCOM.h"
#include "UIW.h"
#include "UIW_Helpers.h"
#include "UIW_Menu.h"


/////////////////////////
// * COMPONENT SETUP * //
/////////////////////////
#pragma region Component Setup
DECLARE_COMPONENT_VERSION(
	"UI Wizard", "0.2.1",

	"UI Wizard \n"
	"The Sapphire Spell Of Interface Enchantment \n"
	"Lux Etherea Imperat \n\n"

	"A Sacred Chapter Of The Wizardium \n"
	"https://www.The-Wizardium.org \n"
	"https://github.com/The-Wizardium \n\n"

	"Sealed within the luminous Sapphiraz Sanctum of the Holy Foobar Land, "
	"UI Wizard reshapes foobar2000's form with celestial grace. "
	"Craft enchanted interfaces with Aero effects, borderless designs, "
	"and dynamic layouts, all commanded through ActiveX scripting. \n\n"

	"Shape the mystic canvas: \n"
	"- Customize frame styles, transparency, and titles. \n"
	"- Control window positioning and snap-to-edge behavior. \n"
	"- Script fluid layouts with JavaScript. \n\n"

	"The Wizardium Chooses Only The Anointed"
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

	uiwWindow->Initialize();
	uiwWindow->InitWindowMessageLoop();
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

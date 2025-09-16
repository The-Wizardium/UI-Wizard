/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Menu Source File                              * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "UIW.h"
#include "UIW_Menu.h"
#include "UIW_Settings.h"
#include "UIW_Window.h"


//////////////
// * MENU * //
//////////////
#pragma region Menu
void UIWizardMenu::Init() {
	UIWizardMainMenu::InitMainMenuHook();
	// Currently does not work yet, displaying full context menu is very complicated
	// UIWizardContextMenu::InitContextMenuHook();
}

void UIWizardMenu::Quit() {
	MH_DisableHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}
#pragma endregion


///////////////////
// * MAIN MENU * //
///////////////////
#pragma region Main Menu
#if defined(_WIN64)
void __fastcall UIWizardMainMenu::GenerateMenuWin32_Hooked(void* pThis, fb2k::hmenu_t p_menu, uint32_t p_id_base, uint32_t p_id_count, uint32_t p_flags) {
#else
void __fastcall UIWizardMainMenu::GenerateMenuWin32_Hooked(void* pThis, int dummy, fb2k::hmenu_t p_menu, uint32_t p_id_base, uint32_t p_id_count, uint32_t p_flags) {
#endif
	uint32_t new_flags = p_flags;
	if (UIWizardSettings::showHiddenMenu) {
		new_flags &= ~(mainmenu_manager::flag_view_full | mainmenu_manager::flag_view_reduced);
		new_flags |= mainmenu_manager::flag_view_full;
	}

#if defined(_WIN64)
	originalGenerateMenuWin32(pThis, p_menu, p_id_base, p_id_count, new_flags);
#else
	originalGenerateMenuWin32(pThis, 0 /* dummy */, p_menu, p_id_base, p_id_count, new_flags);
#endif
}

void UIWizardMainMenu::InitMainMenuHook() {
	auto mm = mainmenu_manager::get();
	void** vtable = *reinterpret_cast<void***>(mm.get_ptr());
	constexpr int VTABLE_INDEX = 4; // Index for org. generate_menu_win32

	MH_STATUS status = MH_CreateHook(vtable[VTABLE_INDEX],
		&GenerateMenuWin32_Hooked, reinterpret_cast<void**>(&originalGenerateMenuWin32)
	);

	if (status == MH_OK) {
		MH_EnableHook(vtable[VTABLE_INDEX]);
	}
}

void UIWizardMainMenu::HandleMainMenuSystemMenu() {
	if (!UIWizardSettings::mainMenuSystemMenu ||
		UIWizardSettings::frameStyle != static_cast<int>(UIWizardWindow::FrameStyle::Default)) {
		return;
	}

	t_uint32 currentBaseId = 0;
	const t_uint32 idCount = 1000;
	const int menuFlag = UIWizardSettings::showHiddenMenu ? mainmenu_manager::flag_view_full :
		mainmenu_manager::flag_view_reduced;

	HMENU hSystemMenu = CreatePopupMenu();
	std::map<std::string, HMENU> menuMap;
	std::vector<mainmenu_manager::ptr> managers;

	// Enumerate top-level groups
	service_enum_t<mainmenu_group> groupEnum;
	service_ptr_t<mainmenu_group> group;
	while (groupEnum.next(group)) {
		if (group->get_parent() == pfc::guid_null) {
			service_ptr_t<mainmenu_group_popup> popupGroup;
			if (group->service_query_t(popupGroup)) {
				pfc::string8 groupName;
				popupGroup->get_display_string(groupName);

				HMENU hSubMenu = CreatePopupMenu();
				auto mm = standard_api_create_t<mainmenu_manager>();
				mm->instantiate(group->get_guid());
				mm->generate_menu_win32(hSubMenu, currentBaseId, idCount, menuFlag);

				menuMap[std::string(groupName.c_str())] = hSubMenu;
				managers.push_back(mm);
				currentBaseId += idCount;
			}
		}
	}

	// Append menus in original order
	for (const auto& name : { "File", "Edit", "View", "Playback", "Library", "Help" }) {
		auto it = menuMap.find(name);
		if (it != menuMap.end()) {
			AppendMenu(hSystemMenu, MF_POPUP, (UINT_PTR)it->second, pfc::stringcvt::string_wide_from_utf8_fast(name).get_ptr());
		}
	}

	POINT pt = { 0, 0 };
	ClientToScreen(UIWizard::Window()->mainHwnd, &pt);
	UINT selectedCmd = TrackPopupMenu(hSystemMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RETURNCMD, pt.x, pt.y, 0, UIWizard::Window()->mainHwnd, nullptr);

	if (selectedCmd) {
		auto managerIndex = (selectedCmd / idCount);
		if (managerIndex < managers.size()) {
			managers[managerIndex]->execute_command(selectedCmd % idCount);
		}
	}

	DestroyMenu(hSystemMenu);
}
#pragma endregion


//////////////////////
// * CONTEXT MENU * //
//////////////////////
#pragma region Context Menu
void __fastcall UIWizardContextMenu::InitContextEx_Hooked(void* ecx, const pfc::list_base_const_t<metadb_handle_ptr>& data, unsigned flags, const GUID& caller) {
	unsigned new_flags = flags;
	if (UIWizardSettings::showHiddenMenu) {
		new_flags |= contextmenu_manager::flag_view_full;
		new_flags &= ~contextmenu_manager::flag_view_reduced;
	}
	else {
		new_flags |= contextmenu_manager::flag_view_reduced;
		new_flags &= ~contextmenu_manager::flag_view_full;
	}

	// FB2K_console_formatter() << "InitContextEx_Hooked - Original Flags: " << flags << "New Flags: " << new_flags;
	// Call original function with modified flags
	originalInitContextEx(ecx, data, new_flags, caller);
}

void UIWizardContextMenu::InitContextMenuHook() {
	auto cm = contextmenu_manager::g_create();
	void** vtable = *reinterpret_cast<void***>(cm.get_ptr());
	constexpr int VTABLE_INDEX = 5; // Index for init_context_ex

	MH_STATUS status = MH_CreateHook(vtable[VTABLE_INDEX],
		reinterpret_cast<void*>(&UIWizardContextMenu::InitContextEx_Hooked),
		reinterpret_cast<void**>(&originalInitContextEx)
	);

	if (status == MH_OK) {
		MH_EnableHook(vtable[VTABLE_INDEX]);
	}
}
#pragma endregion

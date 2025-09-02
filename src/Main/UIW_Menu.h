/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Menu Header File                              * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once


//////////////
// * MENU * //
//////////////
#pragma region Menu
class UIWizardMenu {
public:
	static void Init();
	static void Quit();
};
#pragma endregion


///////////////////
// * MAIN MENU * //
///////////////////
#pragma region Main Menu
class UIWizardMainMenu : public UIWizardMenu {
public:
#if defined(_WIN64)
	using GenerateMenuWin32_t = void(__fastcall*)(
		void* pThis, fb2k::hmenu_t p_menu, uint32_t p_id_base, uint32_t p_id_count, uint32_t p_flags
	);
#else
	using GenerateMenuWin32_t = void(__fastcall*)(
		void* pThis, int dummy, fb2k::hmenu_t p_menu, uint32_t p_id_base, uint32_t p_id_count, uint32_t p_flags
	);
#endif
	static inline GenerateMenuWin32_t originalGenerateMenuWin32 = nullptr;

#if defined(_WIN64)
	static void __fastcall GenerateMenuWin32_Hooked(
		void* pThis, fb2k::hmenu_t p_menu, uint32_t p_id_base, uint32_t p_id_count, uint32_t p_flags
	);
#else
	static void __fastcall GenerateMenuWin32_Hooked(
		void* pThis, int dummy, fb2k::hmenu_t p_menu, uint32_t p_id_base, uint32_t p_id_count, uint32_t p_flags
	);
#endif
	static void InitMainMenuHook();
	static void HandleMainMenuSystemMenu();
};
#pragma endregion


//////////////////////
// * CONTEXT MENU * //
//////////////////////
#pragma region Context Menu
class UIWizardContextMenu {
public:
	using InitContextEx_t = void(__fastcall*)(
		void*, const pfc::list_base_const_t<metadb_handle_ptr>&, unsigned, const GUID&
	);

	static inline InitContextEx_t originalInitContextEx = nullptr;

	static void __fastcall InitContextEx_Hooked(
		void* ecx, const pfc::list_base_const_t<metadb_handle_ptr>& data, unsigned flags, const GUID& caller
	);

	static void InitContextMenuHook();
};
#pragma endregion

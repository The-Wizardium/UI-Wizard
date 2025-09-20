/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Callbacks Source File                         * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.2.2                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#include "UIW_PCH.h"
#include "UIW.h"
#include "UIW_Callbacks.h"


////////////////////////
// * PUBLIC METHODS * //
////////////////////////
#pragma region Public Methods
unsigned UIWizardCallbacks::get_flags() {
	return flags;
}

void UIWizardCallbacks::updateWindowTitle() const {
	if (!UIWizard::Window()) return;
	UIWizard::Window()->SetWindowTitle();
}
#pragma endregion


///////////////////
// * CALLBACKS * //
///////////////////
#pragma region Callbacks
void UIWizardCallbacks::on_playback_dynamic_info(const file_info& p_info) noexcept {
	updateWindowTitle();
}

void UIWizardCallbacks::on_playback_dynamic_info_track(const file_info& p_info) noexcept {
	// Placeholder, current design doesn't require this action...
}

void UIWizardCallbacks::on_playback_edited(metadb_handle_ptr p_track) noexcept {
	updateWindowTitle();
}

void UIWizardCallbacks::on_playback_new_track(metadb_handle_ptr p_track) noexcept {
	updateWindowTitle();
}

void UIWizardCallbacks::on_playback_pause(bool b_state) noexcept {
	// Placeholder, current design doesn't require this action...
}

void UIWizardCallbacks::on_playback_seek(double p_time) noexcept {
	// Placeholder, current design doesn't require this action...
}

void UIWizardCallbacks::on_playback_starting(play_control::t_track_command p_command, bool p_paused) noexcept {
	// Placeholder, current design doesn't require this action...
}

void UIWizardCallbacks::on_playback_stop(play_control::t_stop_reason p_reason) noexcept {
	updateWindowTitle();
}

void UIWizardCallbacks::on_playback_time(double p_time) noexcept {
	// Placeholder, current design doesn't require this action...
}
void UIWizardCallbacks::on_volume_change(float p_new_val) noexcept {
	// Placeholder, current design doesn't require this action...
}
#pragma endregion

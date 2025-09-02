/////////////////////////////////////////////////////////////////////////////////
// * FB2K Component: UI Wizard                                               * //
// * Description:    UI Wizard Callbacks Header File                         * //
// * Author:         TT                                                      * //
// * Website:        https://github.com/The-Wizardium/UI-Wizard              * //
// * Version:        0.1.0                                                   * //
// * Dev. started:   12-12-2024                                              * //
// * Last change:    01-09-2025                                              * //
/////////////////////////////////////////////////////////////////////////////////


#pragma once


///////////////////
// * CALLBACKS * //
///////////////////
#pragma region Callbacks
class UIWizardCallbacks : public play_callback_static {
public:
	enum { flags = flag_on_playback_all };

	UIWizardCallbacks() = default;
	virtual ~UIWizardCallbacks() = default;

	// * PUBLIC METHODS * //
	unsigned get_flags() override;
	void updateWindowTitle() const;

	// * CALLBACKS * //
	void on_playback_dynamic_info(const file_info& p_info) noexcept override;
	void on_playback_dynamic_info_track(const file_info& p_info) noexcept override;
	void on_playback_edited(metadb_handle_ptr p_track) noexcept override;
	void on_playback_new_track(metadb_handle_ptr p_track) noexcept override;
	void on_playback_pause(bool b_state) noexcept override;
	void on_playback_seek(double p_time) noexcept override;
	void on_playback_starting(play_control::t_track_command p_command, bool p_paused) noexcept override;
	void on_playback_stop(play_control::t_stop_reason p_reason) noexcept override;
	void on_playback_time(double p_time) noexcept override;
	void on_volume_change(float p_new_val) noexcept override;
};

FB2K_SERVICE_FACTORY(UIWizardCallbacks);
#pragma endregion

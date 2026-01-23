<div align="center">
  <br>
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../images/UI-Wizard-Logo.svg">
    <img src="../images/UI-Wizard-Logo.svg" width="400" alt="UI Wizard Logo">
  </picture>
  <br>
  <br>
</div>

<div align="center">
  <h1>
    <picture>
      <source media="(prefers-color-scheme: dark)" srcset="../images/UI-Wizard-Title-Dark.svg">
      <img src="../images/UI-Wizard-Title-Light.svg" alt="UI Wizard Title">
    </picture>
  </h1>
</div>

<div align="center">
  <picture>
    <source media="(prefers-color-scheme: dark)" srcset="../images/UI-Wizard-Subtitle-Dark.svg">
    <img src="../images/UI-Wizard-Subtitle-Light.svg" alt="UI Wizard Subtitle">
  </picture>
</div>

<br>
<br>

*Deep within the luminous **Sapphiraz Sanctum**, arcane scholars of **The Wizardium**
labor to decipher the sealed passages of the **UI Wizard**, a radiant chapter in the eternal grimoire.
Though not all pages are yet restored, each revelation refines the **Sapphire Spell**,
weaving elegance and stability into foobar2000’s ethereal form.*

<br>

<h3 align="center"><em><span title="The Ethereal Light Commands">⸺ Lux Etherea Imperat ⸺</span></em></h3>
<div align="center"><a href="https://github.com/The-Wizardium">A Sacred Chapter Of The Wizardium</a></div>

<br>
<h2></h2>
<br>
<br>

## Version 0.2.6 - 23-01-2026
This release resolves a critical COM ABI compatibility issue in 32-bit builds and ensures correct window background color handling when set from JavaScript.

<br>

### Fixed:
- **COM ABI Mismatch on VARIANT Parameters**: Fixed a COM ABI incompatibility in VARIANT handling that caused crashes on x86 builds.
- **Window Background Color Channel Swapping**: Corrected COLORREF byte-order handling in `WindowBgColor` setter/getter, ensuring colors set via the JavaScript `RGB` helper now display exactly as intended.

<br>
<br>

## Version 0.2.5 - 15-01-2026
This release implements a more robust window validation system, preventing accidental resets while fully supporting window-snapping features.

<br>

### Fixed:
- **Premature Window Resets**: Fixed an issue where the window would incorrectly reset to the center or default position when placed near monitor edges. This restoration also ensures seamless compatibility with native window-snapping features.
- **Persistence of Accessibility**: Improved boundary logic to align with standard OS behavior, where a window position is only invalidated if the caption area becomes unreachable. The window now allows a **50 px overhang** for maintaining "grab-ability" before triggering a safety reset.
- **Multi-Monitor Boundary Logic**: Corrected an error where windows were being validated against the primary monitor's work area instead of the specific monitor they currently occupy.

<br>
<br>

## Version 0.2.4 - 16-12-2025
This release is a small update that restores the default ESC key behavior.

<br>

### Changed:
- Removed the forced fullscreen exit when pressing the ESC key.
- Developers should now control this behavior using the `ExitFullscreen` API in JavaScript.


<br>
<br>

## Version 0.2.3 - 08-11-2025
This release is a hotfix addressing fullscreen and window state issues.

<br>

### Fixed:
- Fixed bug in exiting fullscreen toggle via the API `ExitFullscreen` method.
- Fixed Alt + Tab handling while in fullscreen window state.
- Fixed window focus restoration when clicking on the title bar.
- Fixed window state enum mappings to match API docs for `WindowState` getter: // 0 (Normal), 1 (Maximized), 2 (Fullscreen)
- Fixed display of the caption area indicator when changing caption area values in the UI Wizard preferences.

<br>
<br>

## Version 0.2.2 - 20-09-2025
This release is a hotfix addressing a Windows 11-specific issue with rounded corners
causing gaps in maximized and fullscreen window states.

<br>

### Fixed:
- Fixed 2px gaps at monitor edges in maximized and fullscreen modes on Windows 11,
  where rounded window corners prevented the window from reaching full edge-to-edge coverage.

<br>
<br>

## Version 0.2.1 - 19-09-2025
This release is a hotfix for "NoBorder" window maximization issues.

<br>

### Fixed:
- Fixed window restore issue in maximized state for "NoBorder" frame style.
- Fixed cosmetic fullscreen glitch during window maximization with "NoBorder" frame style.
- Fixed position offset issue when dragging a maximized window with "NoBorder" frame style.

<br>
<br>

## Version 0.2.0 - 16-09-2025
This release enhances fullscreen support, improves Windows 11 compatibility, and fixes several window management issues.

<br>

### Added:
- Added `WindowMinimize()` and `WindowRestore()` API methods.
- Added true fullscreen support for all frame styles, not just "NoBorder".
- Added default rounded window corners when using Windows 11.
- Added default drag-and-drop support in maximized state when the mouse is dragged via the titlebar or caption area.
- Added default disabling of window resizing or repositioning when the window is in fullscreen state.

<br>

### Changed:
- Changed the maximized state to transition directly to the fullscreen state.
- Disabled custom window shadow on Windows 11, as the Windows DWM handles shadows for all frame styles.
  The custom window shadow is supported only on Windows 10 or earlier.

<br>

### Fixed:
- Fixed Alt + Tab titlebar glitch in fullscreen mode.
- Fixed window refresh issue when changing frame styles and using foo_ui_columns.
- Fixed window focus state on foobar2000 startup.
- Fixed window maximized and fullscreen states on foobar2000 startup.
- Fixed maximized state when minimizing and restoring the window.
- Fixed an issue with foo_ui_columns where the column divider disappeared during resizing or turned black after minimizing and restoring.
- Fixed custom font sizes in UI Wizard Preferences when changing DPI settings.

<br>
<br>

## Version 0.1.0 - 02-09-2025
- Initial release of UI Wizard.

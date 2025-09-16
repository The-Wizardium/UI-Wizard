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

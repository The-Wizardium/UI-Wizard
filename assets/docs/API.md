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

*In the radiant depths of the **Sapphiraz Sanctum**, where the **Sapphire Spell** weaves its luminous threads,
the scribes of **The Wizardium** have transcribed the arcane sigils of control.
This sacred codex unveils the complete incantations - properties to bind, methods to command,
and exemplars to guide - granting worthy scholars absolute dominion over the window's ethereal form,
from its crystalline geometry to its very state of being.*

<br>

<h3 align="center"><em><span title="The Ethereal Light Commands">⸺ Lux Etherea Imperat ⸺</span></em></h3>
<div align="center"><a href="https://github.com/The-Wizardium">A Sacred Chapter Of The Wizardium</a></div>

<br>
<h2></h2>
<br>
<br>

# UI Wizard - API Reference

*Version 0.2 - Last Updated: 16.09.2025*

UI Wizard provides a JavaScript API for advanced window management and appearance customization in foobar2000,
accessible via a COM/ActiveX interface in scripting environments like
[Spider Monkey Panel](https://github.com/TheQwertiest/foo_spider_monkey_panel) or
[JSplitter](https://foobar2000.ru/forum/viewtopic.php?t=6378).
This document is for foobar2000 JavaScript developers, offering practical examples and a complete API reference.

<br>
<br>

## Usage Examples

Instantiate the `UIWizard` ActiveX object in foobar2000:

```javascript
const UIWizard = new ActiveXObject('UIWizard');
```

The following examples demonstrate how to use UI Wizard to manipulate window appearance, position, size, and state.
Refer to the [API Reference](#api-reference) for available properties and methods.

<br>
<br>

### Window Appearance Customization

Set window frame style and background color:

```javascript
/**
 * Customizes the window's frame style and background color.
 */
function customizeWindowAppearance() {
	if (!UIWizard) return;

	UIWizard.FrameStyle = 3; // Set to No Border
	console.log('Frame Style set to:', UIWizard.FrameStyle); // 3 (No Border)

	UIWizard.WindowBgColor = RGB(255, 0, 0); // Set background to red (RGB)
	console.log('Window Background Color set to:', UIWizard.WindowBgColor.toString(16));
}
```

**Notes**:
- `FrameStyle`: 0 (Default), 1 (Small Caption), 2 (No Caption), 3 (No Border).
- `WindowBgColor`: Accepts RGB values (e.g., `RGB(255, 0, 0)` for red).
- Use `try-catch` for error handling.

<br>
<br>

### Window Position and Size

Adjust window position and size with constraints:

```javascript
/**
 * Adjusts the window's position and size with optional constraints.
 */
function adjustWindowGeometry() {
	if (!UIWizard) return;

	UIWizard.SetWindowPosition(100, 100); // Move window to (100, 100)
	console.log('Window Position:', UIWizard.WindowX, UIWizard.WindowY);

	UIWizard.SetWindowSize(800, 600); // Set size to 800x600
	console.log('Window Size:', UIWizard.WindowWidth, UIWizard.WindowHeight);

	UIWizard.WindowMinSize = true; // Enable minimum size constraints
	UIWizard.WindowMinWidth = 400;
	UIWizard.WindowMinHeight = 300;
	UIWizard.WindowMaxSize = true; // Enable maximum size constraints
	UIWizard.WindowMaxWidth = 1200;
	UIWizard.WindowMaxHeight = 900;
	UIWizard.SetWindowSizeLimits(400, 300, 1200, 900); // Apply constraints
	console.log('Size Constraints:', UIWizard.WindowMinWidth, UIWizard.WindowMinHeight, UIWizard.WindowMaxWidth, UIWizard.WindowMaxHeight);

	UIWizard.SetWindowPositionInGrid('top-left'); // Snap to top-left grid
	console.log('Window snapped to top-left grid');
}
```

**Notes**:
- Use `SetWindowPositionInGrid` with positions like `top-left`, `center`, etc.
- `SetWindowSizeLimits` ensures size stays within min/max bounds.
- Save and load window metrics with `SaveWindowMetrics` and `LoadWindowMetrics`.

<br>
<br>

### Window Behavior

Configure window dragging and ESC key behavior:

```javascript
/**
 * Configures window dragging and ESC key behavior.
 */
function configureWindowBehavior() {
	if (!UIWizard) return;

	UIWizard.MoveStyle = 2; // Ctrl + Alt + Left mouse button
	console.log('Move Style set to:', UIWizard.MoveStyle);

	UIWizard.SetCaptionAreaSize(0, 0, 200, 30); // Set caption area for dragging
	console.log('Caption Area set to: (0, 0, 200, 30)');

	UIWizard.DisableWindowMaximizing = true; // Disable maximizing
	console.log('Window Maximizing disabled:', UIWizard.DisableWindowMaximizing);

	UIWizard.DisableWindowSizing = false; // Enable resizing
	console.log('Window Sizing enabled:', !UIWizard.DisableWindowSizing);
}
```

**Notes**:
- `MoveStyle`: 0 (Caption only), 1 (Middle mouse), 2 (Ctrl+Alt+Left), 3 (Any method).
- `SetCaptionAreaSize` defines the draggable area for No Caption/No Border styles.
- Use `DisableWindowMaximizing` and `DisableWindowSizing` to lock window behaviors.

<br>
<br>

### Window State Management

Toggle window states programmatically:

```javascript
/**
 * Manages window state transitions (e.g., fullscreen, maximized).
 */
function manageWindowState() {
	if (!UIWizard) return;

	console.log('Current Window State:', UIWizard.WindowState); // 0 (Normal), 1 (Maximized), 2 (Fullscreen)

	UIWizard.ToggleFullscreen(); // Enter fullscreen
	console.log('Toggled to Fullscreen:', UIWizard.WindowState);

	setTimeout(() => {
		UIWizard.ExitFullscreen(); // Exit fullscreen
		console.log('Exited Fullscreen:', UIWizard.WindowState);
	}, 2000);

	UIWizard.ToggleMaximize(); // Maximize window
	console.log('Toggled to Maximized:', UIWizard.WindowState);

	setTimeout(() => {
		UIWizard.ExitMaximize(); // Restore to normal
		console.log('Exited Maximized:', UIWizard.WindowState);
	}, 4000);
}
```

**Notes**:
- `WindowState`: 0 (Normal), 1 (Maximized), 2 (Fullscreen).
- Use `ToggleFullscreen` and `ToggleMaximize` for state changes.
- `ExitFullscreen` and `ExitMaximize` restore the window to normal state.

<br>
<br>

### Display Information

Retrieve display-related properties:

```javascript
/**
 * Retrieves and logs display-related properties.
 */
function getDisplayInfo() {
	if (!UIWizard) return;

	console.log('Display DPI:', UIWizard.DisplayDPI);
	console.log('Display Resolution Mode:', UIWizard.DisplayResolutionMode);
	console.log('Display Resolution:', UIWizard.DisplayResolution);
	console.log('Multi-Monitor Resolution:', UIWizard.DisplayResolutionMultiMonitors);
}
```

**Notes**:
- Use these properties to adapt UI elements to display characteristics.
- Values are read-only and reflect current system settings.

<br>
<br>

## API Reference

The **API Reference** provides tables listing all properties and methods of the `UIWizard` ActiveX object, with notes for additional context where needed.
Refer to [Usage Examples](#usage-examples) for practical applications.

### Properties

| Name                              | Type       | Access        | Description                                                                           |
|:----------------------------------|:-----------|:--------------|:--------------------------------------------------------------------------------------|
| DisplayDPI                        | number     | Read-only     | Current display DPI setting.                                                          |
| DisplayResolutionMode             | string     | Read-only     | Display resolution mode (e.g., scaling mode).                                         |
| DisplayResolution                 | string     | Read-only     | Primary display resolution (e.g., "1920x1080").                                       |
| DisplayResolutionMultiMonitors    | string     | Read-only     | Resolution across multiple monitors.                                                  |
| WindowX                           | number     | Read-only     | Window's current x-coordinate (left edge).                                            |
| WindowY                           | number     | Read-only     | Window's current y-coordinate (top edge).                                             |
| WindowWidth                       | number     | Read-only     | Window's current width in pixels.                                                     |
| WindowHeight                      | number     | Read-only     | Window's current height in pixels.                                                    |
| WindowMinSize                     | boolean    | Read/Write    | Enable/disable minimum size constraints.                                              |
| WindowMinWidth                    | number     | Read/Write    | Minimum window width (pixels).                                                        |
| WindowMinHeight                   | number     | Read/Write    | Minimum window height (pixels).                                                       |
| WindowMaxSize                     | boolean    | Read/Write    | Enable/disable maximum size constraints.                                              |
| WindowMaxWidth                    | number     | Read/Write    | Maximum window width (pixels).                                                        |
| WindowMaxHeight                   | number     | Read/Write    | Maximum window height (pixels).                                                       |
| WindowBgColor                     | number     | Read/Write    | Window background color (RGB value, e.g., 0xFF0000 for red).                          |
| DarkMode                          | boolean    | Read/Write    | Enable/disable dark mode for the window.                                              |
| FrameStyle                        | number     | Read/Write    | Window frame style: 0 (Default), 1 (Small Caption), 2 (No Caption), 3 (No Border).    |
| MoveStyle                         | number     | Read/Write    | Window drag style: 0 (Caption), 1 (Middle mouse), 2 (Ctrl+Alt+Left), 3 (Any).         |
| DisableWindowMaximizing           | boolean    | Read/Write    | Enable/disable window maximizing.                                                     |
| DisableWindowSizing               | boolean    | Read/Write    | Enable/disable window resizing.                                                       |
| WindowState                       | number     | Read-only     | Current window state: 0 (Normal), 1 (Maximized), 2 (Fullscreen).                      |

- **Display Properties**:
  - `DisplayDPI`, `DisplayResolutionMode`, `DisplayResolution`, `DisplayResolutionMultiMonitors`: Provide system display information for UI adaptation.

- **Window Geometry**:
  - `WindowX`, `WindowY`, `WindowWidth`, `WindowHeight`: Reflect current window position and size.

- **Size Constraints**:
  - `WindowMinSize`, `WindowMaxSize`: Enable/disable constraints.
  - `WindowMinWidth`, `WindowMinHeight`, `WindowMaxWidth`, `WindowMaxHeight`: Set size limits.

- **Appearance**:
  - `WindowBgColor`: Accepts RGB values, e.g. RGB(255, 0, 0) for consistent theming.
  - `FrameStyle`: Changes window border and caption appearance.

- **Behavior**:
  - `MoveStyle`: Defines how the window can be dragged.
  - `DisableWindowMaximizing`, `DisableWindowSizing`: Lock specific window behaviors.

- **State**:
  - `WindowState`: Read-only, reflects current window mode.

<br>
<br>

### Methods

| Name                        | Signature                                                           | Description                                                  |
|:----------------------------|:--------------------------------------------------------------------|:-------------------------------------------------------------|
| SetCaptionAreaSize          | (x: number, y: number, w: number, h: number) -> void                | Sets the draggable caption area for No Caption/No Border.    |
| SetWindowPosition           | (x: number, y: number) -> void                                      | Sets the window's top-left position.                         |
| SetWindowPositionInGrid     | (position: string) -> void                                          | Snaps window to a grid position*.                            |
| SetWindowSize               | (width: number, height: number) -> void                             | Sets the window's size in pixels.                            |
| SetWindowSizeLimits         | (minW: number, minH: number, maxW: number, maxH: number) -> void    | Sets min/max size constraints.                               |
| LoadWindowMetrics           | () -> void                                                          | Loads saved window position and size.                        |
| SaveWindowMetrics           | () -> void                                                          | Saves current window position and size.                      |
| ToggleFullscreen            | () -> void                                                          | Toggles between fullscreen and normal state.                 |
| ExitFullscreen              | () -> void                                                          | Exits fullscreen mode, restoring normal state.               |
| ToggleMaximize              | () -> void                                                          | Toggles between maximized and normal state.                  |
| ExitMaximize                | () -> void                                                          | Exits maximized mode, restoring normal state.                |
| WindowMinimize              | () -> void                                                          | Minimizes the window to the taskbar.                         |
| WindowRestore               | () -> void                                                          | Restores the window from the minimized state.                |

* SetWindowPositionInGrid aligns the window to a 3x3 grid on the monitor with following values:<br>
  - 'top-left', 'top-center', 'top-right'<br>
  - 'center-left', 'center', 'center-right'<br>
  - 'bottom-left', 'bottom-center', 'bottom-right'

- **Window Manipulation**:
  - `SetCaptionAreaSize`: Defines the draggable area for custom window dragging.
  - `SetWindowPosition`, `SetWindowPositionInGrid`, `SetWindowSize`: Control window placement and size.
  - `SetWindowSizeLimits`: Applies size constraints programmatically.

- **Window Metrics**:
  - `LoadWindowMetrics`, `SaveWindowMetrics`: Manage persistent window settings.

- **Window State**:
  - `ToggleFullscreen`, `ExitFullscreen`, `ToggleMaximize`, `ExitMaximize`, `WindowMinimize`, `WindowRestore`: Control window display modes.

<br>
<br>

## Error Handling

Common errors include:
- `E_POINTER`: Invalid pointer (e.g., null or invalid arguments in methods). Ensure valid inputs.
- `E_INVALIDARG`: Invalid argument (e.g., out-of-range values for size or position). Check parameter type and ranges.

<br>
<br>

## Support

- [UI Wizard on Github](https://github.com/The-Wizardium/UI-Wizard)

//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Input.h
// Purpose: Class for Handling Input from Input Devices
//          it uses DirectInput
//          - Supports: Gamepad, Mouse and Keyboard
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#pragma once

// Define the DInput Version
#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x0800
#endif

// including the DirectInput libraries:
#pragma comment(lib, "DInput8.lib")
#pragma comment(lib, "DXGUID.lib")
#pragma comment(lib, "DxErr.lib")

// Includes:
#include <dinput.h>
#include <DxErr.h>
#include <d3dx9.h>
#include <cassert>
#include "core/system/ErrorHandler.h"

// XBOX 360 Controller
// Left Stick: x,y
// Right Stick: Rx, Ry
// Digital: POV 1
// LT: z+
// RT: z-
// A: 0
// B: 1
// X: 2
// Y: 3
// LB: 4
// RB: 5
// Back: 6
// Start: 7
// L3: 8
// R3: 9

// static global GUID for the Gamepad.
static GUID GamepadGUID;

/* Callback function is called ONCE for every InputDevice, when we enumerate through the devices. */
static BOOL CALLBACK DeviceCallback(LPCDIDEVICEINSTANCE DeviceInstance, void* pRef)
{
	// Use the first Gamepad connected to the system.
	GamepadGUID = DeviceInstance->guidInstance;


	// Stop the enumeration
	return DIENUM_STOP;
}

/* Class for Handling HID Devices. */
class Input
{
	// Functions:
	// Public Functions:
public:
	Input(const HINSTANCE appInstance, const HWND hWnd);
	~Input();

	void Initialize();
	void Shutdown();

	// Get the Newest Infos from the HID's
	void UpdateInput();

	// Get New Input Devices, if there where new additions to the systems!
	void GetNewInputDevices();

	// Keyboard Functions:
	/* Is a key pressed? */
	const bool KeyIsPressed(UCHAR keycode) const;
	/* Tipping only occurs when a button is pressed for only the one frame
	 * the pressing was recognized, so it will only react ONCE even if
	 * you hold the key down.*/
	const bool KeyIsTipped(UCHAR keycode) const;

	// Mouse Functions:
	/* Is the left Mouse Button pressed? */
	const bool MouseLeftPressed() const;
	/* Tipping only occurs in one single frame even if you hold the button. */
	const bool MouseLeftTipped() const;
	/* Is the right Mouse Button pressed? */
	const bool MouseRightPressed() const;
	/* Tipping only occurs in one single frame even if you hold the button. */
	const bool MouseRightTipped() const;
	/* Is the Mousewheel Pressed? */
	const bool MouseMiddlePressed() const;
	/* Tipping only occurs in one single frame even if you hold the button. */
	const bool MouseMiddleTipped() const;
	/* Get Mousemovement*/
	const LONG GetMouseMovementX() const;
	const LONG GetMouseMovementY() const;
	/* Get MouseWheel Movement*/
	const LONG GetMouseWheelMovement() const;

	// Gamepad Functions:
	const LONG GetGamePadXAxis1() const;
	const LONG GetGamePadYAxis1() const;
	const LONG GetGamePadXAxis2() const;
	const LONG GetGamePadYAxis2() const;
	const bool GamePadButtonIsPressed(UCHAR Number) const;
	const bool GamePadButtonIsTipped(UCHAR Number) const;
	const DWORD GetGamePadPov(UCHAR Number) const;

	// Private Functions:
private:
	/* Calibrate the Joystick (DeadZone, Saturation and Range). */
	void CalibrateJoystick();
	// Member:
	// Private Member:
private:
	const HWND hWnd;
	const HINSTANCE appInstance;

	/* the main DirectInput8 interface. */
	LPDIRECTINPUT8 directInput;
	// The Supported InputDevices
	LPDIRECTINPUTDEVICE8 keyboard;
	LPDIRECTINPUTDEVICE8 mouse;
	LPDIRECTINPUTDEVICE8 gamepad;

	// Data storage for the Input Devices.
	/* Storage for the Keyboard. */
	BYTE currentkeys[256];
	BYTE lastkeys[256];
	/* Storage for the mouse. */
	DIMOUSESTATE currentMouseData;
	DIMOUSESTATE lastMouseData;
	/* Storage for the Gamepad. */
	DIJOYSTATE currentGamepadData;
	DIJOYSTATE lastGamepadData;

	/* ErrorHandler: */
	ErrorHandler* errorHandler;
};


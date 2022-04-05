//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: Input.h
// Purpose: Class for Handling Input from Input Devices
//          it uses DirectInput
//          - Supports: Gamepad, Mouse and Keyboard
//////////////////////////////////////////////////////////////////

// Includes
#include "Input.h"

// Functions:
// Public Functions:
Input::Input(const HINSTANCE appInstance, const HWND hWnd)
	:
	hWnd(hWnd),
	appInstance(appInstance),
	directInput(nullptr),
	keyboard(nullptr),
	mouse(nullptr),
	gamepad(nullptr),
	errorHandler(ErrorHandler::GetInstance())
{
}
Input::~Input()
{
}


void Input::Initialize()
{
	HRESULT result;

	errorHandler->WriteLog("Initialiazing Inputsystem ...");

	// 1st: create the DirectInput8 interface:
	result = DirectInput8Create(appInstance,	// Instance of the programm.
		0x0800,									// DirectInput Version (DirectX9 didn't make changes to DInput, 
		// so there is no DInput9! (Use the "newest" Version 8.0 instead))
		IID_IDirectInput8,						// Let the programm decide if we use ANSI- or UNICODE version.
		(void**)(&directInput),				// Fillout the Interface.
		NULL);									// Not needed (Something with COM aggregation).

	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to create a valid DirectInput8 Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 2nd: Create a valid GamepadDevice
	// 2.1.: Enumerate through all Gamecontroller devices
	result = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL,			// Want to Look for GameController (Xbox360 Controller, Joysticks or Steeringwheel).
		DeviceCallback,											// The Function that is called for every found Device.
		NULL,							// No extra information for the Callbackfunction.
		DIEDFL_ATTACHEDONLY);			// Want to look only for attached devices.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to enumerate through the GamepadDevices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 2.2.: Create the GamecontrollerDevice.
	result = directInput->CreateDevice(GamepadGUID, &gamepad, NULL);
	if (FAILED(result) && result != REGDB_E_CLASSNOTREG)
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to create a valid Gamepad Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	if (result != REGDB_E_CLASSNOTREG)
	{
		// 2.3.: Set the Dataformat for the Device (how shall DInput Interpret this device?)
		result = gamepad->SetDataFormat(&c_dfDIJoystick);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Input.Initialize(): Failed to set the Gamepad Dataformat: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

		// 2.4.: Set the Cooperationlevel (special instructions for device-access).
		result = gamepad->SetCooperativeLevel(hWnd,
			DISCL_NONEXCLUSIVE |		// Other applications may use this device.
			DISCL_FOREGROUND);			// Only get data from the device if the window is focused.
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Input.Initialize(): Failed to set the Gamepad Cooperation Level: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

		// 2.5.: Calibrate the Gamepad.
		CalibrateJoystick();

		// 2.6.: Acquire Access to the Interface
		result = gamepad->Acquire();
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Input.Initialize(): Failed to Acquire the Gamepad: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}
	}


	// 3rd: Create a valid Keyboard Device
	// 3.1.: Create the Keyboard Device with the primary, systemkeyboard.
	result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to create a valid Keyboard Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3.2.: Set the Dataformat for the Device (how shall DInput Interpret this device?)
	result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to set the Keyboard Dataformat: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3.3.: Set the Cooperationlevel (special instructions for device-access).
	result = keyboard->SetCooperativeLevel(hWnd,
		DISCL_NONEXCLUSIVE |		// Other applications may use this device.
		DISCL_FOREGROUND);			// Only get data from the device if the window is focused.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to set the Keyboard Cooperation Level: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 3.4.: Acquire Access to the Interface
	result = keyboard->Acquire();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to Acquire the Keyboard: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 4th: Create a valid Mouse Device
	// 4.1.: Create the Mouse Device with the primary, systemmouse.
	result = directInput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to create a valid Mouse Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4.2.: Set the Dataformat for the Device (how shall DInput Interpret this device?)
	result = mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to set the Mouse Dataformat: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}

	// 4.3.: Set the Cooperationlevel (special instructions for device-access).
	result = mouse->SetCooperativeLevel(hWnd,
		DISCL_NONEXCLUSIVE |		// Other applications may use this device.
		DISCL_FOREGROUND);			// Only get data from the device if the window is focused.
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to set the Mouse Cooperation Level: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 4.4.: Acquire Access to the Interface
	result = mouse->Acquire();
	if (FAILED(result))
	{
		char buffer[256];
		sprintf(buffer, "Input.Initialize(): Failed to Acquire the Mouse: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
		errorHandler->CreateFatalError(buffer);
		return;
	}


	// 5th: Set all Data for the Devices to Null
	// 5.1. Keyboard
	for (auto& entry : currentkeys)
	{
		entry = 0;
	}

	for (auto& entry : lastkeys)
	{
		entry = 0;
	}

	// 5.2. Mouse
	currentMouseData.lX = 0;
	currentMouseData.lY = 0;
	currentMouseData.lZ = 0;
	for (auto& Entry : currentMouseData.rgbButtons)
	{
		Entry = 0;
	}

	lastMouseData.lX = 0;
	lastMouseData.lY = 0;
	lastMouseData.lZ = 0;
	for (auto& Entry : lastMouseData.rgbButtons)
	{
		Entry = 0;
	}

	// 5.3. Gamepad
	currentGamepadData.lX = 0;
	currentGamepadData.lY = 0;
	currentGamepadData.lZ = 0;
	currentGamepadData.lRx = 0;
	currentGamepadData.lRy = 0;
	currentGamepadData.lRz = 0;
	currentGamepadData.rglSlider[0] = 0;
	currentGamepadData.rglSlider[1] = 0;
	currentGamepadData.rgdwPOV[0] = -1;
	currentGamepadData.rgdwPOV[1] = -1;
	currentGamepadData.rgdwPOV[2] = -1;
	currentGamepadData.rgdwPOV[3] = -1;
	for (auto& entry : currentGamepadData.rgbButtons)
	{
		entry = 0;
	}

	lastGamepadData.lX = 0;
	lastGamepadData.lY = 0;
	lastGamepadData.lZ = 0;
	lastGamepadData.lRx = 0;
	lastGamepadData.lRy = 0;
	lastGamepadData.lRz = 0;
	lastGamepadData.rglSlider[0] = 0;
	lastGamepadData.rglSlider[1] = 0;
	lastGamepadData.rgdwPOV[0] = -1;
	lastGamepadData.rgdwPOV[1] = -1;
	lastGamepadData.rgdwPOV[2] = -1;
	lastGamepadData.rgdwPOV[3] = -1;
	for (auto& entry : lastGamepadData.rgbButtons)
	{
		entry = 0;
	}

	errorHandler->WriteLog("Inputsystem Initialized.");

	return;
}
void Input::Shutdown()
{
	errorHandler->WriteLog("Shutdown Inputsystem ...");

	// Release the Interfaces.
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = nullptr;
	}

	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = nullptr;
	}

	if (gamepad)
	{
		gamepad->Unacquire();
		gamepad->Release();
		gamepad = nullptr;
	}

	if (directInput)
	{
		directInput->Release();
		directInput = nullptr;
	}

	errorHandler->WriteLog("Inputsystem Shutdown successful.");

	return;
}

void Input::UpdateInput()
{
	HRESULT result;

	// 1st: Get the Information from the keyboard.
	// 1.1: Set the lastkeys to the currentkeys
	for (int i = 0; i < 256; i++)
	{
		lastkeys[i] = currentkeys[i];
	}

	// 1.2: Set every entry of the currentkeys to NULL.
	for (auto& Entry : currentkeys)
	{
		Entry = 0;
	}


	// 1.3. Get the Data from the Device
	if (keyboard)	// Only do that if we have a valid interface
	{
		result = keyboard->GetDeviceState(256, currentkeys);
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))	// Couldn't get the Input-Data.
		{
			result = keyboard->Acquire();
			if (FAILED(result) && hWnd == GetForegroundWindow())
			{
				char buffer[256];
				sprintf(buffer, "Input.UpdateInput(): Failed to Acquiere the Keyboard: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateMinorError(buffer);
			}
		}
	}

	// 2nd: Get the Information from the mouse.
	// 2.1. set the lastMouseData to the currentMouseData
	lastMouseData = currentMouseData;

	// 2.2. Reset the Mouse Data
	currentMouseData.lX = 0;
	currentMouseData.lY = 0;
	currentMouseData.lZ = 0;
	for (auto& Entry : currentMouseData.rgbButtons)
	{
		Entry = 0;
	}


	// 2.3 Get the Data from the Device.
	if (mouse) // Only do that if we have a valid interface
	{
		result = mouse->Poll();
		result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), &currentMouseData);
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))	// Couldn't get the Input-Data.
		{
			result = mouse->Acquire();
			if (FAILED(result) && hWnd == GetForegroundWindow())
			{
				char buffer[256];
				sprintf(buffer, "Input.UpdateInput(): Failed to Acquiere the Mouse: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateMinorError(buffer);
			}
		}

	}

	// 3rd: Get the information from the Gamepad.
	// 3.1. set the lastGamepaData to the currentGamepadData
	lastGamepadData = currentGamepadData;


	// 3.1.: Reset the Gamepad Data
	currentGamepadData.lX = 0;
	currentGamepadData.lY = 0;
	currentGamepadData.lZ = 0;
	currentGamepadData.lRx = 0;
	currentGamepadData.lRy = 0;
	currentGamepadData.lRz = 0;
	currentGamepadData.rglSlider[0] = 0;
	currentGamepadData.rglSlider[1] = 0;
	currentGamepadData.rgdwPOV[0] = -1;
	currentGamepadData.rgdwPOV[1] = -1;
	currentGamepadData.rgdwPOV[2] = -1;
	currentGamepadData.rgdwPOV[3] = -1;
	for (auto& Entry : currentGamepadData.rgbButtons)
	{
		Entry = 0;
	}

	// 3.2 Get the Data from the Device.
	if (gamepad) // Only do that if we have a valid interface
	{
		result = gamepad->Poll();
		result = gamepad->GetDeviceState(sizeof(DIJOYSTATE), &currentGamepadData);
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))	// Couldn't get the Input-Data.
		{
			result = gamepad->Acquire();
			if (FAILED(result) && hWnd == GetForegroundWindow())
			{
				char buffer[256];
				sprintf(buffer, "Input.UpdateInput(): Failed to Acquiere the Gamepad: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateMinorError(buffer);
			}
		}
	}
}
void Input::GetNewInputDevices()
{
	HRESULT result;

	if (directInput != nullptr)
	{
		// 1st: Check Gamepad Device
		if (gamepad == nullptr)
		{
			// 1.1.: Enumerate through all Gamecontroller devices
			result = directInput->EnumDevices(DI8DEVCLASS_GAMECTRL,			// Want to Look for GameController (Xbox360 Controller, Joysticks or Steeringwheel).
				DeviceCallback,											// The Function that is called for every found Device.
				NULL,							// No extra information for the Callbackfunction.
				DIEDFL_ATTACHEDONLY);			// Want to look only for attached devices.
			if (FAILED(result))
			{
				char buffer[256];
				sprintf(buffer, "Input.GetNewInputDevices(): Failed to enumerate through the GamepadDevices: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			// 1.2.: Create the GamecontrollerDevice.
			result = directInput->CreateDevice(GamepadGUID, &gamepad, NULL);
			if (FAILED(result) && result != REGDB_E_CLASSNOTREG)
			{
				char buffer[256];
				sprintf(buffer, "Input.GetNewInputDevices(): Failed to create a valid Gamepad Interface: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
				errorHandler->CreateFatalError(buffer);
				return;
			}

			if (result != REGDB_E_CLASSNOTREG)
			{
				// 1.3.: Set the Dataformat for the Device (how shall DInput Interpret this device?)
				result = gamepad->SetDataFormat(&c_dfDIJoystick);
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Input.GetNewInputDevices(): Failed to set the Gamepad Dataformat: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}

				// 1.4.: Set the Cooperationlevel (special instructions for device-access).
				result = gamepad->SetCooperativeLevel(hWnd,
					DISCL_NONEXCLUSIVE |		// Other applications may use this device.
					DISCL_FOREGROUND);			// Only get data from the device if the window is focused.
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Input.GetNewInputDevices(): Failed to set the Gamepad Cooperation Level: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}

				// 1.5.: Calibrate the Gamepad.
				CalibrateJoystick();

				// 1.6.: Acquire Access to the Interface
				result = gamepad->Acquire();
				if (FAILED(result))
				{
					char buffer[256];
					sprintf(buffer, "Input.GetNewInputDevices(): Failed to Acquire the Gamepad: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
					errorHandler->CreateFatalError(buffer);
					return;
				}
			}
		}
	}
}

const bool Input::KeyIsPressed(UCHAR keycode) const
{

	// 1st: Check if the Key is pressed
	if (currentkeys[keycode] & 0x80)	// Is the 8th Bit set ?(the key is then pressed!)
	{
		return true;
	}
	// The Key wasn't pressed!
	return false;

}
const bool Input::KeyIsTipped(UCHAR keycode) const
{
	if (!(lastkeys[keycode] & 0x80) && (currentkeys[keycode] & 0x80))
	{
		// Last Frame there was nothing and this frame there is, so it's tipped!
		return true;
	}
	return false;
}

const bool Input::MouseLeftPressed() const
{
	// 1st: Check if LeftMouseButton is pressed.
	if (currentMouseData.rgbButtons[0] & 0x80) // Is the 8th Bit set ?(the leftmousebutton is then pressed!)
	{
		return true;
	}
	// The LeftMouseButton wasn't pressed!
	return false;
}
const bool Input::MouseLeftTipped() const
{
	if (!(lastMouseData.rgbButtons[0] & 0x80) && (currentMouseData.rgbButtons[0] & 0x80))
	{
		// Last Frame there was nothing and this frame there is, so it's tipped!
		return true;
	}
	return false;
}
const bool Input::MouseRightPressed() const
{
	// 1st: Check if RightMouseButton is pressed.
	if (currentMouseData.rgbButtons[1] & 0x80) // Is the 8th Bit set ?(the RightMouseButton is then pressed!)
	{
		return true;
	}
	// The RightMouseButton wasn't pressed!
	return false;
}
const bool Input::MouseRightTipped() const
{
	if (!(lastMouseData.rgbButtons[1] & 0x80) && (currentMouseData.rgbButtons[1] & 0x80))
	{
		// Last Frame there was nothing and this frame there is, so it's tipped!
		return true;
	}
	return false;
}
const bool Input::MouseMiddlePressed() const
{
	// 1st: Check if MiddleMouseButton is pressed.
	if (currentMouseData.rgbButtons[2] & 0x80) // Is the 8th Bit set ?(the MiddleMouseButton is then pressed!)
	{
		return true;
	}
	// The MiddleMouseButton wasn't pressed!
	return false;
}
const bool Input::MouseMiddleTipped() const
{
	if (!(lastMouseData.rgbButtons[2] & 0x80) && (currentMouseData.rgbButtons[2] & 0x80))
	{
		// Last Frame there was nothing and this frame there is, so it's tipped!
		return true;
	}
	return false;
}
const LONG Input::GetMouseMovementX() const
{
	// 1st: Return the MouseMovementX Value.
	return currentMouseData.lX;
}
const LONG Input::GetMouseMovementY() const
{
	// 1st: Return the MouseMovementY Value.
	return currentMouseData.lY;
}
const LONG Input::GetMouseWheelMovement() const
{
	// 1st: Return the negative MouseMovementZ Value.
	return -currentMouseData.lZ;
}

const LONG Input::GetGamePadXAxis1() const
{
	// 1st: Return the GamePadX Value.
	return currentGamepadData.lX;
}
const LONG Input::GetGamePadYAxis1() const
{
	// 1st: Return the GamePadY Value.
	return currentGamepadData.lY;
}
const LONG Input::GetGamePadXAxis2() const
{
	// 1st: Return the GamePadRotationX Value.
	return currentGamepadData.lRx;
}
const LONG Input::GetGamePadYAxis2() const
{
	// 1st: Return the GamePadRotationY Value.
	return currentGamepadData.lRy;
}
const bool Input::GamePadButtonIsPressed(UCHAR Number) const
{
	// 1st: Check if the GamepadButton is pressed.
	if (currentGamepadData.rgbButtons[Number] & 0x80) // Is the 8th Bit set ?(the GamepadButton is then pressed!)
	{
		return true;
	}
	// The GamepadButton wasn't pressed!
	return false;
}
const bool Input::GamePadButtonIsTipped(UCHAR Number) const
{
	if (!(lastGamepadData.rgbButtons[Number] & 0x80) && (currentGamepadData.rgbButtons[Number] & 0x80))
	{
		// Last Frame there was nothing and this frame there is, so it's tipped!
		return true;
	}
	return false;
}
const DWORD Input::GetGamePadPov(UCHAR Number) const
{
	// 1st: Return the GamePadPov Value.
	return currentGamepadData.rgdwPOV[Number];
}

// Private Functions:
void Input::CalibrateJoystick()
{
	if (gamepad)	// Only do that if we have a valid interface
	{
		HRESULT result;

		DIPROPRANGE Range;				// Scaling of the Axes.
		DIPROPDWORD DeadZone;			// DeadZone of the Axes.
		DIPROPDWORD Saturation;			// Saturation of the Axes.

		// 1st: Axes Scaling:
		Range.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		Range.diph.dwSize = sizeof(DIPROPRANGE);
		Range.diph.dwObj = 0;
		Range.diph.dwHow = DIPH_DEVICE;			// Changes made shall be used for the whole device (therefore dwObj must be 0!)
		Range.lMin = -1000;
		Range.lMax = +1000;
		result = gamepad->SetProperty(DIPROP_RANGE, &Range.diph);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Input.CalibrateJoystick(): Failed to set the Scaling of the Gamepad Axes: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

		// 2nd: DeadZone.
		DeadZone.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		DeadZone.diph.dwSize = sizeof(DIPROPDWORD);
		DeadZone.diph.dwObj = 0;
		DeadZone.diph.dwHow = DIPH_DEVICE;		// Changes made shall be used for the whole device (therefore dwObj must be 0!)
		DeadZone.dwData = 1200;					// 5,000 = 50% DeadZone! (1200 = 12% DeadZone).
		result = gamepad->SetProperty(DIPROP_DEADZONE, &DeadZone.diph);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Input.CalibrateJoystick(): Failed to set the Deadzone of the Gamepad Axes: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

		// 3rd: Saturation.
		Saturation.diph.dwHeaderSize = sizeof(DIPROPHEADER);
		Saturation.diph.dwSize = sizeof(DIPROPDWORD);
		Saturation.diph.dwObj = 0;
		Saturation.diph.dwHow = DIPH_DEVICE;		// Changes made shall be used for the whole device (therefore dwObj must be 0!)
		Saturation.dwData = 9500;					// 5,000 = 50% Saturation! (9500 = 95% Saturation).
		result = gamepad->SetProperty(DIPROP_SATURATION, &Saturation.diph);
		if (FAILED(result))
		{
			char buffer[256];
			sprintf(buffer, "Input.CalibrateJoystick(): Failed to set the Saturation of the Gamepad Axes: %s, %s", DXGetErrorStringA(result), DXGetErrorDescriptionA(result));
			errorHandler->CreateFatalError(buffer);
			return;
		}

	}
}

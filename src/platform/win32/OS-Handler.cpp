//////////////////////////////////////////////////////////////////
// Orchid 2D Framework
// Version: Alpha 0.1
// File: OS-Handler.cpp
// Purpose: Holds the Windows main function and message handler.
//////////////////////////////////////////////////////////////////

// Preprocessor Directives:
#define WIN32_LEAN_AND_MEAN

// Includes
#include <Windows.h>
#include <d3d9.h>
#include <cassert>
#include <fcntl.h>
#include <io.h>
#include <Dbt.h>
#include <stdio.h>
#include <vector>
#include "resource/resource.h"
#include "core/system/Settings.h"
#include "game/Game.h"

// Libraries
// DirectX Libraries
#pragma comment(lib, "legacy_stdio_definitions.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "dxerr.lib")

// Global Data
/* The Settings of the Engine */
static SETTINGS g_settings;
/* The Main Direct3D Interface */
static LPDIRECT3D9 g_p_direct3D = nullptr;
/* std::vector of all supported Displaymodes of the Device */
static std::vector<D3DDISPLAYMODE> g_displayModes;
/* std::vector of all supported MultiSampleTypes. */
static std::vector<D3DMULTISAMPLE_TYPE> g_multisampleTypes;
/* Shall the Framework start? */
static bool g_startWindow = false;
/* Did some Devices Change? */
static bool g_deviceChange = false;

// Calculation Functions
void SetRenderingTypes(HWND hDlg)
{
	// Variables:
	char string[50];

	// 1st: Add Hardware Acceleration to the list.
	strcpy(string, "Hardware Acceleration");
	SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_ADDSTRING, 0, (LPARAM)(string));

	// 2nd: Add Software Emulation to the list.
	strcpy(string, "Software Emulation");
	SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_ADDSTRING, 0, (LPARAM)(string));
	SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_SETCURSEL, 0, 0);
}
void FindResolutions(HWND hDlg)
{
	// Variables:
	HRESULT				result;
	int					numVideoModes;
	const D3DFORMAT		formats[] = { D3DFMT_X8R8G8B8, D3DFMT_R5G6B5 };
	D3DDISPLAYMODE		videoMode;
	char				formatString[256];
	char				videoModeString[256];
	int					listIndex;
	const UINT			primaryResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
	const UINT			primaryResolutionHeight = GetSystemMetrics(SM_CYSCREEN);

	// 1st: Reset the Data
	SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_RESETCONTENT, 0, 0);
	g_displayModes.clear();

	// 2nd: Fill Data
	// 2.1. Go through every Adapter format supported
	for (int iFormat = 0; iFormat < sizeof(formats) / sizeof(D3DFORMAT); iFormat++)
	{
		// 2.2. Get The Number of VideoModes for that Adapterformat
		numVideoModes = g_p_direct3D->GetAdapterModeCount(D3DADAPTER_DEFAULT, formats[iFormat]);

		// 2.3. Go through every VideoMode supported
		for (int iVideoMode = 0; iVideoMode < numVideoModes; iVideoMode++)
		{
			// 2.4. Get the VideoMode
			result = g_p_direct3D->EnumAdapterModes(D3DADAPTER_DEFAULT, formats[iFormat], iVideoMode, &videoMode);
			assert(!FAILED(result));

			// 2.5. Create a String according to that VideoMode.
			if (videoMode.Format == D3DFMT_X8R8G8B8)
			{
				strcpy(formatString, "X8R8G8B8");
			}
			else if (videoMode.Format == D3DFMT_R5G6B5)
			{
				strcpy(formatString, "R5G6B5");
			}
			sprintf(videoModeString, "%d x %d (%s, %d Hz)", videoMode.Width, videoMode.Height, formatString, videoMode.RefreshRate);

			// 2.6. Is the Windowed Radio checked?
			if (IsDlgButtonChecked(hDlg, IDC_RADIO_WINDOWED))
			{
				// 2.7 Only if the Windowed resolution doesn't exceed the primary resolution.
				if (videoMode.Height <= primaryResolutionHeight || videoMode.Width <= primaryResolutionWidth)
				{
					// 2.7.1. Make a new list entry.
					listIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)(videoModeString)));

					// 2.7.2. Asign Data to that
					g_displayModes.push_back(videoMode);
				}
			}
			else // 2.8. The Windowed Radio is not checked!
			{
				// 2.8.1. Make a new list entry.
				listIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_ADDSTRING, 0, (LPARAM)(videoModeString)));

				// 2.8.2. Asign Data to that
				g_displayModes.push_back(videoMode);
			}
		}

		// 2.9. Select first videoMode
		SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_SETCURSEL, 0, 0);
	}
}
void SetBufferCount(HWND hDlg)
{
	// Variables:
	char string[50];

	// 1st: Add Bouble Buffering to the List
	strcpy(string, "2: Doublebuffering");
	SendDlgItemMessageA(hDlg, IDC_COMBO_BUFFERCOUNT, CB_ADDSTRING, 0, (LPARAM)(string));

	// 2nd: Add Tripple Buffering to the List
	strcpy(string, "3: Triplebuffering");
	SendDlgItemMessageA(hDlg, IDC_COMBO_BUFFERCOUNT, CB_ADDSTRING, 0, (LPARAM)(string));

	// 3rd: Select DoubleBuffering as the standart
	SendDlgItemMessageA(hDlg, IDC_COMBO_BUFFERCOUNT, CB_SETCURSEL, 0, 0);
}
void FindMultiSampleTypes(HWND hDlg)
{
	// Variables:
	int selectedDevicetypeIndex;
	D3DDEVTYPE selectedDevicetype;
	int selectedVideomodeIndex;
	D3DDISPLAYMODE selectedVideoMode;
	BOOL windowed;
	D3DFORMAT selectedBackBufferFormat;
	const D3DFORMAT selectedDepthStencilBufferFormat = D3DFMT_D24S8;
	const D3DMULTISAMPLE_TYPE		multiSamplingType[] = { D3DMULTISAMPLE_NONE,
		D3DMULTISAMPLE_NONMASKABLE,
		D3DMULTISAMPLE_2_SAMPLES,
		D3DMULTISAMPLE_3_SAMPLES,
		D3DMULTISAMPLE_4_SAMPLES,
		D3DMULTISAMPLE_5_SAMPLES,
		D3DMULTISAMPLE_6_SAMPLES,
		D3DMULTISAMPLE_7_SAMPLES,
		D3DMULTISAMPLE_8_SAMPLES,
		D3DMULTISAMPLE_9_SAMPLES,
		D3DMULTISAMPLE_10_SAMPLES,
		D3DMULTISAMPLE_11_SAMPLES,
		D3DMULTISAMPLE_12_SAMPLES,
		D3DMULTISAMPLE_13_SAMPLES,
		D3DMULTISAMPLE_14_SAMPLES,
		D3DMULTISAMPLE_15_SAMPLES,
		D3DMULTISAMPLE_16_SAMPLES };
	char multiSamplingTypeString[256];
	DWORD numQualities;

	// 1st: Get the selected Options.
	selectedDevicetypeIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_GETCURSEL, 0, 0));
	if (selectedDevicetypeIndex == 0)
	{
		selectedDevicetype = D3DDEVTYPE_HAL;
	}
	else if (selectedDevicetypeIndex == 1)
	{
		selectedDevicetype = D3DDEVTYPE_REF;
	}
	selectedVideomodeIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_GETCURSEL, 0, 0));
	selectedVideoMode = g_displayModes[selectedVideomodeIndex];
	windowed = IsDlgButtonChecked(hDlg, IDC_RADIO_WINDOWED) == BST_CHECKED;
	if (selectedVideoMode.Format == D3DFMT_X8R8G8B8)
	{
		selectedBackBufferFormat = D3DFMT_A8R8G8B8;
	}
	else if (selectedVideoMode.Format == D3DFMT_R5G6B5)
	{
		selectedBackBufferFormat = D3DFMT_X1R5G5B5;
	}

	// 2nd: Reset the Data.
	SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPTYPE, CB_RESETCONTENT, 0, 0);
	g_multisampleTypes.clear();

	// 3rd: Go through all Multi-Sample-Types
	for (int iMultiSampleType = 0; iMultiSampleType < 17; iMultiSampleType++)
	{
		// 3.1. Is this Type compatible?
		if (SUCCEEDED(g_p_direct3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, selectedDevicetype, selectedBackBufferFormat,
			windowed, multiSamplingType[iMultiSampleType], &numQualities)))
		{
			// 3.2. only if this SampleType got at least one quality option
			if (numQualities > 0)
			{
				// 3.3. Is it compatible with the Depth-Stencil-Buffer?
				if (SUCCEEDED(g_p_direct3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, selectedDevicetype, selectedDepthStencilBufferFormat,
					windowed, multiSamplingType[iMultiSampleType], &numQualities)))
				{
					// 3.4. only if this SampleType got at least one quality option
					if (numQualities > 0)
					{
						// 3.5. This MultiSampleType is OK!
						if (iMultiSampleType == 0)
						{
							strcpy(multiSamplingTypeString, "No Multi-Sampling");
						}
						else if (iMultiSampleType == 1)
						{
							strcpy(multiSamplingTypeString, "Non-Maskable");
						}
						else
						{
							sprintf(multiSamplingTypeString, "%d Samples", iMultiSampleType);
						}

						// 3.6. Create an Entry
						SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPTYPE, CB_ADDSTRING,
							0, (LPARAM)(multiSamplingTypeString));
						g_multisampleTypes.push_back(multiSamplingType[iMultiSampleType]);
					}
				}
			}
		}
	}

	// 3.7. Select the First Entry.
	SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPTYPE, CB_SETCURSEL, 0, 0);

}
void FindMultiSampleQual(HWND hDlg)
{
	// Variables:
	int						selectedDevicetypeIndex;
	D3DDEVTYPE				selectedDeviceType;
	int						selectedVideomodeIndex;
	D3DDISPLAYMODE			selectedVideoMode;
	BOOL					windowed;
	D3DFORMAT				selectedBackBufferFormat;
	int						selectedMultisamplingTypeIndex;
	D3DMULTISAMPLE_TYPE		selectedMultiSamplingType;
	DWORD					numMultiSamplingQualities;
	char					multiSamplingQualityString[256];

	// 1st: Get the used Options.
	selectedDevicetypeIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_GETCURSEL, 0, 0));
	if (selectedDevicetypeIndex == 0)
	{
		selectedDeviceType = D3DDEVTYPE_HAL;
	}
	else if (selectedDevicetypeIndex == 1)
	{
		selectedDeviceType = D3DDEVTYPE_REF;
	}
	selectedVideomodeIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_GETCURSEL, 0, 0));
	selectedVideoMode = g_displayModes[selectedVideomodeIndex];
	windowed = IsDlgButtonChecked(hDlg, IDC_RADIO_WINDOWED) == BST_CHECKED;
	if (selectedVideoMode.Format == D3DFMT_X8R8G8B8)
	{
		selectedBackBufferFormat = D3DFMT_A8R8G8B8;
	}
	else if (selectedVideoMode.Format == D3DFMT_R5G6B5)
	{
		selectedBackBufferFormat = D3DFMT_X1R5G5B5;
	}
	selectedMultisamplingTypeIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPTYPE, CB_GETCURSEL, 0, 0));
	selectedMultiSamplingType = g_multisampleTypes[selectedMultisamplingTypeIndex];

	// 2nd: Reset the Data.
	SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPQUAL, CB_RESETCONTENT, 0, 0);

	// 3rd: Get Count of Qualitylevels
	g_p_direct3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, selectedDeviceType, selectedBackBufferFormat,
		windowed, selectedMultiSamplingType, &numMultiSamplingQualities);

	// 4th: Create Entry
	for (int iQuality = 0; iQuality < (int)(numMultiSamplingQualities); iQuality++)
	{
		sprintf(multiSamplingQualityString, "Quality %d",
			iQuality + 1);
		SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPQUAL, CB_ADDSTRING,
			0, (LPARAM)(multiSamplingQualityString));
	}

	// 5th: Use the highest possible Qualitylevel
	SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPQUAL, CB_SETCURSEL,
		numMultiSamplingQualities - 1, 0);

}
void SaveCurrentSelections(HWND hDlg)
{
	// Variables:
	int selectionIndex;

	// 1st: Which Rendertype?
	selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_GETCURSEL, 0, 0));
	if (selectionIndex == 0)
	{
		g_settings.hardwareRendering = true;
	}
	else if (selectionIndex == 1)
	{
		g_settings.hardwareRendering = false;
	}

	// 2nd: Is the Windowed Radio checked?
	if (IsDlgButtonChecked(hDlg, IDC_RADIO_WINDOWED))
	{
		g_settings.windowed = true;
	}
	else
	{
		g_settings.windowed = false;
	}

	// 3rd: Which Resolution?
	selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_GETCURSEL, 0, 0));
	g_settings.resolutionWidth = g_displayModes[selectionIndex].Width;
	g_settings.resolutionHeight = g_displayModes[selectionIndex].Height;


	// 5th: How many Buffers?
	selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_BUFFERCOUNT, CB_GETCURSEL, 0, 0));
	if (selectionIndex == 0)
	{
		g_settings.bufferCount = 2;
	}
	else if (selectionIndex == 1)
	{
		g_settings.bufferCount = 3;
	}

	// 6th: Is the VSync Checkbox checked?
	if (IsDlgButtonChecked(hDlg, IDC_CHECK_VSYNC))
	{
		g_settings.vSync = true;
	}
	else
	{
		g_settings.vSync = false;
	}

	// 7th: Which MultiSampleType?
	selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPTYPE, CB_GETCURSEL, 0, 0));
	g_settings.multisampleType = g_multisampleTypes[selectionIndex];

	// 8th: Which MultiSampleQuality?
	selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_MSAMPQUAL, CB_GETCURSEL, 0, 0));
	g_settings.multisampleQuality = selectionIndex;

	// 9th: Which BackBufferFormat
	selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_GETCURSEL, 0, 0));
	if (g_displayModes[selectionIndex].Format == D3DFMT_X8R8G8B8)
	{
		// 32-Bit without Alphachannel
		g_settings.backbufferFormat = D3DFMT_A8R8G8B8;
	}
	if (g_displayModes[selectionIndex].Format == D3DFMT_R5G6B5)
	{
		// 16-Bit without Alphachannel
		g_settings.backbufferFormat = D3DFMT_A1R5G5B5;
	}

	// 10th: Swap Effect
	g_settings.swapEffect = D3DSWAPEFFECT_DISCARD;

	// 11th: Auto Depth Stencil Format
	g_settings.enableAutoDepthStencil = true;

	// 12th: 24 Bit Depth-Buffer with 8 bit Stencilbuffer.
	g_settings.autoDepthStencilFormat = D3DFMT_D24S8;

	// 13th: Lockable BackBuffer for 2D Graphictests
	g_settings.flags = NULL;

	// 14th: Refreshrate
	if (g_settings.windowed == true)
	{
		g_settings.fullscreen_RefreshRateInHz = 0;
	}
	else
	{
		selectionIndex = (int)(SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_GETCURSEL, 0, 0)); // Which BackBufferFormat
		g_settings.fullscreen_RefreshRateInHz = g_displayModes[selectionIndex].RefreshRate;
	}
}
void SaveOptions()
{
	// Save the Options to a Settings.dat as binary data.
	FILE* file = nullptr;
	file = fopen("settings.dat", "wb");
	fwrite((const void*)&g_settings, sizeof(SETTINGS), 1, file);
	fclose(file);

}
void LoadOptions(HWND hDlg)
{
	// Not finished yet!.
	// 1st: Load the Data
	FILE* file = nullptr;
	file = fopen("settings.dat", "rb");
	if (file == nullptr)	// couldn't load a file so can't load it.
	{
		return;
	}
	// 1.2.: Found a save file!
	MessageBoxA(NULL, "Options were found and loaded!", "Info!", MB_OK);
	fread((void*)&g_settings, sizeof(SETTINGS), 1, file);
	fclose(file);

	// 2nd: Now chage the selections if possible:
	// 2.1.: Windowed or- Fullscreen
	if (g_settings.windowed)
	{
		CheckRadioButton(hDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_WINDOWED);
	}
	else
	{
		CheckRadioButton(hDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_FULLSCREEN);
	}

	// 2.2.: Rendering Types.
	if (g_settings.hardwareRendering)
	{
		SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_SETCURSEL, 0, 0);
	}
	else
	{
		SendDlgItemMessageA(hDlg, IDC_COMBO_RENDERTYPE, CB_SETCURSEL, 1, 0);
	}

	// 2.3.: Resolution.
	int index = 0;
	for (auto& Entry : g_displayModes)
	{
		index++;
		if (Entry.Height == g_settings.resolutionHeight &&
			Entry.Width == g_settings.resolutionWidth &&
			Entry.RefreshRate == 60	// Test
			)
		{
			if (Entry.Format == D3DFMT_X8R8G8B8 && g_settings.backbufferFormat == D3DFMT_A8R8G8B8 ||
				Entry.Format == D3DFMT_R5G6B5 && g_settings.backbufferFormat == D3DFMT_A1R5G5B5)
			{
				// it is the right format!
				SendDlgItemMessageA(hDlg, IDC_COMBO_RESOLUTION, CB_SETCURSEL, index + 1, 0);
			}
		}
	}

	// 2.4: Render-resolution.


	// 2.5: BufferCount.


	// 2.6. VSync.


	// 2.7: MultiSampleType.


	// 2.8: MultiSampleQuality.


}

// Settings:
/* Setting_Dialog Messagefunction. */
BOOL CALLBACK SettingMsgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:	// The moment this Dialog is Created.
	{

		// 1st: Automatically Set "Windowed"
		CheckRadioButton(hDlg, IDC_RADIO_WINDOWED, IDC_RADIO_FULLSCREEN, IDC_RADIO_WINDOWED);

		// 2nd: Create a valid Direct3D Interface:
		g_p_direct3D = Direct3DCreate9(D3D_SDK_VERSION);
		assert(g_p_direct3D != nullptr);

		// 3rd: Rendering Types.
		// Make a new list entry.
		SetRenderingTypes(hDlg);

		// 4th: Find all useable VideoModes.
		FindResolutions(hDlg);

		// 5th: BufferCount.
		SetBufferCount(hDlg);

		// 6th: MultiSampleType.
		FindMultiSampleTypes(hDlg);

		// 7th: MultiSampleQuality.
		FindMultiSampleQual(hDlg);

		// 8th: Set the InfoBox to a default value.
		HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
		SetWindowTextA(hInfoBox, "Select one item to get detailed informations.");


		// Now load settings from the file if there is one. needs tests!
		//LoadOptions(hDlg);



		return true;
		break;
	}
	case WM_COMMAND:	// One of the Buttons has been pressed.
	{
		switch (LOWORD(wParam))	// Which Button?
		{
		case IDB_OK:	// OK Button was pressed.
		{
			// Save the Current Settings to the Settings.
			SaveCurrentSelections(hDlg);

			g_startWindow = true;

			DestroyWindow(hDlg);
			// Every Setting is set!

			break;
		}
		case IDB_SAVE:	// Save Button was pressed.
		{
			// Save the Current Settings to the Settings.
			SaveCurrentSelections(hDlg);

			//SaveOptions();


			// Save it!
			break;
		}
		case IDB_CANCEL:	// Cancel Button was pressed.
		{
			DestroyWindow(hDlg);
			break;
		}
		case IDC_COMBO_RENDERTYPE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// Recalculate Stuff
				FindMultiSampleTypes(hDlg);
				FindMultiSampleQual(hDlg);
			}
			if (HIWORD(wParam) == CBN_SETFOCUS)
			{
				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "Use Hardware Acceleration or Software Emulation. Hardware Acceleration is much faster. Only use Software Emulation if you have compatibility problems or no available Graphiccard.");
			}
			break;
		}
		case IDC_RADIO_WINDOWED:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// Recalculate the Resolutions:
				FindResolutions(hDlg);

				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "Start the Engine in Window- or in Fullscreenmode.");
			}
			break;
		}
		case IDC_RADIO_FULLSCREEN:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// Recalculate the Resolutions:
				FindResolutions(hDlg);

				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "Start the Engine in Window- or in Fullscreenmode.");
			}
			break;
		}
		case IDC_COMBO_RESOLUTION:	// Display resolution has changed
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// Recalculate Stuff
				FindMultiSampleTypes(hDlg);
				FindMultiSampleQual(hDlg);
			}
			if (HIWORD(wParam) == CBN_SETFOCUS)
			{
				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "Choose the resolution which will be used by your monitor, from a list of resolutions supported by the hardware. (ResolutionWidth x ResolutionHeight (Format, Monitorrefreshrate)).");
			}
			break;
		}
		case IDC_COMBO_BUFFERCOUNT:
		{
			if (HIWORD(wParam) == CBN_SETFOCUS)
			{
				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "Normally you want to use two Buffers (Doublebuffering). Three Buffers (Triplebuffering) will slightly decrease your overall performance, but it will decrease the amount of Laggspikes, if you suffer such problems.");
			}
			break;
		}
		case IDC_CHECK_VSYNC:
		{
			if (HIWORD(wParam) == BN_CLICKED)
			{
				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "VSync (Vertical Synchronization) will force the Engine to synchronize with your Monitorrefreshrate. This way the FPS (Frames per Second) are clamped to your Monitorrefreshrate. This will limit Visual Artifacts, especially screen-tearing.");
			}
			break;
		}
		case IDC_COMBO_MSAMPTYPE:
		{
			if (HIWORD(wParam) == CBN_SELCHANGE)
			{
				// Recalculate Stuff
				FindMultiSampleQual(hDlg);
			}
			if (HIWORD(wParam) == CBN_SETFOCUS)
			{
				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "Multisampling is a Technique that will smudge edges of 3DObjects and will therefore eliminate Aliasing Effects. This will also decrease your performance. The higher the Sample Level ist, the better will the image look like. Only if you use 'Non-Maskeable' the MultiSampleQuality Settings is used.");
			}
			break;
		}
		case IDC_COMBO_MSAMPQUAL:
		{
			if (HIWORD(wParam) == CBN_SETFOCUS)
			{
				// Change the InfoBox
				HWND hInfoBox = GetDlgItem(hDlg, IDC_EDIT_INFO);
				SetWindowTextA(hInfoBox, "The Quality of the MultiSampling. This Option is only used when you choose a Non-Maskable MultiSampleType.");
			}
			break;
		}
		}

		return true;
		break;
	}
	case WM_DESTROY:	// Somewhere a "DestroyWindow()" functions was called.
	{
		// Clean up:
		if (g_p_direct3D)
		{
			g_p_direct3D->Release();
			g_p_direct3D = nullptr;
		}

		g_displayModes.clear();
		g_multisampleTypes.clear();

		PostQuitMessage(0);
		return true;
		break;
	}
	case WM_CLOSE:	// Only Accurs when you press the 'X' Icon
	{
		DestroyWindow(hDlg);
		return true;
	}
	}

	return false;
}

// Windows:
/* Windows message procedure.*/
LRESULT WINAPI MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	g_deviceChange = false;

	switch (msg)
	{
	case WM_CLOSE:	// A quit Message occurred or the user wanted to quit.
	{
		DestroyWindow(hWnd);
		break;
	}
	case WM_DESTROY:	// User wants to exit the program.
	{
		PostQuitMessage(0);
		break;
	}

		// Keyboard messages.
	case WM_KEYDOWN:		// A key is pressed.
	{
		switch (wParam)
		{
		case VK_ESCAPE:		// When the user presses escape the program quits.
		{
			PostQuitMessage(0);
			break;
		}
		}
		break;
	}

	case WM_DEVICECHANGE:
	{

		if (wParam == DBT_DEVNODES_CHANGED)	// A New Device was entered
		{
			// New Device (check if Input devices are still available.
			g_deviceChange = true;
		}
	}

	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}

/* The main Windows function.*/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, INT showCmd)
{
	// 1st: Create Handle to the D3DSettings_Dialog.
	HWND settingHandle = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_SETTINGS), 0, SettingMsgProc);

	// 2nd: Show the Settings_Dialog.
	ShowWindow(settingHandle, showCmd);

	// 3rd: Enter Settings_Dialog Message loop
	MSG settingMsg;
	ZeroMemory(&settingMsg, sizeof(MSG));
	while (GetMessage(&settingMsg, 0, 0, 0))
	{
		if (settingHandle == 0 || !IsDialogMessage(settingHandle, &settingMsg))
		{
			TranslateMessage(&settingMsg);
			DispatchMessage(&settingMsg);
		}
	}

	// 4th: Start the actual Window.
	if (g_startWindow)	// Only if you pressed OK!
	{
		// 5th: Fillout a Windowclass.
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_CLASSDC;
		wc.lpfnWndProc = MsgProc;
		wc.cbClsExtra = NULL;
		wc.cbWndExtra = NULL;
		wc.hInstance = hInstance;
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		if (g_settings.windowed == true)
		{
			wc.hbrBackground = NULL;
		}
		else
		{
			wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
		}
		wc.lpszMenuName = NULL;
		wc.lpszClassName = L"Orchid 2D Framework";

		// 6th: Register the Windowclass.
		RegisterClassEx(&wc);

		// 7th: Create the Rect object to determine the size and position of the window.
		/* Handle to the main Window. */
		HWND hWnd;
		if (g_settings.windowed == true)	// Start it in Windowmode.
		{
			RECT rect;
			UINT primaryResolutionWidth = GetSystemMetrics(SM_CXSCREEN);
			UINT primaryResolutionHeight = GetSystemMetrics(SM_CYSCREEN);
			rect.left = ((primaryResolutionWidth / 2) - (g_settings.resolutionWidth / 2));
			rect.right = g_settings.resolutionWidth + rect.left;
			rect.top = ((primaryResolutionHeight / 2) - (g_settings.resolutionHeight / 2));
			rect.bottom = g_settings.resolutionHeight + rect.top;
			AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

			// Create the Window.
			hWnd = CreateWindowEx(NULL,
				L"Orchid 2D Framework",
				L"Orchid 2D Framework",
				WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME ^ WS_MAXIMIZEBOX,
				rect.left,
				rect.top,
				rect.right - rect.left,
				rect.bottom - rect.top,
				NULL,
				NULL,
				wc.hInstance,
				NULL);
			if (hWnd == 0)
			{
				MessageBoxA(0, "CreateWindow - Failed", 0, 0);
				return false;
			}
		}
		else // Start it in Fullscreenmode.
		{
			hWnd = CreateWindowEx(NULL,
				L"Orchid 2D Framework",
				L"Orchid 2D Framework",
				WS_EX_TOPMOST | WS_POPUP,
				0,
				0,
				g_settings.resolutionWidth,
				g_settings.resolutionHeight,
				NULL,
				NULL,
				wc.hInstance,
				NULL);
			if (hWnd == 0)
			{
				MessageBoxA(0, "CreateWindow - Failed", 0, 0);
				return false;
			}
		}



		// 8th: Show the Console
		// 8.1. Create the Console.
#ifdef _DEBUG	// Console is only for debugging stuff
		AllocConsole();


		// 8.2. Create the Output Handle of the Console.
		HANDLE outHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		int hCrt = _open_osfhandle((long)outHandle, _O_TEXT);
		FILE* hf_Out = _fdopen(hCrt, "w");
		setvbuf(hf_Out, NULL, _IONBF, 1);
		*stdout = *hf_Out;

		// 8.3. Create the Input Handle of the Console.
		HANDLE inHandle = GetStdHandle(STD_INPUT_HANDLE);
		hCrt = _open_osfhandle((long)inHandle, _O_TEXT);
		FILE* hf_in = _fdopen(hCrt, "r");
		setvbuf(hf_in, NULL, _IONBF, 128);
		*stdin = *hf_in;

		// 8.4. Set the right Position of the Console.
		HWND consoleWindow = GetConsoleWindow();
		SetWindowPos(consoleWindow, 0, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

#endif

		// 9th: Show and update the Window.
		ShowWindow(hWnd, SW_SHOWNORMAL);
		UpdateWindow(hWnd);

		// 10th: Don't show the Cursor.
		ShowCursor(false);

		// 11th: Create an instance of the Game
		Game game(g_settings, hWnd, hInstance);
		if (!game.Initialize())
		{
			PostQuitMessage(0);
		}

		// 12th: Now go into the message loop.
		MSG Windowmsg;
		ZeroMemory(&Windowmsg, sizeof(MSG));
		while (Windowmsg.message != WM_QUIT)	// When a quit message occurs the programm shall quit.
		{
			if (PeekMessage(&Windowmsg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&Windowmsg);
				DispatchMessage(&Windowmsg);		// Dispatch Message to the message procedure.
			}
			else
			{
				int a = 10;
				if (g_deviceChange)	// A Device has changed, so re initialize the Input System
				{
					game.system.input.GetNewInputDevices();
				}
				if (!game.FrameCycle()) // If a Fatalerror occurred, FrameCycle returns false, so quit the Framework.
				{
					PostQuitMessage(0);
				}

			}
		}

		game.Shutdown();

		// 13th: Make the Window Disapper
		ShowWindow(hWnd, HIDE_WINDOW);

		// 14th: Unregister the Windows class and end the application.
		UnregisterClass(L"Orchid 2D Framework", wc.hInstance);

		// 15th: Don't let the Console close instaniously:
#ifdef _DEBUG
		system("PAUSE");
#endif
	}




	return 0;
}

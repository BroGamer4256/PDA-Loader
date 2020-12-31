#include <windows.h>
#include <dbt.h>
#include <vector>
#include "Constants.h"
#include "MainModule.h"
#include "Input/Mouse/Mouse.h"
#include "Input/Keyboard/Keyboard.h"
#include "Input/DirectInput/DirectInput.h"
#include "Input/Xinput/Xinput.h"
#include "Input/DirectInput/Ds4/DualShock4.h"
#include "Components/EmulatorComponent.h"
#include "Components/Input/InputEmulator.h"
#include "Components/Input/TouchPanelEmulator.h"
#include "Components/SysTimer.h"
#include "Components/PlayerDataManager.h"
#include "Components/FrameRateManager.h"
#include "Components/CameraController.h"
#include "Components/FastLoader.h"
#include "Components/DebugComponent.h"
#include "Components/ScaleComponent.h"
#include "Utilities/Stopwatch.h"
#include "FileSystem/ConfigFile.h"

using namespace MLAC::Components;
using namespace MLAC::Utilities;
using namespace MLAC::FileSystem;

LRESULT CALLBACK MessageWindowProcessCallback(HWND, UINT, WPARAM, LPARAM);
DWORD WINAPI WindowMessageDispatcher(LPVOID);
VOID RegisterMessageWindowClass();

struct
{
	DWORD ID = NULL;
	HANDLE Handle = NULL;
} MessageThread;

const wchar_t *MessageWindowClassName = TEXT("MessageWindowClass");
const wchar_t *MessageWindowName = TEXT("MessageWindowTitle");

namespace MLAC
{
	const std::string COMPONENTS_CONFIG_FILE_NAME = "components.ini";

	const DWORD JMP_HOOK_SIZE = 0x5;

	const DWORD HookByteLength = 0x5;
	const void* UpdateReturnAddress = (void*)(ENGINE_UPDATE_HOOK_TARGET_ADDRESS + HookByteLength);
	//const void* TouchReactionReturnAddress = (void*)(TOUCH_REACT_HOOK_TARGET_ADDRESS + HookByteLength);

	const float DefaultResolutionWidth = 1280.0f;
	const float DefaultResolutionHeight = 720.0f;

	void* OriginalTouchReactAetFunc = (void*)0x408510;

	//int* ResolutionWidthPtr = (int*)RESOLUTION_WIDTH_ADDRESS;
	//int* ResolutionHeightPtr = (int*)RESOLUTION_HEIGHT_ADDRESS;

	float RenderWidth, RenderHeight;

	float ElpasedTime;
	bool HasWindowFocus, HadWindowFocus;
	bool FirstUpdateTick = true;
	bool DeviceConnected = true;

	Stopwatch UpdateStopwatch;
	std::vector<EmulatorComponent*> EmulatorComponents;

	void InstallHook(BYTE *address, DWORD overrideAddress, DWORD length)
	{
		DWORD oldProtect, dwBkup, dwRelAddr;

		VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &oldProtect);

		// calculate the distance between our address and our target location
		// and subtract the 5bytes, which is the size of the jmp
		// (0xE9 0xAA 0xBB 0xCC 0xDD) = 5 bytes
		dwRelAddr = (DWORD)(overrideAddress - (DWORD)address) - JMP_HOOK_SIZE;

		*address = JMP_OPCODE;

		// overwrite the next 4 bytes (which is the size of a DWORD)
		// with the dwRelAddr
		*((DWORD *)(address + 1)) = dwRelAddr;

		// overwrite the remaining bytes with the NOP opcode (0x90)
		for (DWORD x = JMP_HOOK_SIZE; x < length; x++)
			*(address + x) = NOP_OPCODE;

		VirtualProtect(address, length, oldProtect, &dwBkup);
	}

	void ParseAddComponents()
	{
		EmulatorComponent *allComponents[]
		{
			new InputEmulator(),
			new TouchPanelEmulator(),
			new SysTimer(),
			new PlayerDataManager(),
			//new FrameRateManager(),
			new CameraController(),
			new FastLoader(),
			new DebugComponent(),
			new ScaleComponent(),
		};

		ConfigFile componentsConfig(MainModule::GetModuleDirectory(), COMPONENTS_CONFIG_FILE_NAME);
		bool success = componentsConfig.OpenRead();

		if (!success)
		{
			printf("ParseAddComponents(): Unable to parse %s\n", COMPONENTS_CONFIG_FILE_NAME.c_str());
			return;
		}

		int componentCount = sizeof(allComponents) / sizeof(EmulatorComponent*);
		EmulatorComponents.reserve(componentCount);

		std::string trueString = "true";
		std::string falseString = "false";

		for (int i = 0; i < componentCount; i++)
		{
			std::string *value;

			auto name = allComponents[i]->GetDisplayName();
			//printf("ParseAddComponents(): searching name: %s\n", name);

			if (componentsConfig.TryGetValue(name, value))
			{
				//printf("ParseAddComponents(): %s found\n", name);

				if (*value == trueString)
				{
					//printf("ParseAddComponents(): enabling %s...\n", name);
					EmulatorComponents.push_back(allComponents[i]);
				}
				else if (*value == falseString)
				{
					//printf("ParseAddComponents(): disabling %s...\n", name);
				}
				else
				{
					//printf("ParseAddComponents(): invalid value %s for component %s\n", value, name);
				}

				delete value;
			}
			else
			{
				//printf("ParseAddComponents(): component %s not found\n", name);
				delete allComponents[i];
			}
		}
	}

	void InitializeTick()
	{
		RegisterMessageWindowClass();
		if ((MessageThread.Handle = CreateThread(0, 0, WindowMessageDispatcher, 0, 0, 0)) == NULL)
			printf("[MLAC] InitializeTick(): CreateThread() Error: %d\n", GetLastError());

		MainModule::DivaWindowHandle = FindWindow(0, MainModule::DivaWindowName);
		if (MainModule::DivaWindowHandle == NULL)
			MainModule::DivaWindowHandle = FindWindow(0, MainModule::GlutDefaultName);

		HRESULT diInitResult = Input::InitializeDirectInput(MainModule::Module);
		if (FAILED(diInitResult))
			printf("InitializeTick(): Failed to initialize DirectInput. Error: 0x%08X\n", diInitResult);

		ParseAddComponents();

		UpdateStopwatch.Start();
	}

	void UpdateTick()
	{
		if (FirstUpdateTick)
		{
			FirstUpdateTick = false;
			InitializeTick();

			for (auto& component : EmulatorComponents)
				component->Initialize();
		}

		if (DeviceConnected)
		{
			DeviceConnected = false;

			if (!Input::DualShock4::InstanceInitialized())
			{
				if (Input::DualShock4::TryInitializeInstance())
					printf("[MLAC] UpdateTick(): DualShock4 connected and initialized\n");
			}
		}

		ElpasedTime = UpdateStopwatch.Restart();

		for (auto& component : EmulatorComponents)
		{
			component->SetElapsedTime(ElpasedTime);
			component->Update();
		}

		HadWindowFocus = HasWindowFocus;
		HasWindowFocus = MainModule::DivaWindowHandle == NULL || GetForegroundWindow() == MainModule::DivaWindowHandle;

		if (HasWindowFocus)
		{
			Input::Keyboard::GetInstance()->PollInput();
			Input::Mouse::GetInstance()->PollInput();
			Input::Xinput::GetInstance()->PollInput();

			if (Input::DualShock4::GetInstance() != nullptr)
			{
				if (!Input::DualShock4::GetInstance()->PollInput())
				{
					Input::DualShock4::DeleteInstance();
					printf("[MLAC] UpdateTick(): DualShock4 connection lost\n");
				}
			}

			for (auto& component : EmulatorComponents)
				component->UpdateInput();
		}

		if (HasWindowFocus && !HadWindowFocus)
		{
			for (auto& component : EmulatorComponents)
				component->OnFocusGain();
		}

		if (!HasWindowFocus && HadWindowFocus)
		{
			for (auto& component : EmulatorComponents)
				component->OnFocusLost();
		}
	}

	void _declspec(naked) UpdateFunctionHook()
	{
		_asm
		{
			call UpdateTick
			jmp[UpdateReturnAddress]
		}
	}

	//void _declspec(naked) TouchReactionAetFunctionHook()
	//{
	//	_asm
	//	{
	//		// this is the function we replaced with the hook jump
	//		call OriginalTouchReactAetFunc

	//		// move X/Y into registers
	//		movss xmm1, [esp + 0x154 + 0x4]
	//		movss xmm2, [esp + 0x154 + 0x8]

	//		// calculate X scale factor
	//		movss xmm3, DefaultResolutionWidth
	//		divss xmm3, RenderWidth

	//		// calculate Y scale factor
	//		movss xmm4, DefaultResolutionHeight
	//		divss xmm4, RenderHeight

	//		// multiply positions by scale factors
	//		mulss xmm1, xmm3
	//		mulss xmm2, xmm4

	//		// move X/Y back into stack variables
	//		movss[esp + 0x154 + 0x4], xmm1
	//		movss[esp + 0x154 + 0x8], xmm2

	//		// return to original function
	//		jmp[TouchReactionReturnAddress]
	//	}
	//}

	void InitializeHooks()
	{
		//RenderWidth = (float)*ResolutionWidthPtr;
		//RenderHeight = (float)*ResolutionHeightPtr;

		InstallHook((BYTE*)ENGINE_UPDATE_HOOK_TARGET_ADDRESS, (DWORD)UpdateFunctionHook, HookByteLength);

		//if (RenderWidth != DefaultResolutionWidth || RenderHeight != DefaultResolutionHeight)
		//	InstallHook((BYTE*)TOUCH_REACT_HOOK_TARGET_ADDRESS, (DWORD)TouchReactionAetFunctionHook, HookByteLength);
	}

	void Dispose()
	{
		for (auto& component : EmulatorComponents)
			delete component;

		delete Input::Keyboard::GetInstance();
		delete Input::Mouse::GetInstance();
		delete Input::DualShock4::GetInstance();

		Input::DisposeDirectInput();

		PostThreadMessage(MessageThread.ID, WM_QUIT, 0, 0);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		if (*(char*)0x005DB6C4 == (char)0x65 || *(char*)0x005DB6C4 == (char)0x00)
		{
		}
		else
		{
			printf("[MLAC] Certain bytes do not match PDA1.01 Exiting... \n");
			break;
		}
		printf("[MLAC] Installing hooks...\n");

		MLAC::InitializeHooks();
		MLAC::MainModule::Module = hModule;

		break;

	case DLL_PROCESS_DETACH:
		MLAC::Dispose();
		break;
	}

	return TRUE;
}

DWORD WINAPI WindowMessageDispatcher(LPVOID lpParam)
{
	HWND windowHandle = CreateWindowW(
		MessageWindowClassName,
		MessageWindowName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL,
		MLAC::MainModule::Module,
		NULL);

	if (!windowHandle)
	{
		printf("WindowMessageDispatcher(): CreateWindowW() Error: %d\n", GetLastError());
		return 1;
	}

	MessageThread.ID = GetCurrentThreadId();

	MSG message;
	DWORD returnValue;

	printf("WindowMessageDispatcher(): Entering message loop...\n");

	while (true)
	{
		returnValue = GetMessage(&message, NULL, 0, 0);
		if (returnValue != -1)
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
		else
		{
			printf("WindowMessageDispatcher(): GetMessage() Error: %d\n", returnValue);
		}
	}

	DestroyWindow(windowHandle);
	return 0;
}

BOOL RegisterDeviceInterface(HWND hWnd, HDEVNOTIFY *hDeviceNotify)
{
	DEV_BROADCAST_DEVICEINTERFACE NotificationFilter = {};

	NotificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
	NotificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

	*hDeviceNotify = RegisterDeviceNotification(hWnd, &NotificationFilter, DEVICE_NOTIFY_ALL_INTERFACE_CLASSES);

	return *hDeviceNotify != NULL;
}

LRESULT CALLBACK MessageWindowProcessCallback(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
	{
		HDEVNOTIFY hDevNotify = NULL;

		if (!RegisterDeviceInterface(hWnd, &hDevNotify))
			printf("[MLAC] MessageWindowProcessCallback(): RegisterDeviceInterface() Error: %d\n", GetLastError());

		break;
	}

	case WM_DEVICECHANGE:
	{
		switch (wParam)
		{
		case DBT_DEVICEARRIVAL:
			MLAC::DeviceConnected = true;
			break;

		default:
			break;
		}
	}

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

VOID RegisterMessageWindowClass()
{
	WNDCLASS windowClass = { };

	windowClass.lpfnWndProc = MessageWindowProcessCallback;
	windowClass.hInstance = MLAC::MainModule::Module;
	windowClass.lpszClassName = MessageWindowClassName;

	RegisterClass(&windowClass);
}
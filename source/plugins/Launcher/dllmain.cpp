#include "framework.h"
#include <detours.h>
#include <simpleini.h>
#pragma comment(lib, "detours.lib")

CSimpleIniA switchini;
const char* config_path = ".\\switches.ini";

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		bool failure = switchini.LoadFile(config_path);
		if (failure)
		{
			MessageBoxW(NULL, L"[Launcher] Loading switches.ini has failed\n", L"Launcher", NULL);
			return TRUE;
		}

		if (switchini.GetBoolValue("switches", "load") == TRUE)
		{
			switchini.SetBoolValue("switches", "load", FALSE, "", true);
			failure = switchini.SaveFile(config_path);
			if (failure)
			{
				MessageBoxW(NULL, L"[Launcher] saving switches.ini has failed\n", L"Launcher", NULL);
				return TRUE;
			}
			return TRUE;
		}
		else
		{
			switchini.SetBoolValue("switches", "load", TRUE, "", true);
			failure = switchini.SaveFile(config_path);
			if (failure)
			{
				MessageBoxW(NULL, L"[Launcher] saving switches.ini has failed\n", L"Launcher", NULL);
				return TRUE;
			}
		}

		if (switchini.GetBoolValue("switches", "windowed") == TRUE) DIVA_EXECUTABLE_LAUNCH_STRING += L"-w ";
		if (switchini.GetBoolValue("switches", "test") == TRUE) DIVA_EXECUTABLE_LAUNCH_STRING += L"-t ";
		if (switchini.GetBoolValue("switches", "1080p") == TRUE) DIVA_EXECUTABLE_LAUNCH_STRING += L"-hdtv1080 ";
		if (switchini.GetBoolValue("switches", "aa") == TRUE) DIVA_EXECUTABLE_LAUNCH_STRING += L"-aa";

		STARTUPINFOW si;
		PROCESS_INFORMATION pi;

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		ZeroMemory(&pi, sizeof(pi));

		LPWSTR DIVA_EXECUTABLE_LAUNCH = const_cast<WCHAR*>(DIVA_EXECUTABLE_LAUNCH_STRING.c_str());
		CreateProcessW(DIVA_EXECUTABLE, DIVA_EXECUTABLE_LAUNCH, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

		TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
	}
	return TRUE;
}
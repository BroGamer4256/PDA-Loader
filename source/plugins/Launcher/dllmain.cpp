#include "framework.h"
#include <detours.h>
#include <iostream>
#pragma comment(lib, "detours.lib")

void returnExe(std::wstring argv);
void returnExe(char* argv);

int hookedMain101(int argc, const char** argv, const char** envp)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--launch")
			return divaMain101(argc, argv, envp);
	}

	returnExe((char*)argv);
}

int hookedMain600(int argc, const char** argv, const char** envp)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--launch")
			return divaMain600(argc, argv, envp);
	}

	returnExe((char*)argv);
}

void returnExe(char* argv)
{
	std::string str(argv);
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &argv[0], (int)str.size(), &wstrTo[0], size_needed);
	returnExe(wstrTo);
}

void returnExe(std::wstring argv)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::wstring DIVA_EXECUTABLE_LAUNCH_STRING_INCLUSIVE = DIVA_EXECUTABLE_LAUNCH_STRING + argv;
	LPWSTR DIVA_EXECUTABLE_LAUNCH = const_cast<WCHAR*>(DIVA_EXECUTABLE_LAUNCH_STRING_INCLUSIVE.c_str());

	CreateProcessW(DIVA_EXECUTABLE, DIVA_EXECUTABLE_LAUNCH, NULL, NULL, false, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	TerminateProcess(GetCurrentProcess(), EXIT_SUCCESS);
}

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	// This is a bad way of doing it, find a better way
	if (ul_reason_for_call == DLL_PROCESS_ATTACH && ( (*(char*)0x005DB6C4 == (char)0x65) || (*(char*)0x005DB6C4 == (char)0x00) ))
	{
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)divaMain101, hookedMain101);
		DetourTransactionCommit();
	}
	else if (ul_reason_for_call == DLL_PROCESS_ATTACH && (*(char*)0x004592CC == (char)0x74 || *(char*)0x004592CC == (char)0x90))
	{
		DisableThreadLibraryCalls(hModule);
			DetourTransactionBegin();
			DetourUpdateThread(GetCurrentThread());
			DetourAttach(&(PVOID&)divaMain600, hookedMain600);
			DetourTransactionCommit();
	}
	return TRUE;
}
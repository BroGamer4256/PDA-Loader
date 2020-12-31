#include "framework.h"
#include <detours.h>
#include <iostream>
#pragma comment(lib, "detours.lib")

void returnExe(std::wstring argv);
void returnExe(int argc, const char** argv);

std::string diva = "diva";
std::string spaceS = " ";

int hookedMain101(int argc, const char** argv, const char** envp)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--launch")
			return divaMain101(argc, argv, envp);
	}

	returnExe(argc, argv);
}

int hookedMain301(int argc, const char** argv, const char** envp)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--launch")
			return divaMain301(argc, argv, envp);
	}

	returnExe(argc, argv);
}

int hookedMain600(int argc, const char** argv, const char** envp)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--launch")
			return divaMain600(argc, argv, envp);
	}

	returnExe(argc, argv);
}

void returnExe(int argc, const char** argv)
{
	std::wstring args;

	//space to wstring
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &spaceS[0], (int)spaceS.size(), NULL, 0);
	std::wstring spaceW(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &spaceS[0], (int)spaceS.size(), &spaceW[0], size_needed);

	for (int i = 0; i < argc; ++i)
	{
		//remove the exe from args
		std::string str((char*)argv[i]);
		if (str.compare(0, diva.length(), diva) == 0) continue;

		int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
		std::wstring wstrTo(size_needed, 0);
		MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
		args = args + spaceW + wstrTo;
	}

	returnExe(args);
}

void returnExe(std::wstring argv)
{
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	std::wstring DIVA_EXECUTABLE_LAUNCH_STRING_INCLUSIVE = DIVA_EXECUTABLE_LAUNCH_STRING + argv;
	std::wcout << DIVA_EXECUTABLE_LAUNCH_STRING_INCLUSIVE << std::endl;
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
	else if (ul_reason_for_call == DLL_PROCESS_ATTACH && *(char*)0x007B1210 == (char)0x83)
	{
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)divaMain301, hookedMain301);
		DetourTransactionCommit();
	}
	return TRUE;
}
#include "framework.h"
#include <detours.h>
#pragma comment(lib, "detours.lib")

int hookedMain(int argc, const char** argv, const char** envp)
{
	for (int i = 0; i < argc; ++i)
	{
		std::string arg = argv[i];
		if (arg == "--launch")
			return divaMain(argc, argv, envp);
	}

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

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
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)divaMain, hookedMain);
		DetourTransactionCommit();
	}
	return TRUE;
}
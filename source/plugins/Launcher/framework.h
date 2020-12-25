#pragma once
#include <windows.h>
#include <string>

int(__cdecl* divaMain)(int argc, const char** argv, const char** envp) = (int(__cdecl*)(int argc, const char** argv, const char** envp))0x00000; // This is why it dosent work

std::wstring ExePath() {
	WCHAR buffer[MAX_PATH];
	GetModuleFileNameW(NULL, buffer, MAX_PATH);
	return std::wstring(buffer);
}

std::wstring DIVA_EXECUTABLE_STRING = ExePath();
LPCWSTR DIVA_EXECUTABLE = DIVA_EXECUTABLE_STRING.c_str();

std::wstring DIVA_EXECUTABLE_LAUNCH_STRING = DIVA_EXECUTABLE_STRING + L" --launch -w -aa";
LPWSTR DIVA_EXECUTABLE_LAUNCH = const_cast<WCHAR*>(DIVA_EXECUTABLE_LAUNCH_STRING.c_str());
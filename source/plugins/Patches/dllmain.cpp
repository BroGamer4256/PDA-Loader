#include "windows.h"
#include "vector"
#include <tchar.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <sstream>
#include <iomanip>
#include "PluginConfigApi.h"
#include <simpleini.h>

#include <detours.h>
#include "framework.h"
#pragma comment(lib, "detours.lib")

unsigned short game_version = 101;

void InjectCode(void* address, const std::vector<uint8_t> data);
void ApplyPatches();

const LPCWSTR CONFIG_FILE = L".\\config.ini";
const LPCWSTR PATCHES_FILE = L".\\patches.ini";

HMODULE *hModulePtr;

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		hModulePtr = &hModule;
		if (*(char*)0x007B1210 == (char)0x83) game_version = 301;
		else if (*(char*)0x004592CC == (char)0x74) game_version = 600;
		ApplyPatches();
	}
	return TRUE;
}

void ApplyPatches() {
	std::string version_string = std::to_string(game_version);
	version_string.insert(version_string.begin() + 1, '.');
	std::cout << "[Patches] Game version " + version_string << std::endl;

	bool create = false;
	if (!std::filesystem::exists(PATCHES_FILE))
	{
		create = true;
		std::ofstream outfile(PATCHES_FILE);
		std::ifstream in(".\\patches.dva", std::ifstream::ate | std::ifstream::binary);
		outfile << "# ONLY FOR ADVANCED USERS" << std::endl << "[patches]" << std::endl;
		outfile.close();
	}

	CSimpleIniA ini;
	ini.LoadFile(PATCHES_FILE);

	if (create)
	{
		switch (game_version)
		{
		case 101:
			for (size_t i = 0; i < _countof(patches_101); i++)
				ini.SetBoolValue("patches", patches_101[i].Name, true);
			break;
		case 301:
			for (size_t i = 0; i < _countof(patches_301); i++)
				ini.SetBoolValue("patches", patches_301[i].Name, true);
			break;
		case 600:
			for (size_t i = 0; i < _countof(patches_600); i++)
				ini.SetBoolValue("patches", patches_600[i].Name, true);
			break;
		}
	}

	switch (game_version)
	{
	case 101:
		for (size_t i = 0; i < _countof(patches_101); i++)
		{
			if (!create && ini.GetValue("patches", patches_101[i].Name)[2] == "false"[2]) continue;
			InjectCode(patches_101[i].Address, patches_101[i].Data);
		}
		break;
	case 301:
		for (size_t i = 0; i < _countof(patches_301); i++)
		{
			if (!create && ini.GetValue("patches", patches_301[i].Name)[2] == "false"[2]) continue;
			InjectCode(patches_301[i].Address, patches_301[i].Data);
		}
		break;
	case 600:
		for (size_t i = 0; i < _countof(patches_600); i++)
		{
			if (!create && ini.GetValue("patches", patches_600[i].Name)[2] == "false"[2]) continue;
			InjectCode(patches_600[i].Address, patches_600[i].Data);
		}
		break;
	}
	if (ini.SaveFile(PATCHES_FILE)) MessageBoxA(NULL, "Saving Patches.ini failed", "Patches", NULL);

	std::cout << "[Patches] Patches loaded" << std::endl;
}

void InjectCode(void* address, const std::vector<uint8_t> data)
{
	const size_t byteCount = data.size() * sizeof(uint8_t);

	DWORD oldProtect;
	VirtualProtect(address, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy(address, data.data(), byteCount);
	VirtualProtect(address, byteCount, oldProtect, nullptr);
}
using namespace PluginConfig;

// Note for developers
// These do jack until theres an actual UI

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void)
{
	return L"Patches";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void)
{
	return L"Applies patches/tweaks to the game before it starts.\nThis plugin is required.";
}

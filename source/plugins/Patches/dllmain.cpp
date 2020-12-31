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

#include "101.h"
#include "301.h"
#include "600.h"

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

	const Patch* patch_ptr = NULL;

	switch (game_version)
	{
	case 101:
		patch_ptr = patches_101;
		break;
	case 301:
		patch_ptr = patches_301;
		break;
	case 600:
		patch_ptr = patches_600;
		break;
	}

	if (patch_ptr == NULL) abort();

	while (patch_ptr->Address != 0x0)
	{
		if (create) ini.SetBoolValue("patches", patch_ptr->Name, true);
		else if (!create && !ini.GetBoolValue("patches", patch_ptr->Name)) continue;
		InjectCode(patch_ptr->Address, patch_ptr->Data);
		patch_ptr += 1;
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

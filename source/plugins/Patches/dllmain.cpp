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
		if (*(char*)0x004592CC == (char)0x74) game_version = 600;
		ApplyPatches();
	}
	return TRUE;
}

void ApplyPatches() {
	
	const struct { void* Address; std::vector<uint8_t> Data; const char* Name; } patches_101[] =
	{
		//Debug
		//Why the fuck did raki remove this shit
		{ (void*)0x00535CC0,{ 0xB0, 0x01 }, "Debug" },
		{ (void*)0x00535CD0,{ 0xB0, 0x01 }, "Debug" },
		{ (void*)0x005A3D80,{ 0xB0, 0x01, 0xC3 }, "Debug" },
		{ (void*)0x005DB542,{ 0x00 }, "Debug" },
		{ (void*)0x005DB6C4,{ 0x00 }, "Debug" },
		{ (void*)0x005DBCC8,{ 0x64, 0xA3 }, "Debug" },
		// Skip amMaster checks
		{ (void*)0x006FAA10,{ 0xB0, 0x01, 0xC3 }, "amMaster" },
		// Skip pcpaOpenClient loop
		{ (void*)0x008AFCE0,{ 0xC2, 0x14, 0x00 }, "pcpaOpenClient" },
		// Disable the keychip time bomb
		{ (void*)0x0049C8B0,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "KeychipBomb" },
		// *But of course we have a valid keychip*, return true
		{ (void*)0x006FC420,{ 0xB0, 0x01, 0xC3 }, "KeychipTrue" },
		// Just completely ignore all SYSTEM_STARTUP errors
		{ (void*)0x005DEC50,{ 0xC3 }, "SYSTEM_STARTUP_ERROR" },
		// Skip parts of the network check state
		{ (void*)0x00705949,{ 0xE9, 0x13, 0x01, 0x00, 0x00 }, "NetworkCheck" },
		// Ignore SYSTEM_STARTUP Location Server checks
		{ (void*)0x007052A1,{ 0x90, 0x90 }, "LocServer" },
		// Set the initial wait timer value to 0
		{ (void*)0x00706D2E,{ 0x00, 0x00 }, "NoWait" },
		{ (void*)0x00706D50,{ 0x00, 0x00 }, "NoWait" },
		{ (void*)0x00706D67,{ 0x00, 0x00 }, "NoWait" },
		// Always exit TASK_MODE_APP_ERROR on the first frame
		{ (void*)0x005DFD0B,{ 0x90, 0x90 }, "TASK_MODE_APP_ERROR" },
		{ (void*)0x005DFD18,{ 0x90, 0x90 }, "TASK_MODE_APP_ERROR" },
		// Jump past the PollInput function so we can write our own input
		{ (void*)0x006FA640,{ 0xC3 }, "PollInputSkip" },
		// Always return true for the SelCredit enter SelPv check
		{ (void*)0x005A3D80,{ 0xB0, 0x01, 0xC3 }, "CreditSkip" },
		// Return early before resetting to the default PlayerData so we don't need to keep updating the PlayerData struct
		{ (void*)0x00642800,{ 0xC3 }, "EarlyPlayerData" },
		// Write ram files to the current directory instead of Y:/ram
		{ (void*)0x006ff101,{ 0xEB }, "ram" }
	};

	const struct { void* Address; std::vector<uint8_t> Data; const char* Name; } patches_600[] =
	{
		// Debug tests
		{ (void*)0x0059f150,{ 0xB0, 0x01 }, "Debug" },
		{ (void*)0x0059f160,{ 0xB0, 0x01 }, "Debug" },
		//{ (void*)0x,{ 0xB0, 0x01, 0xC3 } },
		// NOP out JVS board error JMP
		{ (void*)0x004592CC,{ 0x90, 0x90 }, "JVSBoard" },
		// NOP out sys_am sram/eprom initialization call
		{ (void*)0x0045AF5A,{ 0x90, 0x90, 0x90, 0x90, 0x90 }, "NopSRAM" },
		// Return early to prevent null reference for some sys_am object
		{ (void*)0x004BF730,{ 0xB8, 0x00, 0x00, 0x00, 0x00, 0xC3 }, "sys_amEarly" },
		// Skip the sys_am eprom time bomb checks
		{ (void*)0x00650A33,{ 0xEB, 0x5C, 0x90, 0x90, 0x90 }, "sys_amBomb" },
		// Jump past the Wrong Resolution Setting error
		{ (void*)0x006521F3,{ 0xEB }, "RightResoultion" },
		// Always skip the SYSTEM_STARTUP_ERROR game state
		{ (void*)0x00652394,{ 0xB0, 0x01 }, "SYSTEM_STARTUP_ERROR" },
		// *But of course we have a valid keychip*, return true
		{ (void*)0x007FCF40,{ 0xB0, 0x01, 0xC3 }, "KeychipTrue" },
		// Jump past the initial amMaster initialization
		{ (void*)0x0080384A,{ 0xE9, 0x96, 0x00, 0x00 }, "amMasterSkip" },
		// Move SYSTEM_OK into edx
		{ (void*)0x00808041,{ 0xBA, 0xFF, 0xFF }, "SYSTEM_OK" },
		// Move edx into the LocationServerStatus variable and jump past the router timeout initialization
		{ (void*)0x00808046,{ 0x89, 0x15, 0xB0, 0x6E, 0x08, 0x01, 0xEB, 0x14, 0x90, 0x90 }, "noRouterTimeour" },
		// Let the router checks time out so we can skip the error
		{ (void*)0x00808171,{ 0x90, 0x90 }, "noRouterTimeour" },
		// Then immediately move on to the DHCP 3 initialization phase
		{ (void*)0x008081FF,{ 0xE9, 0x43, 0x01, 0x00, 0x00, 0x90, 0x90 }, "noRouterTimeour" },
		{ (void*)0x008082EC,{ 0x5A }, "noRouterTimeour" },
		// Avoid JVS related null reference
		{ (void*)0x00830E45,{ 0x90, 0x90, 0x90, 0x90, 0x90 }, "NullJVS" },
		// Jump past the PollInput function so we can write our own input
		{ (void*)0x00455438,{ 0xEB }, "PollInput" },
		// Set the full screen glutGameModeString refresh rate to its default value
		{ (void*)0x00459CAA,{ 0x00 }, "glutGameRefresh" },
		// Use GLUT_CURSOR_RIGHT_ARROW instead of GLUT_CURSOR_NONE
		{ (void*)0x00459CD4,{ 0x00 }, "GLUT_CURSOS" },
		// Ignore CLOSE SETTINGS check
		{ (void*)0x004E4550,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "CLOSE_SETTINGS" },
		// Always return true for the SelCredit enter SelPv check
		{ (void*)0x0060C3E0,{ 0xB0, 0x01, 0xC3 }, "CreditSkip" },
		// Return early before resetting to the default PlayerData so we don't need to keep updating the PlayerData struct
		{ (void*)0x006DD720,{ 0xC3 }, "EarlyPlayerData" },
		// Ignore the EngineClear variable to clear the framebuffer at all resolutions
		{ (void*)0x0071708E,{ 0x90, 0x90 }, "CleanFramebuffer" },
		{ (void*)0x0071717D,{ 0xEB }, "CleanFramebuffer" },
		// Jump past the VP SelWatch button enabled checks
		{ (void*)0x00798108,{ 0xEB, 0x2D, 0x90, 0x90, 0x90 }, "FreePVWatch" },
		// Write ram files to the current directory instead of Y:/ram
		{ (void*)0x00800D91,{ 0xEB }, "ram" }
	};
	
	std::string version_string = std::to_string(game_version);
	version_string.insert(version_string.begin()+1, '.');
	std::cout << "[Patches] Game version " + version_string << std::endl;

	bool create = false;
	if (!std::filesystem::exists(PATCHES_FILE))
	{
		create = true;
		std::ofstream outfile(PATCHES_FILE);
		std::ifstream in(".\\patches.dva", std::ifstream::ate | std::ifstream::binary);
		outfile << "# ONLY FOR ADVANCED USERS" << std::endl << "# Size: " << in.tellg() << std::endl << "[patches]" << std::endl;
		outfile.close();
	}
	std::ifstream in(".\\patches.dva", std::ifstream::ate | std::ifstream::binary);
	std::ifstream infile(PATCHES_FILE);;
	char buffer[256];
	infile.getline(buffer, 256); // Read to line 2
	char size[256];
	infile.getline(size, 256);
	if (size != "# Size: " + in.tellg()) create = true;
	infile.close();

	CSimpleIniA ini;
	
	ini.LoadFile(PATCHES_FILE);

	switch (game_version)
	{
	case 600:
		for (size_t i = 0; i < _countof(patches_600); i++)
		{
			if (!create && ini.GetValue("patches", patches_600[i].Name)[2] == "false"[2]) continue;
			else ini.SetBoolValue("patches", patches_600[i].Name, true);
			InjectCode(patches_600[i].Address, patches_600[i].Data);
		}
		break;
	case 101:
		for (size_t i = 0; i < _countof(patches_101); i++)
		{
			if (!create && ini.GetValue("patches", patches_101[i].Name)[2] == "false"[2]) continue;
			else ini.SetBoolValue("patches", patches_101[i].Name, true);
			InjectCode(patches_101[i].Address, patches_101[i].Data);
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

extern "C" __declspec(dllexport) LPCWSTR GetPluginName(void)
{
	return L"Patches";
}

extern "C" __declspec(dllexport) LPCWSTR GetPluginDescription(void)
{
	return L"Applies patches/tweaks to the game before it starts.\nThis plugin is required.";
}
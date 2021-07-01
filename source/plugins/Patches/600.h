#pragma once
#include <vector>
#include "framework.h"

const Patch patches_600[] =
{
	// Enable dwgui
	{ (void*)0x0045AD12,{ 0x00 }, "dwgui" },
	{ (void*)0x0059F150,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x0059F160,{ 0xB0, 0x01}, "dwgui" },
	// Enable Debug Cursor RightClick
	{ (void*)0x0045ad12,{ 0x00 }, "DebugCursor" },
	// Show Cursor in game window
	{ (void*)0x00459cd4,{ 0x00 }, "ShowCursor" },
	// Disable TAA
	{ (void*)0x006E82D0,{ 0x29, 0xC0, 0x90, 0x90 }, "TAA" },
	// Disable MLAA
	// { (void*)0x006EE6F8,{ 0x83, 0xE0, 0x00 }, "MLAA" },
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
	{ (void*)0x00800D91,{ 0xEB }, "ram" },
	PATCHES_END
};
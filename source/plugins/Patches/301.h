#pragma once
#include <vector>
#include "framework.h"

// By Jay39w
const Patch patches_301[] =
{
	// Debug
	{ (void*)0x005643E0,{ 0xB0, 0x01 }, "dwgui" }, //done
	{ (void*)0x005643F0,{ 0xB0, 0x01 }, "dwgui" }, //done
	//{ (void*)0x0060EC48,{ 0x76, 0xEB, 0xFF, 0x00 } }, //broken
	{ (void*)0x0060E634,{ 0x00 }, "ShowCursor" }, //done cursor
	{ (void*)0x0060E4B2,{ 0x00 }, "DebugCursor" }, //done cursor
	// Enable Debug Cursor Moving
	{ (void*)0x0060EC47,{ 0xE8, 0xB4, 0x0B, 0xE3, 0xFF }, "MoveDebugCursor" },
	// NOP out JVS board error JMP //done
	{ (void*)0x0060E02C,{ 0x90, 0x90 }, "JVSBoard" },
	// Skip amMaster checks //done
	{ (void*)0x007B1210,{ 0xB0, 0x01, 0xC3 }, "amMaster" },
	// Skip pcpaOpenClient loop //done
	{ (void*)0x00980730,{ 0xC2, 0x14, 0x00 }, "pcpaOpenClient" },
	// *But of course we have a valid keychip*, return true //done
	{ (void*)0x007B0380,{ 0xB0, 0x01, 0xC3 }, "KeychipTrue" },
	// Just completely ignore all SYSTEM_STARTUP errors //done
	{ (void*)0x0061A3E0,{ 0xC3 }, "SYSTEM_STARTUP_ERROR" },
	// Move SYSTEM_OK into edx //done
	{ (void*)0x007BB0E1,{ 0xBA, 0xFF, 0xFF }, "noRouterTimeout" },
	// Move edx into the LocationServerStatus variable and jump past the router timeout initialization //done
	{ (void*)0x007BB0E6,{ 0x89, 0x15, 0xB0, 0x6E, 0x08, 0x01, 0xEB, 0x14, 0x90, 0x90 }, "noRouterTimeout" },
	// Let the router checks time out so we can skip the error //done
	{ (void*)0x007B9701,{ 0x90, 0x90 }, "noRouterTimeout" },
	// Then immediately move on to the DHCP 3 initialization phase //done
	{ (void*)0x007B978F,{ 0xE9, 0x43, 0x01, 0x00, 0x00, 0x90, 0x90 }, "noRouterTimeout" },
	{ (void*)0x007B987C,{ 0x5A }, "noRouterTimeout" },
	// Always skip the SYSTEM_STARTUP_ERROR game state //done
	{ (void*)0x0061B2B4,{ 0xB0, 0x01 }, "SYSTEM_STARTUP_ERROR" },
	// Avoid JVS related null reference //done
	//{ (void*)0x007E23A5,{ 0x90, 0x90, 0x90, 0x90, 0x90 } },
	// Set the full screen glutGameModeString refresh rate to its default value
	{ (void*)0x0060E60A,{ 0x00 }, "glutGameRefresh" },
	// Jump past the PollInput function so we can write our own input //done
	{ (void*)0x00757E69,{ 0xC3 }, "PollInput" },
	// Ignore CLOSE SETTINGS check //done
	{ (void*)0x004B0F80,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "CLOSE_SETTINGS" },
	// Always return true for the SelCredit enter SelPv check //done
	{ (void*)0x005D3020,{ 0xB0, 0x01, 0xC3 }, "CreditSkip" },
	// Return early before resetting to the default PlayerData so we don't need to keep updating the PlayerData struct //done
	{ (void*)0x0069F270,{ 0xC3 }, "EarlyPlayerData" },
	// Ignore the EngineClear variable to clear the framebuffer at all resolutions //done
	{ (void*)0x006D609E,{ 0x90, 0x90 }, "CleanFramebuffer" },
	{ (void*)0x006D618D,{ 0xEB }, "CleanFramebuffer" },
	// Jump past the VP SelWatch button enabled checks //done
	{ (void*)0x0074B818,{ 0xEB, 0x2D, 0x90, 0x90, 0x90 }, "FreePVWatch" },
	// Write ram files to the current directory instead of Y:/ram //done
	{ (void*)0x007B3E51,{ 0xEB }, "ram" },
	PATCHES_END
};
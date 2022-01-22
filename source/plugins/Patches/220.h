#pragma once
#include <vector>
#include "framework.h"

// By Keikei14
const Patch patches_220[] =
{
	// Enable dwgui
	{ (void*)0x0055de10,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x0055de20,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x005cf670,{ 0xB0, 0x01, 0xC3 }, "dwgui" },
	// Enable Debug Cursor RightClick
	{ (void*)0x0060b042,{ 0x00 }, "DebugCursor" },
	//// Enable Debug Cursor Moving
	{ (void*)0x0060b7d8,{ 0x64, 0xA3 }, "DebugCursor" },
	//// Show Cursor in game window
	{ (void*)0x0060b1c4,{ 0x00 }, "ShowCursor" },
	// Disable TAA
	{ (void*)0x006a3f60,{ 0x29, 0xC0, 0x90, 0x90 }, "TAA" },
	// NOP out JVS board error JMP //done
	{ (void*)0x0060abbc,{ 0x90, 0x90 }, "JVSBoard" },
	// Skip amMaster checks //done
	{ (void*)0x007a5220,{ 0xB0, 0x01, 0xC3 }, "amMaster" },
	// Skip pcpaOpenClient loop //done
	{ (void*)0x00960e50,{ 0xC2, 0x14, 0x00 }, "pcpaOpenClient" },
	// Disable the keychip time bomb
	{ (void*)0x004aa9e0,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "KeychipBomb" },
	// *But of course we have a valid keychip*, return true //done
	{ (void*)0x007a6a80,{ 0xB0, 0x01, 0xC3 }, "KeychipTrue" },
	// Just completely ignore all SYSTEM_STARTUP errors //done
	{ (void*)0x00616f80,{ 0xC3 }, "SYSTEM_STARTUP_ERROR" },
	// Always skip the SYSTEM_STARTUP_ERROR game state //done
	{ (void*)0x00617e44,{ 0xB0, 0x01 }, "SYSTEM_STARTUP_ERROR" },
	// Move SYSTEM_OK into edx //done
	{ (void*)0x007b1fa1,{ 0xBA, 0xFF, 0xFF }, "noRouterTimeout" },
	// Move edx into the LocationServerStatus variable and jump past the router timeout initialization //done
	{ (void*)0x007b1fa6,{ 0x89, 0x15, 0xB0, 0x6E, 0x08, 0x01, 0xEB, 0x14, 0x90, 0x90 }, "noRouterTimeout" },
	// Let the router checks time out so we can skip the error //done
	{ (void*)0x007b06e6,{ 0x90, 0x90 }, "noRouterTimeout" },
	// Ignore SYSTEM_STARTUP Location Server checks
	{ (void*)0x007aff39,{ 0x90, 0x90 }, "LocServer" },
	// Set the initial wait timer value to 0
	{ (void*)0x007b1fb6,{ 0x00, 0x00 }, "NoWait" },
	{ (void*)0x007b1fce,{ 0x00, 0x00 }, "NoWait" },
	{ (void*)0x007b1fe5,{ 0x00, 0x00 }, "NoWait" },
	// Always exit TASK_MODE_APP_ERROR on the first frame
	{ (void*)0x00617e2b,{ 0x90, 0x90 }, "TASK_MODE_APP_ERROR" },
	{ (void*)0x00617e38,{ 0x90, 0x90 }, "TASK_MODE_APP_ERROR" },
	// Set the full screen glutGameModeString refresh rate to its default value
	{ (void*)0x0060b19a,{ 0x00 }, "glutGameRefresh" },
	// Jump past the PollInput function so we can write our own input //done
	{ (void*)0x0074fcb9,{ 0xC3 }, "PollInput" },
	// Ignore CLOSE SETTINGS check //done
	{ (void*)0x004aa9e0,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "CLOSE_SETTINGS" },
	// Always return true for the SelCredit enter SelPv check //done
	{ (void*)0x005cf670,{ 0xB0, 0x01, 0xC3 }, "CreditSkip" },
	// Return early before resetting to the default PlayerData so we don't need to keep updating the PlayerData struct //done
	{ (void*)0x0069be10,{ 0xC3 }, "EarlyPlayerData" },
	// Ignore the EngineClear variable to clear the framebuffer at all resolutions //done
	{ (void*)0x006cfe6e,{ 0x90, 0x90 }, "CleanFramebuffer" },
	{ (void*)0x006cff5d,{ 0xEB }, "CleanFramebuffer" },
	// Jump past the VP SelWatch button enabled checks //done
	{ (void*)0x007435c8,{ 0xEB, 0x2D, 0x90, 0x90, 0x90 }, "FreePVWatch" },
	// Write ram files to the current directory instead of Y:/ram //done
	{ (void*)0x007aa0e1,{ 0xEB }, "ram" },
	PATCHES_END
};
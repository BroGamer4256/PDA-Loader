#pragma once
#include "framework.h"
#include <vector>

const Patch patches_130[] =
{
	//Enable dwgui
	{ (void*)0x0054A080,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x0054A090,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x005A3D80,{ 0xB0, 0x01, 0xC3 }, "dwgui" },

	//Enable debug cursor right click
	{ (void*)0x005f82f2,{ 0x00 }, "DebugCursor" },

	//Enable debug cursor moving
	{ (void*)0x005f8a88,{ 0x64, 0x95 }, "DebugCursor" },

	//Show cursor in game window. Broken, as of now. Causes the game to crash on the first frame.
	{ (void*)0x005F8474,{ 0x00 }, "ShowCursor" },

	//Skip amMaster checks
	{ (void*)0x00731600,{ 0xB0, 0x01, 0xC3 }, "amMaster" },

	//Skip pcpaOpenClient loop
	{ (void*)0x008E1A80,{ 0xC2, 0x14, 0x00 }, "pcpaOpenClient" },

	//Disable the keychip timebomb. Really it should be called "Close setting" but meh.
	{ (void*)0x004AAE40,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "KeychipBomb" },

	//* but of course we have a valid keychip * return true
	{ (void*)0x00733060,{ 0xB0, 0x01, 0xC3 }, "KeychipTrue" },

	//Completely ignore all system startup errors
	{ (void*)0x005FF950,{ 0xC3 }, "SYSTEM_STARTUP_ERROR" },

	//Skip parts of the network check state
	{ (void*)0x0073C5A9,{ 0xE9, 0x13, 0x01, 0x00, 0x00 }, "NetworkCheck" },

	//Ignore SYSTEM_STARTUP Location Server checks
	{ (void*)0x0073BF39,{ 0x90, 0x90 }, "LocServer" },

	//Set the initial wait timer value to 0
	{ (void*)0x0073DEBA,{ 0x00, 0x00 }, "NoWait" },
	{ (void*)0x0073DED2,{ 0x00, 0x00 }, "NoWait" },
	{ (void*)0x0073DEE9,{ 0x00, 0x00 }, "NoWait" },

	//Always exit TASK_MODE_APP_ERROR on the first frame
	{ (void*)0x0060096b,{ 0x90, 0x90 }, "TASK_MODE_APP_ERROR" },
	{ (void*)0x00600978,{ 0x90, 0x90 }, "TASK_MODE_APP_ERROR" },

	//Jump past the PollInput function so we can write our own input
	{ (void*)0x00731220,{ 0xC3 }, "PollInputSkip" },

	//Always return true for the SelCredit enter SelPv check
	{ (void*)0x005C0260,{ 0xB0, 0x01, 0xC3 }, "CreditSkip" },

	//Write ram files to the current directory instead of Y : / ram
	{ (void*)0x00735d91,{ 0xEB }, "ram" },

	//Return early before resetting to the default PlayerData so we don't need to keep updating the PlayerData struct
	{ (void*)0x00669E40,{ 0xC3 }, "EarlyPlayerData" },
	PATCHES_END
};
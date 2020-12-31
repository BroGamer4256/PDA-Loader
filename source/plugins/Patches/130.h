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
	//NG

	//Enable debug cursor moving
	//NG

	//Show cursor in game window
	//NG

	//Skip amMaster checks
	{ (void*)0x00731600,{ 0xB0, 0x01, 0xC3 }, "amMaster" },

	//Skip pcpaOpenClient loop
	{ (void*)0x008E1A80,{ 0xC2, 0x14, 0x00 }, "pcpaOpenClient" },

	//Disable the keychip timebomb.Really it should be called "Close setting" but eh
	{ (void*)0x004AAE40,{ 0xB8, 0x01, 0x00, 0x00, 0x00, 0xC3 }, "KeychipBomb" },

	//* but of course we have a valid keychip * return true
	{ (void*)0x00733060,{ 0xB0, 0x01, 0xC3 }, "KeychipTrue" },

	//Completely ignore all system startup errors
	{ (void*)0x005FF950,{ 0xC3 }, "SYSTEM_STARTUP_ERROR" },

	//Skip parts of the network check state
	//{ (void*),{ 0xE9, 0x13, 0x01, 0x00, 0x00 }, "NetworkCheck" },

	//Ignore SYSTEM_STARTUP Location Server checks
	//NG

	//Set the initial wait timer value to 0
	//NG
	//NG
	//NG

	//Always exit TASK_MODE_APP_ERROR on the first frame
	//NG
	//NG

	//Jump past the PollInput function so we can write our own input
	{ (void*)0x00731220,{ 0xC3 }, "PollInputSkip" },

	//Always return true for the SelCredit enter SelPv check
	{ (void*)0x005c0260,{ 0xB0, 0x01, 0xC3 }, "CreditSkip" },

	//Write ram files to the current directory instead of Y : / ram
	//NG, covered by twintail anyway

	//Return early before resetting to the default PlayerData so we don't need to keep updating the PlayerData struct
	{ (void*)0x00669E40,{ 0xC3 }, "EarlyPlayerData" },
	PATCHES_END
};
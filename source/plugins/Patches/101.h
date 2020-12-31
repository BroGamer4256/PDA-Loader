#pragma once
#include <vector>

const struct { void* Address; std::vector<uint8_t> Data; const char* Name; } patches_101[] =
{
	// Enable dwgui
	{ (void*)0x00535CC0,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x00535CD0,{ 0xB0, 0x01 }, "dwgui" },
	{ (void*)0x005A3D80,{ 0xB0, 0x01, 0xC3 }, "dwgui" },
	// Enable Debug Cursor RightClick
	{ (void*)0x005DB542,{ 0x00 }, "DebugCursor" },
	// Enable Debug Cursor Moving
	{ (void*)0x005DBCC8,{ 0x64, 0xA3 }, "DebugCursor" },
	// Show Cursor in game window
	{ (void*)0x005DB6C4,{ 0x00 }, "ShowCursor" },
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
	{ (void*)0x006ff101,{ 0xEB }, "ram" },
};
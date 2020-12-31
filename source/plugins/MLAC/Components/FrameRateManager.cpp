#include "FrameRateManager.h"
#include "../Constants.h"
#include "GameState.h"
#include <stdio.h>
#include <windows.h>
#include <iostream>

#include <detours.h>
#pragma comment(lib, "detours.lib")

namespace MLAC::Components
{
	FrameRateManager::FrameRateManager()
	{
		float nMotionRate = 300;
		motionSpeedMultiplier = nMotionRate / 60.0f;
	}

	FrameRateManager::~FrameRateManager()
	{
	}

	const char* FrameRateManager::GetDisplayName()
	{
		return "frame_rate_manager";
	}

	float FrameRateManager::fspeed_error = 0; // compensation value for use in this frame
	float FrameRateManager::fspeed_error_next = 0; // save a compensation value to be used in the next frame

	float FrameRateManager::fspeedhook_lastresult = 0; // used by the ageage hair patch to be lazy and avoid setting up for a proper call

	float(*divaGetFrameSpeed)() = (float(*)())0x00702f00;
	// a version of the original function that tries to round the output to more closely match chara motion timings
	float hookedGetFrameSpeed()
	{
		float frameSpeed = divaGetFrameSpeed();

		// below is somewhat based (in concept) on 140194ad0 (motion quantisation thingy func)

		// add the error compensation from last frame
		frameSpeed += FrameRateManager::fspeed_error;

		// separate whole and fractional parts of speed
		// float speed_rounded = floorf(frameSpeed);
		// float speed_remainder = frameSpeed - speed_rounded;
		float speed_rounded;
		float speed_remainder = modff(frameSpeed, &speed_rounded);

		// save the remainder as error compensation for next frame
		// use == 0 to detect new frame and vars must be written
		if (FrameRateManager::fspeed_error_next == 0)
		{
			FrameRateManager::fspeed_error_next = speed_remainder;
			FrameRateManager::fspeedhook_lastresult = speed_rounded;
		}

		return speed_rounded;
	}


	void FrameRateManager::Initialize()
	{
		printf("[MLAC] FrameRateManager::Initialize(): Initialized\n");
		pvFrameRate = (float*)PV_FRAME_RATE_ADDRESS;
		frameSpeed = (float*)FRAME_SPEED_ADDRESS;
		aetFrameDuration = (float*)AET_FRAME_DURATION_ADDRESS;

		// The default is expected to be 1.0 / 60.0
		defaultAetFrameDuration = *aetFrameDuration;

		// This const variable is stored inside a data segment so we don't want to throw any access violations
		DWORD oldProtect;
		VirtualProtect((void*)AET_FRAME_DURATION_ADDRESS, sizeof(float), PAGE_EXECUTE_READWRITE, &oldProtect);

		// by somewhatlurker#7760

		// high fps auto speed fix
		// is in divaGetFrameSpeed
		InjectCode((void*)0x00702f30, { 0xeb, 0x01 }); // (change JBE to JMP)

		// aet fix
		// is in aetobj vftable index 4
		// not too confident in this, but should be fine I guess
		InjectCode((void*)0x0041ec89, { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 }); // (nuke conversions to double precision because framerate is only single : pokefast:)
		InjectCode((void*)0x0041ec8f, { 0xf3 }); // (MULSD to MULSS)
		InjectCode((void*)0x0041ec93, { 0xf3, 0x0f, 0x5e, 0x05, 0xc8, 0xbb, 0xe4, 0x00 }); // (change mult by 1 / 60, or div by 60, to div by framerate)
		InjectCode((void*)0x0041eca0, { 0x90, 0x90, 0x90, 0x90 }); // (nuke conversions from double back to single because it's never double now)

		// edit aet fix
		// is in one of the funcs that reads PV_FRAME_RATE_ADDRESS
		// need to find a constant 60.0 single precision float for this (00 00 70 42) -- 009e74b4 looks good
		InjectCode((void*)0x00702d50, { 0xd9, 0x05, 0xb4, 0x74, 0x9e, 0x00 }); // (change the address to read from)

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread());
		DetourAttach(&(PVOID&)divaGetFrameSpeed, hookedGetFrameSpeed);
		DetourTransactionCommit();
	}

	void FrameRateManager::Update()
	{
		// *aetFrameDuration = 1.0f / GetGameFrameRate();

		if (*(GameState*)CURRENT_GAME_STATE_ADDRESS == GS_GAME)
		{
			*pvFrameRate = 60.0f * motionSpeedMultiplier;
		}
		else
		{
			*pvFrameRate = 60.0f;
		}

		if (*(SubGameState*)CURRENT_GAME_SUB_STATE_ADDRESS == SUB_GAME_MAIN || *(SubGameState*)CURRENT_GAME_SUB_STATE_ADDRESS == SUB_DEMO)
		{
			// enable dynamic framerate
			*(bool*)USE_AUTO_FRAMESPEED_ADDRESS = true;

			// target framerate
			*(float*)AUTO_FRAMESPEED_TARGET_FRAMERATE_ADDRESS = *pvFrameRate;

			// trying to fix meltdown's water
			//if ((uint64_t*)0x1411943f8 != nullptr)
				//*(float*)(*(uint64_t*)0x1411943f8 + 0x1c) = (60.0f / GetGameFrameRate()) / 15.0f;
				//*(float*)(*(uint64_t*)0x1411943f8 + 0x10) = *(float*)(*(uint64_t*)0x1411943f8);
		}
		else
		{
			// enable dynamic framerate
			*(bool*)USE_AUTO_FRAMESPEED_ADDRESS = true;

			// target framerate
			*(float*)AUTO_FRAMESPEED_TARGET_FRAMERATE_ADDRESS = 60.0f;
		}
	}

	void FrameRateManager::UpdateDraw2D()
	{
		// cycle the framespeed timing error once per frame
		fspeed_error = fspeed_error_next;
		fspeed_error_next = 0;
	}

	void FrameRateManager::InjectCode(void* address, const std::vector<uint8_t> data)
	{
		const size_t byteCount = data.size() * sizeof(uint8_t);

		DWORD oldProtect;
		VirtualProtect(address, byteCount, PAGE_EXECUTE_READWRITE, &oldProtect);
		memcpy(address, data.data(), byteCount);
		VirtualProtect(address, byteCount, oldProtect, nullptr);
	}
}

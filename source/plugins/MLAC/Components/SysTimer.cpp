#include "SysTimer.h"
#include "../MainModule.h"
#include "../Constants.h"

namespace MLAC::Components
{
	SysTimer::SysTimer()
	{
	}

	SysTimer::~SysTimer()
	{
	}

	const char *SysTimer::GetDisplayName()
	{
		return "sys_timer";
	}

	void SysTimer::Initialize()
	{
		DWORD oldProtect, bck;
		VirtualProtect((BYTE *)0x006C150B, 3, PAGE_EXECUTE_READWRITE, &oldProtect);
		*((BYTE *)0x006C150B + 0) = 0x90;
		*((BYTE *)0x006C150B + 1) = 0x90;
		*((BYTE *)0x006C150B + 2) = 0x90;
		VirtualProtect((BYTE *)0x006C150B, 3, oldProtect, &bck);
	}

	void SysTimer::Update()
	{
	}
}

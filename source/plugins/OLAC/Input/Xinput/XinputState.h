#pragma once
#include <windows.h>

namespace OLAC::Input
{
	struct XinputState
	{
		bool KeyStates[0x3F];
	
		bool IsDown(BYTE keycode);
	};
}


#pragma once
#include <windows.h>

namespace MLAC::Input
{
	struct MouseState
	{
		POINT Position;
		POINT RelativePosition;
		long MouseWheel;
	};
}


#pragma once
#include <windows.h>

namespace ELAC::Input
{
	struct MouseState
	{
		POINT Position;
		POINT RelativePosition;
		long MouseWheel;
	};
}


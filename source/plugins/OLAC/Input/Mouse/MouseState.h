#pragma once
#include <windows.h>

namespace OLAC::Input
{
	struct MouseState
	{
		POINT Position;
		POINT RelativePosition;
		long MouseWheel;
	};
}


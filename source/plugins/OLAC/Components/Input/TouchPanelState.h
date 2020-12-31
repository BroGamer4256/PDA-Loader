#pragma once
#include "ContactType.h"

namespace OLAC::Components
{
	struct TouchPanelState
	{
		int Padding00[0x18];
		int ConnectionState;
		int Padding01[0x06];
		float XPosition;
		float YPosition;
		float Pressure;
		ContactType ContactType;
	};
}

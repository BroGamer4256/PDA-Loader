#include "XinputState.h"

namespace OLAC::Input
{
	bool XinputState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}

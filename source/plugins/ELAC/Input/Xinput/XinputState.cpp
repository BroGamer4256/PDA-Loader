#include "XinputState.h"

namespace ELAC::Input
{
	bool XinputState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}

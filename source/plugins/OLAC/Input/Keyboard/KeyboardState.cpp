#include "KeyboardState.h"

namespace OLAC::Input
{
	bool KeyboardState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}

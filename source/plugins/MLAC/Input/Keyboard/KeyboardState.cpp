#include "KeyboardState.h"

namespace MLAC::Input
{
	bool KeyboardState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}

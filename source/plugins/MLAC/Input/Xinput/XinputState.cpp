#include "XinputState.h"

namespace MLAC::Input
{
	bool XinputState::IsDown(BYTE keycode)
	{
		return KeyStates[keycode];
	}
}

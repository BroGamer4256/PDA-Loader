#pragma once
#include "DirectInputDevice.h"

namespace MLAC::Input
{
	class Controller : public DirectInputDevice
	{
	protected:
		Controller();
		~Controller();
	};
}
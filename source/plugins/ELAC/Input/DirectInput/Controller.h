#pragma once
#include "DirectInputDevice.h"

namespace ELAC::Input
{
	class Controller : public DirectInputDevice
	{
	protected:
		Controller();
		~Controller();
	};
}
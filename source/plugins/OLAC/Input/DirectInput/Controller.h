#pragma once
#include "DirectInputDevice.h"

namespace OLAC::Input
{
	class Controller : public DirectInputDevice
	{
	protected:
		Controller();
		~Controller();
	};
}
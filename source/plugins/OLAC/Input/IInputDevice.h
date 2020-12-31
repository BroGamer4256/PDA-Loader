#pragma once

namespace OLAC::Input
{
	class IInputDevice
	{
	public:
		virtual bool PollInput() = 0;
	};
}

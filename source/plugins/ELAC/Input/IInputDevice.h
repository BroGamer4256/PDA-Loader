#pragma once

namespace ELAC::Input
{
	class IInputDevice
	{
	public:
		virtual bool PollInput() = 0;
	};
}

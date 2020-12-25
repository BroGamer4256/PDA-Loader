#pragma once

namespace MLAC::Input
{
	class IInputDevice
	{
	public:
		virtual bool PollInput() = 0;
	};
}

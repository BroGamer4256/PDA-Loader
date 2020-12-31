#pragma once
#include "EmulatorComponent.h"

namespace OLAC::Components
{
	class DebugComponent : public EmulatorComponent
	{
	public:
		DebugComponent();
		~DebugComponent();

		virtual const char* GetDisplayName() override;

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void UpdateInput() override;
	};
}


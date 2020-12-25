#pragma once
#include <vector>
#include "IInputBinding.h"

namespace MLAC::Input
{
	class Binding
	{
	public:
		std::vector<IInputBinding*> InputBindings;

		Binding();
		~Binding();

		void AddBinding(IInputBinding* inputBinding);

		bool AnyDown();
		bool AnyTapped();
		bool AnyReleased();
	};
}


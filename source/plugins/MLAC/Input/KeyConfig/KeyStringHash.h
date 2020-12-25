#pragma once
#include "KeyString.h"

namespace MLAC::Input::KeyConfig
{
	struct KeyStringHash
	{
		size_t operator()(const KeyString& key) const;
	};
}

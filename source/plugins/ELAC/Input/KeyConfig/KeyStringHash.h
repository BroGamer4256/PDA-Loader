#pragma once
#include "KeyString.h"

namespace ELAC::Input::KeyConfig
{
	struct KeyStringHash
	{
		size_t operator()(const KeyString& key) const;
	};
}

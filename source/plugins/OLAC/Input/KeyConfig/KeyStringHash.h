#pragma once
#include "KeyString.h"

namespace OLAC::Input::KeyConfig
{
	struct KeyStringHash
	{
		size_t operator()(const KeyString& key) const;
	};
}

#include "CustomPlayerData.h"

namespace OLAC::Components
{
	CustomPlayerData::CustomPlayerData()
	{
	}

	CustomPlayerData::~CustomPlayerData()
	{
		if (PlayerName != nullptr)
			delete PlayerName;

		if (LevelName != nullptr)
			delete LevelName;
	}
}
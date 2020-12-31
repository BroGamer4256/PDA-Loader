#pragma once
#include <string>

namespace ELAC::Components
{
	struct CustomPlayerData
	{
		CustomPlayerData();
		~CustomPlayerData();

		std::string *PlayerName;
		std::string *LevelName;
		int Level;
		int LevelPlateId;
		int VocaloidPoint;
		int SkinEquip;
		int BtnSeEquip;
		int ModuleEquip[2];
		uint8_t UseCard;
	};
}

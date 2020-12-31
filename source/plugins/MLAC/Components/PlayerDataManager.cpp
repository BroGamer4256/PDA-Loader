#include "PlayerDataManager.h"
#include <string>
#include "../MainModule.h"
#include "../Constants.h"
#include "../Input/Keyboard/Keyboard.h"
#include "../FileSystem/ConfigFile.h"
#include <string.h>

const std::string PLAYER_DATA_FILE_NAME = "playerdata.ini";

namespace MLAC::Components
{
	PlayerDataManager::PlayerDataManager()
	{
	}

	PlayerDataManager::~PlayerDataManager()
	{
		delete customPlayerData;
	}

	const char* PlayerDataManager::GetDisplayName()
	{
		return "player_data_manager";
	}

	void PlayerDataManager::Initialize()
	{
		playerData = (PlayerData*)PLAYER_DATA_ADDRESS;
		printf("[MLAC] PlayerDataManager::Initialize(): Initialized\n");

		LoadConfig();
	}

	void PlayerDataManager::Update()
	{
		if (playerData == nullptr)
			return;
		/*
		if (false && Input::Keyboard::GetInstance()->IsTapped(VK_F12))
		{
			printf("PlayerDataManager::Update(): Loading config...\n");
			LoadConfig();
		}
		*/

		// Module Selection Workaround
		for (int i = 0; i < 124; ++i)
			*((BYTE*)0x00DEA4F0 + i) = 0xFF;
		*((BYTE*)0x00DEA56C) = 0x7F;
	}

	void PlayerDataManager::LoadConfig()
	{
		FileSystem::ConfigFile config(MainModule::GetModuleDirectory(), std::wstring(PLAYER_DATA_FILE_NAME.begin(), PLAYER_DATA_FILE_NAME.end()));

		if (!config.OpenRead())
			return;

		if (customPlayerData != nullptr)
			delete customPlayerData;

		customPlayerData = new CustomPlayerData();

		auto parseInt = [&](const std::string &key)
		{
			std::string *stringBuffer;

			int result = 0;

			if (config.TryGetValue(key, stringBuffer))
			{
				result = std::stoi(stringBuffer->c_str());
				delete stringBuffer;
			}

			return result;
		};

		config.TryGetValue("player_name", customPlayerData->PlayerName);
		config.TryGetValue("level_name", customPlayerData->LevelName);

		customPlayerData->VocaloidPoint = parseInt("vocaloid_point");
		customPlayerData->ModuleEquip[0] = parseInt("module_equip[0]");
		customPlayerData->UseCard = true;
		playerData->use_card = true;

		playerData->vocaloid_point = customPlayerData->VocaloidPoint;

		//for (int i = 0; i < sizeof(playerData->module_equip) / sizeof(int); i++)
		//	playerData->module_equip[i] = customPlayerData->ModuleEquip[i];

		if (customPlayerData->PlayerName != nullptr)
		{
		//	playerData->field_70 = 0x10;
		//	playerData->player_name = (char*)customPlayerData->PlayerName->c_str();
			strcpy_s(playerData->player_name, (char*)customPlayerData->PlayerName->c_str());
		}

		if (customPlayerData->LevelName != nullptr)
		{
		//	playerData->field_F4 = 0x10;
		//	playerData->level_name = (char*)customPlayerData->LevelName->c_str();
			strcpy_s(playerData->level_name, (char*)customPlayerData->LevelName->c_str());
		}
	}
}

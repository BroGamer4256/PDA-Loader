#pragma once
#include <stdint.h>

#pragma pack(push, 1)
struct PlayerData
{
	uint8_t use_card; // at 0x00
	uint8_t freeplay; // at 0x01
	uint8_t field_2;
	uint8_t field_3;
	int32_t card_type; // at 0x04
	char* mmuid; // at 0x08
	int32_t field_C;
	uint8_t field_10;
	uint8_t mm_game_id; // at 0x11
	int32_t field_12;
	int32_t access_code; // at 0x16
	int32_t field_1A;
	int32_t field_1E;
	int32_t field_22;
	int32_t field_26;
	uint8_t field_2A;
	int32_t idm; // at 0x2b
	int32_t field_2F;
	int32_t field_33;
	int32_t field_37;
	uint8_t field_3B;
	char* pmm; // at 0x3c
	int32_t field_40;
	int32_t field_44;
	int32_t field_48;
	int32_t field_4C;
	int32_t play_data_id; // at 0x50
	int32_t accept_index; // at 0x54
	int32_t start_index; // at 0x58
	char player_name[31]; // at 0x5c
	char level_name[31]; // at 0x7b
	uint8_t field_9A;
	uint8_t field_9B;
	int32_t vocaloid_point; // at 0x9c
	int32_t hp_vol; // at 0xa0
	int32_t act_vol; // at 0xa4
	int32_t password; // at 0x240
};
#pragma pack(pop)
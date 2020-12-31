#pragma once
#include <stdint.h>

// uint8_t 1 byte
// int32_t 4 bytes

#pragma pack(push, 1)
struct PlayerData
{
uint8_t use_card;
	uint8_t freeplay;
	uint8_t field_2;
	uint8_t field_3;
	int32_t card_type;
};
#pragma pack(pop)
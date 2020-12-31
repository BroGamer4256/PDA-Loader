#pragma once
#include <string>

typedef struct { void* Address; std::vector<uint8_t> Data; const char* Name; } Patch;
#define PATCHES_END { (void*) 0x0, {0x0}, "\x00" }

std::string TrimString(const std::string& str, const std::string& whitespace)
{
	const size_t strBegin = str.find_first_not_of(whitespace);

	if (strBegin == std::string::npos)
		return "";

	const size_t strEnd = str.find_last_not_of(whitespace);
	const size_t strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

std::vector<std::string> SplitString(const std::string& str, const std::string& delim)
{
	std::vector<std::string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = str.find(delim, prev);
		if (pos == std::string::npos)
			pos = str.length();

		std::string token = str.substr(prev, pos - prev);

		if (!token.empty())
			tokens.push_back(token);

		prev = pos + delim.length();
	} while (pos < str.length() && prev < str.length());

	return tokens;
}

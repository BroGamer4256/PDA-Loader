#pragma once

#include "TextFile.h"
#include <unordered_map>

namespace OLAC::FileSystem
{
	class ConfigFile : public TextFile
	{
	public:
		ConfigFile(const std::wstring &path);
		ConfigFile(const std::wstring &directory, const std::wstring &file);

		std::unordered_map<std::string, std::string> ConfigMap;

		bool TryGetValue(const std::string &key, std::string *&value);

	protected:
		virtual void Parse(std::ifstream &fileStream) override;

	private:
		bool IsComment(const std::string &line);
	};
}


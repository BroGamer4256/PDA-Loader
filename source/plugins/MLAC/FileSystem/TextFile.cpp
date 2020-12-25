#include "TextFile.h"
#include <filesystem>

namespace fs = std::filesystem;

namespace MLAC::FileSystem
{
	TextFile::TextFile(const std::string &path)
	{
		FileName = path;
	}

	TextFile::TextFile(const std::string &directory, const std::string &file)
	{
		auto fullPath = directory + "/" + file;
		FileName = fullPath;
	}

	TextFile::~TextFile()
	{
	}

	bool TextFile::OpenRead()
	{
		fs::path configPath(FileName);

		if (!fs::exists(configPath))
			return false;

		std::ifstream fileStream(FileName);
		
		if (!fileStream.is_open())
			return false;

		Parse(fileStream);

		fileStream.close();

		return true;
	}
}

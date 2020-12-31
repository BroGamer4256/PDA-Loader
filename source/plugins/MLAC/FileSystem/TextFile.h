#pragma once

#include <string>
#include <fstream>

namespace MLAC::FileSystem
{
	class TextFile
	{
	public:
		std::wstring FileName;

		TextFile(const std::wstring &path);
		TextFile(const std::wstring &directory, const std::wstring &file);
		~TextFile();
	
		bool OpenRead();
	
	protected:
		virtual void Parse(std::ifstream &fileStream) = 0;
	};
}

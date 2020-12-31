#pragma once
#include <Windows.h>
#include <string>

namespace ELAC
{
	class MainModule
	{
	private:
		static std::wstring *moduleDirectory;

	public:
		static const wchar_t* DivaWindowName;
		static const wchar_t* GlutDefaultName;

		static HWND DivaWindowHandle;
		static HMODULE Module;

		static std::wstring GetModuleDirectory();
		static RECT GetWindowBounds();
	};
}


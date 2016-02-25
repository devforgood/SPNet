#pragma once

#include <windows.h>
#include <string.h>
#include <vector>

namespace System
{
namespace Text
{
namespace Encoding
{
namespace UTF8
{

	inline std::wstring GetString(const std::string & src)
	{
		if(src.length()==0)
			return std::wstring();

		std::vector<wchar_t> buf(MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, 0, 0));

		if(MultiByteToWideChar(CP_UTF8, 0, src.c_str(), -1, &buf[0], (int)buf.size()) == 0)
			return std::wstring();

		return std::wstring(&buf[0]);
	}

	inline std::string GetBytes(const std::wstring & src)
	{
		if(src.length()==0)
			return std::string();

		std::vector<char> buf(WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0));

		if(WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1,  &buf[0], (int)buf.size(), 0, 0) == 0)
			return std::string();

		return std::string(&buf[0]);
	}
}

namespace ACP
{
	inline std::wstring GetString(const std::string & src)
	{
		if(src.length()==0)
			return std::wstring();

		std::vector<wchar_t> buf(MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, 0, 0));

		if(MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, &buf[0], (int)buf.size()) == 0)
			return std::wstring();

		return std::wstring(&buf[0]);
	}

	inline std::string GetBytes(const std::wstring & src)
	{
		if(src.length()==0)
			return std::string();

		std::vector<char> buf(WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1, 0, 0, 0, 0));

		if(WideCharToMultiByte(CP_ACP, 0, src.c_str(), -1,  &buf[0], (int)buf.size(), 0, 0) == 0)
			return std::string();

		return std::string(&buf[0]);
	}
}
}
}
}
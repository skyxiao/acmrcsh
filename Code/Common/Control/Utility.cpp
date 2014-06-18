/*
 * utility.cpp
 *
 *  Created on: 2014Äê3ÔÂ20ÈÕ
 *      Author: acm
 */

//#include <locale>
//#include <codecvt>

#include "boost/filesystem.hpp"
#include "boost/date_time.hpp"


std::string wstring_to_utf8(const std::wstring& src)
{
//	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//
//	return conv.to_bytes(src);
	std::string str;
	for(const wchar_t& chr : src)
	{
		str.push_back(chr);
	}

	return str;
}

std::string recipe_fullname(const std::string& recipe_name)
{
	std::string path = "./recipe/" + recipe_name + ".rcp";

	std::wstring wfullpath = boost::filesystem::system_complete(path).native();

	return wstring_to_utf8(wfullpath);
}

std::string local_time_string()
{
	using namespace boost::posix_time;
	ptime t = microsec_clock::local_time();
	return to_iso_string(t);
}

//std::wstring utf8_to_wstring(const std::string& src)
//{
//	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//
//	return conv.from_bytes(src);
//}




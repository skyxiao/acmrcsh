/*
 * utility.h
 *
 *  Created on: 2014Äê3ÔÂ20ÈÕ
 *      Author: acm
 */

#ifndef UTILITY_H_
#define UTILITY_H_

std::string wstring_to_utf8(const std::wstring& src);

std::string recipe_fullname(const std::string& recipe_name);

std::string local_time_string();

//std::wstring utf8_to_wstring(const std::string& src);

#endif /* UTILITY_H_ */

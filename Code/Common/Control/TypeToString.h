/*
 * TypeToString.h
 *
 *  Created on: 2014Äê3ÔÂ5ÈÕ
 *      Author: acm
 */

#ifndef TYPETOSTRING_H_
#define TYPETOSTRING_H_

template<typename T>
class TypeToString
{
};

template<>
class TypeToString<bool>
{
public:
	static std::string value = "boolean";
};

template<>
class TypeToString<int>
{
public:
	static std::string value = "integer";
};

template<>
class TypeToString<float>
{
public:
	static std::string value = "float";
};

template<>
class TypeToString<std::string>
{
public:
	static std::string value = "string";
};


#endif /* TYPETOSTRING_H_ */

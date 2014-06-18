/*
 * ParameterItem.cpp
 *
 *  Created on: 2014Äê3ÔÂ31ÈÕ
 *      Author: acm
 */

#include "boost/algorithm/string.hpp"

#include "ParameterItem.h"
#include "SystemParameter.h"

ParameterItemBase::ParameterItemBase(int id, const std::string& name, const std::string& path,
		const std::string& description, const std::string& unit)
	: m_id(id), m_name(name), m_path(path), m_description(description), m_unit(unit)
{
	SystemParameter::Instance().Mapping(id, this);
}

bool ParameterItemBase::IsChild(const std::string& path)
{
	return boost::algorithm::equals(m_path, path);
}

bool ParameterItemBase::IsInRange(int from, int to)
{
	return m_id>=from && m_id<=to;
}

bool ParameterItemBase::IsInSet(const std::set<int>& id_set)
{
	return id_set.find(m_id) != id_set.end();
}



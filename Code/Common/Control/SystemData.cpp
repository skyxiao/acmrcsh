/*
 * SystemData.cpp
 *
 *  Created on: 2014Äê2ÔÂ25ÈÕ
 *      Author: acm
 */

#include "boost/algorithm/string.hpp"

#include "SystemData.h"
#include "SystemDataPool.h"

BaseSystemData::BaseSystemData(int id, const std::string& name,
		const std::string& path)
: m_id(id), m_name(name), m_path(path), m_changed_flag(false)
{
	SystemDataPool::Instance().Mapping(id, this);
}

bool BaseSystemData::IsChild(const std::string& path)
{
	return boost::algorithm::equals(m_path, path);
}

bool BaseSystemData::HasChanged()
{
	return m_changed_flag;
}

bool BaseSystemData::IsInRange(int from, int to)
{
	return m_id>=from && m_id<=to;
}

bool BaseSystemData::IsInSet(const std::set<int>& id_set)
{
	return id_set.find(m_id) != id_set.end();
}


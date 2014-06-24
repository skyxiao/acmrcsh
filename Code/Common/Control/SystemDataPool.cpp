/*
 * SystemDataPool.cpp
 *
 *  Created on: 2014Äê3ÔÂ5ÈÕ
 *      Author: acm
 */

#include "boost/lexical_cast.hpp"

#include "SystemDataPool.h"
#include "LogFile.h"

//#include "Data.h"

void SystemDataPool::Initialize()
{
	for(auto& v : m_all_data)
	{
		if(v.second)
		{
			v.second->Initialize();
		}
	}
}

void SystemDataPool::Terminate()
{
	for(auto& v : m_all_data)
	{
		if(v.second)
		{
			v.second->Terminate();
		}
	}

	m_all_data.clear();

	LogDebug("SystemDataPool is terminated.");
}

std::string SystemDataPool::GetJsonData(boost::function<bool (BaseSystemData*)> f)
{
	using namespace boost::property_tree;
	ptree array;
	BaseSystemData* data_ptr = nullptr;
	for(std::map<int, BaseSystemData*>::value_type& v : m_all_data)
	{
		data_ptr = v.second;
		if(data_ptr && (!f || f(data_ptr)))
			data_ptr->Serialize(array);
	}
	ptree pt;
	pt.add_child("systemdata", array);

	std::stringstream ss;
	json_parser::write_json(ss, pt);

	return ss.str();
}

void SystemDataPool::SetData(int id, const std::string& data)
{
	BaseSystemData* p = m_all_data[id];
	if (!p)
		return;

	try
	{
		if (p->Type() == typeid(unsigned))
		{
			SystemData<unsigned>* pp =
					dynamic_cast<SystemData<unsigned>*>(p);
			*pp = boost::lexical_cast<unsigned>(data);
		}
		else if (p->Type() == typeid(int))
		{
			SystemData<int>* pp = dynamic_cast<SystemData<int>*>(p);
			*pp = boost::lexical_cast<int>(data);
		}
		else if (p->Type() == typeid(float))
		{
			SystemData<float>* pp = dynamic_cast<SystemData<float>*>(p);
			*pp = boost::lexical_cast<float>(data);
		}
		else
		{
			SystemData<std::string>* pp = dynamic_cast<SystemData<std::string>*>(p);
			*pp = data;
		}
	}
	catch (boost::bad_lexical_cast& e)
	{
		//report event log of convert error
	}
}

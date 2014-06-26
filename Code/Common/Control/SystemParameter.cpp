/*
 * SystemParameter.cpp
 *
 *  Created on: 2014Äê4ÔÂ17ÈÕ
 *      Author: Administrator
 */

#include "boost/lexical_cast.hpp"

#include "SystemParameter.h"


void SystemParameter::Mapping(int id, ParameterItemBase* cfg_ptr)
{
	m_cfgs[id] = cfg_ptr;
}

void SystemParameter::SetData(int id, const std::string& data)
{
	ParameterItemBase* p = m_cfgs[id];
	if (!p)
		return;

	try
	{
		if (p->Type() == typeid(unsigned))
		{
			ParameterItem<unsigned>* pp =
					dynamic_cast<ParameterItem<unsigned>*>(p);
			*pp = boost::lexical_cast<unsigned>(data);
		}
		else if (p->Type() == typeid(int))
		{
			ParameterItem<int>* pp = dynamic_cast<ParameterItem<int>*>(p);
			*pp = boost::lexical_cast<int>(data);
		}
		else if (p->Type() == typeid(float))
		{
			ParameterItem<float>* pp = dynamic_cast<ParameterItem<float>*>(p);
			*pp = boost::lexical_cast<float>(data);
		}
		else
		{
			ParameterItem<std::string>* pp = dynamic_cast<ParameterItem<
					std::string>*>(p);
			*pp = data;
		}
	}
	catch (boost::bad_lexical_cast& e)
	{
		//report event log of convert error
	}
}

std::string SystemParameter::GetJsonData(boost::function<bool(ParameterItemBase*)> f)
{
	using namespace boost::property_tree;
	ptree array;
	ParameterItemBase* item_ptr = nullptr;
	for (auto &v : m_cfgs)
	{
		item_ptr = v.second;
		if (f && f(item_ptr))
		{
			item_ptr->Serialize(array);
		}
	}
	ptree pt;
	pt.add_child("parameters", array);

	std::stringstream ss;
	json_parser::write_json(ss, pt);

	return ss.str();
}


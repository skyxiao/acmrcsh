/*
 * DeviceManager.cpp
 *
 *  Created on: 2014Äê3ÔÂ3ÈÕ
 *      Author: acm
 */

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "DeviceManager.h"
#include "DeviceFactory.h"
#include "LogFile.h"

void DeviceManager::Initialize()
{
	using namespace boost::property_tree;
	ptree pt;

	try
	{
		json_parser::read_json("./config/device.json", pt);
	}
	catch(json_parser_error& e)
	{
		LogFatal(e.message());
		throw e;
	}

	for(ptree::value_type &v : pt.get_child("DeviceList"))
	{
		int id = v.second.get<int>("id");
		std::string name = v.second.get<std::string>("name");
		boost::shared_ptr<Device> dev_ptr = DeviceFactory::CreateDevice(id, name);
		if(dev_ptr)
		{
			dev_ptr->Initialize();
			m_devices[id] = dev_ptr;
		}
	}
}

void DeviceManager::Terminate()
{
	for(auto& v : m_devices)
	{
		v.second->Terminate();
		v.second.reset();
	}
	m_devices.clear();

	LogDebug("DeviceManager is terminated.");
}

boost::shared_ptr<Device> DeviceManager::GetDevice(int id)
{
	auto itor = m_devices.find(id);
	if(itor == m_devices.end())
	{
		std::stringstream ss;
		ss<<"Device "<<id<<" doesn't exist.";
		LogFatal(ss.str());
		throw ss.str();
	}

	return itor->second;
}

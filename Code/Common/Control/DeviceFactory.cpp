/*
 * DeviceFactory.cpp
 *
 *  Created on: 2014Äê4ÔÂ7ÈÕ
 *      Author: Administrator
 */

#include "boost/algorithm/string.hpp"

#include "DeviceFactory.h"
#include "DummyDevice.h"
#include "AdsDevice.h"

boost::shared_ptr<Device> DeviceFactory::CreateDevice(int id, const std::string& name)
{
	if(boost::algorithm::iequals(name, "Dummy"))
	{
		return boost::shared_ptr<Device>(new DummyDevice(id));
	}
	else if(boost::algorithm::iequals(name, "Ads"))
	{
		return boost::shared_ptr<Device>(new AdsDevice(id));
	}

	return boost::shared_ptr<Device>(nullptr);
}

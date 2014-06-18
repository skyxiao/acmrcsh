/*
 * DeviceFactory.h
 *
 *  Created on: 2014Äê4ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef DEVICEFACTORY_H_
#define DEVICEFACTORY_H_

#include "boost/shared_ptr.hpp"

#include "Device.h"

class DeviceFactory
{
public:
	DeviceFactory() = delete;
	virtual ~DeviceFactory() = default;

	static boost::shared_ptr<Device> CreateDevice(int id, const std::string& name);
public:

};

#endif /* DEVICEFACTORY_H_ */

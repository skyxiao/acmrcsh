/*
 * AdsDevice.h
 *
 *  Created on: 2014Äê4ÔÂ8ÈÕ
 *      Author: Administrator
 */

#ifndef ADSDEVICE_H_
#define ADSDEVICE_H_

#include <map>

#include "boost/shared_ptr.hpp"

#include "Device.h"

class AdsDevice : public Device
{
public:
	AdsDevice(int dev_id) : Device(dev_id){};
	virtual ~AdsDevice() = default;

	virtual std::string GetName();
	virtual void Initialize();
	virtual void Terminate();
};

#endif /* ADSDEVICE_H_ */

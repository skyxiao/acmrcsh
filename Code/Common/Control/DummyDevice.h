/*
 * DummyDevice.h
 *
 *  Created on: 2014Äê4ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef DUMMYDEVICE_H_
#define DUMMYDEVICE_H_

#include "Device.h"

class DummyDevice: public Device
{
public:
	DummyDevice(int dev_id) : Device(dev_id){};
	virtual ~DummyDevice() = default;

	virtual std::string GetName();
	virtual void Initialize();
	virtual void Terminate();
	//for real device
	virtual void Write(unsigned int value, unsigned block, unsigned byte_offset, unsigned bit_offset, unsigned bits);
	virtual unsigned int Read(unsigned block, unsigned byte_offset, unsigned bit_offset, unsigned bits);

	virtual unsigned int Follow(unsigned block, boost::function<void ()> f);
	virtual void Unfollow(unsigned block, unsigned token);
};

#endif /* DUMMYDEVICE_H_ */

/*
 * Device.h
 *
 *  Created on: 2014Äê2ÔÂ27ÈÕ
 *      Author: acm
 */

#ifndef DEVICE_H_
#define DEVICE_H_

#include <map>

#include "boost/thread.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/scoped_ptr.hpp"

#include "Block.h"

class Device
{
public:
	Device(int dev_id) : m_dev_id(dev_id) {};
	virtual ~Device() = default;

	virtual bool IsSimulator();

	virtual std::string GetName() = 0;
	virtual void Initialize();
	virtual void Terminate();

	//for real device
	virtual void Write(unsigned int value, unsigned block, unsigned byte_offset,
			unsigned bit_offset, unsigned bits);
	virtual unsigned int Read(unsigned block, unsigned byte_offset, unsigned bit_offset, unsigned bits);

	//for simulator
	virtual void Write(int id, float value);
	virtual float Read(int id);

	virtual unsigned int Follow(unsigned block, boost::function<void ()> f);
	virtual void Unfollow(unsigned block, unsigned token);

private:
	void sync();

protected:
	std::map<unsigned, boost::shared_ptr<Block>> m_blocks;
	int m_dev_id;
	boost::scoped_ptr<boost::thread> m_thrd;
};

#endif /* DEVICE_H_ */

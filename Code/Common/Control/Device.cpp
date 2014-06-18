/*
 * Device.cpp
 *
 *  Created on: 2014Äê2ÔÂ27ÈÕ
 *      Author: acm
 */


#include "Device.h"


bool Device::IsSimulator()
{
	return false;
}

void Device::Initialize()
{
	for(auto& x : m_blocks)
	{
		x.second->Initialize();
	}

	if(!m_thrd)
		m_thrd.reset(new boost::thread([this](){this->sync();}));
}

void Device::Terminate()
{
	if(m_thrd)
	{
		m_thrd->interrupt();
		m_thrd->join();
		m_thrd.reset(nullptr);
	}

	for(auto& x : m_blocks)
	{
		x.second->Terminate();
	}

	m_blocks.clear();
}

void Device::sync()
{
	boost::this_thread::disable_interruption di;

	using namespace boost::posix_time;
	auto t = microsec_clock::universal_time();

	while(true)
	{
		t += milliseconds(50);
		{
			for(auto& x : m_blocks)
			{
				x.second->Sync();
			}
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep(t);
	}
}

void Device::Write(unsigned int value, unsigned block, unsigned byte_offset, unsigned bit_offset, unsigned bits)
{
	m_blocks[block]->Write(value, byte_offset, bit_offset, bits);
}

unsigned int Device::Read(unsigned block, unsigned byte_offset, unsigned bit_offset, unsigned bits)
{
	return m_blocks[block]->Read(byte_offset, bit_offset, bits);
}

void Device::Write(int id, float value)
{

}

float Device::Read(int id)
{
	return .0f;
}


unsigned int Device::Follow(unsigned block, boost::function<void ()> f)
{
	return m_blocks[block]->Follow(f);
}

void Device::Unfollow(unsigned block, unsigned token)
{
	m_blocks[block]->Unfollow(token);
}

/*
 * DummyDevice.cpp
 *
 *  Created on: 2014Äê4ÔÂ7ÈÕ
 *      Author: Administrator
 */

#include <sstream>

#include "DummyDevice.h"
#include "LogFile.h"

void DummyDevice::Initialize()
{
}

void DummyDevice::Terminate()
{
}

std::string DummyDevice::GetName()
{
	return "Dummy";
}

//for real device
void DummyDevice::Write(unsigned int value, unsigned block, unsigned byte_offset,
		unsigned bit_offset, unsigned bits)
{
	std::stringstream ss;
	ss<<"<write>--[dummy device]--[block: "<<block<<"]--[byte offset: "<<byte_offset<<"]--[bit offset: "
			<<bit_offset<<"]--[bits: "<<bits<<"]--[value: "<<value<<"]";
	LogInfo(ss.str());
}

unsigned int DummyDevice::Read(unsigned block, unsigned byte_offset, unsigned bit_offset, unsigned bits)
{
	std::stringstream ss;
	ss<<"<read>--[dummy device]--[block: "<<block<<"]--[byte offset: "<<byte_offset<<"]--[bit offset: "
			<<bit_offset<<"]--[bits: "<<bits<<"]";
	LogInfo(ss.str());

	return 0;
}

unsigned int DummyDevice::Follow(unsigned block, boost::function<void ()> f)
{
	return 0;
}

void DummyDevice::Unfollow(unsigned block, unsigned token)
{

}

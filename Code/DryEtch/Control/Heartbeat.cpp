/*
 * Heartbeat.cpp
 *
 *  Created on: 2014Äê3ÔÂ20ÈÕ
 *      Author: acm
 */

#include "boost/date_time/posix_time/ptime.hpp"

#include "Heartbeat.h"
#include "Data.h"

void Heartbeat::Initialize()
{
	//start heartbeat thread
	m_thrd.reset(new boost::thread([this](){do_work();}));
}

void Heartbeat::Terminate()
{
	if(m_thrd)
	{
		m_thrd->interrupt();
		m_thrd->join();
		m_thrd.reset(nullptr);
	}
}

void Heartbeat::do_work()
{
	boost::this_thread::disable_interruption di;

	using namespace boost::posix_time;
	auto t = microsec_clock::universal_time();

	while(true)
	{
		Data::doHeartbeat = Data::doHeartbeat ? 0 : 1;
		
		if (boost::this_thread::interruption_requested())
			break;

		t += milliseconds(500);
		boost::this_thread::sleep(t);
	}
}

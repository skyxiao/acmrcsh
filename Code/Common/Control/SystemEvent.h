/*
 * SystemEvent.h
 *
 *  Created on: 2014Äê3ÔÂ17ÈÕ
 *      Author: acm
 */

#ifndef SYSTEMEVENT_H_
#define SYSTEMEVENT_H_

#include "boost/format.hpp"

#include "EventLog.h"

class SystemEvent
{
public:
	SystemEvent(unsigned int evt_id, EventLevel evt_level, const std::string& format_string)
		: m_evt_id(evt_id), m_evt_level(evt_level), m_format_string(format_string)
	{

	}
	~SystemEvent(){};

public:
	void Report()
	{
		EventLogger::Instance().Log(m_evt_id, m_evt_level, m_format_string);
	}

	template<typename T1>
	void Report(T1 arg1)
	{
		EventLogger::Instance().Log(m_evt_id, m_evt_level, boost::str(boost::format(m_format_string)%arg1));
	}

	template<typename T1, typename T2>
	void Report(T1 arg1, T2 arg2)
	{
		EventLogger::Instance().Log(m_evt_id, m_evt_level, boost::str(boost::format(m_format_string)%arg1%arg2));
	}

	template<typename T1, typename T2, typename T3>
	void Report(T1 arg1, T2 arg2, T3 arg3)
	{
		EventLogger::Instance().Log(m_evt_id, m_evt_level, boost::str(boost::format(m_format_string)%arg1%arg2%arg3));
	}

	template<typename T1, typename T2, typename T3, typename T4>
	void Report(T1 arg1, T2 arg2, T3 arg3, T4 arg4)
	{
		EventLogger::Instance().Log(m_evt_id, m_evt_level, boost::str(boost::format(m_format_string)%arg1%arg2%arg3%arg4));
	}

	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	void Report(T1 arg1, T2 arg2, T3 arg3, T4 arg4, T5 arg5)
	{
		EventLogger::Instance().Log(m_evt_id, m_evt_level, boost::str(boost::format(m_format_string)%arg1%arg2%arg3%arg4%arg5));
	}

private:
	unsigned int m_evt_id;
	EventLevel m_evt_level;
	std::string m_format_string;
};


#endif /* SYSTEMEVENT_H_ */

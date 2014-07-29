/*
 * MonitorItem.cpp
 *
 *  Created on: 2014Äê5ÔÂ13ÈÕ
 *      Author: Administrator
 */

#include "MonitorItem.h"
#include "Event.h"

using namespace boost::chrono;

MonitorItem::MonitorItem(const std::string& name, boost::function<float ()> getter)
: m_name(name), m_getter(getter), m_setpoint(.0f), m_warn_offset(.0f), m_alarm_offset(.0f),
  m_delay(.0f), m_enable_flag(false), m_report_flag(0), m_level(0)
{
	m_start_time = system_clock::now();
	m_warn_trigger_time = m_start_time;
	m_alarm_trigger_time = m_start_time;
}

void MonitorItem::Reset(float delay, float timeout, float setpoint, float warn_offset, float alarm_offset)
{
	m_delay = delay;
	m_timeout = timeout;
	m_setpoint = setpoint;
	m_warn_offset = warn_offset;
	m_alarm_offset = alarm_offset;
	m_start_time = system_clock::now() + boost::chrono::milliseconds((int)(m_delay*1000));
	m_warn_trigger_time = m_start_time + boost::chrono::milliseconds((int)(m_timeout*1000));
	m_alarm_trigger_time = m_start_time + boost::chrono::milliseconds((int)(m_timeout*1000));
	m_enable_flag = true;
	m_report_flag = 0;
	m_level = 0;
}

// void MonitorItem::Reset(float setpoint)
// {
// 	m_setpoint = setpoint;
// 	m_start_time = system_clock::now() + boost::chrono::milliseconds(m_delay);
// 	m_enable_flag = true;
// 	m_report_flag = 0;
// 	m_level = 0;
// }

void MonitorItem::Enable()
{
	m_start_time = system_clock::now() + boost::chrono::milliseconds((int)(m_delay*1000));
	m_warn_trigger_time = m_start_time + boost::chrono::milliseconds((int)(m_timeout*1000));
	m_alarm_trigger_time = m_start_time + boost::chrono::milliseconds((int)(m_timeout*1000));
	m_enable_flag = true;
	m_report_flag = 0;
	m_level = 0;
}

void MonitorItem::Disable()
{
	m_enable_flag = false;
	m_report_flag = 0;
	m_level = 0;
}

void MonitorItem::Monitor()
{
	if(!m_enable_flag)
		return;

	if(system_clock::now() < m_start_time)
		return;

	float value = m_getter();
	float warn_low = m_setpoint - m_warn_offset;
	float warn_high = m_setpoint + m_warn_offset;
	float alarm_low = m_setpoint - m_alarm_offset;
	float alarm_high = m_setpoint + m_alarm_offset;

	if(value < warn_high && value > warn_low)
	{
		m_warn_trigger_time = system_clock::now() + boost::chrono::milliseconds((int)(m_timeout*1000));
	}

	if(value < alarm_high && value > alarm_low)
	{
		m_alarm_trigger_time = system_clock::now() + boost::chrono::milliseconds((int)(m_timeout*1000));
	}

	if(value > alarm_high || value < alarm_low)
	{
		if(m_alarm_trigger_time < system_clock::now())
		{
			if(m_report_flag < 2)
			{
				EVT::MonitorAlarm.Report(value, alarm_low, alarm_high, m_name);
				m_level = 2;
			}
			m_report_flag = 2;
			return;
		}
	}

	if(value > warn_high || value < warn_low)
	{
		if(m_warn_trigger_time < system_clock::now())
		{
			if(m_report_flag < 1)
			{
				EVT::MonitorWarning.Report(value, warn_low, warn_high, m_name);
				if(m_level < 1)
				{
					m_level = 1;
				}
			}
			m_report_flag = 1;
			return;
		}
	}

	m_report_flag = 0;
}

bool MonitorItem::HasWarning()
{
	return m_level >= 1;
}

bool MonitorItem::HasAlarm()
{
	return m_level >= 2;
}

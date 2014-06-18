/*
 * MonitorItem.h
 *
 *  Created on: 2014Äê5ÔÂ13ÈÕ
 *      Author: Administrator
 */

#ifndef MONITORITEM_H_
#define MONITORITEM_H_

#include "boost/function.hpp"
#include "boost/noncopyable.hpp"
#include "boost/chrono.hpp"

class MonitorItem : public boost::noncopyable
{
	typedef boost::chrono::time_point<boost::chrono::system_clock> time_point;
public:
	MonitorItem(boost::function<float ()> getter, float delay,
			boost::function<void (float, float, float)> warn_evt,
			boost::function<void (float, float, float)> alarm_evt);

	void Reset(float setpoint, float warn_offset, float alarm_offset);
	void Reset(float setpoint);
	void Enable();
	void Disable();
	void Monitor();
	bool HasWarning();
	bool HasAlarm();

private:
	boost::function<float ()> m_getter;
	float m_setpoint;
	float m_warn_offset;
	float m_alarm_offset;
	float m_delay;
	boost::function<void (float, float, float)> m_warn_evt;
	boost::function<void (float, float, float)> m_alarm_evt;

	bool m_enable_flag;
	unsigned m_report_flag;	//no:0, warn:1, alarm:2
	unsigned m_level;	//no:0, warn:1, alarm:2
	time_point m_start_time;
};

#endif /* MONITORITEM_H_ */

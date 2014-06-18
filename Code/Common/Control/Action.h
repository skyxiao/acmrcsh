/*
 * WaitCondition.h
 *
 *  Created on: 2014Äê2ÔÂ19ÈÕ
 *      Author: acm
 */

#ifndef ACTION_H_
#define ACTION_H_

#include "boost/chrono.hpp"
#include "boost/function.hpp"

const int RESULT_UNFINISHED = 1;
const int RESULT_OK = 0;
const int RESULT_FAILED = -1;

class WaitCondition
{
public:
	WaitCondition(boost::function<bool ()> f, unsigned int timeout,
			boost::function<void ()> evt_f) : m_flag(true), m_timeout(timeout),
			m_f(f), m_evt_f(evt_f)
	{

	}
	~WaitCondition() {};

	int operator ()()
	{
		if(m_flag)
		{
			m_expired_time = boost::chrono::system_clock::now() + boost::chrono::milliseconds(m_timeout);
			m_flag = false;
		}

		if(m_f())
		{
			m_flag = true;
			return RESULT_OK;
		}

		if(boost::chrono::system_clock::now() > m_expired_time)
		{
			m_flag = true;
			if(m_evt_f)
			{
				m_evt_f();
			}
			return RESULT_FAILED;
		}

		return RESULT_UNFINISHED;
	}

private:
	bool m_flag;
	unsigned int m_timeout;
	boost::chrono::time_point<boost::chrono::system_clock> m_expired_time;
	boost::function<bool ()> m_f;
	boost::function<void ()> m_evt_f;
};

class Wait
{
public:
	Wait(unsigned int timeout) :
		m_flag(true), m_timeout(timeout)
	{

	}
	~Wait() {};

	int operator ()()
	{
		if(m_flag)
		{
			m_expired_time = boost::chrono::system_clock::now() + boost::chrono::milliseconds(m_timeout);
			m_flag = false;
		}

		if(boost::chrono::system_clock::now() > m_expired_time)
		{
			m_flag = true;
			return RESULT_OK;
		}

		return RESULT_UNFINISHED;
	}

private:
	bool m_flag;
	unsigned int m_timeout;
	boost::chrono::time_point<boost::chrono::system_clock> m_expired_time;
};

class Command
{
public:
	Command(boost::function<void ()> f) : m_f(f)
	{

	}
	~Command() {};

	int operator ()()
	{
		m_f();
		return RESULT_OK;
	}

private:
	boost::function<void ()> m_f;
};

class Expect
{
public:
	Expect(boost::function<bool ()> f, unsigned int duration,
			boost::function<void ()> evt_f) : m_flag(true), m_duration(duration),
			m_f(f), m_evt_f(evt_f)
	{

	}
	~Expect() {};

	int operator ()()
	{
		if(m_flag)
		{
			m_expired_time = boost::chrono::system_clock::now() + boost::chrono::milliseconds(m_duration);
			m_flag = false;
		}

		if(boost::chrono::system_clock::now() > m_expired_time)
		{
			m_flag = true;
			return RESULT_OK;
		}

		if(!m_f())
		{
			m_flag = true;
			return RESULT_FAILED;
		}

		return RESULT_UNFINISHED;
	}

private:
	bool m_flag;
	unsigned int m_duration;
	boost::chrono::time_point<boost::chrono::system_clock> m_expired_time;
	boost::function<bool ()> m_f;
	boost::function<void ()> m_evt_f;
};

#endif /* WAITCONDITION_H_ */

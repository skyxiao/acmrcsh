/*
 * Worker.cpp
 *
 *  Created on: 2014Äê3ÔÂ6ÈÕ
 *      Author: acm
 */

#include "Worker.h"
#include "LogFile.h"

void Worker::Initialize()
{
	if(m_init_flag)
		return;

	int thread_count = 4;
	for(int i=0; i<thread_count; i++)
	{
		m_work_thrds.push_back(new boost::thread(boost::bind(&Worker::DoWork, this)));
	}

	m_init_flag = true;
}

void Worker::Terminate()
{
	if(!m_init_flag)
		return;

	for(boost::thread* thrd : m_work_thrds)
	{
		thrd->interrupt();
		thrd->join();
		delete thrd;
	}
	m_work_thrds.clear();

	m_init_flag = false;

	LogDebug("Worker is terminated.");
}

void Worker::Post(boost::function<void ()> f)
{
	if(!m_init_flag)
		return;

	boost::mutex::scoped_lock lock(mutex_);
	m_jobs.push_back(f);
}

void Worker::DoWork()
{
	boost::this_thread::disable_interruption di;
	LogDebug("worker thread start.");
	std::stringstream ss;

	while(true)
	{
		boost::function<void ()> f;
		size_t size = 0;
		{
			boost::mutex::scoped_lock lock(mutex_);
			size = m_jobs.size();
			if(size > 0)
			{
				f = m_jobs[0];
				m_jobs.pop_front();
			}
		}
		if(f)
		{
			try
			{
				f();
			}
			catch(std::exception& e)
			{
				LogDebug(e.what());
			}
			ss.str("");
			ss<<"jobs count in queue: "<<size-1<<".";
			LogDebug(ss.str());
			continue;
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	}
	LogDebug("worker thread exit.");
}

/*
 * DataRecorder.cpp
 *
 *  Created on: 2014Äê3ÔÂ20ÈÕ
 *      Author: acm
 */

#include "boost/make_shared.hpp"
#include "boost/date_time/posix_time/ptime.hpp"
#include "boost/algorithm/string.hpp"

#include "DataRecorder.h"
#include "Database.h"
#include "LogFile.h"

RecordItem::RecordItem(const std::string& name, bool enable)
	: m_name(name), m_enable_flag(enable)
{
}

bool RecordItem::operator == (const RecordItem& rhs)
{
	return boost::algorithm::equals(m_name, rhs.m_name);
}

bool RecordItem::IsName(const std::string& name)
{
	return boost::algorithm::equals(m_name, name);
}

void RecordItem::Enable()
{
	m_enable_flag = true;
}

void RecordItem::Disable()
{
	m_enable_flag = false;
}


IntervalRecordItem::IntervalRecordItem(const std::string& name, unsigned interval, boost::function<float ()> f, bool enable)
	: RecordItem(name, enable), m_interval(interval), m_f(f)
{
	m_last_record_time = boost::chrono::system_clock::now();
}

void IntervalRecordItem::Monitor()
{
	if(!m_enable_flag)
		return;

	using namespace boost::chrono;
	auto dur = system_clock::now() - m_last_record_time;
	if(dur > milliseconds(m_interval))
	{
		//record data to database
		float data = m_f();
		Database::Instance().RecordData(m_name, data);
		m_last_record_time = system_clock::now();
	}
}


SwitchRecordItem::SwitchRecordItem(const std::string& name, boost::function<unsigned ()> f, bool enable)
	: RecordItem(name, enable), m_old_value(0), m_f(f)
{
}

void SwitchRecordItem::Monitor()
{
	if(!m_enable_flag)
		return;

	using namespace boost::chrono;
	unsigned value = m_f();
	if(value != m_old_value)
	{
		//record data to database
		Database::Instance().RecordData(m_name, value);
		m_old_value = value;
	}
}


void DataRecorder::Initialize()
{
	if(!m_items.empty())
		return;

	//Add Recorder Item
	//TODO
	//m_items.push_back(boost::make_shared<RecordItem>(RecordItem_N2, "N2", 1000, [](){return 0.0f;}));

	//start record thread
	if(m_thrd)
		m_thrd.reset(new boost::thread(boost::bind(&DataRecorder::monitor, this)));

}

void DataRecorder::Terminate()
{
	if(!m_thrd)
		return;

	m_thrd->interrupt();
	m_thrd->join();
	m_thrd.reset(nullptr);

	m_items.clear();
	LogDebug("DataRecorder is terminated.");
}

void DataRecorder::Enable(const std::string& name)
{
	boost::mutex::scoped_lock lock(m_mtx);
	for(auto& item : m_items)
	{
		if(item->IsName(name))
		{
			item->Enable();
			break;
		}
	}
}

void DataRecorder::Disable(const std::string& name)
{
	boost::mutex::scoped_lock lock(m_mtx);
	for(auto& item : m_items)
	{
		if(item->IsName(name))
		{
			item->Disable();
			break;
		}
	}
}

void DataRecorder::Add(boost::shared_ptr<RecordItem> item)
{
	boost::mutex::scoped_lock lock(m_mtx);
	for(auto& x : m_items)
	{
		if(*item == *x)
		{
			break;
		}
	}

	m_items.push_back(item);
}

void DataRecorder::Remove(const std::string& name)
{
	boost::mutex::scoped_lock lock(m_mtx);
	m_items.remove_if([&name](boost::shared_ptr<RecordItem>& x){return x->IsName(name);});
}

void DataRecorder::monitor()
{
	boost::this_thread::disable_interruption di;

	using namespace boost::posix_time;
	auto t = microsec_clock::universal_time();

	while(true)
	{
		t += milliseconds(50);
		{
			boost::mutex::scoped_lock lock(m_mtx);
			for(auto& x : m_items)
			{
				x->Monitor();
			}
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep(t);
	}
}

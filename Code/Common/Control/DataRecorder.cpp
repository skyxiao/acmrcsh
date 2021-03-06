/*
 * DataRecorder.cpp
 *
 *  Created on: 2014��3��20��
 *      Author: acm
 */

#include "boost/make_shared.hpp"
#include "boost/date_time/posix_time/ptime.hpp"

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
	// if(!m_items.empty())
	// 	return;

	//Add Recorder Item
	//TODO
	//m_items.push_back(boost::make_shared<RecordItem>(RecordItem_N2, "N2", 500, [&](){return 0.0f;}));

	//start record thread
	m_thrd.reset(new boost::thread([this](){monitor();}));

}

void DataRecorder::Terminate()
{
	if(m_thrd)
	{
		m_thrd->interrupt();
		m_thrd->join();
		m_thrd.reset(nullptr);
	}

	m_items.clear();
	LogDebug("DataRecorder is terminated.");
}

void DataRecorder::Enable(const std::string& id)
{
	boost::mutex::scoped_lock lock(m_mtx);
	auto rcd_item_ptr = m_items[id];
	if(rcd_item_ptr)
		rcd_item_ptr->Enable();
}

void DataRecorder::Disable(const std::string& id)
{
	boost::mutex::scoped_lock lock(m_mtx);
	auto rcd_item_ptr = m_items[id];
	if(rcd_item_ptr)
		rcd_item_ptr->Disable();
}

void DataRecorder::Add(const std::string& id, boost::shared_ptr<RecordItem> item)
{
	boost::mutex::scoped_lock lock(m_mtx);
	m_items[id] = item;
}

void DataRecorder::Remove(const std::string& id)
{
	boost::mutex::scoped_lock lock(m_mtx);
	m_items[id] = nullptr;
}

void DataRecorder::Clear()
{
	boost::mutex::scoped_lock lock(m_mtx);
	m_items.clear();
}

void DataRecorder::monitor()
{
	boost::this_thread::disable_interruption di;
	LogDebug("data recorder thread start.");

	using namespace boost::posix_time;
	auto t = microsec_clock::universal_time();

	while(true)
	{
		t += milliseconds(50);
		{
			boost::mutex::scoped_lock lock(m_mtx);
			for(auto& x : m_items)
			{
				if(x.second)
					x.second->Monitor();
			}
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep(t);
	}

	LogDebug("data recorder thread exit.");
}

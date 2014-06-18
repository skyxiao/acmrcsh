/*
 * EventLog.cpp
 *
 *  Created on: 2014Äê4ÔÂ3ÈÕ
 *      Author: acm
 */
#include <utility>
#include <sstream>

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "Utility.h"
#include "EventLog.h"

std::string event_level_to_string(EventLevel level)
{
	std::string str;
	if(level == EVENT_LEVEL_INFO)
		str = "info";
	else if(level == EVENT_LEVEL_WARNING)
		str = "warning";
	else if(level == EVENT_LEVEL_ERROR)
		str = "error";
	else if(level == EVENT_LEVEL_FATAL)
		str = "fatal";

	return str;
}

EventLogger::EventLogger() : m_filter(0xFF)
{
};

void EventLogger::Initialize()
{
}

void EventLogger::Terminate()
{
	m_logs.clear();
	m_sinks.clear();
}

void EventLogger::SetFilter(unsigned int filter)
{
	m_filter = filter;
}

void EventLogger::Log(unsigned int evt_id, EventLevel evt_level, const std::string& info)
{
	if (evt_level & m_filter)
	{
		LogItem item(local_time_string(), evt_id, evt_level, info);
		boost::recursive_mutex::scoped_lock lock(m_mtx);
		m_logs.push_back(item);
		if(m_logs.size()>100)
			m_logs.pop_front();

		for(auto& v : m_sinks)
		{
			auto sink = v.second;
			if(sink)
				sink(evt_id, evt_level, info);
		}
	}
}

std::string EventLogger::FetchLogs()
{
	std::list<LogItem> logs;
	{
		boost::recursive_mutex::scoped_lock lock(m_mtx);
		logs = std::move(m_logs);
	}

	using namespace boost::property_tree;
	ptree pt, root;
	for (LogItem& item : logs)
	{
		ptree node;
		node.put("time", item.get<0>());
		node.put("id", item.get<1>());
		node.put("level", event_level_to_string(item.get<2>()));
		node.put("info", item.get<3>());
		pt.push_back(std::make_pair("", node));
	}
	root.add_child("event_logs", pt);

	std::stringstream ss;
	json_parser::write_json(ss, root);

	return ss.str();
}

void EventLogger::Subscribe(const std::string& name, boost::function<void (unsigned, EventLevel, const std::string&)> sink)
{
	boost::recursive_mutex::scoped_lock lock(m_mtx);
	m_sinks[name] = sink;
}

void EventLogger::Unsubscribe(const std::string& name)
{
	boost::recursive_mutex::scoped_lock lock(m_mtx);
	m_sinks[name].clear();
}

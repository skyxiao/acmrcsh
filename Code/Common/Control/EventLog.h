#pragma once

#include <list>
#include <map>
#include <string>

#include "boost/thread/recursive_mutex.hpp"
#include "boost/tuple/tuple.hpp"
#include "boost/function.hpp"

#include "Singleton.h"
#include "EventLevel.h"

class EventLogger : public SingletonT<EventLogger>
{
	typedef boost::tuple<std::string, unsigned int, EventLevel, std::string> LogItem;
protected:
	EventLogger();
	EventLogger(const EventLogger&) = delete;
	EventLogger& operator = (const EventLogger&) = delete;
public:
	~EventLogger() = default;

	void Initialize();
	void Terminate();
	void SetFilter(unsigned int filter);
	void Log(unsigned int evt_id, EventLevel evt_level, const std::string& info);
	std::string FetchLogs();
	void Subscribe(const std::string& name, boost::function<void (unsigned, EventLevel, const std::string&)> sink);
	void Unsubscribe(const std::string& name);

	friend class SingletonT<EventLogger>;

private:
	boost::recursive_mutex m_mtx;
	unsigned int m_filter;
	std::list<LogItem> m_logs;
	std::map<std::string, boost::function<void (unsigned, EventLevel, const std::string&)>> m_sinks;
};


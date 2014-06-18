/*
 * LogFile.cpp
 *
 *  Created on: 2014Äê3ÔÂ27ÈÕ
 *      Author: acm
 */

#include "LogFile.h"
#include "Utility.h"
#include "EventLog.h"

using namespace boost::log;

void LogFile::Initialize()
{
	boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
	add_file_log(
		keywords::target = "log",
		keywords::file_name = "log_%2N.log",
		keywords::rotation_size = 10 * 1024 * 1024,
		keywords::max_size = 50 * 10 * 1024 * 1024,
		keywords::format = "[%TimeStamp%]\t<%Severity%>\t(%ThreadID%) : %Message%",
		keywords::auto_flush = true);
	core::get()->set_filter(trivial::severity >= trivial::debug);
	add_common_attributes();

	EventLogger::Instance().Subscribe("log", LogEvent);
}

void LogFile::Terminate()
{
	EventLogger::Instance().Unsubscribe("log");
	core::get()->remove_all_sinks();
}

void LogFile::Write(trivial::severity_level level, const std::string& msg)
{
	BOOST_LOG_SEV(m_lg, level)<<msg;
}

void Log(boost::log::trivial::severity_level level, const std::string& msg)
{
	LogFile::Instance().Write(level, msg);
}

void LogEvent(int evt_id, EventLevel level, const std::string& msg)
{
	std::stringstream ss;
	ss<<"[["<<evt_id<<"]] "<<msg;

	if(level == EVENT_LEVEL_INFO)
		Log(trivial::info, ss.str());
	else if(level == EVENT_LEVEL_WARNING)
		Log(trivial::warning, ss.str());
	else if(level == EVENT_LEVEL_ERROR)
		Log(trivial::error, ss.str());
	else
		Log(trivial::fatal, ss.str());
}
void LogTrace(const std::string& msg)
{
	Log(trivial::trace, msg);
}

void LogDebug(const std::string& msg)
{
	Log(trivial::debug, msg);
}

void LogInfo(const std::string& msg)
{
	Log(trivial::info, msg);
}

void LogWarning(const std::string& msg)
{
	Log(trivial::warning, msg);
}

void LogError(const std::string& msg)
{
	Log(trivial::error, msg);
}

void LogFatal(const std::string& msg)
{
	Log(trivial::fatal, msg);
}


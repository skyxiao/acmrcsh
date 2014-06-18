/*
 * LogFile.h
 *
 *  Created on: 2014Äê3ÔÂ27ÈÕ
 *      Author: acm
 */

#ifndef LOGFILE_H_
#define LOGFILE_H_

#include "boost/log/core.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/expressions.hpp"
#include "boost/log/utility/setup/file.hpp"
#include "boost/log/sinks/text_file_backend.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"
#include "boost/log/sources/severity_logger.hpp"
#include "boost/log/sources/record_ostream.hpp"

#include "Singleton.h"
#include "EventLevel.h"

class LogFile : public SingletonT<LogFile>
{
protected:
	LogFile() = default;
	LogFile(const LogFile&) = delete;
	LogFile& operator = (const LogFile&) = delete;
public:
	virtual ~LogFile() = default;

	void Initialize();
	void Terminate();

	void Write(boost::log::trivial::severity_level level, const std::string& msg);

	friend class SingletonT<LogFile>;

private:
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> m_lg;
};

void Log(EventLevel level, const std::string& msg);
void LogTrace(const std::string& msg);
void LogDebug(const std::string& msg);
void LogInfo(const std::string& msg);
void LogWarning(const std::string& msg);
void LogError(const std::string& msg);
void LogFatal(const std::string& msg);
void LogEvent(int evt_id, EventLevel level, const std::string& msg);

#endif /* LOGFILE_H_ */

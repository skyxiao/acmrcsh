/*
 * Monitor.h
 *
 *  Created on: 2014Äê5ÔÂ14ÈÕ
 *      Author: Administrator
 */

#ifndef MONITOR_H_
#define MONITOR_H_

#include <map>

#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/shared_ptr.hpp"

#include "Singleton.h"
#include "MonitorItem.h"

class Monitor : public SingletonT<Monitor>
{
	Monitor() = default;
public:
	virtual ~Monitor() = default;

	void Initialize();
	void Terminate();

	void EnableAll();
	void DisableAll();

	void Enable(const std::string& name);
	void Disable(const std::string& name);

	void Reset(const std::string& name, float setpoint, float warn_offset, float alarm_offset);
	void Reset(const std::string& name, float setpoint);

	bool HasWarning();
	bool HasAlarm();

	friend class SingletonT<Monitor>;

private:
	void do_work();

private:
	boost::mutex m_mtx;
	boost::scoped_ptr<boost::thread> m_thrd_ptr;
	std::map<std::string, boost::shared_ptr<MonitorItem>> m_items;
};

#endif /* MONITOR_H_ */

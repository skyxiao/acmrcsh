/*
 * DataRecorder.h
 *
 *  Created on: 2014Äê3ÔÂ20ÈÕ
 *      Author: acm
 */

#ifndef DATARECORDER_H_
#define DATARECORDER_H_

#include <list>

#include "boost/function.hpp"
#include "boost/chrono.hpp"
#include "boost/thread.hpp"
#include "boost/thread/mutex.hpp"
#include "boost/shared_ptr.hpp"

#include "Singleton.h"


class RecordItem
{
public:
	RecordItem() = delete;
	RecordItem(const std::string& name, bool enable = false);
	RecordItem(const RecordItem&) = delete;
	virtual ~RecordItem() = default;
	RecordItem& operator = (const RecordItem&) = delete;

	bool operator == (const RecordItem&);
	bool IsName(const std::string& name);
	void Enable();
	void Disable();

	virtual void Monitor() = 0;

protected:
	std::string m_name;
	bool m_enable_flag;
};

class IntervalRecordItem : public RecordItem
{
public:
	IntervalRecordItem(const std::string& name, unsigned interval,
			boost::function<float ()> f, bool enable = false);
	virtual ~IntervalRecordItem() = default;

	virtual void Monitor();

private:
	unsigned m_interval;
	boost::function<float ()> m_f;
	boost::chrono::time_point<boost::chrono::system_clock> m_last_record_time;
};

class SwitchRecordItem : public RecordItem
{
public:
	SwitchRecordItem(const std::string& name,
			boost::function<unsigned ()> f, bool enable = false);
	virtual ~SwitchRecordItem() = default;

	virtual void Monitor();

private:
	unsigned m_old_value;
	boost::function<unsigned ()> m_f;
};

class DataRecorder : public SingletonT<DataRecorder>
{
protected:
	DataRecorder() = default;
	DataRecorder(const DataRecorder&) = delete;
	DataRecorder& operator = (const DataRecorder&) = delete;
public:
	virtual ~DataRecorder() = default;

	void Initialize();
	void Terminate();
	void Enable(const std::string& name);
	void Disable(const std::string& name);
	void Add(boost::shared_ptr<RecordItem> item);
	void Remove(const std::string& name);

	friend class SingletonT<DataRecorder>;
private:
	void monitor();

private:
	std::list<boost::shared_ptr<RecordItem>> m_items;
	boost::mutex m_mtx;
	boost::scoped_ptr<boost::thread> m_thrd;
};

#endif /* DATARECORDER_H_ */

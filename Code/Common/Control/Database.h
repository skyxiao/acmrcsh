/*
 * Database.h
 *
 *  Created on: 2014Äê3ÔÂ24ÈÕ
 *      Author: acm
 */

#ifndef DATABASE_H_
#define DATABASE_H_

#include <list>

#include "boost/thread/mutex.hpp"
#include "boost/thread.hpp"
#include "boost/scoped_ptr.hpp"
#include "boost/shared_ptr.hpp"

#include "mysql/mysql.h"

#include "Singleton.h"

class wafer_info
{
public:
	std::string id;
	std::string unit;
	std::string slot;
	std::string size;
	std::string type;
	std::string state;
};

class Database : public SingletonT<Database>
{
	typedef boost::chrono::time_point<boost::chrono::system_clock> time_point;
protected:
	Database() = default;
	Database(const Database&) = delete;
	Database& operator = (const Database&) = delete;

public:
	virtual ~Database() = default;

	void Initialize();
	void Terminate();
	void Log(unsigned int id, const std::string& level, const std::string& info);
	void RecordData(const std::string& name, float data);
	void WaferEnter(const std::string& id, int unit, unsigned short slot, const std::string state);
	void WaferExit(const std::string& id, const std::string& state);
	void ProcessStart(const std::string& id, int unit, unsigned short slot, const std::string& recipe = "");
	void ProcessEnd(const std::string& id, const std::string& state);
	void CreateWafer(const std::string& id, const std::string& type, const std::string& size);
	void RemoveWafer(const std::string& id);

	std::list<boost::shared_ptr<wafer_info>> QueryLoadedWafers();

	friend class SingletonT<Database>;

private:
	void do_work();

private:
	std::list<std::string> m_sql_list;
	boost::mutex m_list_mtx;
	boost::mutex m_db_mtx;
	boost::scoped_ptr<boost::thread> m_thrd;

	MYSQL m_mysql;
};

#endif /* DATABASE_H_ */

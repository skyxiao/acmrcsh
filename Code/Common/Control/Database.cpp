/*
 * Database.cpp
 *
 *  Created on: 2014Äê3ÔÂ24ÈÕ
 *      Author: acm
 */

#include "boost/chrono.hpp"

#include "Database.h"
#include "EventLevel.h"
#include "Utility.h"
#include "LogFile.h"
#include "EventLog.h"
#include "ConfigFile.h"


using namespace boost::chrono;
using namespace boost::property_tree;

void Database::Initialize()
{
	if(m_thrd)
		return;

	ConfigFile cfg_file("database");
	std::string hostname = cfg_file.Read<std::string>("database/host", "127.0.0.1");
	std::string user = cfg_file.Read<std::string>("database/user", "acm");
	std::string password = cfg_file.Read<std::string>("database/password", "acm");
	std::string database = cfg_file.Read<std::string>("database/database", "dry_etch");
	unsigned short port = cfg_file.Read<unsigned short>("database/port", 3306);

	mysql_init(&m_mysql);
	if (!mysql_real_connect(&m_mysql, hostname.c_str(), user.c_str(), password.c_str(),
			database.c_str(), port, NULL, 0))
	{
		std::stringstream ss;
		ss<<"error code: "<<mysql_errno(&m_mysql)<<", error message: "<<mysql_error(&m_mysql);
		LogFatal(ss.str());
		throw ss.str();
	}

	m_thrd.reset(new boost::thread([this](){this->do_work();}));

	EventLogger::Instance().Subscribe("database",
			[this](unsigned id, EventLevel level, const std::string& info)
			{Log(id, event_level_to_string(level), info);});
}

void Database::Terminate()
{
	EventLogger::Instance().Unsubscribe("database");

	if(!m_thrd)
		return;

	m_thrd->interrupt();
	m_thrd->join();
	m_thrd.reset();

	mysql_close(&m_mysql);

	LogDebug("Database is terminated.");
}

void Database::Log(unsigned int id, const std::string& level, const std::string& info)
{
	std::stringstream ss;
	//ss<<boost::chrono::time_fmt(boost::chrono::timezone::local, "%Y-%m-%d %H:%M:%S")<<boost::chrono::system_clock::now();
	ss<<"call add_log('"<<local_time_string()<<"', "<<id<<", '"<<level<<"', \""<<info<<"\");";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::RecordData(const std::string& name, float data)
{
	std::stringstream ss;
	ss<<"call record_data('"<<local_time_string()<<"', '"<<name<<"', "<<data<<");";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::WaferEnter(const std::string& id, int unit, unsigned short slot, const std::string state)
{
	std::stringstream ss;
	ss<<"call wafer_enter('"<<id<<"', "<<unit<<", "<<slot<<", '"<<local_time_string()<<"', '"<<state<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::WaferExit(const std::string& id, const std::string& state)
{
	std::stringstream ss;
	ss<<"call wafer_exit('"<<id<<"', '"<<local_time_string()<<"', '"<<state<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::ProcessStart(const std::string& id, int unit, unsigned short slot, const std::string& recipe)
{
	std::stringstream ss;
	ss<<"call process_start('"<<id<<"', "<<unit<<", "<<slot<<", '"<<local_time_string()<<"', '"<<recipe<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::ProcessEnd(const std::string& id, const std::string& state)
{
	std::stringstream ss;
	ss<<"call process_end('"<<id<<"', '"<<state<<"', '"<<local_time_string()<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::CreateWafer(const std::string& id, const std::string& type, const std::string& size)
{
	std::stringstream ss;
	ss<<"call create_wafer('"<<id<<"', '"<<type<<"', '"<<size<<"', '"<<local_time_string()<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::RemoveWafer(const std::string& id)
{
	std::stringstream ss;
	ss<<"call remove_wafer('"<<id<<"', '"<<local_time_string()<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

std::list<boost::shared_ptr<wafer_info>> Database::QueryLoadedWafers()
{
	std::string sql = "call query_loaded_wafer();";
	
	std::list<boost::shared_ptr<wafer_info>> loaded_wafers;

	MYSQL_RES* result_set;
	{
		//execute query
		LogInfo(sql);
		boost::mutex::scoped_lock lock(m_db_mtx);
		int rtv = mysql_query(&m_mysql, sql.c_str());
		if(rtv)
		{
			LogDebug(sql);
			auto error_code = mysql_error(&m_mysql);
			std::stringstream ss;
			ss<<"mysql query failed, error: "<<error_code<<".";
			LogError(ss.str());
			return loaded_wafers;
		}

		result_set = mysql_store_result(&m_mysql);

		int rows = mysql_num_rows(result_set);
	//	int fields = mysql_num_fields(result_set);
		MYSQL_ROW row;
		for(int i=0; i<rows; i++)
		{
			row = mysql_fetch_row(result_set);

			auto wafer_info_ptr = boost::shared_ptr<wafer_info>(new wafer_info);
			wafer_info_ptr->id = row[0];
			wafer_info_ptr->unit = row[1];
			wafer_info_ptr->slot = row[2];
			wafer_info_ptr->size = row[3];
			wafer_info_ptr->type = row[4];
			wafer_info_ptr->state = row[5];
			loaded_wafers.push_back(wafer_info_ptr);
		}
		mysql_free_result(result_set);
		do 
		{ 
		    result_set = mysql_store_result(&m_mysql); 
		    mysql_free_result(result_set); 
		}while(!mysql_next_result(&m_mysql));
	}

	return loaded_wafers;
}

void Database::do_work()
{
	boost::this_thread::disable_interruption di;
	LogDebug("database thread start.");
	std::stringstream ss;

	while(true)
	{
		std::string sql;
		size_t size = 0;
		{
			boost::mutex::scoped_lock lock(m_list_mtx);
			size = m_sql_list.size();
			if(size > 0)
			{
				sql = *(m_sql_list.begin());
				m_sql_list.pop_front();
			}
		}
		if(!sql.empty())
		{
			boost::mutex::scoped_lock lock(m_db_mtx);
			int rtv = mysql_query(&m_mysql, sql.c_str());
			if(rtv)
			{
				LogDebug(sql);
				auto error_code = mysql_error(&m_mysql);
				ss.str("");
				ss<<"mysql insert failed, error: "<<error_code<<".";
				LogError(ss.str());
			}

			continue;
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	}

	LogDebug("database thread exit.");
}

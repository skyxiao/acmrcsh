/*
 * Database.cpp
 *
 *  Created on: 2014Äê3ÔÂ24ÈÕ
 *      Author: acm
 */

#include "boost/chrono.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "Database.h"
#include "EventLevel.h"
#include "Utility.h"
#include "LogFile.h"
#include "EventLog.h"


using namespace boost::chrono;
using namespace boost::property_tree;

void Database::Initialize()
{
	if(m_thrd)
		return;

	std::string hostname = m_cfg_file.Read<std::string>("database/host", "127.0.0.1");
	std::string user = m_cfg_file.Read<std::string>("database/user", "acm");
	std::string password = m_cfg_file.Read<std::string>("database/password", "acm");
	std::string database = m_cfg_file.Read<std::string>("database/database", "dry_etch");
	unsigned short port = m_cfg_file.Read<unsigned short>("database/port", 3306);

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
	ss<<"call add_log('"<<local_time_string()<<"', "<<id<<", '"<<level<<"', '"<<info<<"');";

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

void Database::ProcessStart(const std::string& id, int unit, const std::string& recipe)
{
	std::stringstream ss;
	ss<<"call process_start('"<<id<<"', "<<unit<<", '"<<local_time_string()<<"', '"<<recipe<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

void Database::ProcessEnd(const std::string& id)
{
	std::stringstream ss;
	ss<<"call process_end('"<<id<<"', '"<<local_time_string()<<"');";

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
	ss<<"call create_wafer('"<<id<<"', '"<<local_time_string()<<"');";

	boost::mutex::scoped_lock lock(m_list_mtx);
	m_sql_list.push_back(ss.str());
}

std::string Database::QueryLog(const time_point& start_time, const time_point& end_time, unsigned level_mask)
{
	std::stringstream ss;
	ss<<"select * from 'dry_etch'.'event_log' where ('time' between '"<<start_time<<"' and '"<<end_time<<"') and ('level' in (";
	if(level_mask & EVENT_LEVEL_INFO)
		ss<<"'info', ";
	if(level_mask & EVENT_LEVEL_WARNING)
		ss<<"'warning', ";
	if(level_mask & EVENT_LEVEL_ERROR)
		ss<<"'error', ";
	if(level_mask & EVENT_LEVEL_FATAL)
		ss<<"'fatal', ";

	ss<<"'')) order by 'time' desc limit 1000;";
	std::string sql = ss.str();
	ss.str("");
	ss.clear();

	MYSQL_RES* result_set;
	{
		//execute query
		LogInfo(sql);
		boost::mutex::scoped_lock lock(m_db_mtx);
		int rtv = mysql_query(&m_mysql, sql.c_str());
		if(!rtv)
		{
			LogError("mysql query log failed.");
			return "";
		}

		result_set = mysql_store_result(&m_mysql);
	}

	ptree pt;

	int rows = mysql_num_rows(result_set);
//	int fields = mysql_num_fields(result_set);
	MYSQL_ROW row;
	for(int i=0; i<rows; i++)
	{
		row = mysql_fetch_row(result_set);
		ptree pt_child;
		pt.put("Time", row[0]);
		pt.put("ID", row[1]);
		pt.put("Level", row[2]);
		pt.put("Info", row[3]);
		pt.push_back(make_pair("", pt_child));
	}
	mysql_free_result(result_set);

	ptree root;
	root.add_child("LogItems", pt);
	json_parser::write_json(ss, root);

	return ss.str();
}

std::string Database::QueryData(const time_point& start_time, const time_point& end_time, const std::string& type)
{
	std::stringstream ss;
	ss<<"select * from 'dry_etch'.'process_data' where ('time' between '"<<start_time<<"' and '"<<end_time<<"') and ('type' = '"<<type<<"') order by 'time' incs limit 1000;";
	std::string sql = ss.str();
	ss.str("");
	ss.clear();

	MYSQL_RES* result_set;
	{
		//execute query
		LogInfo(sql);
		boost::mutex::scoped_lock lock(m_db_mtx);
		int rtv = mysql_query(&m_mysql, sql.c_str());
		if(!rtv)
		{
			LogError("mysql query data failed.");
			return "";
		}

		result_set = mysql_store_result(&m_mysql);
	}

	ptree pt;

	int rows = mysql_num_rows(result_set);
//	int fields = mysql_num_fields(result_set);
	MYSQL_ROW row;
	for(int i=0; i<rows; i++)
	{
		row = mysql_fetch_row(result_set);
		ptree pt_child;
		pt.put("Time", row[0]);
		pt.put("Type", row[1]);
		pt.put("Value", row[2]);
		pt.push_back(make_pair("", pt_child));
	}
	mysql_free_result(result_set);

	ptree root;
	root.add_child("DataRecords", pt);
	json_parser::write_json(ss, root);

	return ss.str();
}

void Database::do_work()
{
	boost::this_thread::disable_interruption di;

	while(true)
	{
		std::string sql;
		{
			boost::mutex::scoped_lock lock(m_list_mtx);
			if(m_sql_list.size() > 0)
			{
				sql = *(m_sql_list.begin());
				m_sql_list.pop_front();
			}
		}
		if(!sql.empty())
		{
			LogInfo(sql);
			boost::mutex::scoped_lock lock(m_db_mtx);
			int rtv = mysql_query(&m_mysql, sql.c_str());
			if(rtv)
			{
				LogError("mysql insert failed.");
			}

			continue;
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
	}
}

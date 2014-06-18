/*
 * ParameterItem.h
 *
 *  Created on: 2014Äê3ÔÂ17ÈÕ
 *      Author: acm
 */

#ifndef PARAMETERITEM_H_
#define PARAMETERITEM_H_

#include <set>
#include <typeinfo>
#include <sstream>

#include "boost/property_tree/ptree.hpp"

#include "ParameterHolder.h"
#include "LogFile.h"
#include "Event.h"

class ParameterItemBase
{
public:
	ParameterItemBase(int id, const std::string& name, const std::string& path,
			const std::string& description, const std::string& unit = "");
	ParameterItemBase() = delete;
	ParameterItemBase(const ParameterItemBase&) = delete;
	virtual ~ParameterItemBase(){};
	ParameterItemBase& operator = (const ParameterItemBase&) = delete;

	bool IsChild(const std::string& path);
	bool IsInRange(int from, int to);
	bool IsInSet(const std::set<int>& id_set);

	virtual void Serialize(boost::property_tree::ptree& pt) = 0;
	virtual const std::type_info& Type() = 0;

protected:
	int m_id;
	std::string m_name;
	std::string m_path;
	std::string m_description;
	std::string m_unit;
	boost::mutex m_mtx;
};

template<typename T>
class ParameterItem : public ParameterItemBase
{
public:
	ParameterItem(int id, const std::string& name,  const std::string& path,const std::string& description,
			T min_value, T max_value, T default_value, const std::string& unit = "")
		: ParameterItemBase(id, name, path, description, unit), m_min(min_value), m_max(max_value), m_default(default_value)
	{
		m_value = ParameterHolder::Instance().Read<T>(path, name, default_value);
	}
	~ParameterItem(void){};

public:
	operator T()
	{
		return m_value;
	}

	T operator = (const T& rhs)
	{
		if(rhs>m_max || rhs<m_min)
		{
			EVT::ValueOutOfRange.Report(rhs, m_min, m_max, m_name);
		}
		else
		{
			T old_value;
			{
				boost::mutex::scoped_lock lock(m_mtx);
				old_value = m_value;
				m_value = rhs;
			}
			ParameterHolder::Instance().Write<T>(m_path, m_name, rhs);
			std::stringstream ss;
			ss<<"Parameter ["<<m_name<<"] changed, "<<old_value<<" -> "<<rhs<<".";
			LogInfo(ss.str());
		}

		return rhs;
	}

	virtual void Serialize(boost::property_tree::ptree& pt)
	{
		using namespace boost::property_tree;
		ptree node;
		node.add<int>("id", m_id);
		node.add<T>("value", m_value);
		pt.push_back(make_pair("", node));
	}

	virtual const std::type_info& Type()
	{
		return typeid(T);
	}

private:
	T m_min;
	T m_max;
	T m_default;
	T m_value;
};

template<>
class ParameterItem<std::string> : public ParameterItemBase
{
public:
	ParameterItem(int id, const std::string& name, const std::string& path, const std::string& description, std::string default_value)
		: ParameterItemBase(id, name, path, description, ""), m_default(default_value)
	{
		m_value = ParameterHolder::Instance().Read<std::string>(path, name, default_value);
	}
	~ParameterItem(void){};

public:
	operator std::string()
	{
		std::string str;
		{
			boost::mutex::scoped_lock lock(m_mtx);
			str = m_value;
		}

		return str;
	}

	ParameterItem& operator = (const std::string& rhs)
	{
		std::string old_value;
		{
			boost::mutex::scoped_lock lock(m_mtx);
			old_value = m_value;
			m_value = rhs;
		}
		ParameterHolder::Instance().Write(m_path, m_name, rhs);
		std::stringstream ss;
		ss<<"Parameter ["<<m_name<<"] changed, "<<old_value<<" -> "<<rhs<<".";
		LogInfo(ss.str());

		return *this;
	}

	virtual void Serialize(boost::property_tree::ptree& pt)
	{
		using namespace boost::property_tree;
		ptree node;
		node.add<int>("id", m_id);
		node.add("value", m_value);
		pt.push_back(make_pair("", node));
	}

	virtual const std::type_info& Type()
	{
		return typeid(std::string);
	}

private:
	std::string m_default;
	std::string m_value;
};

#define PARAMETER_NUMBER(id, type, name, path, description, min, max, default_value, unit) \
	ParameterItem<type> name(id, #name, path, description, min, max, default_value, unit);

#define PARAMETER_STRING(id, name, path, description, default_value) \
		ParameterItem<std::string> name(id, #name, path, description, default_value);

#endif /* PARAMETERITEM_H_ */

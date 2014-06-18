#pragma once

#include <string>

#include "boost/thread.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "Singleton.h"
#include "ConfigFile.h"

class ParameterHolder : public SingletonT<ParameterHolder>
{
protected:
	ParameterHolder() : m_cfg_file("parameters")
	{
	}

	ParameterHolder(const ParameterHolder&) = delete;
	ParameterHolder& operator = (const ParameterHolder&) = delete;
public:
	~ParameterHolder()
	{
	}

	template<typename T>
	T Read(const std::string& path, const std::string& name, const T& default_value)
	{
		std::string full_path = "Parameters/" + path + "/" + name;
		boost::mutex::scoped_lock lock(m_mtx);

		return m_cfg_file.Read(full_path, default_value);
	}

	template<typename T>
	void Write(const std::string& path, const std::string& name, const T& value)
	{
		std::string full_path = "Parameters/" + path + "/" + name;
		boost::mutex::scoped_lock lock(m_mtx);
		m_cfg_file.Write(full_path, value);
	}
//
//	std::string ToJson(const std::string& path)
//	{
//		std::string full_path = "Parameters/" + path;
//		boost::mutex::scoped_lock lock(m_mtx);
//		return m_cfg_file.ToString(full_path);
//	}

	friend class SingletonT<ParameterHolder>;

private:
	ConfigFile m_cfg_file;
	boost::mutex m_mtx;
};


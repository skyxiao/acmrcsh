/*
 * ConfigFile.h
 *
 *  Created on: 2014Äê3ÔÂ25ÈÕ
 *      Author: acm
 */

#ifndef CONFIGFILE_H_
#define CONFIGFILE_H_

#include "boost/property_tree/ptree.hpp"


class ConfigFile
{
public:
	ConfigFile() = delete;
	ConfigFile(const std::string& filename);
	ConfigFile(const ConfigFile&) = delete;
	virtual ~ConfigFile();
	ConfigFile& operator = (const ConfigFile&) = delete;

	template<typename T>
	T Read(const std::string& path, const T& default_value)
	{
		using namespace boost::property_tree;

		try
		{
			return m_pt.get<T>(ptree::path_type(path, '/'));
		}
		catch (...)
		{
			m_pt.put<T>(ptree::path_type(path, '/'), default_value);
		}

		return default_value;
	}

	template<typename T>
	void Write(const std::string& path, const T& value)
	{
		using namespace boost::property_tree;
		m_pt.put<T>(ptree::path_type(path, '/'), value);
		save_to_file();
	}

	std::string ToString(const std::string& path);

private:
	void save_to_file();

private:
	std::string m_filename;
	boost::property_tree::ptree m_pt;
};

#endif /* CONFIGFILE_H_ */

/*
 * ConfigFile.cpp
 *
 *  Created on: 2014Äê3ÔÂ25ÈÕ
 *      Author: acm
 */

#include <sstream>

#include "boost/property_tree/json_parser.hpp"
#include "boost/filesystem.hpp"

#include "ConfigFile.h"
#include "Utility.h"

const static std::string config_path = "./config/";

ConfigFile::ConfigFile(const std::string& name)
{
	using namespace boost::property_tree;

	try
	{
		m_filename = config_path + name + ".json";
		json_parser::read_json(m_filename, m_pt);
	}
	catch (...)
	{
	}
}

ConfigFile::~ConfigFile()
{
	save_to_file();
	m_pt.clear();
}

std::string ConfigFile::ToString(const std::string& path)
{
	using namespace boost::property_tree;
	ptree pt = m_pt.get_child(path);

	std::stringstream ss;
	json_parser::write_json(ss, pt);

	return ss.str();
}

void ConfigFile::save_to_file()
{
	using namespace boost::property_tree;
	using namespace boost::filesystem;

	if(!exists(config_path))
	{
		create_directory(config_path);
	}
	json_parser::write_json(m_filename, m_pt);
}

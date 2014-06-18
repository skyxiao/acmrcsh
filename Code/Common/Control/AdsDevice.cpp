/*
 * AdsDevice.cpp
 *
 *  Created on: 2014Äê4ÔÂ8ÈÕ
 *      Author: Administrator
 */

#include "boost/regex.hpp"
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "boost/make_shared.hpp"

#include "AdsDevice.h"
#include "AdsBlock.h"
#include "LogFile.h"

void AdsDevice::Initialize()
{
	try
	{
		AdsPortOpen();

		using namespace boost::property_tree;
		ptree pt;
		json_parser::read_json("./config/ads.json", pt);

		std::string name, var, net_id;
		unsigned id;
		unsigned short port, read_start, read_end, write_start, write_end;
		for(auto &v : pt.get_child("Blocks"))
		{
			id = v.second.get<unsigned>("ID");
			name = v.second.get<std::string>("Name");
			var = v.second.get<std::string>("Variable");
			net_id = v.second.get<std::string>("NetID");
			port = v.second.get<unsigned short>("Port");
			read_start = v.second.get<unsigned short>("ReadStart");
			read_end = v.second.get<unsigned short>("ReadEnd");
			write_start = v.second.get<unsigned short>("WriteStart");
			write_end = v.second.get<unsigned short>("WriteEnd");
			m_blocks[id] = boost::make_shared<AdsBlock>(id, name, var, net_id, port, read_start,
					read_end, write_start, write_end);
		}

		Device::Initialize();
	}
	catch(std::exception& e)
	{
		LogFatal(e.what());
		throw e;
	}
}

void AdsDevice::Terminate()
{
	Device::Terminate();

	AdsPortClose();
}

std::string AdsDevice::GetName()
{
	return "Ads";
}

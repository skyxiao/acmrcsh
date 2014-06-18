/*
 * RecipeStep.cpp
 *
 *  Created on: 2014Äê5ÔÂ21ÈÕ
 *      Author: Administrator
 */


#include "boost/algorithm/string.hpp"

#include "RecipeStep.h"

using namespace boost::property_tree;

const std::string& RecipeStep::Name() const
{
	return m_name;
}

unsigned short RecipeStep::Duration() const
{
	return m_duration;
}

APCMode RecipeStep::Mode() const
{
	return m_apc_mode;
}

unsigned short RecipeStep::Position() const
{
	return m_position;
}

unsigned short RecipeStep::Pressure() const
{
	return m_pressure;
}

unsigned short RecipeStep::RotateSpeed() const
{
	return m_rotate_speed;
}

BypassMode RecipeStep::HFBypass() const
{
	return m_HF_bypass;
}

unsigned short RecipeStep::HFFlowrate() const
{
	return m_HF_flowrate;
}

BypassMode RecipeStep::EtOHBypass() const
{
	return m_EtOH_bypass;
}

unsigned short RecipeStep::EtOHFlowrate() const
{
	return m_EtOH_flowrate;
}

unsigned short RecipeStep::N2Flowrate() const
{
	return m_N2_flowrate;
}

unsigned short RecipeStep::N2PurgeFlowrate() const
{
	return m_N2_purge_flowrate;
}

BypassMode RecipeStep::bypass_from_string(const std::string& mode)
{
	if(boost::algorithm::equals(mode, "chamber"))
	{
		return BypassMode_Chamber;
	}
	else if(boost::algorithm::equals(mode, "bypass"))
	{
		return BypassMode_Bypass;
	}
	else
	{
		return BypassMode_None;
	}
}

std::string RecipeStep::bypass_to_string(BypassMode mode)
{
	if(mode == BypassMode_Chamber)
	{
		return "chamber";
	}
	else if(mode == BypassMode_Bypass)
	{
		return "bypass";
	}
	else
	{
		return "none";
	}
}

APCMode RecipeStep::apc_from_string(const std::string& mode)
{
	if(boost::algorithm::equals(mode, "pressure"))
	{
		return APCMode_Pressure;
	}
	else
	{
		return APCMode_Position;
	}
}

std::string RecipeStep::apc_to_string(APCMode mode)
{
	if(mode == APCMode_Pressure)
	{
		return "pressure";
	}
	else
	{
		return "position";
	}
}

RecipeStep& RecipeStep::operator = (const RecipeStep& rhs)
{
	m_name = rhs.m_name;
	m_duration = rhs.m_duration;
	m_apc_mode = rhs.m_apc_mode;
	m_position = rhs.m_position;
	m_pressure = rhs.m_pressure;
	m_rotate_speed = rhs.m_rotate_speed;
	m_HF_bypass = rhs.m_HF_bypass;
	m_HF_flowrate = rhs.m_HF_flowrate;
	m_EtOH_bypass = rhs.m_EtOH_bypass;
	m_EtOH_flowrate = rhs.m_EtOH_flowrate;
	m_N2_flowrate = rhs.m_N2_flowrate;
	m_N2_purge_flowrate = rhs.m_N2_purge_flowrate;

	return *this;
}

void RecipeStep::Load(const boost::property_tree::ptree& pt)
{
	m_name = pt.get<std::string>("Name");
	m_duration = pt.get<unsigned short>("Time");
	std::string tmp = pt.get<std::string>("APC Mode");
	m_apc_mode = apc_from_string(tmp);
	m_position = pt.get<unsigned short>("Position");
	m_pressure = pt.get<unsigned short>("Pressure");
	m_rotate_speed = pt.get<unsigned short>("Rotate Speed");
	tmp = pt.get<std::string>("HF Bypass");
	m_HF_bypass = bypass_from_string(tmp);
	m_HF_flowrate = pt.get<unsigned short>("HF");
	tmp = pt.get<std::string>("EtOH Bypass");
	m_EtOH_bypass = bypass_from_string(tmp);
	m_EtOH_flowrate = pt.get<int>("EtOH");
	m_N2_flowrate = pt.get<int>("N2");
	m_N2_purge_flowrate = pt.get<int>("N2 Purge");
}

void RecipeStep::Save(boost::property_tree::ptree& pt)
{
	pt.put("Name", m_name);
	pt.put("Time", m_duration);
	std::string tmp = apc_to_string(m_apc_mode);
	pt.put("APC Mode", tmp);
	pt.put("Position", m_position);
	pt.put("Pressure", m_pressure);
	pt.put("Rotate Speed", m_rotate_speed);
	tmp = bypass_to_string(m_HF_bypass);
	pt.put("HF Bypass", tmp);
	pt.put("HF", m_HF_flowrate);
	tmp = bypass_to_string(m_EtOH_bypass);
	pt.put("EtOH Bypass", tmp);
	pt.put("EtOH", m_EtOH_flowrate);
	pt.put("N2", m_N2_flowrate);
	pt.put("N2 Purge", m_N2_purge_flowrate);
}


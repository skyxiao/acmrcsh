/*
 * UnitManager.cpp
 *
 *  Created on: 2014Äê4ÔÂ24ÈÕ
 *      Author: Administrator
 */

#include "UnitManager.h"
#include "ProcessUnit.h"
#include "LogFile.h"

void UnitManager::Initialize()
{
	m_units[0] = boost::shared_ptr<SmartUnit>(new ProcessUnit(0, "ProcessUnit"));
	m_units[0]->Initialize();
}

void UnitManager::Terminate()
{
	m_units[0]->Terminate();
	m_units.clear();
	LogDebug("UnitManager is terminated.");
}


void UnitManager::Invoke(int unit, unsigned cmd, unsigned param1, unsigned param2)
{
	boost::shared_ptr<SmartUnit> unit_ptr = m_units[unit];
	if(unit_ptr)
	{
		unit_ptr->Invoke(cmd, param1, param2);
	}
}

void UnitManager::AlarmCheck()
{
	for(auto& v : m_units)
	{
		if(v.second)
		{
			v.second->AlarmCheck();
		}
	}
}

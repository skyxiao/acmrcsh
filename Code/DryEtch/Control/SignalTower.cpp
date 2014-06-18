/*
 * SignalTower.cpp
 *
 *  Created on: 2014Äê5ÔÂ19ÈÕ
 *      Author: Administrator
 */

#include "SignalTower.h"
#include "EventLog.h"
#include "ConfigFile.h"
#include "Data.h"
#include "LogFile.h"

void SignalTower::Initialize()
{
	ConfigFile cfg("signaltower");
	m_light_config[0] = cfg.Read("Manual", 0x0040);
	m_light_config[1] = cfg.Read("Auto", 0x0040);
	m_light_config[2] = cfg.Read("Maintenance", 0x0010);
	m_light_config[3] = cfg.Read("Warning", 0x0104);
	m_light_config[4] = cfg.Read("Alarm", 0x0202);
	m_light_config[5] = cfg.Read("Emergency", 0x0202);

	change_light(0);

	EventLogger::Instance().Subscribe("light",
			[this](unsigned id, EventLevel level, const std::string& info)
			{this->on_event(id, level, info);});
}

void SignalTower::Terminate()
{
	EventLogger::Instance().Unsubscribe("light");
	LogDebug("SignalTower is terminated.");
}

void SignalTower::ChangeMode(LightMode mode)
{
	m_mode = mode;
	if(m_evt == LightEvent_None)
	{
		change_light(mode);
	}
}

void SignalTower::ReportEvent(LightEvent evt)
{
	if(evt > m_evt)
	{
		m_evt = evt;
		change_light(evt+3);
	}
}

void SignalTower::ClearEvent()
{
	m_evt = LightEvent_None;
	change_light(m_mode);
}

void SignalTower::on_event(unsigned, EventLevel level, const std::string&)
{
	LightEvent evt = LightEvent_None;
	switch(level)
	{
	case EVENT_LEVEL_WARNING:
		evt = LightEvent_Warning;
		break;
	case EVENT_LEVEL_ERROR:
		evt = LightEvent_Alarm;
		break;
	case EVENT_LEVEL_FATAL:
		evt = LightEvent_Emergency;
		break;
	default:
		break;
	}

	ReportEvent(evt);
}

void SignalTower::change_light(unsigned cfg_index)
{
	if(cfg_index >= 6)
		return;

	unsigned cfg = m_light_config[cfg_index];
	unsigned control_word = 0;

	SystemData<unsigned>* light[5] = {&Data::aoRedLight, &Data::aoYellowLight, &Data::aoGreenLight, &Data::aoBlueLight, &Data::aoBuzzer};
	for(unsigned i=0; i<5; i++)
	{
		control_word = (cfg & 0x1) | (cfg & 0x2)<<7;
		*light[i] = control_word;
		cfg = cfg>>2;
	}
}

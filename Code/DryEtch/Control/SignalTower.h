/*
 * SingnalLight.h
 *
 *  Created on: 2014Äê5ÔÂ19ÈÕ
 *      Author: Administrator
 */

#ifndef SINGNALTOWER_H_
#define SINGNALTOWER_H_

#include "boost/thread/mutex.hpp"

#include "Singleton.h"
#include "EventLevel.h"

enum LightMode : unsigned
{
	LightMode_Manual,
	LightMode_Auto,
	LightMode_Maintenance
};

enum LightEvent : unsigned
{
	LightEvent_None,
	LightEvent_Warning,
	LightEvent_Alarm,
	LightEvent_Emergency
};

enum LightControl : unsigned
{
	Light_Off = 0,
	Light_On = 1,
	Light_Flash = 256
};

enum BuzzerControl : unsigned
{
	Buzzer_Off = 0,
	Buzzer_Warning = 1,
	Buzzer_Alarm = 256
};

class SignalTower : public SingletonT<SignalTower>
{
	SignalTower() = default;
public:
	virtual ~SignalTower() = default;

	void Initialize();
	void Terminate();

	void ChangeMode(LightMode mode);
	void ReportEvent(LightEvent evt);
	void ClearEvent();

	friend class SingletonT<SignalTower>;

private:
	void on_event(unsigned, EventLevel, const std::string&);
	void change_light(unsigned cfg_index);

private:
	boost::mutex m_mtx;
	LightMode m_mode = LightMode_Manual;
	LightEvent m_evt = LightEvent_None;
	unsigned m_light_config[6] = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0};


};

#endif /* SINGNALTOWER_H_ */

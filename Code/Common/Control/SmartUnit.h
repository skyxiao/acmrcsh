/*
 * SmartUnit.h
 *
 *  Created on: 2014Äê2ÔÂ14ÈÕ
 *      Author: acm
 */

#ifndef SMARTUNIT_H_
#define SMARTUNIT_H_

#include "boost/scoped_ptr.hpp"
#include "boost/thread.hpp"

#include "TaskStep.h"

enum UnitState //: unsigned
{
	State_Idle = 0,
	State_Running,
	State_Error,
};

#define COMMAND_ONLINE 0
#define COMMAND_OFFLINE 1
#define COMMAND_RETRY 2
#define COMMAND_ABORT 3
#define COMMAND_PAUSE 4
#define COMMAND_RESUME 5

#define COMMAND_DELIMITER 1000
#define	COMMAND_NONE 1000

#define	COMMAND_HOME 1001
#define	COMMAND_LOAD 1002
#define	COMMAND_UNLOAD 1003
#define	COMMAND_PROCESS 1004
#define	COMMAND_CLEAN 1005
#define	COMMAND_ALIGN 1006

#define COMMAND_PUMP 1007
#define COMMAND_VENT 1008
#define COMMAND_PURGE 1009

#define COMMAND_PINUP 1010
#define COMMAND_PINDOWN 1011

#define COMMAND_TURNON_HEATER 1012
#define COMMAND_TURNOFF_HEATER 1013

#define COMMAND_LEAKCHECK 1014

#define COMMAND_ROTATE_FORWARD 1015
#define COMMAND_ROTATE_BACKWARD 1016

#define COMMAND_OPEN_DOOR 1017
#define COMMAND_CLOSE_DOOR 1018


struct UnitTask
{
	unsigned command;
	unsigned para1;
	unsigned para2;
};


class SmartUnit
{
public:
	SmartUnit(int id, const std::string& name);
	virtual ~SmartUnit();

public:
	virtual void Initialize();
	virtual void Terminate();
	void Invoke(unsigned cmd, unsigned param1, unsigned param2);

protected:
	void Online(int hp = __INT_MAX__);
	void Offline();
	void Retry();
	void Abort();
	void Pause();
	void Resume();

protected:
	virtual UnitTask GetNextTask() = 0;
	virtual void UpdateUnitInfo() = 0;
	virtual void SafeHandle() = 0;
	virtual void TranslateTask(const UnitTask& task) = 0;
	virtual void Notify(const std::string& msg) = 0;
	virtual void OnAbort() = 0;
	virtual bool OnlinePrecheck() = 0;

private:
	void work_fun();
//	bool CanPause();
	bool CanManualOperate();
	void Translate(const UnitTask& task);

protected:
	void HomeComplete();

protected:
	int m_id;
	std::string m_name;
	unsigned m_hp;
	bool m_need_home;
	UnitState m_state;
	bool m_pause_flag;
	std::string m_step_name;
	bool m_can_retry;
	UnitTask m_task;
	UnitTask m_last_task;
	boost::scoped_ptr<boost::thread> m_thrd;
	boost::recursive_mutex m_mtx;
	std::list<boost::shared_ptr<TaskStep> > m_steps;
	boost::shared_ptr<TaskStep> m_cur_step;

};


#define NEW_UNIT_STEP(name, retry) { \
	boost::shared_ptr<TaskStep> step(new TaskStep(name, retry));


#define ADD_STEP_COMMAND(func) step->Add(Command(func));
#define ADD_STEP_WAIT_CONDITION(cond, timeout, evt) step->Add(WaitCondition(cond, 1000*timeout, evt));
#define ADD_STEP_WAIT(timeout) step->Add(Wait(1000*timeout));
#define ADD_STEP_EXPECT(cond, duration, evt) step->Add(Expect(cond, 1000*duration, evt));

#define END_UNIT_STEP m_steps.push_back(step); \
	}


#endif /* SMARTUNIT_H_ */

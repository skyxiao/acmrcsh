/*
 * SmartUnit.cpp
 *
 *  Created on: 2014Äê2ÔÂ19ÈÕ
 *      Author: acm
 */
#include <sstream>

#include "SmartUnit.h"
#include "LogFile.h"

SmartUnit::SmartUnit(int id, const std::string& name) :
		m_id(id), m_name(name), m_hp(0), m_need_home(true), m_state(State_Idle),
		m_pause_flag(false), m_step_name(""), m_can_retry(false)
{
	m_last_task = UnitTask{COMMAND_NONE, 0, 0};
	m_task = UnitTask{COMMAND_NONE, 0, 0};
}

SmartUnit::~SmartUnit()
{
}

void SmartUnit::Initialize()
{
	//subclass should call __base::Initialize in the last
	m_thrd.reset(new boost::thread([this](){work_fun();}));
}

void SmartUnit::Terminate()
{
	//subclass should call __base::Terminate in the first
	m_thrd->interrupt();
	m_thrd->join();
	m_thrd.reset(nullptr);
}

void SmartUnit::Invoke(unsigned cmd, unsigned param1, unsigned param2)
{
	//high priority command can be execute any time
	boost::recursive_mutex::scoped_lock lock(m_mtx);
	if(cmd < COMMAND_DELIMITER)
	{
		switch(cmd)
		{
		case COMMAND_ONLINE:
			Online();
			break;
		case COMMAND_OFFLINE:
			Offline();
			break;
		case COMMAND_RETRY:
			Retry();
			break;
		case COMMAND_ABORT:
			Abort();
			break;
		case COMMAND_PAUSE:
			Pause();
			break;
		case COMMAND_RESUME:
			Resume();
			break;
		}
		return;
	}

	if(!CanManualOperate())
	{
		Notify("Unit is busy or online, operation is aborted.");
		return;
	}

	if((m_need_home && cmd == COMMAND_HOME) || !m_need_home)
	{
		Translate(UnitTask{cmd, param1, param2});
	}
	else
	{
		Notify("Homing is needed before doing other operation.");
	}
}

void SmartUnit::Online(int hp)
{
//	if(m_need_home)
//	{
//		Notify("Homing is needed before put unit online.");
//		return;
//	}
	if(!OnlinePrecheck())
	{
		return;
	}

	m_hp = hp;
}
void SmartUnit::Offline()
{
	m_hp = 0;
}

void SmartUnit::Retry()
{
	if (m_state == State_Error && m_can_retry)
	{
		m_cur_step->Reset();
		m_state = State_Running;
	}
}

void SmartUnit::Abort()
{
//	if (m_state != State_Error)
//		return;

	m_cur_step.reset();
	m_steps.clear();
	m_step_name = "";
	m_can_retry = false;
	m_hp = 0;
	//m_need_home = true;
	m_state = State_Idle;

	OnAbort();
}

//do nothing, not support
void SmartUnit::Pause()
{
//	if (m_state == State_Error)
//		return;

//	if (CanPause())
//		m_pause_flag = true;
}

void SmartUnit::Resume()
{
//	m_pause_flag = false;
}

void SmartUnit::Translate(const UnitTask& task)
{
	if (m_task.command != COMMAND_NONE && task.command != m_task.command)
		m_last_task = m_task;

	m_task = task;

	if (task.command == COMMAND_NONE)
		return;

	{
		if(m_hp>0)
		{
			m_hp--;
		}
	}

	std::stringstream ss;
	ss<<"Translate task ["<<task.command<<", "<<task.para1<<", "<<task.para2<<"].";
	LogDebug(ss.str());

	TranslateTask(task);
}

void SmartUnit::work_fun()
{
	boost::this_thread::disable_interruption di;
	unsigned int loop_interval = 50; //ms, this value can read from config in the future

	while (true)
	{
		{
			boost::recursive_mutex::scoped_lock lock(m_mtx);
			if (m_state == State_Idle)
			{
				if (!m_pause_flag)
				{
					if (m_steps.size() > 0)
					{
						m_state = State_Running;
						continue;
					}
					else
					{
						if (m_hp > 0)
						{
							UnitTask task = GetNextTask();
							Translate(task);
						}
					}
				}
			}
			else if (m_state == State_Running)
			{
				if (!m_cur_step && m_steps.empty())
				{
					m_state = State_Idle;
					continue;
				}

				if (!m_cur_step)
				{
					if(!m_steps.empty())
					{
						m_cur_step = *(m_steps.begin());
						m_steps.pop_front();
						m_step_name = m_cur_step->GetName();
						m_can_retry = m_cur_step->CanRetry();
					}
				}

				if (m_cur_step)
				{
					int rtv = m_cur_step->Execute();
					std::cout<<"execute result: "<<rtv<<std::endl;
					if (rtv == RESULT_OK)
					{
						m_step_name = "";
						m_can_retry = false;
						m_cur_step.reset();
					}
					else if (rtv == RESULT_FAILED)
					{
						SafeHandle();
						m_state = State_Error;
					}
				}
			}
			else
			{
				//error state, do nothing
			}

			//update unit information
			UpdateUnitInfo();
		}

		if (boost::this_thread::interruption_requested())
			break;

		boost::this_thread::sleep_for(
				boost::chrono::milliseconds(loop_interval));
	}

	std::cout<<"smart unit thread exit!"<<std::endl;
}

//bool SmartUnit::CanPause()
//{
//	switch (m_task.command)
//	{
//	case COMMAND_PROCESS:
//	case COMMAND_CLEAN:
//	case COMMAND_LOAD:
//	case COMMAND_UNLOAD:
//		return false;
//	default:
//		return true;
//	}
//
//	return true;
//}

bool SmartUnit::CanManualOperate()
{
	if(m_hp > 0 || m_state != State_Idle)
		return false;

	if(!m_steps.empty())
		return false;

	return true;
}
void SmartUnit::HomeComplete()
{
	m_need_home = false;
}

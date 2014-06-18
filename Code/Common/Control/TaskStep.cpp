/*
 * TaskStep.cpp
 *
 *  Created on: 2014Äê2ÔÂ19ÈÕ
 *      Author: acm
 */

#include "TaskStep.h"

TaskStep::TaskStep(const std::string& name, bool can_retry) :
		m_can_retry(can_retry), m_index(0), m_name(name)
{
}

TaskStep::~TaskStep()
{
}

int TaskStep::Execute()
{
	int rtv;
	do
	{
		boost::function<int()>& f = m_actions[m_index++];
		rtv = f();
	} while (rtv == RESULT_OK && m_index < m_actions.size());

	if (rtv == RESULT_OK && m_index >= m_actions.size())
		return RESULT_OK;

	m_index--;
	return rtv;
}

void TaskStep::Reset()
{
	m_index = 0;
}

void TaskStep::Add(boost::function<int()> f)
{
	m_actions.push_back(f);
}

bool TaskStep::CanRetry()
{
	return m_can_retry;
}

std::string TaskStep::GetName()
{
	return m_name;
}


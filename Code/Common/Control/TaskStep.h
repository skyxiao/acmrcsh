/*
 * TaskStep.h
 *
 *  Created on: 2014Äê2ÔÂ19ÈÕ
 *      Author: acm
 */

#ifndef TASKSTEP_H_
#define TASKSTEP_H_

#include "Action.h"

class TaskStep
{
public:
	TaskStep(const std::string& name, bool can_retry = false);
	~TaskStep();

	int Execute();
	void Reset();
	void Add(boost::function<int ()> f);
	bool CanRetry();
	std::string GetName();

private:
	bool m_can_retry;
	std::size_t m_index;
	std::string m_name;
	std::vector<boost::function<int ()> > m_actions;
};


#endif /* TASKSTEP_H_ */

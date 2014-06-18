/*
 * Worker.h
 *
 *  Created on: 2014Äê3ÔÂ6ÈÕ
 *      Author: acm
 */

#ifndef WORKER_H_
#define WORKER_H_

#include <vector>
#include <deque>

#include "boost/function.hpp"
#include "boost/thread.hpp"

#include "Singleton.h"

class Worker : public SingletonT<Worker>
{
protected:
	Worker() : m_init_flag(false){};
	Worker& operator = (const Worker&) = delete;
	Worker(const Worker&) = delete;
public:
	virtual ~Worker() = default;

	void Initialize();
	void Terminate();
	void Post(boost::function<void ()> f);

	friend class SingletonT<Worker>;

private:
	void DoWork();

private:
	bool m_init_flag;
	std::vector<boost::thread*> m_work_thrds;
	std::deque<boost::function<void ()>> m_jobs;

};

#endif /* WORKER_H_ */

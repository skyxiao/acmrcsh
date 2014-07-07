/*
 * Heartbeat.h
 *
 *  Created on: 2014Äê3ÔÂ20ÈÕ
 *      Author: acm
 */

#ifndef HEARTBEAT_H_
#define HEARTBEAT_H_

#include "boost/thread.hpp"
 
#include "Singleton.h"

class Heartbeat : public SingletonT<Heartbeat>
{
protected:
	Heartbeat() = default;
	Heartbeat(const Heartbeat&) = delete;
	Heartbeat& operator = (const Heartbeat&) = delete;
public:
	virtual ~Heartbeat() = default;

	void Initialize();
	void Terminate();

	friend class SingletonT<Heartbeat>;
private:
	void do_work();

private:
	boost::scoped_ptr<boost::thread> m_thrd;
};

#endif /* HEARTBEAT_H_ */

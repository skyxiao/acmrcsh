/*
 * Interlock.h
 *
 *  Created on: 2014Äê3ÔÂ17ÈÕ
 *      Author: acm
 */

#ifndef INTERLOCK_H_
#define INTERLOCK_H_

#include "Singleton.h"

class Interlock : public SingletonT<Interlock>
{
protected:
	Interlock() = default;
	Interlock(const Interlock&) = delete;
	Interlock& operator = (const Interlock&) = delete;
public:
	~Interlock() = default;

	void Initialize();
	void Terminate();

	friend class SingletonT<Interlock>;

private:
	void shut_chemical_except_n2();
	void shut_all_chemical();
	void report_alcohol_alarm();
};

#define POST_INTERLOCK_EX(di, func) Data::di.AddChangeSink(func);

#define POST_INTERLOCK(di, di_val, doo, do_val) Data::di.AddChangeSink([&](unsigned, unsigned value) \
	{ \
		if(value == di_val) \
		{ Data::doo = do_val; } \
	});

#define POST_INTERLOCK_EVT(di, di_val, evt) Data::di.AddChangeSink([&](unsigned, unsigned value) \
	{ \
		if(value == di_val) \
		{ EVT::evt.Report(); } \
	});

#define POST_INTERLOCK_EVT_STRING(di, di_val, evt_str) Data::di.AddChangeSink([&](unsigned, unsigned value) \
	{ \
		if(value == di_val) \
		{ EVT::GenericWarning.Report(evt_str); } \
	});

#define POST_INTERLOCK_AI_EVT(ai, param, evt, name) Data::ai.AddChangeSink([&](unsigned last_value, unsigned value) \
	{ \
		if(value>Parameters::param && last_value<Parameters::param) \
		{ EVT::evt.Report(name, value); } \
	});

#define POST_INTERLOCK_MEMBER(di, di_val, mem_func) Data::di.AddChangeSink([this](unsigned, unsigned value) \
	{ \
		if(value == di_val) \
		{ mem_func(); } \
	});

#define PRE_INTERLOCK(doo, do_val, di, di_val) \
		Data::doo.AddPrecheck([&](unsigned new_value) \
		{ \
			if(new_value == do_val && Data::di == di_val) \
			{ \
				EVT::InterlockTrigger.Report(#doo, do_val, #di, "=", di_val); \
				return true; \
			} \
			return false; \
		});

#define PRE_INTERLOCK_AI_LT(doo, do_val, ai, ai_val) \
		Data::doo.AddPrecheck([&](unsigned new_value) \
		{ \
			float ai_val1 = ai_val; \
			if(new_value == do_val && Data::ai < ai_val1) \
			{ \
				EVT::InterlockTrigger.Report(#doo, do_val, #ai, "<", ai_val1); \
				return true; \
			} \
			return false; \
		});

#define PRE_INTERLOCK_EX(doo, func) Data::doo.AddPrecheck(func);

#endif /* INTERLOCK_H_ */

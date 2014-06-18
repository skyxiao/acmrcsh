/*
 * Interlock.h
 *
 *  Created on: 2014��3��17��
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
};

#define DI_INTERLOCK_EX(di, func) Data::di.AddUpdateSink(func);

#define DI_INTERLOCK(di, di_val, doo, do_val) Data::di.AddUpdateSink([&](unsigned value) \
	{ \
		if(value == di_val) \
		{ Data::doo = do_val; } \
	});

#define DI_INTERLOCK_MEMBER(di, di_val, mem_func) Data::di.AddUpdateSink([this](unsigned value) \
	{ \
		if(value == di_val) \
		{ mem_func(); } \
	});

#define DO_INTERLOCK(doo, do_val, di, di_val) \
		Data::doo.AddPrecheck([&](unsigned new_value) \
		{ \
			if(new_value == do_val && Data::di == di_val) \
			{ \
				EVT::InterlockTrigger.Report(#doo, do_val, #di, "=", di_val); \
				return true; \
			} \
			return false; \
		});

#define DO_INTERLOCK_AI_LT(doo, do_val, ai, ai_val) \
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

#define DO_INTERLOCK_EX(doo, func) Data::doo.AddPrecheck(func);

#endif /* INTERLOCK_H_ */

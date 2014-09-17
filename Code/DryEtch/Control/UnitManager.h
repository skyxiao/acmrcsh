/*
 * UnitManager.h
 *
 *  Created on: 2014Äê4ÔÂ24ÈÕ
 *      Author: Administrator
 */

#ifndef UNITMANAGER_H_
#define UNITMANAGER_H_

#include <map>

#include "boost/shared_ptr.hpp"

#include "SmartUnit.h"
#include "Singleton.h"

class UnitManager : public SingletonT<UnitManager>
{
protected:
	UnitManager() = default;
public:
	virtual ~UnitManager() = default;

	void Initialize();
	void Terminate();

	void Invoke(int unit, unsigned cmd, unsigned param1, unsigned param2);
	void AlarmCheck();

	friend class SingletonT<UnitManager>;
private:
	std::map<int, boost::shared_ptr<SmartUnit>> m_units;
};

#endif /* UNITMANAGER_H_ */

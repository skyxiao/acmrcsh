/*
 * SystemParameter.h
 *
 *  Created on: 2014Äê3ÔÂ17ÈÕ
 *      Author: acm
 */

#ifndef SYSTEMPARAMETER_H_
#define SYSTEMPARAMETER_H_

#include "ParameterItem.h"

//template<typename T>
//class ParameterItem;

class SystemParameter : public SingletonT<SystemParameter>
{
protected:
	SystemParameter() = default;

	SystemParameter(const SystemParameter&) = delete;
	SystemParameter& operator = (const SystemParameter&) = delete;
public:
	~SystemParameter() = default;

	void SetData(int id, const std::string& data);
	std::string GetJsonData(boost::function<bool (ParameterItemBase*)> f);

	friend class SingletonT<SystemParameter>;
	friend class ParameterItemBase;

protected:
	void Mapping(int id, ParameterItemBase* cfg_ptr);

protected:
	std::map<int, ParameterItemBase*> m_cfgs;
};


#endif /* SYSTEMPARAMETER_H_ */

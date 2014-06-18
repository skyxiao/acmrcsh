/*
 * DeviceManager.h
 *
 *  Created on: 2014Äê3ÔÂ3ÈÕ
 *      Author: acm
 */

#ifndef DEVICEMANAGER_H_
#define DEVICEMANAGER_H_

#include "Device.h"
#include "Singleton.h"

class DeviceManager : public SingletonT<DeviceManager>
{
protected:
	DeviceManager() = default;
	DeviceManager& operator = (const DeviceManager&) = delete;
	DeviceManager(const DeviceManager&) = delete;
public:
	virtual ~DeviceManager() = default;

	void Initialize();
	void Terminate();

	boost::shared_ptr<Device> GetDevice(int id);

	friend class SingletonT<DeviceManager>;

private:
	std::map<int, boost::shared_ptr<Device>> m_devices;
};

#endif /* DEVICEMANAGER_H_ */

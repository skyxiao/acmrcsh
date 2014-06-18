/*
 * Singleton.h
 *
 *  Created on: 2014Äê3ÔÂ6ÈÕ
 *      Author: acm
 */

#ifndef SINGLETON_H_
#define SINGLETON_H_

#include "boost/scoped_ptr.hpp"
#include "boost/thread/mutex.hpp"

template<typename T>
class SingletonT
{
protected:
	SingletonT() = default;
	SingletonT(const SingletonT<T>&) = delete;
	SingletonT<T>& operator=(const SingletonT<T>&) = delete;

public:
	virtual ~SingletonT() = default;

	static T& Instance()
	{
		if(!instance_)
		{
			boost::mutex::scoped_lock lock(mutex_);
			if(!instance_)
			{
				instance_.reset(new T);
			}
		}

		return *instance_;
	}

protected:
	static boost::scoped_ptr<T> instance_;
	static boost::mutex mutex_;
};

template<typename T>
boost::scoped_ptr<T> SingletonT<T>::instance_;

template<typename T>
boost::mutex SingletonT<T>::mutex_;

#endif /* SINGLETON_H_ */

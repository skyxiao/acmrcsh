/*
 * SystemData.h
 *
 *  Created on: 2014Äê2ÔÂ25ÈÕ
 *      Author: acm
 */

#ifndef SYSTEMDATA_H_
#define SYSTEMDATA_H_

#include <string>
#include <sstream>
#include <typeinfo>
#include <iomanip>

#include "boost/property_tree/ptree.hpp"
#include "boost/function.hpp"
#include "boost/thread/mutex.hpp"

#include "DeviceManager.h"
#include "Worker.h"
#include "LogFile.h"
#include "Event.h"

const int NoRelatedDevice = -1;

//typedef boost::function<unsigned int (const std::vector<unsigned char>&)> ReadFunc;
//typedef boost::function<void (std::vector<unsigned char>&, unsigned int)> WriteFunc;
template<typename T>
T ConvertDefault(T data)
{
	return data;
}

template<typename T1, typename T2>
T1 ConvertMemery(T2 data)
{
	static_assert(sizeof(T1) == sizeof(T2), "The size of T1 and T2 are different in MemeryDataConvert.");

	return *((T1*)&data);
}

template<typename T1, typename T2>
T1 ConvertStatic(T2 data)
{
	return static_cast<T1>(data);
}

template<typename T>
T ConvertNot(T data)
{
	return data ? 0 : 1;
}

template<typename T1, typename T2>
class ConvertLinear
{
public:
	ConvertLinear(T2 input_low, T2 input_high, T1 output_low, T1 output_high) : m_input_low(input_low),
		m_input_high(input_high), m_output_low(output_low), m_output_high(output_high) {};

	T1 operator()(T2 input)
	{
		double delt = 1.0*(m_output_high - m_output_low)/(m_input_high - m_input_low);
		return (T1)((input*1.0 - m_input_low)*delt + m_output_low);
	}

private:
	T2 m_input_low;
	T2 m_input_high;
	T1 m_output_low;
	T1 m_output_high;
};

class BaseSystemData
{
public:
	BaseSystemData(int id, const std::string& name,	const std::string& path);
	virtual ~BaseSystemData() = default;

	virtual void Serialize(boost::property_tree::ptree& pt) = 0;
	virtual const std::type_info& Type() = 0;
	virtual void Initialize() = 0;
	virtual void Terminate() = 0;

	int GetID()
	{
		return m_id;
	}
	bool HasChanged();
	bool IsChild(const std::string& path);
	bool IsInRange(int from, int to);
	bool IsInSet(const std::set<int>& id_set);

protected:
	int m_id;
	std::string m_name;
	std::string m_path;
	bool m_changed_flag;
};

template<typename T>
class SystemData : public BaseSystemData
{
public:
	SystemData(int id, const std::string& name, const std::string& path, T min, T max, int precision, int device,
			unsigned int block, unsigned int byte_offset, unsigned short bit_offset,
			unsigned short bits, bool writable, const std::string& unit = "",
			boost::function<T (unsigned int)> raw_to_real = ConvertDefault<unsigned int>,
			boost::function<unsigned int (T)> real_to_raw = ConvertDefault<unsigned int>)
	: BaseSystemData(id, name, path), m_unit(unit), m_min(min), m_max(max), m_precision(precision), m_device(device),
	  m_block(block), m_byte_offset(byte_offset), m_bit_offset(bit_offset), m_bits(bits), m_writable(writable), m_data(T()),
	  m_token(0), m_raw_to_real(raw_to_real), m_real_to_raw(real_to_raw){};

	SystemData(int id, const std::string& name, const std::string& path, T min, T max, int precision =0, const std::string& unit = "")
	: SystemData(id, name, path, min, max, precision, NoRelatedDevice, 0, 0, 0, 0, true, unit){};

	virtual ~SystemData(){};

	operator T()
	{
		return m_data;
	}

	SystemData& operator = (const T& rhs)
	{
		if(!m_writable)
		{
			std::stringstream ss;
			ss<<m_name<<" can't be written.";
			LogWarning(ss.str());
			return *this;
		}

		if(rhs<=m_max && rhs>=m_min)
		{
			if(rhs != m_data)
			{
				boost::mutex::scoped_lock lock(m_mtx);
				for(boost::function<bool (T)> &f : m_precheck_funcs)
				{
					if(f(rhs))
						return *this;
				}

				T old_data = m_data;
				m_data = rhs;
				m_changed_flag = true;

				for(boost::function<void (T, T)>& f : m_update_sinks)
				{
					if(f)
						Worker::Instance().Post([this, f, old_data](){f(old_data, this->m_data);});
				}

				std::stringstream ss;
				ss<<"system data is modified, [id="<<m_id<<", value="<<rhs<<"].";
				LogDebug(ss.str());
			}
			else
			{
				return *this;
			}
		}
		else
		{
			EVT::ValueOutOfRange.Report(rhs, m_min, m_max, m_name);

			return *this;
		}

		WriteData();

		return *this;
	}

	virtual void Serialize(boost::property_tree::ptree& pt)
	{
		using namespace boost::property_tree;
		ptree node;
		node.add<int>("id", m_id);
		{
			std::stringstream ss;
			ss<<std::fixed<<std::setfill('0')<<std::setprecision(m_precision)<<m_data;
			boost::mutex::scoped_lock lock(m_mtx);
			node.add("value", ss.str());
			m_changed_flag = false;
		}
		pt.push_back(make_pair("", node));
	}

	virtual const std::type_info& Type()
	{
		return typeid(T);
	}

	virtual void Initialize()
	{
		if(m_device == NoRelatedDevice)
			return;

		boost::shared_ptr<Device> dev_ptr = DeviceManager::Instance().GetDevice(m_device);
		if(dev_ptr)
		{
			m_get_func = [dev_ptr, this](){return dev_ptr->Read(m_block, m_byte_offset, m_bit_offset, m_bits);};
			ReadData();
			if(m_writable)
				m_put_func = [dev_ptr, this](unsigned value){dev_ptr->Write(value, m_block, m_byte_offset, m_bit_offset, m_bits);};
			else
				m_token = dev_ptr->Follow(m_block, [this](){ReadData();});
		}
		else
		{
			std::stringstream ss;
			ss<<"No device related with system data[id="<<m_id<<", name="<<m_name<<", device="<<m_device<<"].";
			LogDebug(ss.str());
		}
	}

	virtual void Terminate()
	{
		if(m_id == NoRelatedDevice)
			return;

		if(m_writable)
			m_put_func.clear();
		else
		{
			boost::shared_ptr<Device> dev_ptr = DeviceManager::Instance().GetDevice(m_device);
			dev_ptr->Unfollow(m_block, m_token);
			m_precheck_funcs.clear();
			m_update_sinks.clear();
		}
	}

	void AddPrecheck(boost::function<bool (T)> checker)
	{
		boost::mutex::scoped_lock lock(m_mtx);
		m_precheck_funcs.push_back(checker);
	}

	unsigned int AddUpdateSink(boost::function<void (T, T)> sink)
	{
		boost::mutex::scoped_lock lock(m_mtx);
		unsigned int i=0;
		for(boost::function<void (T, T)>& v : m_update_sinks)
		{
			if(v)
			{
				++i;
			}
			else
			{
				v = sink;
				break;
			}
		}
		if(i == m_update_sinks.size())
			m_update_sinks.push_back(sink);

		return i;
	}

	void RemoveUpdateSink(unsigned int token)
	{
		boost::mutex::scoped_lock lock(m_mtx);
		if(token >= m_update_sinks.size())
			return;

		m_update_sinks[token] = nullptr;
	}

private:
	void WriteData()
	{
		if(m_put_func)
		{
			unsigned int raw = 0;
			raw = m_real_to_raw(m_data);

			m_put_func(raw);
		}
	}

	void ReadData()
	{
		unsigned int raw = 0;
		if(m_get_func)
			raw = m_get_func();

		boost::mutex::scoped_lock lock(m_mtx);
		T old_data = m_data;
		m_data = m_raw_to_real(raw);
		if(m_data != old_data)
			m_changed_flag = true;

		for(boost::function<void (T, T)>& f : m_update_sinks)
		{
			if(f)
				Worker::Instance().Post([this, f, old_data](){f(old_data, this->m_data);});
		}
	}

private:
	std::string m_unit;
	T m_min;
	T m_max;
	int m_precision;
	int m_device;
	unsigned int m_block;
	unsigned int m_byte_offset;
	unsigned short m_bit_offset;
	unsigned short m_bits;
	bool m_writable;
	T m_data;
	unsigned int m_token;
	boost::function<T (unsigned int)> m_raw_to_real;
	boost::function<unsigned int (T)> m_real_to_raw;

	boost::function<unsigned int ()> m_get_func;
	boost::function<void (unsigned int)> m_put_func;

	std::vector<boost::function<bool (T)>> m_precheck_funcs;
	std::vector<boost::function<void (T, T)>> m_update_sinks;

	boost::mutex m_mtx;
};

template<>
class SystemData<std::string> : public BaseSystemData
{
public:
	SystemData(int id, const std::string& name, const std::string& path)
	: BaseSystemData(id, name, path){};

	virtual ~SystemData(){};

	operator std::string()
	{
		return m_data;
	}

	SystemData& operator = (const std::string& rhs)
	{
		boost::mutex::scoped_lock lock(m_mtx);
		if(m_data != rhs)
		{
			m_data = rhs;
			m_changed_flag = true;

			std::stringstream ss;
			ss<<"system data is modified, [id="<<m_id<<", value="<<rhs<<"].";
			LogDebug(ss.str());
		}
		return *this;
	}

	virtual void Serialize(boost::property_tree::ptree& pt)
	{
		using namespace boost::property_tree;
		ptree node;
		node.add<int>("id", m_id);
		{
			boost::mutex::scoped_lock lock(m_mtx);
			node.add("value", m_data);
			m_changed_flag = false;
		}
		pt.push_back(make_pair("", node));
	}

	virtual const std::type_info& Type()
	{
		return typeid(std::string);
	}

	virtual void Initialize()
	{

	}

	virtual void Terminate()
	{

	}

private:
	std::string m_data;
	boost::mutex m_mtx;
};


#endif /* SYSTEMDATA_H_ */

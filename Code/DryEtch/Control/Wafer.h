/*
 * Wafer.h
 *
 *  Created on: 2014Äê2ÔÂ21ÈÕ
 *      Author: acm
 */

#ifndef WAFER_H_
#define WAFER_H_

#include <list>

#include "boost/chrono.hpp"

enum WaferState : unsigned
{
	WaferState_Unprocessed,
	WaferState_Processing,
	WaferState_Processed,
	WaferState_Semi_Processed,
	WaferState_Broken,
};

enum WaferType
{
	WaferType_Product,
	WaferType_Dummy,
};

enum WaferSize
{
	WaferSize_100mm,
	WaferSize_150mm,
	WaferSize_200mm,
	WaferSize_300mm,
};

class wafer_info_convertor
{
public:
	static std::string wafer_type_to_string(WaferType type);
	static std::string wafer_size_to_string(WaferSize size);
	static std::string wafer_state_to_string(WaferState state);

	static WaferType wafer_type_from_string(const std::string& type);
	static WaferSize wafer_size_from_string(const std::string& size);
	static WaferState wafer_state_from_string(const std::string& state);
};

typedef std::pair<int, unsigned short> WaferSite;
typedef boost::chrono::time_point<boost::chrono::system_clock> TimePoint;

class Wafer
{
public:
	Wafer(const std::string& id, int unit, unsigned short slot, WaferSize size,
			WaferType type = WaferType_Product, WaferState state = WaferState_Unprocessed);
	virtual ~Wafer();

	WaferSite GetOriginalSite();
//	WaferSite GetSite();
	WaferType GetType();
	WaferSize GetSize();
	WaferState GetState();
	std::string GetID();

	void Transfer(int dest_unit, unsigned short dest_slot = 0);
	void ProcessStart(const std::string& recipe);
	void ProcessEnd();
	void ProcessAbort();
	void Broken();

	void SetRoute(const std::list<WaferSite>& m_route);
	bool HasRoute();
	WaferSite GetNextSite();

	bool IsProcessed();
	bool IsUnprocessed();
	bool IsDummy();

private:
	std::string m_id;
	int m_original_unit;
	unsigned short m_original_slot;
	int m_unit;
	unsigned short m_slot;
	WaferSize m_size;
	WaferType m_type;
	WaferState m_state;
	std::string m_recipe;
	TimePoint m_process_start_time;
	TimePoint m_process_end_time;
	std::list<WaferSite> m_route;
};

#endif /* WAFER_H_ */

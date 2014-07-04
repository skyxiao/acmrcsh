/*
 * WaferManager.cpp
 *
 *  Created on: 2014Äê2ÔÂ24ÈÕ
 *      Author: acm
 */

#include <iomanip>
#include <sstream>

#include "boost/date_time.hpp"
#include "boost/algorithm/string.hpp"

#include "WaferManager.h"
#include "Data.h"
#include "LogFile.h"
#include "Database.h"

void WaferManager::Initialize()
{
	auto loaded_wafers = Database::Instance().QueryLoadedWafers();
	for(auto& wafer_info_ptr : loaded_wafers)
	{
		std::string id = wafer_info_ptr->id;
		int unit = boost::lexical_cast<int>(wafer_info_ptr->unit);
		unsigned short slot = boost::lexical_cast<unsigned short>(wafer_info_ptr->slot);
		WaferSize size = wafer_info_convertor::wafer_size_from_string(wafer_info_ptr->size);
		WaferType type = wafer_info_convertor::wafer_type_from_string(wafer_info_ptr->type);
		WaferState state = wafer_info_convertor::wafer_state_from_string(wafer_info_ptr->state);
		m_wafers[unit*100+slot] = boost::shared_ptr<Wafer>(new Wafer(id, unit, slot, size, type, state));
	}
	loaded_wafers.clear();

	update_wafer_info(0, 0x7);
}

void WaferManager::Terminate()
{
	m_wafers.clear();
	LogDebug("WaferManager is terminated.");
}

void WaferManager::CreateWafer(int unit, unsigned short slot, const std::string& batch_id, WaferSize size, WaferType type)
{
	int index = unit*100 + slot;
	boost::mutex::scoped_lock lock(m_mtx);
	std::string wafer_id = generate_wafer_id(batch_id);
	m_wafers[index] = boost::shared_ptr<Wafer>(new Wafer(wafer_id, unit, slot, size, type));
	update_wafer_info(unit, 0x1<<slot);
}

//void WaferManager::CreateWafer(int unit, const std::string& casset_id, WaferSize size, WaferType type, unsigned int mapping)
//{
//	mapping = mapping & DEFAULT_FOUP_MAPPING;
//
//	boost::mutex::scoped_lock lock(m_mtx);
//	for(unsigned short i=0; i<MAX_SLOT_NUM; i++)
//	{
//		int index = unit*100 + i;
//		bool flag = mapping & 0x1;
//		if(flag)
//		{
//			m_wafers[index] = boost::shared_ptr<Wafer>(new Wafer(generate_wafer_id(casset_id), unit, i, size, type));
//		}
//		mapping = mapping>>1;
//		if(mapping == 0)
//			return;
//	}
//}

//void WaferManager::RemoveWafer(int unit, unsigned int mapping)
//{
//	mapping = mapping & DEFAULT_FOUP_MAPPING;
//
//	boost::mutex::scoped_lock lock(m_mtx);
//	for(int i=0; i<MAX_SLOT_NUM; i++)
//	{
//		int index = unit*100 + i;
//		bool flag = mapping & 0x1;
//		if(flag)
//		{
//			m_wafers[index].reset();
//		}
//		mapping = mapping>>1;
//		if(mapping == 0)
//			return;
//	}
//}

void WaferManager::RemoveWafer(int unit, unsigned short slot)
{
	int index = unit*100 + slot;

	{
		boost::mutex::scoped_lock lock(m_mtx);
		m_wafers[index].reset();
	}
	update_wafer_info(unit, 0x1<<slot);
}

void WaferManager::Transfer(int src_unit, unsigned short src_slot, int dest_unit, unsigned short dest_slot)
{
	int dest_index = dest_unit*100 + dest_slot;
	int src_index = src_unit*100 + src_slot;

	boost::mutex::scoped_lock lock(m_mtx);
	auto wf_ptr = m_wafers[src_index];
	m_wafers[dest_index] = wf_ptr;
	m_wafers[src_index].reset();
	if(wf_ptr)
		wf_ptr->Transfer(dest_unit, dest_slot);
	update_wafer_info(src_unit, 0x1<<src_slot);
	update_wafer_info(dest_unit, 0x1<<dest_slot);
}

bool WaferManager::HasWafer(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;
	boost::shared_ptr<Wafer> wf = m_wafers[index];

	return wf ? true : false;
}

WaferSite WaferManager::GetOriginalSite(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	return m_wafers[index]->GetOriginalSite();
}

WaferType WaferManager::GetType(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	return m_wafers[index]->GetType();
}

WaferSize WaferManager::GetSize(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	return m_wafers[index]->GetSize();
}

void WaferManager::ProcessStart(int unit, const std::string& recipe)
{
	boost::mutex::scoped_lock lock(m_mtx);

	for(int i=0; i<3; i++)
	{
		int index = unit*100 + i;
		auto wf_ptr = m_wafers[index];
		if(wf_ptr)
			wf_ptr->ProcessStart(recipe);
	}
	update_wafer_info(unit, 0x7);
}

void WaferManager::ProcessEnd(int unit)
{
	boost::mutex::scoped_lock lock(m_mtx);
	for(int i=0; i<3; i++)
	{
		int index = unit*100 + i;
		auto wf_ptr = m_wafers[index];
		if(wf_ptr)
			wf_ptr->ProcessEnd();
	}
	update_wafer_info(unit, 0x7);
}

void WaferManager::ProcessAbort(int unit)
{
	boost::mutex::scoped_lock lock(m_mtx);
	for(int i=0; i<3; i++)
	{
		int index = unit*100 + i;
		auto wf_ptr = m_wafers[index];
		if(wf_ptr)
			wf_ptr->ProcessAbort();
	}
	update_wafer_info(unit, 0x7);
}

void WaferManager::WaferBroken(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	boost::shared_ptr<Wafer> wf = m_wafers[index];
	if(wf)
	{
		wf->Broken();
//		WaferSite site = wf->GetOriginalSite();
//		int orig_index = site.first*100 + site.second;
//		m_wafers[orig_index] = wf;
//		m_wafers[index].reset();
		wf = nullptr;
	}
	update_wafer_info(unit, 0x1<<slot);
}

void WaferManager::SetRoute(int unit, unsigned short slot, const std::list<WaferSite>& route)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	m_wafers[index]->SetRoute(route);
}

bool WaferManager::HasRoute(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	return m_wafers[index]->HasRoute();
}

WaferSite WaferManager::GetNextSite(int unit, unsigned short slot)
{
	boost::mutex::scoped_lock lock(m_mtx);
	int index = unit*100 + slot;

	return m_wafers[index]->GetNextSite();
}

bool WaferManager::IsWaferAllProcessed(int unit, unsigned int mapping)
{
	mapping = mapping & DEFAULT_FOUP_MAPPING;

	boost::mutex::scoped_lock lock(m_mtx);
	boost::shared_ptr<Wafer> wf;
	for(int i=0; i<MAX_SLOT_NUM; i++)
	{
		bool flag = mapping & 0x1;
		if(flag)
		{
			int index = unit*100 + i;
			wf = m_wafers[index];
			if(wf && !wf->IsProcessed())
				return false;
		}
		mapping = mapping>>1;
		if(mapping == 0)
			break;
	}

	return true;
}

bool WaferManager::HasWaferUnprocessed(int unit, unsigned int mapping)
{
	mapping = mapping & DEFAULT_FOUP_MAPPING;

	boost::mutex::scoped_lock lock(m_mtx);
	boost::shared_ptr<Wafer> wf;
	for(int i=0; i<MAX_SLOT_NUM; i++)
	{
		bool flag = mapping & 0x1;
		if(flag)
		{
			int index = unit*100 + i;
			wf = m_wafers[index];
			if(wf && wf->IsUnprocessed())
				return true;
		}
		mapping = mapping>>1;
		if(mapping == 0)
			break;
	}

	return false;
}

bool WaferManager::UnitHasWafer(int unit, unsigned int mapping)
{
	boost::mutex::scoped_lock lock(m_mtx);
	boost::shared_ptr<Wafer> wf;
	for(int i=0; i<MAX_SLOT_NUM; i++)
	{
		bool flag = mapping & 0x1;
		if(flag)
		{
			int index = unit*100 + i;
			wf = m_wafers[index];
			if(wf)
				return true;
		}
		mapping = mapping>>1;
		if(mapping == 0)
			break;
	}

	return false;
}

unsigned short WaferManager::GetUnprocessedWaferSlot(int unit, unsigned int mapping)
{
	mapping = mapping & DEFAULT_FOUP_MAPPING;

	boost::mutex::scoped_lock lock(m_mtx);
	boost::shared_ptr<Wafer> wf;
	for(unsigned short i=0; i<MAX_SLOT_NUM; i++)
	{
		bool flag = mapping & 0x1;
		if(flag)
		{
			int index = unit*100 + i;
			wf = m_wafers[index];
			if(wf && wf->IsUnprocessed())
				return i;
		}
		mapping = mapping>>1;
		if(mapping == 0)
			break;
	}

	return MAX_SLOT_NUM;
}

std::string WaferManager::generate_wafer_id(const std::string& casset_id)
{
	if(!boost::algorithm::equals(casset_id, m_last_batch_id))
	{
		m_last_batch_id = casset_id;
		Data::WaferBatchCount = 0;
	}

	Data::WaferBatchCount = Data::WaferBatchCount + 1;
	Data::WaferTotalCount = Data::WaferTotalCount + 1;

	using namespace boost::posix_time;
	ptime t = second_clock::local_time();
	std::stringstream ss;
	ss<<casset_id<<"-"<<to_iso_string(t).substr(9, 6)<<"-"<<std::setw(2)<<std::setfill('0')<<Data::WaferBatchCount;


	return ss.str();
}

void WaferManager::update_wafer_info(int unit, unsigned mapping)
{
	if(unit == 1)
	{
		int index = unit*100;
		auto wf = m_wafers[index];

		if(wf)
		{
			Data::ForkWaferID = wf->GetID();
			Data::ForkWaferState = wf->GetState() + 1;
		}
		else
		{
			Data::ForkWaferID = "";
			Data::ForkWaferState = 0;
		}

		return;
	}

	SystemData<std::string>* wafer_id_ptr[3] = {&Data::Slot1WaferID, &Data::Slot2WaferID, &Data::Slot3WaferID};
	SystemData<unsigned>* wafer_state_ptr[3] = {&Data::Slot1WaferState, &Data::Slot2WaferState, &Data::Slot3WaferState};

	for(unsigned short i=0; i<3; i++)
	{
		bool flag = mapping & 0x1;
		if(flag)
		{
			int index = unit*100 + i;
			auto wf = m_wafers[index];
			if(wf)
			{
				*wafer_id_ptr[i] = wf->GetID();
				*wafer_state_ptr[i] = wf->GetState()+1;
			}
			else
			{
				*wafer_id_ptr[i] = "";
				*wafer_state_ptr[i] = 0;
			}
		}
		mapping = mapping>>1;
		if(mapping == 0)
			break;
	}
}

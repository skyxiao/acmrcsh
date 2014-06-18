/*
 * AdsBlock.cpp
 *
 *  Created on: 2014Äê4ÔÂ9ÈÕ
 *      Author: Administrator
 */

#include <sstream>
#include <algorithm>

#include "boost/regex.hpp"
#include "boost/lexical_cast.hpp"

#include "AdsBlock.h"
#include "LogFile.h"

static unsigned int bits_mask[32] =
{
		0x1,
		0x3,
		0x7,
		0xF,
		0x1F,
		0x3F,
		0x7F,
		0xFF,
		0x1FF,
		0x3FF,
		0x7FF,
		0xFFF,
		0x1FFF,
		0x3FFF,
		0x7FFF,
		0xFFFF,
		0x1FFFF,
		0x3FFFF,
		0x7FFFF,
		0xFFFFF,
		0x1FFFFF,
		0x3FFFFF,
		0x7FFFFF,
		0xFFFFFF,
		0x1FFFFFF,
		0x3FFFFFF,
		0x7FFFFFF,
		0xFFFFFFF,
		0x1FFFFFFF,
		0x3FFFFFFF,
		0x7FFFFFFF,
		0xFFFFFFFF
};

AdsBlock::AdsBlock(unsigned id, const std::string& name, const std::string& var_name, const std::string& ip,
		unsigned short port, unsigned short read_start, unsigned short read_end,
		unsigned short write_start, unsigned short write_end) :
		Block(id), m_name(name), m_var_name(var_name), m_read_start(read_start), m_read_end(read_end),
		m_write_start(write_start), m_write_end(write_end), m_handle(0), m_dirty(false)
{
	if(read_end<read_start || write_end<write_start)
	{
		std::stringstream ss;
		ss<<"Ads configuration is incorrect in block id="<<id;
		LogFatal(ss.str());
		throw ss.str();
	}
	convert_address(ip, port);
	unsigned long block_size = (m_read_end - m_read_start) + (m_write_end - m_write_start);
	m_comm_buf.reset(new unsigned char[block_size]);
}

AdsBlock::~AdsBlock()
{
}

void AdsBlock::Initialize()
{
	long rtv = AdsSyncReadWriteReq(&m_addr, ADSIGRP_SYM_HNDBYNAME, 0x0, sizeof(m_handle), &m_handle,
			m_var_name.length(), &(*(m_var_name.begin())));
	if(rtv)
	{
		std::stringstream ss;
		ss<<"Failed to get handle of block ["<<m_id<<", "<<m_name<<"].";
		LogFatal(ss.str());
		throw ss.str();
	}

	unsigned long read_size = (m_read_end - m_read_start) + (m_write_end - m_write_start);
	rtv = AdsSyncReadReq(&m_addr, ADSIGRP_SYM_VALBYHND, m_handle, read_size, m_buffer);
	if(rtv)
	{
		std::stringstream ss;
		ss<<"Block ["<<m_id<<", "<<m_name<<"] initialize failed.";
		LogFatal(ss.str());
		throw ss.str();
	}
}

void AdsBlock::Terminate()
{
}

void AdsBlock::Sync()
{
	unsigned read_size = m_read_end - m_read_start;
	unsigned write_size = m_write_end - m_write_start;
	unsigned block_size = read_size + write_size;
	if(read_size > 0)
	{
		long rtv = AdsSyncReadReq(&m_addr, ADSIGRP_SYM_VALBYHND, m_handle, block_size, &m_comm_buf[0]);
		if(rtv)
		{
			//write log
		}
	}

	bool write_flag = false;
	{
		boost::recursive_mutex::scoped_lock lock(m_mtx);
		if(read_size > 0)
		{
			std::copy(&m_comm_buf[m_read_start], &m_comm_buf[m_read_end],
					m_buffer+m_read_start);
		}
		if(write_size > 0 && m_dirty)
		{
			std::copy(m_buffer+m_write_start, m_buffer+m_write_end, &m_comm_buf[m_write_start]);
			write_flag = true;
			m_dirty = false;
		}
	}

	if(write_flag)
	{
		long rtv = AdsSyncWriteReq(&m_addr, ADSIGRP_SYM_VALBYHND, m_handle, block_size,	&m_comm_buf[0]);
		if(rtv)
		{
			//write log
		}
	}

	Block::Sync();
}

void AdsBlock::Write(unsigned int value, unsigned byte_offset,
		unsigned bit_offset, unsigned bits)
{
	unsigned int mask = (bits_mask[bits-1]<<bit_offset);
	value <<= bit_offset;
	value &= mask;
	{
		unsigned int rtv;
		boost::recursive_mutex::scoped_lock lock(m_mtx);
		memcpy(&rtv, m_buffer+byte_offset, sizeof(unsigned int));
		rtv &= ~mask;
		rtv |= value;
		memcpy(m_buffer+byte_offset, &rtv, sizeof(unsigned int));
		m_dirty = true;
	}
}

unsigned int AdsBlock::Read(unsigned byte_offset, unsigned bit_offset,
		unsigned bits)
{
	unsigned int rtv;
	{
		boost::recursive_mutex::scoped_lock lock(m_mtx);
		memcpy(&rtv, m_buffer+byte_offset, sizeof(unsigned int));
	}

	rtv >>= bit_offset;
	rtv &= bits_mask[bits-1];

	return rtv;
}

void AdsBlock::convert_address(const std::string& ip, unsigned short port)
{
	boost::regex reg("^(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})$");
	boost::cmatch mat;
	bool r = boost::regex_match(ip.c_str(), mat, reg);
	if(r)
	{
		int i=0;
		for (auto itor = ++mat.begin(); itor != mat.end(); ++itor)
		{
			m_addr.netId.b[i++] = boost::lexical_cast<unsigned short>(*itor);
		}
		m_addr.port = port;
	}
	else
	{
		std::stringstream ss;
		ss<<"IP address ("<<ip<<") can't be parsed for block ["<<m_id<<", "<<m_name<<"].";
		LogFatal(ss.str());
		throw ss.str();
	}
}

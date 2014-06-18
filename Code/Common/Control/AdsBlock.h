/*
 * AdsBlock.h
 *
 *  Created on: 2014Äê4ÔÂ9ÈÕ
 *      Author: Administrator
 */

#ifndef ADSBLOCK_H_
#define ADSBLOCK_H_

#include "ads/TcAdsDef.h"
#include "ads/TcAdsAPI.h"

#include "Block.h"

#define BUFFER_SIZE 4*1024


class AdsBlock : public Block
{
public:
	AdsBlock(unsigned id, const std::string& name, const std::string& var_name, const std::string& ip,
			unsigned short port, unsigned short read_start,	unsigned short read_end,
			unsigned short write_start, unsigned short write_end);
	~AdsBlock();

	virtual void Initialize();
	virtual void Terminate();
	virtual void Sync();
	virtual void Write(unsigned int value, unsigned byte_offset, unsigned bit_offset, unsigned bits);
	virtual unsigned int Read(unsigned byte_offset, unsigned bit_offset, unsigned bits);

private:
	void convert_address(const std::string& str, unsigned short port);

private:
	std::string m_name;
	std::string m_var_name;
	unsigned short m_read_start;
	unsigned short m_read_end;
	unsigned short m_write_start;
	unsigned short m_write_end;

	unsigned char m_buffer[BUFFER_SIZE];
	boost::scoped_array<unsigned char> m_comm_buf;
	AmsAddr m_addr;
	unsigned long m_handle;
	bool m_dirty;
};


#endif /* ADSBLOCK_H_ */

/*
 * Block.h
 *
 *  Created on: 2014Äê4ÔÂ10ÈÕ
 *      Author: Administrator
 */

#ifndef BLOCK_H_
#define BLOCK_H_

#include <vector>

#include "boost/function.hpp"
#include "boost/thread/recursive_mutex.hpp"

class Block
{
public:
	Block(unsigned id) : m_id(id) {};
	virtual ~Block() = default;

	virtual void Initialize() = 0;
	virtual void Terminate() = 0;
	virtual void Sync();
	virtual void Write(unsigned int value, unsigned byte_offset, unsigned bit_offset, unsigned bits) = 0;
	virtual unsigned int Read(unsigned byte_offset, unsigned bit_offset, unsigned bits) = 0;
	virtual unsigned int Follow(boost::function<void ()> f);
	virtual void Unfollow(unsigned token);

protected:
	void Notify();

protected:
	unsigned m_id;
	boost::recursive_mutex m_mtx;
	std::vector<boost::function<void ()>> m_follower;
};

#endif /* BLOCK_H_ */

/*
 * Block.cpp
 *
 *  Created on: 2014Äê4ÔÂ10ÈÕ
 *      Author: Administrator
 */

#include "Block.h"


void Block::Sync()
{
	Notify();
}

unsigned Block::Follow(boost::function<void ()> f)
{
	boost::recursive_mutex::scoped_lock lock(m_mtx);
	unsigned token = m_follower.size();
	m_follower.push_back(f);

	return token;
}

void Block::Unfollow(unsigned token)
{
	boost::recursive_mutex::scoped_lock lock(m_mtx);

	if(token >= m_follower.size())
		return;

	m_follower[token].clear();
}

void Block::Notify()
{
	boost::recursive_mutex::scoped_lock lock(m_mtx);

	for(auto& f : m_follower)
	{
		if(f)
		{
			f();
		}
	}
}

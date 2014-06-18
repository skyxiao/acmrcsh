/*
 * EventLevel.h
 *
 *  Created on: 2014Äê3ÔÂ26ÈÕ
 *      Author: acm
 */

#ifndef EVENTLEVEL_H_
#define EVENTLEVEL_H_


enum EventLevel : unsigned int
{
	EVENT_LEVEL_INFO		= 0x01,
	EVENT_LEVEL_WARNING		= 0x02,
	EVENT_LEVEL_ERROR		= 0x04,
	EVENT_LEVEL_FATAL		= 0x08,
};


std::string event_level_to_string(EventLevel level);

#endif /* EVENTLEVEL_H_ */

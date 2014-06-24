/*
 * Event.cpp
 *
 *  Created on: 2014Äê2ÔÂ20ÈÕ
 *      Author: acm
 */

#include "SystemEvent.h"

namespace EVT
{
	SystemEvent GenericInfo(1001, EVENT_LEVEL_INFO, "%s");
	SystemEvent GenericWarning(1002, EVENT_LEVEL_WARNING, "%s");
	SystemEvent GenericError(1003, EVENT_LEVEL_ERROR, "%s");

	SystemEvent ValueOutOfRange(1004, EVENT_LEVEL_WARNING, "New value %f is out of range [%f, %f] for %s.");
	SystemEvent LoadRecipeFailed(1006, EVENT_LEVEL_WARNING, "Failed to load recipe %s.");
	SystemEvent PinTimeout(1008, EVENT_LEVEL_ERROR, "Pin %s timeout.");
	SystemEvent TurnOnHeaterTimeout(1010, EVENT_LEVEL_ERROR, "Turn on heater timeout.");
	SystemEvent RotateTimeout(1012, EVENT_LEVEL_ERROR, "Rotate to %f degrees timeout.");
	SystemEvent HomingTimeout(1013, EVENT_LEVEL_ERROR, "Homing chuck timeout.");
	SystemEvent GateValveTimeout(1014, EVENT_LEVEL_ERROR, "Open/Close gate valve timeout.");
	SystemEvent PumpTimeout(1016, EVENT_LEVEL_ERROR, "Pumping timeout from %f (torr) to %f (torr).");
	SystemEvent PumpError(1018, EVENT_LEVEL_ERROR, "Pump warning/alarm.");
	SystemEvent UnknownWaferSize(1020, EVENT_LEVEL_ERROR, "Wafer size is unknown, wafer can't be created.");
	SystemEvent LoadUnloadTimeout(1022, EVENT_LEVEL_ERROR, "Load/unload wafer timeout.");
	SystemEvent VentTimeout(1024, EVENT_LEVEL_ERROR, "Vent %s chamber timeout.");
	SystemEvent ChamberNotClean(1026, EVENT_LEVEL_WARNING, "%s chamber is not clean, please purge it before vent.");
	SystemEvent ChamberNotVacuum(1028, EVENT_LEVEL_WARNING, "%s chamber isn't vacuum, can't process wafer.");
	SystemEvent ProcessAlarm(1030, EVENT_LEVEL_WARNING, "Alarm reported in process.");
	SystemEvent InterlockTrigger(1032, EVENT_LEVEL_WARNING, "%s=%d trigger interlock for %s%s%d.");
	SystemEvent ArmTimeout(1037, EVENT_LEVEL_ERROR, "Arm move %s timeout.");
	SystemEvent ForkTimeout(1038, EVENT_LEVEL_ERROR, "Rotate fork timeout.");
	SystemEvent HeaterTempOutRange(1040, EVENT_LEVEL_WARNING, "Heater temperature is out of range.");

}



#pragma once

#include "SystemEvent.h"

namespace EVT
{

extern SystemEvent GenericInfo;
extern SystemEvent GenericWarning;
extern SystemEvent GenericError;

extern SystemEvent ValueOutOfRange;
extern SystemEvent LoadRecipeFailed;
extern SystemEvent PinTimeout;
extern SystemEvent TurnOnHeaterTimeout;
extern SystemEvent RotateTimeout;
extern SystemEvent HomingTimeout;
extern SystemEvent GateValveTimeout;
extern SystemEvent PumpTimeout;
extern SystemEvent PumpError;
extern SystemEvent UnknownWaferSize;
extern SystemEvent LoadUnloadTimeout;
extern SystemEvent VentTimeout;
extern SystemEvent ChamberNotClean;
extern SystemEvent ChamberNotVacuum;
extern SystemEvent ProcessAlarm;
extern SystemEvent InterlockTrigger;
extern SystemEvent ArmTimeout;
extern SystemEvent ForkTimeout;
extern SystemEvent HeaterTempOutRange;
extern SystemEvent MonitorWarning;
extern SystemEvent MonitorAlarm;

}

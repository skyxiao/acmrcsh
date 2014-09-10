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
extern SystemEvent HeaterTempOutWarnRange;
extern SystemEvent HeaterTempOutAlarmRange;
extern SystemEvent MonitorWarning;
extern SystemEvent MonitorAlarm;
extern SystemEvent OverTemp;
extern SystemEvent AlcGasPressureLow;
extern SystemEvent N2FacPressureHigh;
extern SystemEvent N2FacPressureLow;
extern SystemEvent FacVacuumPressureLow;
extern SystemEvent ExpVacuumPressureLow;
extern SystemEvent VapMainHeaterAlarm;
extern SystemEvent VapVapHeaterAlarm;
extern SystemEvent HeartbeatFail;
extern SystemEvent GasboxExhaustPresAlarm;
extern SystemEvent FacMainInletVacPresHigh;
extern SystemEvent AlcoholLeak;
extern SystemEvent AlcoholGasLeak;
extern SystemEvent FrameExhaustPresAlarm;
extern SystemEvent GasboxDoorOpen;
extern SystemEvent GasboxHFLeak;
extern SystemEvent HardwareInterlock;
extern SystemEvent AlcoholTankLow;
extern SystemEvent PumpWarning;
extern SystemEvent PumpAlarm;
extern SystemEvent PlumbingHeaterAlarm;
extern SystemEvent ChamberHFLeak;
extern SystemEvent ArmVacuumDoorEStop;
extern SystemEvent CDAPressureLow;
extern SystemEvent ProcChamberPressHigh;
extern SystemEvent ExpChamberDirty;
extern SystemEvent ProcChamberDirty;
extern SystemEvent ProcDoorLockHFInletValve;

}

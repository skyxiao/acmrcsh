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
	SystemEvent MonitorWarning(1043, EVENT_LEVEL_WARNING, "%f is out of warning range [%f, %f] of %s.");
	SystemEvent MonitorAlarm(1045, EVENT_LEVEL_ERROR, "%f is out of alarm range [%f, %f] of %s.");
	SystemEvent OverTemp(1046, EVENT_LEVEL_WARNING, "%s over temperature, current temperature is %f ¡æ.");

	SystemEvent AlcGasPressureLow(1050, EVENT_LEVEL_WARNING, "Alcohol gas pressure is below lower limit.");
	SystemEvent N2FacPressureHigh(1051, EVENT_LEVEL_WARNING, "N2 facility supply pressure is over upper limit.");
	SystemEvent N2FacPressureLow(1052, EVENT_LEVEL_WARNING, "N2 facility supply pressure is below lower limit.");
	SystemEvent FacVacuumPressureLow(1053, EVENT_LEVEL_WARNING, "Facility vacuum pressure is below lower limit.");
	SystemEvent ExpVacuumPressureLow(1054, EVENT_LEVEL_WARNING, "Expansion chamber vacuum pressure is below lower limit.");
	SystemEvent VapMainHeaterAlarm(1055, EVENT_LEVEL_ERROR, "Vaporizer main heater alarm.");
	SystemEvent VapVapHeaterAlarm(1056, EVENT_LEVEL_ERROR, "Vaporizer vapor heater alarm.");
	SystemEvent HeartbeatFail(1057, EVENT_LEVEL_ERROR, "Heartbeat failed.");
	SystemEvent GasboxExhaustPresAlarm(1058, EVENT_LEVEL_ERROR, "Gasbox exhaust pressure alarm.");
	SystemEvent FacMainInletVacPresHigh(1059, EVENT_LEVEL_WARNING, "Facility main inlet vacuum pressure is over upper limit.");
	SystemEvent AlcoholLeak(1060, EVENT_LEVEL_WARNING, "Alcohol leak.");
	SystemEvent AlcoholGasLeak(1061, EVENT_LEVEL_WARNING, "Alcohol gas leak.");
	SystemEvent FrameExhaustPresAlarm(1062, EVENT_LEVEL_ERROR, "Machine frame exhaust pressure alarm.");
	SystemEvent GasboxDoorOpen(1063, EVENT_LEVEL_WARNING, "Gasbox door is open.");
	SystemEvent GasboxHFLeak(1064, EVENT_LEVEL_ERROR, "Gasbox HF leak.");
	SystemEvent HardwareInterlock(1065, EVENT_LEVEL_WARNING, "Hardware interlock.");
	SystemEvent AlcoholTankLow(1066, EVENT_LEVEL_WARNING, "Alcohol tank level is too low.");
	SystemEvent PumpWarning(1067, EVENT_LEVEL_WARNING, "Vacuum pump warning.");
	SystemEvent PumpAlarm(1068, EVENT_LEVEL_ERROR, "Vacuum pump Alarm.");
	SystemEvent PlumbingHeaterAlarm(1069, EVENT_LEVEL_ERROR, "Plumbing heater alarm.");
	SystemEvent ChamberHFLeak(1070, EVENT_LEVEL_ERROR, "Process chamber HF leak.");
	SystemEvent ArmVacuumDoorEStop(1071, EVENT_LEVEL_WARNING, "Arm & vacuum door EStop.");

}


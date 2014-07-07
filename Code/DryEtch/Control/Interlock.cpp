/*
 * Interlock.cpp
 *
 *  Created on: 2014Äê3ÔÂ17ÈÕ
 *      Author: acm
 */

#include "Interlock.h"
#include "Data.h"
#include "Event.h"
#include "Parameters.h"
#include "LogFile.h"

void Interlock::Initialize()
{
	POST_INTERLOCK_MEMBER(diGasboxDoorClose, 0, shut_chemical_except_n2)
	POST_INTERLOCK_MEMBER(diExhaustAlarm, 1, shut_chemical_except_n2)
	POST_INTERLOCK_MEMBER(diExhaustPresAlarm, 1, shut_chemical_except_n2)
//	POST_INTERLOCK_MEMBER(diFacVPrsSwtLLmt, 1, shut_chemical_except_n2)

	//POST_INTERLOCK_MEMBER(diPrcCbVacuumFail, 1, shut_chemical_except_n2)
	//POST_INTERLOCK(diPrcCbVacuumFail, 1, doExpCbSupplyCbVal, 0)

	POST_INTERLOCK_MEMBER(diGasBoxHFLeak, 1, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diProcCbHFLeak, 1, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diAlcoholGasLeak, 1, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diAlcoholLeak, 1, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diPrcCbDoorClose, 0, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diCbLeftDoorClose, 0, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diCbRightDoorClose, 0, shut_all_chemical)
	POST_INTERLOCK_MEMBER(diAlcTkLow, 1, report_alcohol_alarm)
	
	POST_INTERLOCK_EVT(diAlcPrsLLmt, 1, AlcGasPressureLow)

	POST_INTERLOCK_EVT_STRING(diN2PgHFPrsULmt, 1, "N2 purge HF tube pressure is over upper limit.")
	POST_INTERLOCK_EVT_STRING(diN2PgHFPrsLLmt, 1, "N2 purge HF tube pressure is below lower limit.")
	
	POST_INTERLOCK_EVT_STRING(diN2PgAlcTkPrsULmt, 1, "N2 purge alcohol tank pressure is over upper limit.")
	POST_INTERLOCK_EVT_STRING(diN2PgAlcTkPrsLLmt, 1, "N2 purge alcohol tank pressure is below lower limit.")
	
	POST_INTERLOCK_EVT_STRING(diN2PgExpCbPrsULmt, 1, "N2 purge expansion chamber pressure is over upper limit.")
	POST_INTERLOCK_EVT_STRING(diN2PgExpCbPrsLLmt, 1, "N2 purge expansion chamber pressure is below lower limit.")
	
	POST_INTERLOCK_EVT_STRING(diN2SplVPumpPrsULmt, 1, "N2 supply vacuum pump pressure is over upper limit.")
	POST_INTERLOCK_EVT_STRING(diN2SplVPumpPrsLLmt, 1, "N2 supply vacuum pump pressure is below lower limit.")
	
	POST_INTERLOCK_EVT_STRING(diN2PgPrcCbPrsULmt, 1, "N2 purge process chamber pressure is over upper limit.")
	POST_INTERLOCK_EVT_STRING(diN2PgPrcCbPrsLLmt, 1, "N2 purge process chamber pressure is below lower limit.")
	
	POST_INTERLOCK_EVT(diN2FacSplPrsULmt, 1, N2FacPressureHigh)
	POST_INTERLOCK_EVT(diN2FacSplPrsLLmt, 1, N2FacPressureLow)
	
	POST_INTERLOCK_EVT_STRING(diN2PgVapPrsULmt, 1, "N2 purge vaporizer pressure is over upper limit.")
	POST_INTERLOCK_EVT_STRING(diN2PgVapPrsLLmt, 1, "N2 purge vaporizer pressure is below lower limit.")
	
	POST_INTERLOCK_EVT(diFacVPrsSwtLLmt, 1, FacVacuumPressureLow)

	POST_INTERLOCK_EVT_STRING(diExpCbVPrsSwtULmt, 1, "Expansion chamber vacuum pressure is over upper limit.")
	POST_INTERLOCK_EVT(diExpCbVPrsSwtLLmt, 1, ExpVacuumPressureLow)
	
	POST_INTERLOCK_EVT_STRING(diIODoorClose, 0, "IO door is open.")
	POST_INTERLOCK_EVT_STRING(diPCDoorClose, 0, "PC door is open.")
	POST_INTERLOCK_EVT_STRING(diPneuDoorClose, 0, "Pneumatic door is open.")
	POST_INTERLOCK_EVT_STRING(diAcFrmDoorClose, 0, "AC frame door is open.")
	POST_INTERLOCK_EVT_STRING(diAcDoorOverride, 1, "AC door override.")

	POST_INTERLOCK_EVT(diVaporMHeaterAlarm, 1, VapMainHeaterAlarm)
	POST_INTERLOCK_EVT(diVaporVHeaterAlarm, 1, VapVapHeaterAlarm)
	POST_INTERLOCK_EVT(diHeartbeatFail, 1, HeartbeatFail)
	POST_INTERLOCK_EVT(diExhaustAlarm, 1, GasboxExhaustPresAlarm)
	POST_INTERLOCK_EVT(diFacInletVPrsULmt, 1, FacMainInletVacPresHigh)
	POST_INTERLOCK_EVT(diAlcoholLeak, 1, AlcoholLeak)
	POST_INTERLOCK_EVT(diAlcoholGasLeak, 1, AlcoholGasLeak)
	POST_INTERLOCK_EVT(diExhaustPresAlarm, 1, FrameExhaustPresAlarm)
	POST_INTERLOCK_EVT(diGasboxDoorClose, 0, GasboxDoorOpen)
	POST_INTERLOCK_EVT(diGasBoxHFLeak, 1, GasboxHFLeak)
	POST_INTERLOCK_EVT(diHWInterlock, 1, HardwareInterlock)
	POST_INTERLOCK_EVT(diAlcTkLow, 0, AlcoholTankLow)
	POST_INTERLOCK_EVT(diVPumpWarning, 1, PumpWarning)
	POST_INTERLOCK_EVT(diVPumpAlarm, 1, PumpAlarm)
	
	POST_INTERLOCK_EVT(diPlumbing1Alarm, 1, PlumbingHeaterAlarm)
	POST_INTERLOCK_EVT(diPlumbing2Alarm, 1, PlumbingHeaterAlarm)
	POST_INTERLOCK_EVT(diPlumbing3Alarm, 1, PlumbingHeaterAlarm)
	POST_INTERLOCK_EVT(diPlumbing4Alarm, 1, PlumbingHeaterAlarm)
	POST_INTERLOCK_EVT(diPlumbing5Alarm, 1, PlumbingHeaterAlarm)
	
	POST_INTERLOCK_EVT(diProcCbHFLeak, 1, ChamberHFLeak)
	POST_INTERLOCK_EVT(diEStop, 1, ArmVacuumDoorEStop)

	POST_INTERLOCK_AI_EVT(aiGasboxTC, GasboxWarnTemp, OverTemp, "Gasbox TC")
	POST_INTERLOCK_AI_EVT(aiPipelineTC, PipeWarnTemp, OverTemp, "Pipeline TC")

	//PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diPrcCbVacuumFail, 1)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doExpCbSupplyCbVal, 1, diCbRightDoorClose, 0)
	POST_INTERLOCK_EX(doExpCbSupplyCbVal, [&](unsigned, unsigned value)
	{	if(value==1)
		{
			if(Data::ProcChamberDirty==1)
			{
				Data::ExpChamberDirty = 1;
				return;
			}
			if(Data::ExpChamberDirty == 1)
			{
				Data::ProcChamberDirty = 1;
			}
		}
	})

//	PRE_INTERLOCK(doPurgeAlcTank, 1, diPrcCbVacuumFail, 1)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diGasBoxHFLeak, 1)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diProcCbHFLeak, 1)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diAlcoholGasLeak, 1)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diAlcoholLeak, 1)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diPrcCbDoorClose, 0)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diCbLeftDoorClose, 0)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diCbRightDoorClose, 0)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diGasboxDoorClose, 0)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diExhaustAlarm, 1)
//	PRE_INTERLOCK(doPurgeAlcTank, 1, diFacVPrsSwtLLmt, 1)

	//PRE_INTERLOCK(doAlcTankOpen, 1, diPrcCbVacuumFail, 1)
	PRE_INTERLOCK(doAlcTankOpen, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doAlcTankOpen, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doAlcTankOpen, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doAlcTankOpen, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doAlcTankOpen, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doAlcTankOpen, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doAlcTankOpen, 1, diCbRightDoorClose, 0)
	PRE_INTERLOCK(doAlcTankOpen, 1, diGasboxDoorClose, 0)
	PRE_INTERLOCK(doAlcTankOpen, 1, diExhaustAlarm, 1)
//	PRE_INTERLOCK(doAlcTankOpen, 1, diFacVPrsSwtLLmt, 1)

	//PRE_INTERLOCK(doVaSupplyIPAValve, 1, diPrcCbVacuumFail, 1)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diCbRightDoorClose, 0)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diGasboxDoorClose, 0)
	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diExhaustAlarm, 1)
//	PRE_INTERLOCK(doVaSupplyIPAValve, 1, diFacVPrsSwtLLmt, 1)

	//PRE_INTERLOCK(doHFFacSupplyVal, 1, diPrcCbVacuumFail, 1)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diCbRightDoorClose, 0)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diGasboxDoorClose, 0)
	PRE_INTERLOCK(doHFFacSupplyVal, 1, diExhaustAlarm, 1)
//	PRE_INTERLOCK(doHFFacSupplyVal, 1, diFacVPrsSwtLLmt, 1)

	PRE_INTERLOCK(doN2PurgeHFVal, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doN2PurgeHFVal, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doN2PurgeHFVal, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doN2PurgeHFVal, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doN2PurgeHFVal, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doN2PurgeHFVal, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doN2PurgeHFVal, 1, diCbRightDoorClose, 0)

	PRE_INTERLOCK(doExpCbVapVacValve, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doExpCbVapVacValve, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doExpCbVapVacValve, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doExpCbVapVacValve, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doExpCbVapVacValve, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doExpCbVapVacValve, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doExpCbVapVacValve, 1, diCbRightDoorClose, 0)

	PRE_INTERLOCK(doVaVapValve, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doVaVapValve, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doVaVapValve, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doVaVapValve, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doVaVapValve, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doVaVapValve, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doVaVapValve, 1, diCbRightDoorClose, 0)

	PRE_INTERLOCK(doVaHFValve, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doVaHFValve, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doVaHFValve, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doVaHFValve, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doVaHFValve, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doVaHFValve, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doVaHFValve, 1, diCbRightDoorClose, 0)

	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diGasBoxHFLeak, 1)
	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diProcCbHFLeak, 1)
	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diAlcoholGasLeak, 1)
	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diAlcoholLeak, 1)
	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diPrcCbDoorClose, 0)
	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diCbLeftDoorClose, 0)
	PRE_INTERLOCK(doExpCbVacIPASupply, 1, diCbRightDoorClose, 0)

	PRE_INTERLOCK(doCbGateClose, 1, diArmOut, 0)
	PRE_INTERLOCK(doCbGateOpen, 1, ProcChamberDirty, 1)
	PRE_INTERLOCK_AI_LT(doCbGateOpen, 1, aiProcChamPressure, Parameters::ATMPressure)

	PRE_INTERLOCK(doAxisExecute, 0, diPinDown, 0)
	PRE_INTERLOCK(doAxisExecute, 0, diPinUp, 1)
	PRE_INTERLOCK(doAxisExecute, 1, diPinDown, 0)
	PRE_INTERLOCK(doAxisExecute, 1, diPinUp, 1)

	PRE_INTERLOCK(doPinUp, 1, diAxisMoving, 1)
	PRE_INTERLOCK(doPinDown, 0, diAxisMoving, 1)

	PRE_INTERLOCK(doArmIn, 1, diPrcCbDoorClose, 1)
	PRE_INTERLOCK(doArmIn, 1, diPrcCbDoorOpen, 0)
	PRE_INTERLOCK(doArmIn, 1, diForkVertical, 0)
	PRE_INTERLOCK(doArmOut, 0, diPrcCbDoorClose, 1)
	PRE_INTERLOCK(doArmOut, 0, diPrcCbDoorOpen, 0)
	PRE_INTERLOCK(doArmOut, 0, diForkVertical, 0)

	PRE_INTERLOCK(doForkHorizontal, 1, diArmOut, 0)
	PRE_INTERLOCK(doForkVertical, 0, diArmOut, 0)
}

void Interlock::Terminate()
{
	LogDebug("Interlock is terminated.");
}

void Interlock::shut_chemical_except_n2()
{
//	Data::doPurgeAlcTank = 0;
	Data::doAlcTankOpen = 0;
	Data::doVaSupplyIPAValve = 0;
	Data::doHFFacSupplyVal = 0;
}

void Interlock::shut_all_chemical()
{
//	Data::doPurgeAlcTank = 0;
	Data::doAlcTankOpen = 0;
	Data::doVaSupplyIPAValve = 0;
	Data::doHFFacSupplyVal = 0;
	Data::doExpCbSupplyCbVal = 0;
	Data::doN2PurgeHFVal = 0;
	Data::doExpCbVapVacValve = 0;
	Data::doVaVapValve = 0;
	Data::doVaHFValve = 0;
	Data::doExpCbVacIPASupply = 0;
}

void Interlock::report_alcohol_alarm()
{
	EVT::GenericWarning.Report("Alcohol tank level is too low.");
}

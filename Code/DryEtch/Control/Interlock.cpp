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
	DI_INTERLOCK_MEMBER(diGasboxDoorClose, 0, shut_chemical_except_n2)
	DI_INTERLOCK_MEMBER(diExhaustAlarm, 1, shut_chemical_except_n2)

	DI_INTERLOCK_MEMBER(diPrcCbVacuumFail, 1, shut_chemical_except_n2)
	DI_INTERLOCK(diPrcCbVacuumFail, 1, doExpCbSupplyCbVal, 0)

	DI_INTERLOCK_MEMBER(diGasBoxHFLeak, 1, shut_all_chemical)
	DI_INTERLOCK_MEMBER(diProcCbHFLeak, 1, shut_all_chemical)
	DI_INTERLOCK_MEMBER(diAlcoholGasLeak, 1, shut_all_chemical)
	DI_INTERLOCK_MEMBER(diAlcoholLeak, 1, shut_all_chemical)
	DI_INTERLOCK_MEMBER(diPrcCbDoorClose, 0, shut_all_chemical)
	DI_INTERLOCK_MEMBER(diCbLeftDoorClose, 0, shut_all_chemical)
	DI_INTERLOCK_MEMBER(diCbRightDoorClose, 0, shut_all_chemical)

	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diPrcCbVacuumFail, 1)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doExpCbSupplyCbVal, 1, diCbRightDoorClose, 0)

	DO_INTERLOCK(doPurgeAlcTank, 1, diPrcCbVacuumFail, 1)
	DO_INTERLOCK(doPurgeAlcTank, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doPurgeAlcTank, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doPurgeAlcTank, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doPurgeAlcTank, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doPurgeAlcTank, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doPurgeAlcTank, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doPurgeAlcTank, 1, diCbRightDoorClose, 0)
	DO_INTERLOCK(doPurgeAlcTank, 1, diGasboxDoorClose, 0)
	DO_INTERLOCK(doPurgeAlcTank, 1, diExhaustAlarm, 1)

	DO_INTERLOCK(doAlcTankOpen, 1, diPrcCbVacuumFail, 1)
	DO_INTERLOCK(doAlcTankOpen, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doAlcTankOpen, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doAlcTankOpen, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doAlcTankOpen, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doAlcTankOpen, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doAlcTankOpen, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doAlcTankOpen, 1, diCbRightDoorClose, 0)
	DO_INTERLOCK(doAlcTankOpen, 1, diGasboxDoorClose, 0)
	DO_INTERLOCK(doAlcTankOpen, 1, diExhaustAlarm, 1)

	DO_INTERLOCK(doVaSupplyIPAValve, 1, diPrcCbVacuumFail, 1)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diCbRightDoorClose, 0)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diGasboxDoorClose, 0)
	DO_INTERLOCK(doVaSupplyIPAValve, 1, diExhaustAlarm, 1)

	DO_INTERLOCK(doHFFacSupplyVal, 1, diPrcCbVacuumFail, 1)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diCbRightDoorClose, 0)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diGasboxDoorClose, 0)
	DO_INTERLOCK(doHFFacSupplyVal, 1, diExhaustAlarm, 1)

	DO_INTERLOCK(doN2PurgeHFVal, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doN2PurgeHFVal, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doN2PurgeHFVal, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doN2PurgeHFVal, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doN2PurgeHFVal, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doN2PurgeHFVal, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doN2PurgeHFVal, 1, diCbRightDoorClose, 0)

	DO_INTERLOCK(doExpCbVapVacValve, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doExpCbVapVacValve, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doExpCbVapVacValve, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doExpCbVapVacValve, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doExpCbVapVacValve, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doExpCbVapVacValve, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doExpCbVapVacValve, 1, diCbRightDoorClose, 0)

	DO_INTERLOCK(doVaVapValve, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doVaVapValve, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doVaVapValve, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doVaVapValve, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doVaVapValve, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doVaVapValve, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doVaVapValve, 1, diCbRightDoorClose, 0)

	DO_INTERLOCK(doVaHFValve, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doVaHFValve, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doVaHFValve, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doVaHFValve, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doVaHFValve, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doVaHFValve, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doVaHFValve, 1, diCbRightDoorClose, 0)

	DO_INTERLOCK(doExpCbVacIPASupply, 1, diGasBoxHFLeak, 1)
	DO_INTERLOCK(doExpCbVacIPASupply, 1, diProcCbHFLeak, 1)
	DO_INTERLOCK(doExpCbVacIPASupply, 1, diAlcoholGasLeak, 1)
	DO_INTERLOCK(doExpCbVacIPASupply, 1, diAlcoholLeak, 1)
	DO_INTERLOCK(doExpCbVacIPASupply, 1, diPrcCbDoorClose, 0)
	DO_INTERLOCK(doExpCbVacIPASupply, 1, diCbLeftDoorClose, 0)
	DO_INTERLOCK(doExpCbVacIPASupply, 1, diCbRightDoorClose, 0)

	DO_INTERLOCK(doCbGateVal, 0, diArmOut, 0)
	DO_INTERLOCK_AI_LT(doCbGateVal, 1, aiProcChamPressure, Parameters::ATMPressure)

	DO_INTERLOCK(doAxisExecute, 0, diPinDown, 0)
	DO_INTERLOCK(doAxisExecute, 0, diPinUp, 1)
	DO_INTERLOCK(doAxisExecute, 1, diPinDown, 0)
	DO_INTERLOCK(doAxisExecute, 1, diPinUp, 1)

	DO_INTERLOCK(doPinUp, 1, diAxisMoving, 1)
	DO_INTERLOCK(doPinDown, 0, diAxisMoving, 1)

	DO_INTERLOCK(doArmIn, 1, diPrcCbDoorClose, 1)
	DO_INTERLOCK(doArmIn, 1, diPrcCbDoorOpen, 0)
	DO_INTERLOCK(doArmIn, 1, diForkVertical, 0)
	DO_INTERLOCK(doArmOut, 0, diPrcCbDoorClose, 1)
	DO_INTERLOCK(doArmOut, 0, diPrcCbDoorOpen, 0)
	DO_INTERLOCK(doArmOut, 0, diForkVertical, 0)

	DO_INTERLOCK(doForkHorizontal, 1, diArmOut, 0)
	DO_INTERLOCK(doForkVertical, 0, diArmOut, 0)
}

void Interlock::Terminate()
{
	LogDebug("Interlock is terminated.");
}

void Interlock::shut_chemical_except_n2()
{
	Data::doPurgeAlcTank = 0;
	Data::doAlcTankOpen = 0;
	Data::doVaSupplyIPAValve = 0;
	Data::doHFFacSupplyVal = 0;
}

void Interlock::shut_all_chemical()
{
	Data::doPurgeAlcTank = 0;
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

/*
 * Data.h
 *
 *  Created on: 2014Äê3ÔÂ5ÈÕ
 *      Author: acm
 */

#ifndef DATA_H_
#define DATA_H_

#include "SystemData.h"

namespace Data
{
extern SystemData<unsigned> diAlcPrsULmt;
extern SystemData<unsigned> diAlcPrsLLmt;
extern SystemData<unsigned> diAlcTkPrsULmt;
extern SystemData<unsigned> diAlcTkPrsLLmt;
extern SystemData<unsigned> diHFSplPrsULmt;
extern SystemData<unsigned> diHFSplPrsLLmt;
extern SystemData<unsigned> diN2PgHFPrsULmt;
extern SystemData<unsigned> diN2PgHFPrsLLmt;
extern SystemData<unsigned> diN2PgAlcTkPrsULmt;
extern SystemData<unsigned> diN2PgAlcTkPrsLLmt;
extern SystemData<unsigned> diN2PgExpCbPrsULmt;
extern SystemData<unsigned> diN2PgExpCbPrsLLmt;
extern SystemData<unsigned> diN2SplVPumpPrsULmt;
extern SystemData<unsigned> diN2SplVPumpPrsLLmt;
extern SystemData<unsigned> diN2PgPrcCbPrsULmt;
extern SystemData<unsigned> diN2PgPrcCbPrsLLmt;
extern SystemData<unsigned> diN2FacSplPrsULmt;
extern SystemData<unsigned> diN2FacSplPrsLLmt;
extern SystemData<unsigned> diN2PgVapPrsULmt;
extern SystemData<unsigned> diN2PgVapPrsLLmt;
extern SystemData<unsigned> diFacVPrsSwtLLmt;
extern SystemData<unsigned> diExpCbVPrsSwtULmt;
extern SystemData<unsigned> diExpCbVPrsSwtLLmt;
extern SystemData<unsigned> diIODoorClose;
extern SystemData<unsigned> diPCDoorClose;
extern SystemData<unsigned> diPneuDoorClose;
extern SystemData<unsigned> diAcFrmDoorClose;
extern SystemData<unsigned> diAcDoorOverride;
extern SystemData<unsigned> diVaporMHeaterAlarm;
extern SystemData<unsigned> diVaporVHeaterAlarm;
extern SystemData<unsigned> diHFReady;
extern SystemData<unsigned> diHeartbeatFail;
extern SystemData<unsigned> diExhaustAlarm;
extern SystemData<unsigned> diFacInletVPrsULmt;
extern SystemData<unsigned> diAlcoholLeak;
extern SystemData<unsigned> diPrcCbVacuumFail;
extern SystemData<unsigned> diPrcCbDoorClose;
extern SystemData<unsigned> diAlcoholGasLeak;
extern SystemData<unsigned> diExhaustPresAlarm;
extern SystemData<unsigned> diGasboxDoorClose;
extern SystemData<unsigned> diDoorOverride;
extern SystemData<unsigned> diInterlockReset;
extern SystemData<unsigned> diGasBoxHFLeak;
extern SystemData<unsigned> diThrottleValveOpen;
extern SystemData<unsigned> diHWInterlock;
extern SystemData<unsigned> diAlcTkLow;
extern SystemData<unsigned> diAlcTkHigh;
extern SystemData<unsigned> diAlcTkHighHigh;
extern SystemData<unsigned> diAlcTkOverfill;
extern SystemData<unsigned> diVPumpWarning;
extern SystemData<unsigned> diVPumpAlarm;
extern SystemData<unsigned> diProcCbBodyHeater;
extern SystemData<unsigned> diProcCbLidHeater;
extern SystemData<unsigned> diProcCbChuckHeater;

extern SystemData<unsigned> diCbA1MotorAlarm;
extern SystemData<unsigned> diCbA1MotorCWLmit;
extern SystemData<unsigned> diCbA1MotorCCWLmit;
extern SystemData<unsigned> diCbMotorHome;

extern SystemData<unsigned> diPrcCbDoorOpen;
extern SystemData<unsigned> diPinUp;
extern SystemData<unsigned> diPinDown;
extern SystemData<unsigned> diForkVertical;
extern SystemData<unsigned> diForkHorizontal;
extern SystemData<unsigned> diArmIn;
extern SystemData<unsigned> diArmOut;
//extern SystemData<unsigned> diPrcCbLidLeftOpen;
//extern SystemData<unsigned> diPrcCbLidLeftClose;
//extern SystemData<unsigned> diPrcCbLidRightOpen;
//extern SystemData<unsigned> diPrcCbLidRightClose;
extern SystemData<unsigned> diCbLeftDoorClose;
extern SystemData<unsigned> diCbRightDoorClose;
extern SystemData<unsigned> diProcCbHFLeak;
extern SystemData<unsigned> diCDAInletAlarm;

//DO
extern SystemData<unsigned> aoRedLight;
extern SystemData<unsigned> aoYellowLight;
extern SystemData<unsigned> aoGreenLight;
extern SystemData<unsigned> aoBlueLight;
extern SystemData<unsigned> aoBuzzer;
extern SystemData<unsigned> doHeartbeat;
extern SystemData<unsigned> doEnableVPump;
extern SystemData<unsigned> doHFRequest;
extern SystemData<unsigned> doN2PurgeGasValve;
extern SystemData<unsigned> doExpCbVacValve;
extern SystemData<unsigned> doExpCbVapVacValve;
extern SystemData<unsigned> doAlcMFCVal1;
extern SystemData<unsigned> doAlcMFCVal2;
extern SystemData<unsigned> doAlcMFCVal3;
extern SystemData<unsigned> doVapSupplyN2Valve;
extern SystemData<unsigned> doVapBypassValve;
extern SystemData<unsigned> doHFMFCVal1;
extern SystemData<unsigned> doHFMFCVal2;
extern SystemData<unsigned> doHFMFCVal3;
extern SystemData<unsigned> doPurgeN2MFCVal1;
extern SystemData<unsigned> doPurgeN2MFCVal2;
extern SystemData<unsigned> doVaVapValve;
extern SystemData<unsigned> doVaHFValve;
extern SystemData<unsigned> doVaSupplyIPAValve;
extern SystemData<unsigned> doExpCbVacIPASupply;
extern SystemData<unsigned> doAlcTankOpen;
extern SystemData<unsigned> doPurgeAlcTank;
extern SystemData<unsigned> doExpCbHFInletVal;
extern SystemData<unsigned> doHFFacSupplyVal;
extern SystemData<unsigned> doN2PurgeHFVal;
extern SystemData<unsigned> doExpCbSupplyCbVal;
extern SystemData<unsigned> doN2SupplyVacVal;
extern SystemData<unsigned> doN2SupplyProcVal;
extern SystemData<unsigned> doN2MFCVal1;
extern SystemData<unsigned> doN2MFCVal2;
extern SystemData<unsigned> doPinUp;
extern SystemData<unsigned> doArmIn;
extern SystemData<unsigned> doForkVertical;
extern SystemData<unsigned> doVacSlowProcCbVal;
extern SystemData<unsigned> doVacFastProcCbVal;
extern SystemData<unsigned> doPinDown;
extern SystemData<unsigned> doArmOut;
extern SystemData<unsigned> doForkHorizontal;
extern SystemData<unsigned> doVapInletVal;
extern SystemData<unsigned> doCbGateVal;


//AI
extern SystemData<float> aiFacPressure;
extern SystemData<float> aiExpPressure;
extern SystemData<float> aiAlcTankPressure;
extern SystemData<float> aiAlcPressure;
extern SystemData<float> aiHFSupplyPressure;
extern SystemData<float> aiAlcGasLeak;
extern SystemData<float> aiHFGasboxPPM;
extern SystemData<float> aiHFChamberPPM;


//AO


//Body Heater
extern SystemData<float> aiBodyHTTemp;
extern SystemData<unsigned> aiBodyHTErrCode;
extern SystemData<unsigned> diBodyHTPowRdy;
extern SystemData<unsigned> diBodyHTErr;
extern SystemData<unsigned> doBodyHTEnable;
extern SystemData<float> aoBodyHTTempSet;


//Lid Heater
extern SystemData<float> aiLidHTTemp;
extern SystemData<unsigned> aiLidHTErrCode;
extern SystemData<unsigned> diLidHTPowRdy;
extern SystemData<unsigned> diLidHTErr;
extern SystemData<unsigned> doLidHTEnable;
extern SystemData<float> aoLidHTTempSet;


//Chuck Heater
extern SystemData<float> aiChuckHTTemp;
extern SystemData<unsigned> aiChuckHTErrCode;
extern SystemData<unsigned> diChuckHTPowRdy;
extern SystemData<unsigned> diChuckHTErr;
extern SystemData<unsigned> doChuckHTEnable;
extern SystemData<float> aoChuckHTTempSet;


//Motion
extern SystemData<unsigned> diHomeSensor;
extern SystemData<unsigned> diCWLimitStatus;
extern SystemData<unsigned> diCCWLimitStatus;
extern SystemData<unsigned> diReachPosition;
extern SystemData<unsigned> diDriverAlarm;
extern SystemData<unsigned> diAxisError;
extern SystemData<unsigned> diAxisErrStop;
extern SystemData<unsigned> diAxisDisabled;
extern SystemData<unsigned> diAxisStandStill;
extern SystemData<unsigned> diAxisMoving;
extern SystemData<unsigned> diAxisNotMoving;
extern SystemData<unsigned> aiAxisErrCode;
extern SystemData<unsigned> diAxisServoDone;
extern SystemData<unsigned> diAxisHomeDone;
extern SystemData<float> aiActualPosition;
extern SystemData<float> aiActualVelocity;
extern SystemData<unsigned> aoAxisControl;
extern SystemData<unsigned> doAxisExecute;
extern SystemData<unsigned> doAxisServoOn;
extern SystemData<unsigned> doAxisReset;
extern SystemData<float> aoAxisStopDec;
extern SystemData<float> aoAxisStopJerk;
extern SystemData<float> aoAxisAcc;
extern SystemData<float> aoAxisDec;
extern SystemData<float> aoAxisJerk;
extern SystemData<float> aoAxisVelocity;
extern SystemData<float> aoAxisPosition;
extern SystemData<float> aoAxisDistance;
extern SystemData<float> aoAxisVelOverride;
extern SystemData<float> aoAxisPatrolPos1;
extern SystemData<float> aoAxisPatrolPos2;

//DeviceNet
extern SystemData<unsigned> aiAPCStatus;
extern SystemData<float> aiAPCPressure;
extern SystemData<float> aiAPCPosition;
extern SystemData<unsigned> aiAPCOverrideStatus;
extern SystemData<unsigned> aiVapMFCStatus;
extern SystemData<float> aiEtOHFlowrate;
extern SystemData<unsigned> aiHFMFCStatus;
extern SystemData<float> aiHFFlowrate;
extern SystemData<unsigned> aiPurgeN2MFCStatus;
extern SystemData<float> aiPurgeN2Flowrate;
extern SystemData<unsigned> aiN2MFCStatus;
extern SystemData<float> aiN2Flowrate;
extern SystemData<float> aiProcChamPressure;
extern SystemData<unsigned> aiProcChamManoStatus;
extern SystemData<float> aiExpChamPressure;
extern SystemData<unsigned> aiExpChamManoStatus;
extern SystemData<float> aoAPCPressure;
extern SystemData<unsigned> aoAPCOverride;
extern SystemData<unsigned> aoAPCControlMode;
extern SystemData<float> aoEtOHFlowSetpoint;
extern SystemData<float> aoHFFlowSetpoint;
extern SystemData<float> aoPurgeN2FlowSetpoint;
extern SystemData<float> aoN2FlowSetpoint;
extern SystemData<float> aoAPCPosition;


//user defined
extern SystemData<unsigned> TotalSteps;
extern SystemData<unsigned> CurrentStep;

extern SystemData<std::string> RecipeName;

extern SystemData<unsigned> RecipeTotalTime;
extern SystemData<unsigned> RecipeElapseTime;
extern SystemData<unsigned> CurrentStepTime;
extern SystemData<unsigned> StepElapseTime;

extern SystemData<unsigned> LoadUnloadState;
extern SystemData<unsigned> LoadUnloadSkip;
extern SystemData<unsigned> LoadUnloadOK;
extern SystemData<unsigned> WaferProcessedCount;
extern SystemData<std::string> WaferBatchID;
extern SystemData<unsigned> WaferBatchCount;
extern SystemData<unsigned> WaferTotalCount;

extern SystemData<std::string> LeakCheckResult;
extern SystemData<float> LeakRate;

extern SystemData<std::string> Slot1WaferID;
extern SystemData<std::string> Slot2WaferID;
extern SystemData<std::string> Slot3WaferID;
extern SystemData<unsigned> Slot1WaferState;
extern SystemData<unsigned> Slot2WaferState;
extern SystemData<unsigned> Slot3WaferState;
extern SystemData<std::string> ForkWaferID;
extern SystemData<unsigned> ForkWaferState;

extern SystemData<unsigned> ProcUnitState;
extern SystemData<unsigned> ProcUnitRetryable;
extern SystemData<unsigned> ProcUnitCommand;
extern SystemData<unsigned> ProcUnitParam1;
extern SystemData<unsigned> ProcUnitParam2;
extern SystemData<std::string> ProcUnitStepName;

extern SystemData<unsigned> ProcChamberDirty;
extern SystemData<unsigned> ExpChamberDirty;

};

#endif /* DATA_H_ */

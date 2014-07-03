/*
 * Data.cpp
 *
 *  Created on: 2014Äê3ÔÂ5ÈÕ
 *      Author: acm
 */

#include "SystemData.h"

namespace Data
{

//DI
SystemData<unsigned> diAlcPrsULmt(0, "diAlcPrsULmt", "DI", 0, 1, 0, 0, 0, 0, 0, 1, false);
SystemData<unsigned> diAlcPrsLLmt(1, "diAlcPrsLLmt", "DI", 0, 1, 0, 0, 0, 1, 0, 1, false);
SystemData<unsigned> diAlcTkPrsULmt(2, "diAlcTkPrsULmt", "DI", 0, 1, 0, 0, 0, 2, 0, 1, false);
SystemData<unsigned> diAlcTkPrsLLmt(3, "diAlcTkPrsLLmt", "DI", 0, 1, 0, 0, 0, 3, 0, 1, false);
SystemData<unsigned> diHFSplPrsULmt(4, "diHFSplPrsULmt", "DI", 0, 1, 0, 0, 0, 4, 0, 1, false);
SystemData<unsigned> diHFSplPrsLLmt(5, "diHFSplPrsLLmt", "DI", 0, 1, 0, 0, 0, 5, 0, 1, false);
SystemData<unsigned> diN2PgHFPrsULmt(6, "diN2PgHFPrsULmt", "DI", 0, 1, 0, 0, 0, 6, 0, 1, false);
SystemData<unsigned> diN2PgHFPrsLLmt(7, "diN2PgHFPrsLLmt", "DI", 0, 1, 0, 0, 0, 7, 0, 1, false);
SystemData<unsigned> diN2PgAlcTkPrsULmt(8, "diN2PgAlcTkPrsULmt", "DI", 0, 1, 0, 0, 0, 8, 0, 1, false);
SystemData<unsigned> diN2PgAlcTkPrsLLmt(9, "diN2PgAlcTkPrsLLmt", "DI", 0, 1, 0, 0, 0, 9, 0, 1, false);
SystemData<unsigned> diN2PgExpCbPrsULmt(10, "diN2PgExpCbPrsULmt", "DI", 0, 1, 0, 0, 0, 10, 0, 1, false);
SystemData<unsigned> diN2PgExpCbPrsLLmt(11, "diN2PgExpCbPrsLLmt", "DI", 0, 1, 0, 0, 0, 11, 0, 1, false);
SystemData<unsigned> diN2SplVPumpPrsULmt(12, "diN2SplVPumpPrsULmt", "DI", 0, 1, 0, 0, 0, 12, 0, 1, false);
SystemData<unsigned> diN2SplVPumpPrsLLmt(13, "diN2SplVPumpPrsLLmt", "DI", 0, 1, 0, 0, 0, 13, 0, 1, false);
SystemData<unsigned> diN2PgPrcCbPrsULmt(14, "diN2PgPrcCbPrsULmt", "DI", 0, 1, 0, 0, 0, 14, 0, 1, false);
SystemData<unsigned> diN2PgPrcCbPrsLLmt(15, "diN2PgPrcCbPrsLLmt", "DI", 0, 1, 0, 0, 0, 15, 0, 1, false);
SystemData<unsigned> diN2FacSplPrsULmt(16, "diN2FacSplPrsULmt", "DI", 0, 1, 0, 0, 0, 16, 0, 1, false);
SystemData<unsigned> diN2FacSplPrsLLmt(17, "diN2FacSplPrsLLmt", "DI", 0, 1, 0, 0, 0, 17, 0, 1, false);
SystemData<unsigned> diN2PgVapPrsULmt(18, "diN2PgVapPrsULmt", "DI", 0, 1, 0, 0, 0, 18, 0, 1, false);
SystemData<unsigned> diN2PgVapPrsLLmt(19, "diN2PgVapPrsLLmt", "DI", 0, 1, 0, 0, 0, 19, 0, 1, false);
SystemData<unsigned> diFacVPrsSwtLLmt(21, "diFacVPrsSwtLLmt", "DI", 0, 1, 0, 0, 0, 21, 0, 1, false);
SystemData<unsigned> diExpCbVPrsSwtULmt(22, "diExpCbVPrsSwtULmt", "DI", 0, 1, 0, 0, 0, 22, 0, 1, false);
SystemData<unsigned> diExpCbVPrsSwtLLmt(23, "diExpCbVPrsSwtLLmt", "DI", 0, 1, 0, 0, 0, 23, 0, 1, false);
SystemData<unsigned> diIODoorClose(24, "diIODoorClose", "DI", 0, 1, 0, 0, 0, 24, 0, 1, false);
SystemData<unsigned> diPCDoorClose(25, "diPCDoorClose", "DI", 0, 1, 0, 0, 0, 25, 0, 1, false);
SystemData<unsigned> diPneuDoorClose(26, "diPneuDoorClose", "DI", 0, 1, 0, 0, 0, 26, 0, 1, false);
SystemData<unsigned> diAcFrmDoorClose(27, "diAcFrmDoorClose", "DI", 0, 1, 0, 0, 0, 27, 0, 1, false);
SystemData<unsigned> diAcDoorOverride(28, "diAcDoorOverride", "DI", 0, 1, 0, 0, 0, 28, 0, 1, false);
SystemData<unsigned> diVaporMHeaterAlarm(29, "diVaporMHeaterAlarm", "DI", 0, 1, 0, 0, 0, 29, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diVaporVHeaterAlarm(30, "diVaporVHeaterAlarm", "DI", 0, 1, 0, 0, 0, 30, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diHFReady(31, "diHFReady", "DI", 0, 1, 0, 0, 0, 31, 0, 1, false);
SystemData<unsigned> diHeartbeatFail(32, "diHeartbeatFail", "DI", 0, 1, 0, 0, 0, 32, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diExhaustAlarm(33, "diExhaustAlarm", "DI", 0, 1, 0, 0, 0, 33, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diFacInletVPrsULmt(34, "diFacInletVPrsULmt", "DI", 0, 1, 0, 0, 0, 34, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diAlcoholLeak(35, "diAlcoholLeak", "DI", 0, 1, 0, 0, 0, 35, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diPrcCbVacuumFail(36, "diPrcCbVacuumFail", "DI", 0, 1, 0, 0, 0, 36, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diPrcCbDoorClose(37, "diPrcCbDoorClose", "DI", 0, 1, 0, 0, 0, 37, 0, 1, false);
SystemData<unsigned> diAlcoholGasLeak(38, "diAlcoholGasLeak", "DI", 0, 1, 0, 0, 0, 38, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diExhaustPresAlarm(39, "diExhaustPresAlarm", "DI", 0, 1, 0, 0, 0, 39, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diGasboxDoorClose(40, "diGasboxDoorClose", "DI", 0, 1, 0, 0, 0, 40, 0, 1, false);
SystemData<unsigned> diDoorOverride(41, "diDoorOverride", "DI", 0, 1, 0, 0, 0, 41, 0, 1, false);
SystemData<unsigned> diInterlockReset(42, "diInterlockReset", "DI", 0, 1, 0, 0, 0, 42, 0, 1, false);
SystemData<unsigned> diGasBoxHFLeak(43, "diGasBoxHFLeak", "DI", 0, 1, 0, 0, 0, 43, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diThrottleValveOpen(44, "diThrottleValveOpen", "DI", 0, 1, 0, 0, 0, 44, 0, 1, false);
SystemData<unsigned> diHWInterlock(45, "diHWInterlock", "DI", 0, 1, 0, 0, 0, 45, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diAlcTkLow(46, "diAlcTkLow", "DI", 0, 1, 0, 0, 0, 46, 0, 1, false);
SystemData<unsigned> diAlcTkHigh(47, "diAlcTkHigh", "DI", 0, 1, 0, 0, 0, 47, 0, 1, false);
SystemData<unsigned> diAlcTkHighHigh(48, "diAlcTkHighHigh", "DI", 0, 1, 0, 0, 0, 48, 0, 1, false);
SystemData<unsigned> diAlcTkOverfill(49, "diAlcTkOverfill", "DI", 0, 1, 0, 0, 0, 49, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diVPumpWarning(50, "diVPumpWarning", "DI", 0, 1, 0, 0, 0, 50, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diVPumpAlarm(51, "diVPumpAlarm", "DI", 0, 1, 0, 0, 0, 51, 0, 1, false,"",ConvertNot<unsigned>);
SystemData<unsigned> diPrcCbDoorOpen(60, "diPrcCbDoorOpen", "DI", 0, 1, 0, 0, 0, 60, 0, 1, false);
SystemData<unsigned> diPinUp(61, "diPinUp", "DI", 0, 1, 0, 0, 0, 61, 0, 1, false);
SystemData<unsigned> diPinDown(62, "diPinDown", "DI", 0, 1, 0, 0, 0, 62, 0, 1, false);
SystemData<unsigned> diForkVertical(63, "diForkVertical", "DI", 0, 1, 0, 0, 0, 63, 0, 1, false);
SystemData<unsigned> diForkHorizontal(64, "diForkHorizontal", "DI", 0, 1, 0, 0, 0, 64, 0, 1, false);
SystemData<unsigned> diArmIn(65, "diArmIn", "DI", 0, 1, 0, 0, 0, 65, 0, 1, false);
SystemData<unsigned> diArmOut(66, "diArmOut", "DI", 0, 1, 0, 0, 0, 66, 0, 1, false);
//SystemData<unsigned> diPrcCbLidLeftOpen(67, "diPrcCbLidLeftOpen", "DI", 0, 1, 0, 0, 0, 67, 0, 1, false);
//SystemData<unsigned> diPrcCbLidLeftClose(68, "diPrcCbLidLeftClose", "DI", 0, 1, 0, 0, 0, 68, 0, 1, false);
//SystemData<unsigned> diPrcCbLidRightOpen(69, "diPrcCbLidRightOpen", "DI", 0, 1, 0, 0, 0, 69, 0, 1, false);
//SystemData<unsigned> diPrcCbLidRightClose(70, "diPrcCbLidRightClose", "DI", 0, 1, 0, 0, 0, 70, 0, 1, false);
SystemData<unsigned> diCbLeftDoorClose(76, "diCbLeftDoorClose", "DI", 0, 1, 0, 0, 0, 76, 0, 1, false);
SystemData<unsigned> diCbRightDoorClose(77, "diCbRightDoorClose", "DI", 0, 1, 0, 0, 0, 77, 0, 1, false);
SystemData<unsigned> diProcCbHFLeak(78, "diProcCbHFLeak","DI",0,1,0,0,0,78,0,1,false,"",ConvertNot<unsigned>);
SystemData<unsigned> diCDAInletAlarm(79, "diCDAInletAlarm", "DI", 0, 1, 0, 0, 0, 79, 0, 1, false, "", ConvertNot<unsigned>);


//DO
SystemData<unsigned> aoRedLight(1000,"aoRedLight","DO",0,256,0,0,1,0,0,16,true);
SystemData<unsigned> aoYellowLight(1002,"aoYellowLight","DO",0,256,0,0,1,2,0,16,true);
SystemData<unsigned> aoGreenLight(1004,"aoGreenLight","DO",0,256,0,0,1,4,0,16,true);
SystemData<unsigned> aoBlueLight(1006,"aoBlueLight","DO",0,256,0,0,1,6,0,16,true);
SystemData<unsigned> aoBuzzer(1008,"aoBuzzerWarn","DO",0,256,0,0,1,8,0,16,true);
SystemData<unsigned> doHeartBeat(1010,"doHeartBeat","DO",0,1,0,0,1,10,0,1,true);
SystemData<unsigned> doEnableVPump(1011,"doEnableVPump","DO",0,1,0,0,1,11,0,1,true);
SystemData<unsigned> doHFRequest(1012,"doHFRequest","DO",0,1,0,0,1,12,0,1,true);
SystemData<unsigned> doN2PurgeGBValve(1016,"doN2PurgeGBValve","DO",0,1,0,0,1,16,0,1,true);
SystemData<unsigned> doExpCbVacValve(1018,"doExpCbVacValve","DO",0,1,0,0,1,18,0,1,true);
SystemData<unsigned> doExpCbVapVacValve(1019,"doExpCbVapVacValve","DO",0,1,0,0,1,19,0,1,true);
SystemData<unsigned> doAlcMFCVal1(1021,"doAlcMFCVal1","DO",0,1,0,0,1,21,0,1,true);
SystemData<unsigned> doAlcMFCVal2(1022,"doAlcMFCVal2","DO",0,1,0,0,1,22,0,1,true);
SystemData<unsigned> doAlcMFCVal3(1023,"doAlcMFCVal3","DO",0,1,0,0,1,23,0,1,true,"",ConvertNot<unsigned>,ConvertNot<unsigned>);
SystemData<unsigned> doVapSupplyN2Valve(1024,"doVapSupplyN2Valve","DO",0,1,0,0,1,24,0,1,true);
SystemData<unsigned> doVapBypassValve(1025,"doVapBypassValve","DO",0,1,0,0,1,25,0,1,true);
SystemData<unsigned> doHFMFCVal1(1026,"doHFMFCVal1","DO",0,1,0,0,1,26,0,1,true);
SystemData<unsigned> doHFMFCVal2(1027,"doHFMFCVal2","DO",0,1,0,0,1,27,0,1,true);
SystemData<unsigned> doHFMFCVal3(1028,"doHFMFCVal3","DO",0,1,0,0,1,28,0,1,true,"",ConvertNot<unsigned>,ConvertNot<unsigned>);
SystemData<unsigned> doPurgeN2MFCVal1(1029,"doPurgeN2MFCVal1","DO",0,1,0,0,1,29,0,1,true);
SystemData<unsigned> doPurgeN2MFCVal2(1030,"doPurgeN2MFCVal2","DO",0,1,0,0,1,30,0,1,true);
SystemData<unsigned> doVaVapValve(1032,"doVaVapValve","DO",0,1,0,0,1,32,0,1,true);
SystemData<unsigned> doVaHFValve(1033,"doVaHFValve","DO",0,1,0,0,1,33,0,1,true);
SystemData<unsigned> doVaSupplyIPAValve(1034,"doVaSupplyIPAValve","DO",0,1,0,0,1,34,0,1,true);
SystemData<unsigned> doExpCbVacIPASupply(1035,"doExpCbVacIPASupply","DO",0,1,0,0,1,35,0,1,true);
SystemData<unsigned> doAlcTankOpen(1036,"doAlcTankOpen","DO",0,1,0,0,1,36,0,1,true);
SystemData<unsigned> doPurgeAlcTank(1037,"doPurgeAlcTank","DO",0,1,0,0,1,37,0,1,true);
SystemData<unsigned> doExpCbHFInletVal(1038,"doExpCbHFInletVal","DO",0,1,0,0,1,38,0,1,true);
SystemData<unsigned> doHFFacSupplyVal(1039,"doHFFacSupplyVal","DO",0,1,0,0,1,39,0,1,true);
SystemData<unsigned> doN2PurgeHFVal(1040,"doN2PurgeHFVal","DO",0,1,0,0,1,40,0,1,true);
SystemData<unsigned> doExpCbSupplyCbVal(1041,"doExpCbSupplyCbVal","DO",0,1,0,0,1,41,0,1,true);
SystemData<unsigned> doN2SupplyVacVal(1048,"doN2SupplyVacVal","DO",0,1,0,0,1,48,0,1,true);
SystemData<unsigned> doN2SupplyProcVal(1049,"doN2SupplyProcVal","DO",0,1,0,0,1,49,0,1,true);
SystemData<unsigned> doN2MFCVal1(1050,"doN2MFCVal1","DO",0,1,0,0,1,50,0,1,true);
SystemData<unsigned> doN2MFCVal2(1051,"doN2MFCVal2","DO",0,1,0,0,1,51,0,1,true);
SystemData<unsigned> doPinUp(1052,"doPinUp","DO",0,1,0,0,1,52,0,1,true);
SystemData<unsigned> doArmIn(1053,"doArmIn","DO",0,1,0,0,1,53,0,1,true);
SystemData<unsigned> doForkVertical(1054,"doForkVertical","DO",0,1,0,0,1,54,0,1,true);
SystemData<unsigned> doVacSlowProcCbVal(1056,"doVacSlowProcCbVal","DO",0,1,0,0,1,56,0,1,true);
SystemData<unsigned> doVacFastProcCbVal(1057,"doVacFastProcCbVal","DO",0,1,0,0,1,57,0,1,true);
SystemData<unsigned> doPinDown(1063,"doPinDown","DO",0,1,0,0,1,63,0,1,true);
SystemData<unsigned> doArmOut(1064,"doArmOut","DO",0,1,0,0,1,64,0,1,true);
SystemData<unsigned> doForkHorizontal(1065,"doForkHorizontal","DO",0,1,0,0,1,65,0,1,true);
SystemData<unsigned> doVapInletVal(1067,"doVapInletVal","DO",0,1,0,0,1,67,0,1,true);
SystemData<unsigned> doCbGateOpen(1071,"doCbGateOpen","DO",0,1,0,0,1,71,0,1,true);
SystemData<unsigned> doCbGateClose(1072,"doCbGateClose","DO",0,1,0,0,1,72,0,1,true);



//AI
SystemData<float> aiFacPressure(2000,"aiFacPressure","AI",0,0,2,0,2,0,0,16,false,"kPa", ConvertLinear<float,unsigned>(3276, 16383, -100.0f, 1000.0f));
SystemData<float> aiExpPressure(2002,"aiExpPressure","AI",0,0,2,0,2,2,0,16,false,"kPa", ConvertLinear<float,unsigned>(3276, 16383, -100.0f, 1000.0f));
SystemData<float> aiAlcPressure(2004,"aiAlcPressure","AI",0,0,2,0,2,4,0,16,false,"kPa", ConvertLinear<float,unsigned>(3276, 16383, -100.0f, 100.0f));
SystemData<float> aiAlcTankPressure(2006,"aiAlcTankPressure","AI",0,0,2,0,2,6,0,16,false,"kPa", ConvertLinear<float,unsigned>(3276, 16383, -100.0f, 1000.0f));
SystemData<float> aiHFSupplyPressure(2008,"aiHFSupplyPressure","AI",0,0,2,0,2,8,0,16,false,"kPa", ConvertLinear<float,unsigned>(3276, 16383, -100.0f, 1000.0f));
SystemData<float> aiAlcGasLeak(2010,"aiAlcGasLeak","AI",0,0,1,0,2,10,0,16,false,"%lel", ConvertLinear<float,unsigned>(6540, 32701, 0.0f, 100.0f));
SystemData<float> aiHFGasboxPPM(2012,"aiHFGasboxPPM","AI",0,0,2,0,2,12,0,16,false,"ppm", ConvertLinear<float,unsigned>(6540, 32701, 0.0f, 10.0f));
SystemData<float> aiHFChamberPPM(2014,"aiHFChamberPPM","AI",0,0,2,0,2,14,0,16,false,"ppm", ConvertLinear<float,unsigned>(6540, 32701, 0.0f, 10.0f));


//AO


//Body Heater
SystemData<float> aiBodyHTTemp(4000, "aiBodyHTTemp","Heater",0,0,1,0,4,0,0,32,false,"Cels",ConvertMemery<float, unsigned>);
SystemData<unsigned> aiBodyHTErrCode(4008, "aiBodyHTErrCode","Heater",0,0,0,0,4,8,0,16,false);
SystemData<unsigned> diBodyHTPowRdy(4010, "diBodyHTPowRdy","Heater",0,1,0,0,4,10,0,1,false);
SystemData<unsigned> diBodyHTErr(4011, "diBodyHTErr","Heater",0,1,0,0,4,11,0,1,false);
SystemData<unsigned> doBodyHTEnable(4018, "doBodyHTEnable","Heater",0,1,0,0,4,18,0,1,true);
SystemData<float> aoBodyHTTempSet(4026, "aoBodyHTTempSet","Heater",0,1000,1,0,4,26,0,32,true,"Cels",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);

//Lid Heater
SystemData<float> aiLidHTTemp(5000, "aiLidHTTemp","Heater",0,0,1,0,5,0,0,32,false,"Cels",ConvertMemery<float, unsigned>);
SystemData<unsigned> aiLidHTErrCode(5008, "aiLidHTErrCode","Heater",0,0,0,0,5,8,0,16,false);
SystemData<unsigned> diLidHTPowRdy(5010, "diLidHTPowRdy","Heater",0,1,0,0,5,10,0,1,false);
SystemData<unsigned> diLidHTErr(5011, "diLidHTErr","Heater",0,1,0,0,5,11,0,1,false);
SystemData<unsigned> doLidHTEnable(5018, "doLidHTEnable","Heater",0,1,0,0,5,18,0,1,true);
SystemData<float> aoLidHTTempSet(5026, "aoLidHTTempSet","Heater",0,1000,1,0,5,26,0,32,true,"Cels",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);


//Chuck Heater
SystemData<float> aiChuckHTTemp(6000, "aiChuckHTTemp","Heater",0,0,1,0,6,0,0,32,false,"Cels",ConvertMemery<float, unsigned>);
SystemData<unsigned> aiChuckHTErrCode(6008, "aiChuckHTErrCode","Heater",0,0,0,0,6,8,0,16,false);
SystemData<unsigned> diChuckHTPowRdy(6010, "diChuckHTPowRdy","Heater",0,1,0,0,6,10,0,1,false);
SystemData<unsigned> diChuckHTErr(6011, "diChuckHTErr","Heater",0,1,0,0,6,11,0,1,false);
SystemData<unsigned> doChuckHTEnable(6018, "doChuckHTEnable","Heater",0,1,0,0,6,18,0,1,true);
SystemData<float> aoChuckHTTempSet(6026, "aoChuckHTTempSet","Heater",0,1000,1,0,6,26,0,32,true,"Cels",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);


//Motion
SystemData<unsigned> diHomeSensor(7000,"diHomeSensor","Motion",0,1,0,0,7,0,0,1,false);
SystemData<unsigned> diCWLimitStatus(7001,"diCWLimitStatus","Motion",0,1,0,0,7,1,0,1,false,"",ConvertNot<unsigned>);
SystemData<unsigned> diCCWLimitStatus(7002,"diCCWLimitStatus","Motion",0,1,0,0,7,2,0,1,false,"",ConvertNot<unsigned>);
SystemData<unsigned> diReachPosition(7003,"diReachPosition","Motion",0,1,0,0,7,3,0,1,false);
SystemData<unsigned> diDriverAlarm(7004,"diDriverAlarm","Motion",0,1,0,0,7,4,0,1,false,"",ConvertNot<unsigned>);
SystemData<unsigned> diAxisError(7008,"diAxisError","Motion",0,1,0,0,7,8,0,1,false);
SystemData<unsigned> diAxisErrStop(7009,"diAxisErrStop","Motion",0,1,0,0,7,9,0,1,false);
SystemData<unsigned> diAxisDisabled(7010,"diAxisDisabled","Motion",0,1,0,0,7,10,0,1,false);
SystemData<unsigned> diAxisStandStill(7011,"diAxisStandStill","Motion",0,1,0,0,7,11,0,1,false);
SystemData<unsigned> diAxisMoving(7012,"diAxisMoving","Motion",0,1,0,0,7,12,0,1,false);
SystemData<unsigned> diAxisNotMoving(7013,"diAxisNotMoving","Motion",0,1,0,0,7,13,0,1,false);
//SystemData<unsigned> diAxisStopCmdBusy(7018,"diAxisStopCmdBusy","Motion",0,1,0,0,7,18,0,1,false);
//SystemData<unsigned> diAxisStopDone(7019,"diAxisStopDone","Motion",0,1,0,0,7,19,0,1,false);
//SystemData<unsigned> diAxisRstCmdBusy(7020,"diAxisRstCmdBusy","Motion",0,1,0,0,7,20,0,1,false);
//SystemData<unsigned> diAxisRstDone(7021,"diAxisRstDone","Motion",0,1,0,0,7,21,0,1,false);
//SystemData<unsigned> diAxisServoCmdBusy(7022,"diAxisServoCmdBusy","Motion",0,1,0,0,7,22,0,1,false);
SystemData<unsigned> diAxisServoDone(7023,"diAxisServoDone","Motion",0,1,0,0,7,23,0,1,false);
SystemData<unsigned> diAxisHomeDone(7025,"diAxisHomeDone","Motion",0,1,0,0,7,25,0,1,false);
SystemData<unsigned> aiAxisErrCode(7040, "aiAxisErrCode","Motion",0,0,0,0,7,40,0,32,false);
SystemData<float> aiActualPosition(7044, "aiActualPosition","Motion",0,0,1,0,7,44,0,32,false,"deg",ConvertMemery<float, unsigned>);
SystemData<float> aiActualVelocity(7048, "aiActualVelocity","Motion",0,0,1,0,7,48,0,32,false,"deg/s",ConvertMemery<float, unsigned>);

SystemData<unsigned> aoAxisControl(7052, "aoAxisControl","Motion",0,255,0,0,7,52,0,8,true);
SystemData<unsigned> doAxisExecute(7053, "doAxisExecute","Motion",0,1,0,0,7,53,0,1,true);
SystemData<unsigned> doAxisServoOn(7054, "doAxisServoOn","Motion",0,1,0,0,7,54,0,1,true);
SystemData<unsigned> doAxisReset(7055, "doAxisReset","Motion",0,1,0,0,7,55,0,1,true);
SystemData<float> aoAxisStopDec(7061, "aoAxisStopDec","Motion",0,100,1,0,7,61,0,32,true,"deg/s2",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisStopJerk(7065, "aoAxisStopJerk","Motion",0,100000,1,0,7,65,0,32,true,"deg/s3",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisAcc(7069, "aoAxisAcc","Motion",0,100,1,0,7,69,0,32,true,"deg/s2",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisDec(7073, "aoAxisDec","Motion",0,100,1,0,7,73,0,32,true,"deg/s2",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisJerk(7077, "aoAxisJerk","Motion",0,100000,1,0,7,77,0,32,true,"deg/s3",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisVelocity(7081, "aoAxisVelocity","Motion",-100,100,1,0,7,81,0,32,true,"deg/s",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisPosition(7085, "aoAxisPosition","Motion",-160,160,1,0,7,85,0,32,true,"deg",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisDistance(7089, "aoAxisDistance","Motion",-320,320,1,0,7,89,0,32,true,"deg",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisVelOverride(7093, "aoAxisVelOverride","Motion",0,100,1,0,7,93,0,32,true,"%",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisPatrolPos1(7097, "aoAxisPatrolPos1","Motion",-160,160,1,0,7,97,0,32,true,"deg",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);
SystemData<float> aoAxisPatrolPos2(7101, "aoAxisPatrolPos1","Motion",-160,160,1,0,7,101,0,32,true,"deg",ConvertMemery<float, unsigned>,ConvertMemery<unsigned, float>);


//DeviceNet
SystemData<unsigned> aiAPCStatus(8000, "aiAPCStatus","DeviceNet",0,0,0,0,8,0,0,8,false);
SystemData<float> aiAPCPressure(8001, "aiAPCPressure","DeviceNet",0,0,2,0,8,1,0,32,false,"torr",ConvertLinear<float,unsigned>(0, 23405, 0.0f, 1000.0f));
SystemData<float> aiAPCPosition(8005, "aiAPCPosition","DeviceNet",0,0,1,0,8,5,0,16,false,"%",ConvertLinear<float,unsigned>(0, 32767, 0.0f, 100.0f));
SystemData<unsigned> aiAPCOverrideStatus(8007, "aiAPCOverrideStatus","DeviceNet",0,0,0,0,8,7,0,8,false);
SystemData<unsigned> aiVapMFCStatus(8008, "aiVapMFCStatus","DeviceNet",0,0,0,0,8,8,0,8,false);
SystemData<float> aiEtOHFlowrate(8009, "aiEtOHFlowrate","DeviceNet",0,0,0,0,8,9,0,16,false,"sccm",ConvertLinear2<float,unsigned>(0, 32767, 0.0f, 972.4f));
SystemData<unsigned> aiHFMFCStatus(8011, "aiHFMFCStatus","DeviceNet",0,0,0,0,8,11,0,8,false);
SystemData<float> aiHFFlowrate(8012, "aiHFFlowrate","DeviceNet",0,0,0,0,8,12,0,16,false,"sccm",ConvertLinear2<float,unsigned>(0, 32767, 0.0f, 2670.0f));
SystemData<unsigned> aiPurgeN2MFCStatus(8014, "aiPurgeN2MFCStatus","DeviceNet",0,0,0,0,8,14,0,8,false);
SystemData<float> aiPurgeN2Flowrate(8015, "aiPurgeN2Flowrate","DeviceNet",0,0,0,0,8,15,0,16,false,"sccm",ConvertLinear2<float,unsigned>(0, 32767, 0.0f, 6670.0f));
SystemData<unsigned> aiN2MFCStatus(8017, "aiN2MFCStatus","DeviceNet",0,0,0,0,8,17,0,8,false);
SystemData<float> aiN2Flowrate(8018, "aiN2Flowrate","DeviceNet",0,0,0,0,8,18,0,16,false,"sccm",ConvertLinear2<float,unsigned>(0, 32767, 0.0f, 6670.0f));
SystemData<float> aiProcChamPressure(8020, "aiProcChamPressure","DeviceNet",0,0,2,0,8,20,0,16,false,"torr",ConvertLinear<float,unsigned>(0, 23405, 0.0f, 1000.0f));
SystemData<unsigned> aiProcChamManoStatus(8022, "aiProcChamManoStatus","DeviceNet",0,0,0,0,8,22,0,8,false);
SystemData<float> aiExpChamPressure(8023, "aiExpChamPressure","DeviceNet",0,0,2,0,8,23,0,16,false,"torr",ConvertLinear<float,unsigned>(0, 23405, 0.0f, 1000.0f));
SystemData<unsigned> aiExpChamManoStatus(8025, "aiExpChamManoStatus","DeviceNet",0,0,0,0,8,25,0,8,false);

SystemData<float> aoAPCPressure(8026, "aoAPCPressure","DeviceNet",0,1000,1,0,8,26,0,16,true,"torr",ConvertLinear<float,unsigned>(0, 32767, 0.0f, 1000.0f),ConvertLinear<unsigned,float>(0.0f, 1000.0f, 0, 32767));
SystemData<unsigned> aoAPCOverride(8028, "aoAPCOverride","DeviceNet",0,255,0,0,8,28,0,8,true);
SystemData<unsigned> aoAPCControlMode(8029, "aoAPCControlMode","DeviceNet",0,255,0,0,8,29,0,8,true);
SystemData<float> aoEtOHFlowSetpoint(8030, "aoEtOHFlowSetpoint","DeviceNet",0,700,0,0,8,30,0,16,true,"sccm",ConvertLinear<float,unsigned>(0, 24576, 0.0f, 729.3f),ConvertLinear<unsigned,float>(0.0f, 729.3f, 0, 24576));
SystemData<float> aoHFFlowSetpoint(8032, "aoHFFlowSetpoint","DeviceNet",0,2000,0,0,8,32,0,16,true,"sccm",ConvertLinear<float,unsigned>(0, 24576, 0.0f, 2000.0f),ConvertLinear<unsigned,float>(0.0f, 2000.0f, 0, 24576));
SystemData<float> aoPurgeN2FlowSetpoint(8034, "aoPurgeN2FlowSetpoint","DeviceNet",0,5000,0,0,8,34,0,16,true,"sccm",ConvertLinear<float,unsigned>(0, 24576, 0.0f, 5000.0f),ConvertLinear<unsigned,float>(0.0f, 5000.0f, 0, 24576));
SystemData<float> aoN2FlowSetpoint(8036, "aoN2FlowSetpoint","DeviceNet",0,5000,0,0,8,36,0,16,true,"sccm",ConvertLinear<float,unsigned>(0, 24576, 0.0f, 5000.0f),ConvertLinear<unsigned,float>(0.0f, 5000.0f, 0, 24576));
SystemData<float> aoAPCPosition(8038, "aoAPCPosition","DeviceNet",0,7000,1,0,8,38,0,16,true,"%",ConvertLinear<float,unsigned>(0, 32767, 0.0f, 100.0f),ConvertLinear<unsigned,float>(0.0f, 100.0f, 0, 32767));




//system defined
SystemData<unsigned> TotalSteps(100011, "TotalSteps", "System", 0, 1000);
SystemData<unsigned> CurrentStep(100012, "CurrentStep", "System", 0, 1000);
SystemData<std::string> RecipeName(100013, "RecipeName", "System");
SystemData<unsigned> RecipeTotalTime(100014, "RecipeTotalTime", "System", 0, 10000, 0, "s");
SystemData<unsigned> RecipeElapseTime(100015, "RecipeElapseTime", "System", 0, 10000, 0, "s");
SystemData<unsigned> CurrentStepTime(100016, "CurrentStepTime", "System", 0, 3000, 0, "s");
SystemData<unsigned> StepElapseTime(100017, "StepElapseTime", "System", 0, 3000, 0, "s");

SystemData<unsigned> LoadUnloadState(100018, "LoadUnloadState", "System", 0, 1);
SystemData<unsigned> LoadUnloadSkip(100019, "LoadUnloadSkip", "System", 0, 1);
SystemData<unsigned> LoadUnloadOK(100020, "LoadUnloadOK", "System", 0, 1);
SystemData<unsigned> WaferProcessedCount(100021, "WaferProcessedCount", "System", 0, 1000);
SystemData<std::string> WaferBatchID(100022, "WaferBatchID", "System");
SystemData<unsigned> WaferBatchCount(100023, "WaferBatchCount", "System", 0, 100000);
SystemData<unsigned> WaferTotalCount(100024, "WaferTotalCount", "System", 0, 100000);

SystemData<std::string> LeakCheckResult(100030, "LeakCheckResult", "System");
SystemData<float> LeakRate(100031, "LeakRate", "System", 0, 1000, 2, "torr/min");

SystemData<std::string> Slot1WaferID(100040, "Slot1WaferID", "System");
SystemData<std::string> Slot2WaferID(100041, "Slot2WaferID", "System");
SystemData<std::string> Slot3WaferID(100042, "Slot3WaferID", "System");
SystemData<unsigned> Slot1WaferState(100043, "Slot1WaferState", "System", 0, 5);
SystemData<unsigned> Slot2WaferState(100044, "Slot2WaferState", "System", 0, 5);
SystemData<unsigned> Slot3WaferState(100045, "Slot3WaferState", "System", 0, 5);
SystemData<std::string> ForkWaferID(100046, "ForkWaferID", "System");
SystemData<unsigned> ForkWaferState(100047, "ForkWaferState", "System", 0, 5);

SystemData<unsigned> ProcUnitState(100050, "ProcUnitState", "System", 0, 2);
SystemData<unsigned> ProcUnitRetryable(100051, "ProcUnitRetryable", "System", 0, 1);
SystemData<unsigned> ProcUnitCommand(100052, "ProcUnitCommand", "System", 0, 10000);
SystemData<unsigned> ProcUnitParam1(100053, "ProcUnitParam1", "System", 0, 10000);
SystemData<unsigned> ProcUnitParam2(100054, "ProcUnitParam2", "System", 0, 10000);
SystemData<std::string> ProcUnitStepName(100055, "ProcUnitStepName", "System");

SystemData<unsigned> ProcChamberDirty(100060, "ProcChamberDirty", "System", 0, 1);
SystemData<unsigned> ExpChamberDirty(100061, "ExpChamberDirty", "System", 0, 1);

}

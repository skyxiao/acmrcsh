/*
 * Parameter.cpp
 *
 *  Created on: 2014��2��20��
 *      Author: acm
 */

#include "ParameterItem.h"

namespace Parameters
{

//define all needed parameters here
//temperature
ParameterItem<float> LidTemp(20, "LidTemp", "Heater", "Chamber lid temperature", 0, 70, 40, "Cels");
ParameterItem<float> BodyTemp(21, "BodyTemp", "Heater", "Chamber body temperature", 0, 70, 40, "Cels");
ParameterItem<float> ChuckTemp(22, "ChuckTemp", "Heater", "Chamber chuck temperature", 0, 70, 40, "Cels");
ParameterItem<float> TempWarnOffset(25, "TempWarnOffset", "Heater", "Relative temperature offset of warning to setting value.", 0, 10, 0.5, "Cels");
ParameterItem<float> TempAlarmOffset(26, "TempAlarmOffset", "Heater", "Relative temperature offset of alarm to setting value.", 0, 10, 1.0, "Cels");
ParameterItem<float> TempMonitorDelay(27, "TempMonitorDelay", "Heater", "Delay time from recipe step start.", 0, 20, 5, "s");
ParameterItem<float> GasboxWarnTemp(28, "GasboxWarnTemp", "Heater", "Warning temperature for gasbox.", 0, 100, 40, "Cels");
ParameterItem<float> PipeWarnTemp(29, "PipeWarnTemp", "Heater", "Warning temperature for pipe.", 0, 100, 40, "Cels");
ParameterItem<float> TempTriggerTimeout(30, "TempTriggerTimeout", "Heater", "Time duration of temperature out of range before warn/alarm trigger.", 0, 300, 2, "s");


//Chuck position
ParameterItem<float> ChuckPos1(1001,"ChuckPos1","Chuck","chuck position1",-160,160,-120,"deg");
ParameterItem<float> ChuckPos2(1002,"ChuckPos2","Chuck","chuck position2",-160,160,0,"deg");
ParameterItem<float> ChuckPos3(1003,"ChuckPos3","Chuck","chuck position3",-160,160,120,"deg");


//Flow
ParameterItem<unsigned> FlowWarnProportion(2011,"FlowWarnProportion","Flow","Warning proportion of flow rate.",0,100,5,"%");
ParameterItem<unsigned> FlowWarnMinimum(2012,"FlowWarnMinimum","Flow","Minimum warning of flow rate.",0,1000,50,"sccm");
ParameterItem<unsigned> FlowAlarmProportion(2013,"FlowAlarmProportion","Flow","Alarm proportion of flow rate.",0,100,5,"%");
ParameterItem<unsigned> FlowAlarmMinimum(2014,"FlowAlarmMinimum","Flow","Minimum alarm of flow rate.",0,1000,50,"sccm");
ParameterItem<float> FlowMonitorDelay(2015, "FlowMonitorDelay", "Flow", "Delay time for gas flow.", 0, 300, 5, "s");
ParameterItem<float> FlowTriggerTimeout(2017, "FlowTriggerTimeout", "Flow", "Time duration of gas flow out of range before warn/alarm trigger.", 0, 300, 2, "s");
ParameterItem<unsigned> EtOHWarnProportion(2021,"EtOHWarnProportion","Flow","Warning proportion of EtOH flow rate.",0,100,10,"%");
ParameterItem<unsigned> EtOHWarnMinimum(2022,"EtOHWarnMinimum","Flow","Minimum warning of EtOH flow rate.",0,1000,50,"sccm");
ParameterItem<unsigned> EtOHAlarmProportion(2023,"EtOHAlarmProportion","Flow","Alarm proportion of EtOH flow rate.",0,100,20,"%");
ParameterItem<unsigned> EtOHAlarmMinimum(2024,"EtOHAlarmMinimum","Flow","Minimum alarm of EtOH flow rate.",0,1000,50,"sccm");
ParameterItem<float> EtOHMonitorDelay(2025, "EtOHMonitorDelay", "Flow", "Delay time for EtOH flow.", 0, 300, 20, "s");
ParameterItem<float> EtOHTriggerTimeout(2027, "EtOHTriggerTimeout", "Flow", "Time duration of EtOH flow out of range before warn/alarm trigger.", 0, 300, 5, "s");


//Pressure
ParameterItem<float> ATMPressure(3001,"ATMPressure","Pressure","ATM pressure",600,800,730,"torr");
ParameterItem<float> VacuumPressure(3002,"VacuumPressure","Pressure","Vacuum pressure",0,100,10,"torr");
ParameterItem<float> PressureDiffAllowance(3003,"PressureDiffAllowance","Pressure","Pressure difference allowance",0,100,60,"torr");
ParameterItem<float> PressureWarnOffset(3005, "PressureWarnOffset", "Pressure", "Relative pressure offset of warning to setting value.", 0, 100, 10, "torr");
ParameterItem<float> PressureAlarmOffset(3006, "PressureAlarmOffset", "Pressure", "Relative pressure offset of alarm to setting value.", 0, 100, 20, "torr");
ParameterItem<float> PressureMonitorDelay(3007, "PressureMonitorDelay", "Pressure", "Delay time from recipe step start.", 0, 300, 5, "s");
ParameterItem<float> PressureTriggerTimeout(3009, "PressureTriggerTimeout", "Pressure", "Time duration of pressure out of range before warn/alarm trigger.", 0, 300, 2, "s");


//Purge
ParameterItem<unsigned> N2PurgeFlow(4000,"N2PurgeFlow","Purge","N2 purge flowrate.",0,5000,1000,"sccm");
ParameterItem<unsigned> PurgeRepeatTimes(4002,"PurgeRepeatTimes","Purge","Repeat times for purge.",3,20,3,"times");
ParameterItem<float> PurgeTargetPressure(4003,"PurgeTargetPressure","Purge","Purge target pressure.",0,500,400,"torr");
//ParameterItem<unsigned> FlushTime(4004,"FlushTime","Purge","Flush time after purge.",0,100,30,"s");
ParameterItem<unsigned> FlushFlowrate(4005,"FlushFlowrate","Purge","Flush flowrate of N2.",0,5000,1000,"sccm");
ParameterItem<unsigned> PurgeHoldTime(4006,"PurgeHoldTime","Purge","Hold time after reaching PurgeTargetPressure.",0,100,5,"s");
ParameterItem<unsigned> PumpHoldTime(4007,"PumpHoldTime","Purge","Hold time after reaching PumpDownTargetPressure.",0,100,5,"s");
ParameterItem<unsigned> HFPurgeFlow(4009,"HFPurgeFlow","Purge","HF purge N2 flowrate.",0,2000,1000,"sccm");
ParameterItem<unsigned> EtOHPurgeFlow(4010,"EtOHPurgeFlow","Purge","EtOH purge N2 flowrate.",0,700,500,"sccm");


//Valve
ParameterItem<unsigned> GateValveTimeout(5000,"GateValveTimeout","Valve","Open/close gate valve timeout.",0,100,10,"s");


//Motion
ParameterItem<unsigned> PinTimeout(6004,"PinTimeout","Motion","Pin up/down timeout.",0,100,10,"s");
ParameterItem<unsigned> RotateTimeout(6014,"RotateTimeout","Motion","Chuck rotation timeout.",0,100,20,"s");
ParameterItem<unsigned> HomingTimeout(6015,"HomingTimeout","Motion","Chuck rotation timeout.",0,100,60,"s");
ParameterItem<unsigned> ArmTimeout(6024,"ArmTimeout","Motion","Arm move in/out timeout.",0,100,60,"s");
ParameterItem<unsigned> ForkTimeout(6025,"ForkTimeout","Motion","Rotate fork timeout.",0,100,30,"s");
ParameterItem<float> AxisStopDeceleration(6040,"AxisStopDeceleration","Motion","Deceleration for axis stopping.",0,500,100,"deg/s2");
ParameterItem<float> AxisDeceleration(6041,"AxisDeceleration","Motion","Axis deceleration.",0,500,100,"deg/s2");
ParameterItem<float> AxisAcceleration(6042,"AxisAcceleration","Motion","Axis acceleration.",0,500,100,"deg/s2");
ParameterItem<float> AxisPatrolPos1(6054,"AxisPatrolPos1","Motion","Axis patrol position 1",-160,160,-160,"deg");
ParameterItem<float> AxisPatrolPos2(6055,"AxisPatrolPos2","Motion","Axis patrol position 2",-160,160,160,"deg");
ParameterItem<float> RotateSpeed(6060,"RotateSpeed","Motion","Speed of axis rotate forward or backward.",0,100,10,"deg/s");
ParameterItem<float> PositionErrorLimit(6070,"PositionErrorLimit","Motion","Maximum error limit of chuck position.",0,10,2,"deg");


//Pump
ParameterItem<float> FastSlowSwitchPressure(7000,"FastSlowSwitchPressure","Pump","Turn on fast valve and close slow valve when pressure lower than this value.",0,700,400,"torr");
ParameterItem<float> PumpDownTargetPressure(7001,"PumpDownTargetPressure","Pump","Pump down procedure finished when pressure lower than this value.",0,100,0.1,"torr");
ParameterItem<unsigned> SlowPumpTimeout(7003,"SlowPumpTimeout","Pump","Pump timeout from ATM to FastSlowSwitchPressure.",0,100,60,"s");
ParameterItem<unsigned> FastPumpTimeout(7004,"FastPumpTimeout","Pump","Pump timeout from FastSlowSwitchPressure to Vacuum.",0,100,60,"s");
ParameterItem<unsigned> PumpExpTimeout(7005,"PumpExpTimeout","Pump","Pump expansion chamber timeout.",0,100,60,"s");
ParameterItem<unsigned> PostPumpTime(7007,"PostPumpTime","Pump","Pump time after process chamber pressure reaching VacuumPressure.",0,300,60,"s");


//Wafer
ParameterItem<unsigned> WaferSize(8000,"WaferSize","Wafer","Wafer size(150/200/300mm).",0,1000,300,"mm");


//LeakCheck
ParameterItem<float> LeakCheckPressure(9000,"LeakCheckPressure","LeakCheck","Initial pressure of leak checking.",0,100,10,"torr");
ParameterItem<unsigned> LeakCheckPumpHoldTime(9001,"LeakCheckPumpHoldTime","LeakCheck","Time for keep pumping after reaching leak check pressure.",0,600,30,"s");
ParameterItem<unsigned> LeakCheckTime(9002,"LeakCheckTime","LeakCheck","Leak check time.",0,3600,1200,"s");
ParameterItem<float> LeakCheckThreshold(9003,"LeakCheckThreshold","LeakCheck","Leak check threshold, pass if less than this value.",0,10,5,"torr/min");


//Vent
ParameterItem<unsigned> VentProcTimeout(10000,"VentProcTimeout","Vent","Vent process chamber timeout.",0,100,60,"s");
ParameterItem<unsigned> VentExpTimeout(10001,"VentExpTimeout","Vent","Vent expansion chamber timeout.",0,100,60,"s");
ParameterItem<unsigned> ExpVentFlowrate(10002,"ExpVentFlowrate","Vent","Expansion chamber vent flow rate.",0,5000,2000,"sccm");

}




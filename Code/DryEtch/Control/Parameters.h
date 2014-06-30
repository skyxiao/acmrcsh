#ifndef PARAMETERS_H_
#define PARAMETERS_H_

#include "ParameterItem.h"

namespace Parameters
{

//temperature
extern ParameterItem<float> LidTemp;
extern ParameterItem<float> BodyTemp;
extern ParameterItem<float> ChuckTemp;
extern ParameterItem<float> TempWarnOffset;
extern ParameterItem<float> TempAlarmOffset;
extern ParameterItem<float> TempMonitorDelay;

//Chuck position
extern ParameterItem<float> ChuckPos1;
extern ParameterItem<float> ChuckPos2;
extern ParameterItem<float> ChuckPos3;


//Flow
extern ParameterItem<unsigned> FlowWarnProportion;
extern ParameterItem<unsigned> FlowWarnMinimum;
extern ParameterItem<unsigned> FlowAlarmProportion;
extern ParameterItem<unsigned> FlowAlarmMinimum;
extern ParameterItem<float> FlowMonitorDelay;
extern ParameterItem<unsigned> EtOHWarnProportion;
extern ParameterItem<unsigned> EtOHWarnMinimum;
extern ParameterItem<unsigned> EtOHAlarmProportion;
extern ParameterItem<unsigned> EtOHAlarmMinimum;
extern ParameterItem<float> EtOHMonitorDelay;


//Pressure
extern ParameterItem<float> ATMPressure;
extern ParameterItem<float> VacuumPressure;
extern ParameterItem<float> PressureDiffAllowance;
extern ParameterItem<float> PressureWarnOffset;
extern ParameterItem<float> PressureAlarmOffset;
extern ParameterItem<float> PressureMonitorDelay;


//Purge
extern ParameterItem<unsigned> N2PurgeFlow;
extern ParameterItem<unsigned> PurgeRepeatTimes;
extern ParameterItem<float> PurgeTargetPressure;
extern ParameterItem<unsigned> FlushTime;
extern ParameterItem<unsigned> FlushFlowrate;
extern ParameterItem<unsigned> PurgeHoldTime;
extern ParameterItem<unsigned> PumpHoldTime;
extern ParameterItem<unsigned> HFPurgeFlow;
extern ParameterItem<unsigned> EtOHPurgeFlow;


//Valve
extern ParameterItem<unsigned> GateValveTimeout;


//Motion
extern ParameterItem<unsigned> PinTimeout;
extern ParameterItem<unsigned> RotateTimeout;
extern ParameterItem<unsigned> HomingTimeout;
extern ParameterItem<unsigned> ArmTimeout;
extern ParameterItem<unsigned> ForkTimeout;
extern ParameterItem<float> AxisStopDeceleration;
extern ParameterItem<float> AxisDeceleration;
extern ParameterItem<float> AxisAcceleration;
extern ParameterItem<float> AxisPatrolPos1;
extern ParameterItem<float> AxisPatrolPos2;
extern ParameterItem<float> RotateSpeed;
extern ParameterItem<float> PositionErrorLimit;

//Pump
extern ParameterItem<float> FastSlowSwitchPressure;
extern ParameterItem<float> PumpDownTargetPressure;
extern ParameterItem<unsigned> SlowPumpTimeout;
extern ParameterItem<unsigned> FastPumpTimeout;
extern ParameterItem<unsigned> PumpExpTimeout;


//Wafer
extern ParameterItem<unsigned> WaferSize;


//LeakCheck
extern ParameterItem<float> LeakCheckPressure;
extern ParameterItem<unsigned> LeakCheckPumpHoldTime;
extern ParameterItem<unsigned> LeakCheckTime;
extern ParameterItem<float> LeakCheckThreshold;


//Vent
extern ParameterItem<unsigned> VentProcTimeout;
extern ParameterItem<unsigned> VentExpTimeout;
extern ParameterItem<unsigned> ExpVentFlowrate;


}

#endif

/*
 * ProcessChamber.cpp
 *
 *  Created on: 2014Äê2ÔÂ20ÈÕ
 *      Author: acm
 */

#include "Event.h"
#include "ProcessUnit.h"
#include "Parameters.h"
#include "Data.h"
#include "WaferManager.h"
#include "Monitor.h"
#include "RecipeManager.h"
#include "DataRecorder.h"

using namespace boost::chrono;

ProcessUnit::ProcessUnit(int id, const std::string& name) :
		SmartUnit(id, name), m_leak_check_init_pressure(0)
{
}

ProcessUnit::~ProcessUnit()
{
}

void ProcessUnit::Initialize()
{
	//base class initialize
	SmartUnit::Initialize();

	if(Data::diPrcCbDoorClose == 0)
	{
		Data::ProcChamberDirty = 0;
	}
	else
	{
		Data::ProcChamberDirty = 1;
	}

	if(Data::doExpCbSupplyCbVal == 1)
	{
		Data::ExpChamberDirty = (unsigned)Data::ProcChamberDirty;
	}
	else
	{
		Data::ExpChamberDirty = 1;
	}
	//initialize hardware parameters

	//initialize monitor
	Monitor::Instance().Add("HF flowrate", [&](){return (float)Data::aiHFFlowrate;});
	Monitor::Instance().Add("EtOH flowrate", [&](){return (float)Data::aiEtOHFlowrate;});
	Monitor::Instance().Add("N2 flowrate", [&](){return (float)Data::aiN2Flowrate;});
	Monitor::Instance().Add("Process pressure", [&](){return (float)Data::aiProcChamPressure;});
	Monitor::Instance().Add("Chuck temperature", [&](){return (float)Data::aiChuckHTTemp;});
	Monitor::Instance().Add("Body temperature", [&](){return (float)Data::aiBodyHTTemp;});
	Monitor::Instance().Add("Lid temperature", [&](){return (float)Data::aiLidHTTemp;});

	//Initialize data recorder
	DataRecorder::Instance().Add("HF", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("HF", 1000, [&](){return (float)Data::aiHFFlowrate;})));
	DataRecorder::Instance().Add("EtOH", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("EtOH", 1000, [&](){return (float)Data::aiEtOHFlowrate;})));
	DataRecorder::Instance().Add("N2", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("N2", 1000, [&](){return (float)Data::aiN2Flowrate;})));
	DataRecorder::Instance().Add("Pressure", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Pressure", 1000, [&](){return (float)Data::aiProcChamPressure;})));
	DataRecorder::Instance().Add("Chuck", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Chuck", 1000, [&](){return (float)Data::aiChuckHTTemp;})));
	DataRecorder::Instance().Add("Lid", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Lid", 1000, [&](){return (float)Data::aiLidHTTemp;})));
	DataRecorder::Instance().Add("Body", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Body", 1000, [&](){return (float)Data::aiBodyHTTemp;})));
	DataRecorder::Instance().Add("Tank", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Tank", 1000, [&](){return (float)Data::aiAlcTankPressure;})));
	DataRecorder::Instance().Add("ChuckLF", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Chuck", 60000, [&](){return (float)Data::aiChuckHTTemp;}, true)));
	DataRecorder::Instance().Add("LidLF", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Lid", 60000, [&](){return (float)Data::aiLidHTTemp;}, true)));
	DataRecorder::Instance().Add("BodyLF", boost::shared_ptr<RecordItem>(new 
		IntervalRecordItem("Body", 60000, [&](){return (float)Data::aiBodyHTTemp;}, true)));
}

void ProcessUnit::Terminate()
{
	//terminate data recorder
	DataRecorder::Instance().Clear();

	//terminate process monitor
	Monitor::Instance().Clear();

	//base class terminate
	SmartUnit::Terminate();
}

UnitTask ProcessUnit::GetNextTask()
{
	UnitTask last_task = (m_task.command == COMMAND_NONE) ? m_last_task : m_task;
	if (last_task.command == COMMAND_NONE || Data::diAxisHomeDone == 0)
	{
		return UnitTask{COMMAND_HOME, 0, 0};
	}

	if(Data::diBodyHTPowRdy==0 || Data::diLidHTPowRdy==0 || Data::diChuckHTPowRdy==0)
	{
		return UnitTask{COMMAND_TURNON_HEATER, 0, 0};
	}

	if(Data::aiExpChamPressure > Parameters::VacuumPressure)
	{
		return UnitTask{COMMAND_PUMP, 1, 0};
	}

	if (WaferManager::Instance().UnitHasWafer(m_id))
	{
		if(WaferManager::Instance().IsWaferAllProcessed(m_id, 0x7))
		{
			if(Data::ProcChamberDirty == 1)
			{
				return UnitTask{COMMAND_PURGE, 0, 0};
			}
			else
			{
				if(Data::aiProcChamPressure > Parameters::ATMPressure)
				{
					return UnitTask{COMMAND_UNLOAD, 0, 0};
				}
				else
				{
					return UnitTask{COMMAND_VENT, 0, 0};
				}
			}
		}
		else
		{
			if(Data::aiProcChamPressure > Parameters::VacuumPressure)
			{
				return UnitTask{COMMAND_PUMP, 0, 0};
			}
			else
			{
				if(!alarm_check())
				{
					return UnitTask{COMMAND_OFFLINE, 0, 0};
				}
				else
					return UnitTask{COMMAND_PROCESS, 0, 0};
			}
		}
	}
	else
	{
		if(Data::ProcChamberDirty == 1)
		{
			if(Data::aiProcChamPressure > Parameters::VacuumPressure)
			{
				return UnitTask{COMMAND_PUMP, 0, 0};
			}
			else
			{
				return UnitTask{COMMAND_PURGE, 0, 0};
			}
		}
		else
		{
			if(Data::aiProcChamPressure < Parameters::ATMPressure)
			{
				return UnitTask{COMMAND_VENT, 0, 0};
			}
			else
			{
				return UnitTask{COMMAND_LOAD, 0, 0};
			}
		}
	}

	return UnitTask{COMMAND_NONE, 0, 0};
}

void ProcessUnit::UpdateUnitInfo()
{
	Data::ProcUnitState = m_state;
	Data::ProcUnitCommand = m_task.command;
	Data::ProcUnitParam1 = m_task.para1;
	Data::ProcUnitParam2 = m_task.para2;
	Data::ProcUnitRetryable = m_can_retry ? 1 : 0;
	Data::ProcUnitStepName = m_step_name;
}

void ProcessUnit::shut_all_chemical()
{
	Data::doHFFacSupplyVal = 0;
	Data::doVapSupplyN2Valve = 0;
	Data::doAlcMFCVal1 = 0;
	Data::doAlcMFCVal2 = 0;
	Data::aoEtOHFlowSetpoint = 0;
	Data::doHFMFCVal1 = 0;
	Data::doHFMFCVal2 = 0;
	Data::aoHFFlowSetpoint = 0;
	Data::doN2MFCVal1 = 0;
	Data::doN2MFCVal2 = 0;
	Data::aoN2FlowSetpoint = 0;
	Data::doPurgeN2MFCVal1 = 0;
	Data::doPurgeN2MFCVal2 = 0;
	Data::aoPurgeN2FlowSetpoint = 0;
	Data::doHFRequest = 0;
	Data::doVapInletVal = 0;
}

void ProcessUnit::open_apc()
{
	Data::aoAPCControlMode = APCControl_Position;
	Data::aoAPCPosition = 100;
}

void ProcessUnit::stop_motor()
{
	Data::aoAxisControl = AxisControl_Stop;
	Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);
}

void ProcessUnit::SafeHandle()
{
	//do something to insure safety and change state information
	if(m_task.command == COMMAND_PROCESS)
	{
		shut_all_chemical();
		open_apc();
		stop_motor();
		WaferManager::Instance().ProcessAbort(m_id);
		DataRecorder::Instance().Disable("HF");
		DataRecorder::Instance().Disable("EtOH");
		DataRecorder::Instance().Disable("N2");
		DataRecorder::Instance().Disable("Pressure");
		DataRecorder::Instance().Disable("Chuck");
		DataRecorder::Instance().Disable("Lid");
		DataRecorder::Instance().Disable("Body");
		DataRecorder::Instance().Disable("Tank");
		DataRecorder::Instance().Enable("ChuckLF");
		DataRecorder::Instance().Enable("LidLF");
		DataRecorder::Instance().Enable("BodyLF");
		Monitor::Instance().DisableAll();
	}
	else if(m_task.command == COMMAND_VENT)
	{
		Data::doN2SupplyProcVal = 0;
	}
	else if(m_task.command == COMMAND_PURGE)
	{
		if(m_task.para1 == 0)
		{
			Data::doN2SupplyProcVal = 0;
		}
		else if(m_task.para1 == 1)
		{
			Data::doPurgeN2MFCVal1 = 0;
			Data::doPurgeN2MFCVal2 = 0;
			Data::aoPurgeN2FlowSetpoint = 0;
		}
		else if(m_task.para1 == 2)
		{
			Data::doN2PurgeHFVal = 0;
			Data::doHFMFCVal1 = 0;
			Data::doHFMFCVal2 = 0;
			Data::doHFMFCVal3 = 0;
			Data::aoHFFlowSetpoint = 0;
		}
		else if(m_task.para1 == 3)
		{
			Data::doPurgeAlcTank = 0;
			Data::doAlcMFCVal1 = 0;
			Data::doAlcMFCVal2 = 0;
			Data::doAlcMFCVal3 = 0;
			Data::doVapInletVal = 0;
			Data::aoEtOHFlowSetpoint = 0;
		}
	}
}

void ProcessUnit::Notify(const std::string& msg)
{
	EVT::GenericInfo.Report(msg);
}

void ProcessUnit::OnAbort()
{
	SafeHandle();

	if(m_task.command == COMMAND_LEAKCHECK)
	{
		Data::LeakCheckResult = "ABORTED";
	}
}

bool ProcessUnit::alarm_check()
{
	if(Data::diAlcTkLow == 1)
	{
		EVT::AlcoholTankLow.Report();
		return false;
	}
	else if(Data::diAlcPrsLLmt == 1)
	{
		EVT::AlcGasPressureLow.Report();
		return false;
	}
	else if(Data::diN2FacSplPrsULmt == 1)
	{
		EVT::N2FacPressureHigh.Report();
		return false;
	}
	else if(Data::diN2FacSplPrsLLmt == 1)
	{
		EVT::N2FacPressureLow.Report();
		return false;
	}
	else if(Data::diFacVPrsSwtLLmt == 1)
	{
		EVT::FacVacuumPressureLow.Report();
		return false;
	}
	else if(Data::diExpCbVPrsSwtLLmt == 1)
	{
		EVT::ExpVacuumPressureLow.Report();
		return false;
	}
	else if(Data::diVaporMHeaterAlarm == 1)
	{
		EVT::VapMainHeaterAlarm.Report();
		return false;
	}
	else if(Data::diVaporVHeaterAlarm == 1)
	{
		EVT::VapVapHeaterAlarm.Report();
		return false;
	}
	else if(Data::diHeartbeatFail == 1)
	{
		EVT::HeartbeatFail.Report();
		return false;
	}
	else if(Data::diExhaustAlarm == 1)
	{
		EVT::GasboxExhaustPresAlarm.Report();
		return false;
	}
	else if(Data::diFacInletVPrsULmt == 1)
	{
		EVT::FacMainInletVacPresHigh.Report();
		return false;
	}
	else if(Data::diAlcoholLeak == 1)
	{
		EVT::AlcoholLeak.Report();
		return false;
	}
	else if(Data::diAlcoholGasLeak == 1)
	{
		EVT::AlcoholGasLeak.Report();
		return false;
	}
	else if(Data::diExhaustPresAlarm == 1)
	{
		EVT::FrameExhaustPresAlarm.Report();
		return false;
	}
	else if(Data::diGasboxDoorClose == 0)
	{
		EVT::GasboxDoorOpen.Report();
		return false;
	}
	else if(Data::diGasBoxHFLeak == 1)
	{
		EVT::GasboxHFLeak.Report();
		return false;
	}
	else if(Data::diHWInterlock == 1)
	{
		EVT::HardwareInterlock.Report();
		return false;
	}
	else if(Data::diVPumpWarning == 1)
	{
		EVT::PumpWarning.Report();
		return false;
	}
	else if(Data::diVPumpAlarm == 1)
	{
		EVT::PumpAlarm.Report();
		return false;
	}
	else if(Data::diPlumbing1Alarm == 1 || Data::diPlumbing2Alarm == 1 || Data::diPlumbing3Alarm == 1 || 
		Data::diPlumbing4Alarm == 1 || Data::diPlumbing5Alarm == 1)
	{
		EVT::PlumbingHeaterAlarm.Report();
		return false;
	}
	else if(Data::diProcCbHFLeak == 1)
	{
		EVT::ChamberHFLeak.Report();
		return false;
	}

	return true;
}

bool ProcessUnit::OnlinePrecheck()
{
	float error_offset = Parameters::TempWarnOffset;
	float lid_offset = fabs(Data::aiLidHTTemp - Parameters::LidTemp);
	float body_offset = fabs(Data::aiBodyHTTemp - Parameters::BodyTemp);
	float chuck_offset = fabs(Data::aiChuckHTTemp - Parameters::ChuckTemp);
	if(lid_offset>error_offset || body_offset>error_offset || chuck_offset>error_offset)
	{
		EVT::HeaterTempOutRange.Report();
		return false;
	}

	if(!alarm_check())
	{
		return false;
	}

	Data::doPurgeAlcTank = 1;

	return true;
}

bool ProcessUnit::TaskPrecheck(const UnitTask& task)
{
//	if(Data::diHWInterlock == 1)
//	{
//		EVT::GenericWarning.Report("Hardware interlock triggered.");
//		return false;
//	}

	if(task.command != COMMAND_HOME && Data::diAxisHomeDone == 0)
	{
		EVT::GenericWarning.Report("Chuck motor need homing.");
		return false;
	}

	return true;
}

void ProcessUnit::TranslateTask(const UnitTask& task)
{
	switch (task.command)
	{
	case COMMAND_HOME:
		OnHome();
		break;
	case COMMAND_LOAD:
		OnLoad();
		break;
	case COMMAND_PROCESS:
		OnProcess();
		break;
	case COMMAND_UNLOAD:
		OnUnload();
		break;
	case COMMAND_PINUP:
		OnPinUp();
		break;
	case COMMAND_PINDOWN:
		OnPinDown();
		break;
	case COMMAND_EXTEND_ARM:
		OnExtendArm();
		break;
	case COMMAND_RETRACT_ARM:
		OnRetractArm();
		break;
	case COMMAND_FORK_VERTICAL:
		OnForkVertical();
		break;
	case COMMAND_FORK_HORIZONTAL:
		OnForkHorizontal();
		break;
	case COMMAND_ROTATE_FORWARD:
		OnRotateForward();
		break;
	case COMMAND_ROTATE_BACKWARD:
		OnRotateBackward();
		break;
	case COMMAND_PUMP:
		OnPump(task.para1);
		break;
	case COMMAND_VENT:
		OnVent(task.para1);
		break;
	case COMMAND_PURGE:
		OnPurge(task.para1);
		break;
	case COMMAND_TURNON_HEATER:
		OnTurnOnHeater();
		break;
	case COMMAND_TURNOFF_HEATER:
		OnTurnOffHeater();
		break;
	case COMMAND_OPEN_DOOR:
		OnOpenDoor();
		break;
	case COMMAND_CLOSE_DOOR:
		OnCloseDoor();
		break;
	case COMMAND_LEAKCHECK:
		OnLeakCheck(task.para1);
		break;
	default:
		;
	}
}

void ProcessUnit::OnHome()
{
	OnRetractArm();
	OnForkVertical();
	OnCloseDoor();
	OnPinDown();

	NEW_UNIT_STEP("home chuck motor", false)
		ADD_STEP_COMMAND([&](){	Data::doAxisServoOn = 1;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diAxisServoDone == 1;},
			Parameters::HomingTimeout,
			[&](){	EVT::HomingTimeout.Report();})
		ADD_STEP_COMMAND([&]()
		{	Data::aoAxisStopDec = Parameters::AxisStopDeceleration;
			Data::aoAxisDec = Parameters::AxisDeceleration;
			Data::aoAxisAcc = Parameters::AxisAcceleration;
			Data::aoAxisPatrolPos1 = Parameters::AxisPatrolPos1;
			Data::aoAxisPatrolPos2 = Parameters::AxisPatrolPos2;
			Data::aoAxisVelOverride = 100;})
		ADD_STEP_COMMAND([&]()
		{	Data::aoAxisControl = AxisControl_Homing;
			Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);})
		ADD_STEP_WAIT(1)
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diAxisHomeDone == 1 && Data::diAxisMoving == 0;},
			Parameters::HomingTimeout,
			[&](){	EVT::HomingTimeout.Report();})
	END_UNIT_STEP

	OnPinUp();
	OnPinDown();
}

void ProcessUnit::create_wafer()
{
	WaferSize wfs;
	switch(Parameters::WaferSize)
	{
	case 100:
		wfs = WaferSize_100mm;
		break;
	case 150:
		wfs = WaferSize_150mm;
		break;
	case 200:
		wfs = WaferSize_200mm;
		break;
	case 300:
		wfs = WaferSize_300mm;
		break;
	default:
		EVT::UnknownWaferSize.Report();
		return;
	}

	WaferManager::Instance().CreateWafer(1, 0, Data::WaferBatchID, wfs);
}

void ProcessUnit::OnLoad()
{
	float pos[3];
	pos[0] = Parameters::ChuckPos1;
	pos[1] = Parameters::ChuckPos2;
	pos[2] = Parameters::ChuckPos3;

	if(!OnOpenDoor())
	{
		return;
	}

	std::stringstream ss;
	for(int i=2; i>=0; i--)
	{
		float position = pos[i];
		ss.str("");
		ss<<"rotate chuck to position "<<i+1;
		NEW_UNIT_STEP(ss.str(), false)
			auto f0 = [&, position]()
			{	Data::aoAxisPosition = position;
				Data::aoAxisVelocity = Parameters::RotateSpeed;
				Data::aoAxisControl = AxisControl_Absolute;
				Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);};
			ADD_STEP_COMMAND(f0)
			ADD_STEP_WAIT(1)
			auto f1 = [&, position]()->bool
			{	return Data::diAxisMoving == 0 && fabs(Data::aiActualPosition-position)<Parameters::PositionErrorLimit;};
			ADD_STEP_WAIT_CONDITION(f1,	Parameters::RotateTimeout,
				[position](){	EVT::RotateTimeout.Report(position);})
		END_UNIT_STEP

		NEW_UNIT_STEP("rotate fork horizontal", false)
			ADD_STEP_COMMAND([&]()
			{	Data::doForkHorizontal = 1;
				Data::doForkVertical = 0;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diForkVertical == 0 && Data::diForkHorizontal == 1;},
				Parameters::ForkTimeout,
				[&](){	EVT::ForkTimeout.Report();})
		END_UNIT_STEP

		ss.str("");
		ss<<"load wafer "<<i+1;
		NEW_UNIT_STEP(ss.str(), false)
			ADD_STEP_COMMAND([&](){Data::LoadUnloadState = 1;})
			ADD_STEP_WAIT_CONDITION([&]()
			{	return Data::LoadUnloadOK == 1;}, UINT_MAX,
				[&](){EVT::LoadUnloadTimeout.Report();})
			ADD_STEP_COMMAND([&]()
			{	Data::LoadUnloadOK = 0;
				Data::LoadUnloadState = 0;})
			ADD_STEP_COMMAND([this](){create_wafer();})
		END_UNIT_STEP

		NEW_UNIT_STEP("rotate fork vertical", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doForkHorizontal = 0;
				Data::doForkVertical = 1;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diForkVertical == 1 && Data::diForkHorizontal == 0;},
				Parameters::ForkTimeout,
				[&](){	EVT::ForkTimeout.Report();})
		END_UNIT_STEP

		NEW_UNIT_STEP("extend arm into chamber", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doArmOut = 0;
				Data::doArmIn = 1;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diArmIn == 1 && Data::diArmOut == 0;},
				Parameters::ArmTimeout,
				[&](){	EVT::ArmTimeout.Report("in");})
		END_UNIT_STEP

		NEW_UNIT_STEP("pin up", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doPinDown = 0;
				Data::doPinUp = 1;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diPinUp == 1 && Data::diPinDown == 0;},
				Parameters::PinTimeout,
				[&](){	EVT::PinTimeout.Report("up");})
			auto f2 = [this, i](){wafer_movein(i);};
			ADD_STEP_COMMAND(f2)
		END_UNIT_STEP

		NEW_UNIT_STEP("retract arm from chamber", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doArmOut = 1;
				Data::doArmIn = 0;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diArmIn == 0 && Data::diArmOut == 1;},
				Parameters::ArmTimeout,
				[&](){	EVT::ArmTimeout.Report("out");})
		END_UNIT_STEP

		NEW_UNIT_STEP("pin down", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doPinDown = 1;
				Data::doPinUp = 0;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diPinUp == 0 && Data::diPinDown == 1;},
				Parameters::PinTimeout,
				[&](){	EVT::PinTimeout.Report("down");})
		END_UNIT_STEP
	}

	OnCloseDoor();
}

void ProcessUnit::remove_wafer()
{
	WaferManager::Instance().RemoveWafer(1, 0);
}

void ProcessUnit::wafer_movein(unsigned short slot)
{
	WaferManager::Instance().Transfer(1, 0, m_id, slot);
}

void ProcessUnit::wafer_moveout(unsigned short slot)
{
	WaferManager::Instance().Transfer(m_id, slot, 1, 0);
}

void ProcessUnit::OnUnload()
{
	float pos[3];
	pos[0] = Parameters::ChuckPos1;
	pos[1] = Parameters::ChuckPos2;
	pos[2] = Parameters::ChuckPos3;

	if(!OnOpenDoor())
	{
		return;
	}

	std::stringstream ss;
	for(int i=0; i<3; i++)
	{
		float position = pos[i];
		ss.str("");
		ss<<"rotate chuck to position "<<i+1;
		NEW_UNIT_STEP(ss.str(), false)
			auto f0 = [&, position]()
			{	Data::aoAxisPosition = position;
				Data::aoAxisVelocity = Parameters::RotateSpeed;
				Data::aoAxisControl = AxisControl_Absolute;
				Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);};
			ADD_STEP_COMMAND(f0)
			ADD_STEP_WAIT(1)
			auto f1 = [&, position]()->bool
			{	return Data::diAxisMoving == 0 && fabs(Data::aiActualPosition-position)<Parameters::PositionErrorLimit;};
			ADD_STEP_WAIT_CONDITION(f1,	Parameters::RotateTimeout,
				[position](){	EVT::RotateTimeout.Report(position);})
		END_UNIT_STEP

		NEW_UNIT_STEP("pin up", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doPinDown = 0;
				Data::doPinUp = 1;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diPinUp == 1 && Data::diPinDown == 0;},
				Parameters::PinTimeout,
				[&](){	EVT::PinTimeout.Report("up");})
		END_UNIT_STEP

		NEW_UNIT_STEP("rotate fork vertical", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doForkHorizontal = 0;
				Data::doForkVertical = 1;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diForkVertical == 1 && Data::diForkHorizontal == 0;},
				Parameters::ForkTimeout,
				[&](){	EVT::ForkTimeout.Report();})
		END_UNIT_STEP

		NEW_UNIT_STEP("extend arm into chamber", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doArmOut = 0;
				Data::doArmIn = 1;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diArmIn == 1 && Data::diArmOut == 0;},
				Parameters::ArmTimeout,
				[&](){	EVT::ArmTimeout.Report("in");})
		END_UNIT_STEP

		NEW_UNIT_STEP("pin down", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doPinDown = 1;
				Data::doPinUp = 0;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diPinUp == 0 && Data::diPinDown == 1;},
				Parameters::PinTimeout,
				[&](){	EVT::PinTimeout.Report("down");})
			auto f2 = [this, i](){wafer_moveout(i);};
			ADD_STEP_COMMAND(f2)
		END_UNIT_STEP

		NEW_UNIT_STEP("retract arm from chamber", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doArmOut = 1;
				Data::doArmIn = 0;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diArmIn == 0 && Data::diArmOut == 1;},
				Parameters::ArmTimeout,
				[&](){	EVT::ArmTimeout.Report("out");})
		END_UNIT_STEP

		NEW_UNIT_STEP("rotate fork horizontal", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doForkHorizontal = 1;
				Data::doForkVertical = 0;})
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diForkVertical == 0 && Data::diForkHorizontal == 1;},
				Parameters::ForkTimeout,
				[&](){	EVT::ForkTimeout.Report();})
		END_UNIT_STEP

		ss.str("");
		ss<<"unload wafer "<<i+1;
		NEW_UNIT_STEP(ss.str(), false)
			ADD_STEP_COMMAND([&](){Data::LoadUnloadState = 1;})
			ADD_STEP_WAIT_CONDITION([&]()
			{	return Data::LoadUnloadOK == 1;}, UINT_MAX,
				[&](){EVT::LoadUnloadTimeout.Report();})
			ADD_STEP_COMMAND([&]()
			{	Data::LoadUnloadOK = 0;
				Data::LoadUnloadState = 0;})
			ADD_STEP_COMMAND([this](){remove_wafer();});
		END_UNIT_STEP
	}
}

void ProcessUnit::process_recipe_step(unsigned index, const RecipeStep& recipe_step)
{
	Data::CurrentStepTime = recipe_step.Duration();
	Data::CurrentStep = index + 1;
	Data::StepElapseTime = 0;
	m_step_start_time = boost::chrono::system_clock::now();
	if (recipe_step.Mode() == APCMode_Position)
	{
		Data::aoAPCControlMode = APCControl_Position;
		Data::aoAPCPosition = recipe_step.Position();
		Monitor::Instance().Disable("Process pressure");
	}
	else
	{
		Data::aoAPCControlMode = APCControl_Pressure;
		float pressure = recipe_step.Pressure();
		float pres_warn_offset = Parameters::PressureWarnOffset;
		float pres_alarm_offset = Parameters::PressureAlarmOffset;
		Data::aoAPCPressure = pressure;
		Monitor::Instance().Reset("Process pressure", Parameters::PressureMonitorDelay, pressure, 
			pres_warn_offset, pres_alarm_offset);
	}
	unsigned short rpm = recipe_step.RotateSpeed();
	if (rpm > 0)
	{
		Data::aoAxisVelocity = rpm * 360 / 60;
		Data::aoAxisControl = AxisControl_Patrol;
	}
	else
	{
		Data::aoAxisControl = AxisControl_Stop;
	}
	Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);

	float flowrate = 0;
	float flow_warn_offset = 0;
	float flow_alarm_offset = 0;
	BypassMode bps_mode = recipe_step.HFBypass();
	if (bps_mode == BypassMode_None)
	{
		Data::doExpCbHFInletVal = 0;
		Data::doVaHFValve = 0;
		Data::aoHFFlowSetpoint = 0;
		Monitor::Instance().Disable("HF flowrate");
	}
	else
	{

		flowrate = recipe_step.HFFlowrate();
		flow_warn_offset = 0.01 * flowrate * Parameters::FlowWarnProportion;
		flow_warn_offset = std::max<float>(Parameters::FlowWarnMinimum, flow_warn_offset);
		flow_alarm_offset = 0.01 * flowrate * Parameters::FlowAlarmProportion;
		flow_alarm_offset = std::max<float>(Parameters::FlowAlarmMinimum, flow_alarm_offset);
		Data::aoHFFlowSetpoint = flowrate;
		if (bps_mode == BypassMode_Bypass)
		{
			Data::doExpCbHFInletVal = 0;
			Data::doVaHFValve = 1;
			Monitor::Instance().Disable("HF flowrate");
		}
		else
		{
			Data::doExpCbHFInletVal = 1;
			Data::doVaHFValve = 0;
			Monitor::Instance().Reset("HF flowrate", Parameters::FlowMonitorDelay, flowrate, flow_warn_offset, flow_alarm_offset);
		}
	}
	Data::doHFMFCVal1 = 1;
	Data::doHFMFCVal2 = 1;

	bps_mode = recipe_step.EtOHBypass();
	if (bps_mode == BypassMode_None)
	{
		Data::doExpCbVacIPASupply = 0;
		Data::doVaVapValve = 0;
		Data::aoEtOHFlowSetpoint = 0;
		Monitor::Instance().Disable("EtOH flowrate");
	}
	else
	{
		flowrate = recipe_step.EtOHFlowrate();
		flow_warn_offset = 0.01 * flowrate * Parameters::EtOHWarnProportion;
		flow_warn_offset = std::max<float>(Parameters::EtOHWarnMinimum, flow_warn_offset);
		flow_alarm_offset = 0.01 * flowrate * Parameters::EtOHAlarmProportion;
		flow_alarm_offset = std::max<float>(Parameters::EtOHAlarmMinimum, flow_alarm_offset);
		Data::aoEtOHFlowSetpoint = flowrate;
		if (bps_mode == BypassMode_Bypass)
		{
			Data::doExpCbVacIPASupply = 0;
			Data::doVaVapValve = 1;
			Monitor::Instance().Disable("EtOH flowrate");
		}
		else
		{
			Data::doExpCbVacIPASupply = 1;
			Data::doVaVapValve = 0;
			Monitor::Instance().Reset("EtOH flowrate", Parameters::EtOHMonitorDelay, flowrate, flow_warn_offset, flow_alarm_offset);
		}
	}
	Data::doAlcMFCVal1 = 1;
	Data::doAlcMFCVal2 = 1;

	Data::doN2MFCVal1 = 1;
	Data::doN2MFCVal2 = 1;
	flowrate = recipe_step.N2Flowrate();
	flow_warn_offset = 0.01 * flowrate * Parameters::FlowWarnProportion;
	flow_warn_offset = std::max<float>(Parameters::FlowWarnMinimum, flow_warn_offset);
	flow_alarm_offset = 0.01 * flowrate * Parameters::FlowAlarmProportion;
	flow_alarm_offset = std::max<float>(Parameters::FlowAlarmMinimum, flow_alarm_offset);
	Data::aoN2FlowSetpoint = flowrate;
	Monitor::Instance().Reset("N2 flowrate", Parameters::FlowMonitorDelay, flowrate, flow_warn_offset, flow_alarm_offset);

	Data::doPurgeN2MFCVal1 = 1;
	Data::doPurgeN2MFCVal2 = 1;
	flowrate = recipe_step.N2PurgeFlowrate();
	flow_warn_offset = 0.01 * flowrate * Parameters::FlowWarnProportion;
	flow_warn_offset = std::max<float>(Parameters::FlowWarnMinimum, flow_warn_offset);
	flow_alarm_offset = 0.01 * flowrate * Parameters::FlowAlarmProportion;
	flow_alarm_offset = std::max<float>(Parameters::FlowAlarmMinimum, flow_alarm_offset);
	Data::aoPurgeN2FlowSetpoint = flowrate;
	//Monitor::Instance().Reset("N2PurgeFlow", flowrate, flow_warn_offset, flow_alarm_offset);
}

void ProcessUnit::OnProcess()
{
	if(Data::aiProcChamPressure > Parameters::VacuumPressure)
	{
		EVT::ChamberNotVacuum.Report("Process");
		return;
	}

	if(Data::aiExpChamPressure > Parameters::VacuumPressure)
	{
		EVT::ChamberNotVacuum.Report("Expansion");
		return;
	}

	Recipe rcp;
	RecipeManager::Instance().GetRecipe(rcp);
	unsigned steps = rcp.TotalStep();
	unsigned recipe_duration = rcp.Duration();
	std::string recipe_name = rcp.Name();

	NEW_UNIT_STEP("prepare process", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbVapVacValve = 0;
			Data::doN2SupplyProcVal = 0;
			Data::doAlcTankOpen = 1;
			Data::doPurgeAlcTank = 1;
			Data::doVaSupplyIPAValve = 1;
			Data::doHFFacSupplyVal = 1;
			Data::doVapBypassValve = 0;
			Data::doVapSupplyN2Valve = 1;
			Data::doVacSlowProcCbVal = 0;
			Data::doVacFastProcCbVal = 1;
			Data::doExpCbSupplyCbVal = 1;
			Data::RecipeElapseTime = 0;
			Data::doAlcMFCVal3 = 0;
			Data::doHFMFCVal3 = 0;
			Data::doExpCbVacValve = 1;
			Data::doHFRequest = 1;
			Data::doVapInletVal = 1;
		})
		auto f = [&, this, steps, recipe_duration, recipe_name]()
		{	m_recipe_start_time = boost::chrono::system_clock::now();
			Data::TotalSteps = steps;
			Data::RecipeTotalTime = recipe_duration;
			WaferManager::Instance().ProcessStart(m_id, recipe_name);
			DataRecorder::Instance().Disable("ChuckLF");
			DataRecorder::Instance().Disable("LidLF");
			DataRecorder::Instance().Disable("BodyLF");
			DataRecorder::Instance().Enable("HF");
			DataRecorder::Instance().Enable("EtOH");
			DataRecorder::Instance().Enable("N2");
			DataRecorder::Instance().Enable("Pressure");
			DataRecorder::Instance().Enable("Chuck");
			DataRecorder::Instance().Enable("Lid");
			DataRecorder::Instance().Enable("Body");
			DataRecorder::Instance().Enable("Tank");
			//start monitor heater when process start.
			Monitor::Instance().Reset("Chuck temperature", Parameters::TempMonitorDelay, Parameters::ChuckTemp, 
				Parameters::TempWarnOffset, Parameters::TempAlarmOffset);
			Monitor::Instance().Reset("Body temperature", Parameters::TempMonitorDelay, Parameters::BodyTemp, 
				Parameters::TempWarnOffset, Parameters::TempAlarmOffset);
			Monitor::Instance().Reset("Lid temperature", Parameters::TempMonitorDelay, Parameters::LidTemp, 
				Parameters::TempWarnOffset, Parameters::TempAlarmOffset);
		};
		ADD_STEP_COMMAND(f)
	END_UNIT_STEP

	std::stringstream ss;
	for(unsigned i=0; i<steps; i++)
	{
		RecipeStep rcp_step = rcp.Step(i);
		unsigned step_time = rcp_step.Duration();
		ss.str("");
		ss<<"process step "<<(i+1);
		NEW_UNIT_STEP(ss.str(), false)
			auto f = [this, i, rcp_step](){ process_recipe_step(i, rcp_step);};
			ADD_STEP_COMMAND(f)
			auto condition = [&, this]()
			{
				auto dur = duration_cast<seconds>(system_clock::now() - m_step_start_time);
				Data::StepElapseTime = dur.count();
				dur = duration_cast<seconds>(system_clock::now() - m_recipe_start_time);
				Data::RecipeElapseTime = dur.count();

				return !Monitor::Instance().HasAlarm();
			};
			ADD_STEP_EXPECT(condition, step_time, [&](){EVT::ProcessAlarm.Report();})
			ADD_STEP_COMMAND([&](){Monitor::Instance().DisableAll();})
			auto f1 = [&, step_time](){Data::StepElapseTime = step_time;};
			ADD_STEP_COMMAND(f1)
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("finish process", false)
		auto f = [&, this, steps, recipe_duration]()
		{	shut_all_chemical();
			open_apc();
			stop_motor();
			Data::CurrentStep = steps;
			Data::RecipeElapseTime = recipe_duration;
			WaferManager::Instance().ProcessEnd(m_id);
			DataRecorder::Instance().Disable("HF");
			DataRecorder::Instance().Disable("EtOH");
			DataRecorder::Instance().Disable("N2");
			DataRecorder::Instance().Disable("Pressure");
			DataRecorder::Instance().Disable("Chuck");
			DataRecorder::Instance().Disable("Lid");
			DataRecorder::Instance().Disable("Body");
			DataRecorder::Instance().Disable("Tank");
			DataRecorder::Instance().Enable("ChuckLF");
			DataRecorder::Instance().Enable("LidLF");
			DataRecorder::Instance().Enable("BodyLF");
			Monitor::Instance().DisableAll();
		};
		ADD_STEP_COMMAND(f)
	END_UNIT_STEP
}

void ProcessUnit::OnLeakCheck(unsigned param)
{
	switch(param)
	{
	case 0:
		OnProcChamberLeakCheck();
		break;
	case 1:
		OnExpChamberLeakCheck();
		break;
	default:
		;
	}
}

void ProcessUnit::OnProcChamberLeakCheck()
{
//	if(Data::diPrcCbLidLeftClose == 0 || Data::diPrcCbLidRightClose == 0)
//	{
//		EVT::GenericWarning.Report("Chamber lid isn't closed, leak check is aborted.");
//		return;
//	}

	NEW_UNIT_STEP("initialize leak check state", true)
		ADD_STEP_COMMAND([&]()
		{	Data::LeakRate = 0;
			Data::LeakCheckResult = "Testing";})
	END_UNIT_STEP

	if(Data::diPrcCbDoorClose == 0)
	{
		OnCloseDoor();
	}

	NEW_UNIT_STEP("prepare pump process chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbSupplyCbVal = 0;
			Data::doN2SupplyProcVal = 0;
			Data::aoAPCPosition = 100;
			Data::aoAPCControlMode = APCControl_Position;})
	END_UNIT_STEP

	if(Data::doEnableVPump == 0)
	{
		NEW_UNIT_STEP("turn on pump", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doVacFastProcCbVal = 0;
				Data::doVacSlowProcCbVal = 0;
				Data::doEnableVPump = 1;})
			ADD_STEP_WAIT(5)
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diVPumpWarning == 0 && Data::diVPumpAlarm == 0;},
				1,
				[&](){	EVT::PumpError.Report();})
		END_UNIT_STEP
	}

	if(Data::aiProcChamPressure > Parameters::FastSlowSwitchPressure)
	{
		NEW_UNIT_STEP("slowly pump process chamber", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doVacFastProcCbVal = 0;
				Data::doVacSlowProcCbVal = 1;})
			auto condition_function = [&](){return Data::aiProcChamPressure < Parameters::FastSlowSwitchPressure;};
			float pressure1 = Data::aiProcChamPressure;
			float pressure2 = Parameters::FastSlowSwitchPressure;
			auto event_function = [=](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
			ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::SlowPumpTimeout, event_function);
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("fast pump process chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doVacFastProcCbVal = 1;
			Data::doVacSlowProcCbVal = 0;})
		auto condition_function = [&](){return Data::aiProcChamPressure < Parameters::LeakCheckPressure;};
		float pressure1 = Data::aiProcChamPressure;
		float pressure2 = Parameters::LeakCheckPressure;
		auto event_function = [=](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
		ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::FastPumpTimeout, event_function);
		ADD_STEP_WAIT(Parameters::LeakCheckPumpHoldTime)
	END_UNIT_STEP


	NEW_UNIT_STEP("leak check", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doVacFastProcCbVal = 0;
			m_leak_check_init_pressure = Data::aiProcChamPressure;})
		ADD_STEP_WAIT(Parameters::LeakCheckTime)
		ADD_STEP_COMMAND([&]()
		{	float leak_rate = 60*(Data::aiProcChamPressure - m_leak_check_init_pressure)/Parameters::LeakCheckTime;
			Data::LeakRate = leak_rate;
			if(leak_rate > Parameters::LeakCheckThreshold)
			{
				Data::LeakCheckResult = "FAIL";
			}
			else
			{
				Data::LeakCheckResult = "PASS";
			}
		})
	END_UNIT_STEP
}

void ProcessUnit::OnExpChamberLeakCheck()
{
	NEW_UNIT_STEP("initialize state", true)
		ADD_STEP_COMMAND([&]()
		{	Data::LeakRate = 0;
			Data::LeakCheckResult = "Testing";})
	END_UNIT_STEP

	if(Data::doEnableVPump == 0)
	{
		NEW_UNIT_STEP("turn on pump", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doVacFastProcCbVal = 0;
				Data::doVacSlowProcCbVal = 0;
				Data::doEnableVPump = 1;})
			ADD_STEP_WAIT(5)
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diVPumpWarning == 0 && Data::diVPumpAlarm == 0;},
				1,
				[&](){	EVT::PumpError.Report();})
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("pump expansion chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbSupplyCbVal = 0;
			Data::doExpCbVacIPASupply = 0;
			Data::doExpCbHFInletVal = 0;
			Data::doPurgeN2MFCVal1 = 0;
			Data::doPurgeN2MFCVal2 = 0;
			Data::aoPurgeN2FlowSetpoint = 0;
			Data::doVaVapValve = 0;
			Data::doVaHFValve = 0;
			Data::doExpCbVacValve = 1;
			Data::doExpCbVapVacValve = 1;})
		auto condition_function = [&](){return Data::aiExpChamPressure < Parameters::LeakCheckPressure;};
		float pressure1 = Data::aiExpChamPressure;
		float pressure2 = Parameters::LeakCheckPressure;
		auto event_function = [&, pressure1, pressure2](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
		ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::PumpExpTimeout, event_function);
		ADD_STEP_WAIT(Parameters::LeakCheckPumpHoldTime)
	END_UNIT_STEP

	NEW_UNIT_STEP("leak check", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbVacValve = 0;
			Data::doExpCbVapVacValve = 0;
			m_leak_check_init_pressure = Data::aiExpChamPressure;})
		ADD_STEP_WAIT(Parameters::LeakCheckTime)
		ADD_STEP_COMMAND([&]()
		{	float leak_rate = 60*(Data::aiExpChamPressure - m_leak_check_init_pressure)/Parameters::LeakCheckTime;
			Data::LeakRate = leak_rate;
			if(leak_rate > Parameters::LeakCheckThreshold)
			{
				Data::LeakCheckResult = "FAIL";
			}
			else
			{
				Data::LeakCheckResult = "PASS";
			}
		})
	END_UNIT_STEP
}

void ProcessUnit::OnPinUp()
{
	NEW_UNIT_STEP("pin up", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doPinDown = 0;
			Data::doPinUp = 1;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diPinUp == 1 && Data::diPinDown == 0;},
			Parameters::PinTimeout,
			[&](){	EVT::PinTimeout.Report("up");})
	END_UNIT_STEP
}

void ProcessUnit::OnPinDown()
{
	NEW_UNIT_STEP("pin down", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doPinDown = 1;
			Data::doPinUp = 0;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diPinUp == 0 && Data::diPinDown == 1;},
			Parameters::PinTimeout,
			[&](){	EVT::PinTimeout.Report("down");})
	END_UNIT_STEP
}

void ProcessUnit::OnExtendArm()
{
	NEW_UNIT_STEP("extend arm", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doArmIn = 1;
			Data::doArmOut = 0;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diArmOut == 0 && Data::diArmIn == 1;},
			Parameters::ArmTimeout,
			[&](){	EVT::ArmTimeout.Report("in");})
	END_UNIT_STEP
}

void ProcessUnit::OnRetractArm()
{
	NEW_UNIT_STEP("retract arm", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doArmIn = 0;
			Data::doArmOut = 1;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diArmOut == 1 && Data::diArmIn == 0;},
			Parameters::ArmTimeout,
			[&](){	EVT::ArmTimeout.Report("out");})
	END_UNIT_STEP
}

void ProcessUnit::OnForkVertical()
{
	NEW_UNIT_STEP("rotate fork vertical", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doForkVertical = 1;
			Data::doForkHorizontal = 0;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diForkHorizontal == 0 && Data::diForkVertical == 1;},
			Parameters::ForkTimeout,
			[&](){	EVT::ForkTimeout.Report();})
	END_UNIT_STEP
}

void ProcessUnit::OnForkHorizontal()
{
	NEW_UNIT_STEP("rotate fork horizontal", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doForkVertical = 0;
			Data::doForkHorizontal = 1;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diForkHorizontal == 1 && Data::diForkVertical == 0;},
			Parameters::ForkTimeout,
			[&](){	EVT::ForkTimeout.Report();})
	END_UNIT_STEP
}

float ProcessUnit::get_next_position()
{
	float pos[3];
	pos[0] = Parameters::ChuckPos1;
	pos[1] = Parameters::ChuckPos2;
	pos[2] = Parameters::ChuckPos3;

	float current_pos = Data::aiActualPosition + 5;
	unsigned int i=0;
	for(i=0; i<3; i++)
	{
		if(current_pos < pos[i])
			break;
	}

	return pos[i%3];
}

void ProcessUnit::OnRotateForward()
{
	float next_pos = get_next_position();
	NEW_UNIT_STEP("rotate forward", true)
		auto f = [&, next_pos]()
		{	Data::aoAxisPosition = next_pos;
			Data::aoAxisVelocity = Parameters::RotateSpeed;
			Data::aoAxisControl = AxisControl_Absolute;
			Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);};
		ADD_STEP_COMMAND(f)
		ADD_STEP_WAIT(1)
		auto f1 = [&, next_pos]()->bool
		{	return Data::diAxisMoving == 0 && fabs(Data::aiActualPosition-next_pos)<Parameters::PositionErrorLimit;};
		ADD_STEP_WAIT_CONDITION(f1, Parameters::RotateTimeout, [next_pos](){EVT::RotateTimeout.Report(next_pos);})
	END_UNIT_STEP
}

float ProcessUnit::get_last_position()
{
	float pos[3];
	pos[0] = Parameters::ChuckPos1;
	pos[1] = Parameters::ChuckPos2;
	pos[2] = Parameters::ChuckPos3;

	float current_pos = Data::aiActualPosition - 5;
	unsigned int i=0;
	for(i=0; i<3; i++)
	{
		if(current_pos < pos[i])
			break;
	}

	return pos[(i+3-1)%3];
}

void ProcessUnit::OnRotateBackward()
{
	float last_pos = get_last_position();
	NEW_UNIT_STEP("rotate backward", true)
		auto f = [&, last_pos]()
		{	Data::aoAxisPosition = last_pos;
			Data::aoAxisVelocity = Parameters::RotateSpeed;
			Data::aoAxisControl = AxisControl_Absolute;
			Data::doAxisExecute = (Data::doAxisExecute ? 0 : 1);};
		ADD_STEP_COMMAND(f)
		ADD_STEP_WAIT(1)
		auto f1 = [&, last_pos]()->bool
		{	return Data::diAxisMoving == 0 && fabs(Data::aiActualPosition-last_pos)<Parameters::PositionErrorLimit;};
		ADD_STEP_WAIT_CONDITION(f1, Parameters::RotateTimeout, [last_pos](){EVT::RotateTimeout.Report(last_pos);})
	END_UNIT_STEP
}

void ProcessUnit::OnPump(unsigned param)
{
	switch(param)
	{
	case 0:
		OnPumpProcChamber();
		break;
	case 1:
		OnPumpExpChamber();
		break;
	default:
		;
	}
}

bool ProcessUnit::OnPumpProcChamber()
{
//	if(Data::diPrcCbLidLeftClose == 0 || Data::diPrcCbLidRightClose == 0)
//	{
//		EVT::GenericWarning.Report("Chamber lid isn't closed, pumping is aborted.");
//		return false;
//	}

	if(Data::diPrcCbDoorClose == 0)
	{
		OnCloseDoor();
	}

	NEW_UNIT_STEP("prepare pump process chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbSupplyCbVal = 0;
			Data::doN2SupplyProcVal = 0;
			Data::aoAPCPosition = 100;
			Data::aoAPCControlMode = APCControl_Position;})
	END_UNIT_STEP

	if(Data::doEnableVPump == 0)
	{
		NEW_UNIT_STEP("turn on pump", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doVacFastProcCbVal = 0;
				Data::doVacSlowProcCbVal = 0;
				Data::doEnableVPump = 1;})
			ADD_STEP_WAIT(5)
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diVPumpWarning == 0 && Data::diVPumpAlarm == 0;},
				1,
				[&](){	EVT::PumpError.Report();})
		END_UNIT_STEP
	}

	if(Data::aiProcChamPressure > Parameters::FastSlowSwitchPressure)
	{
		NEW_UNIT_STEP("slowly pump process chamber", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doVacFastProcCbVal = 0;
				Data::doVacSlowProcCbVal = 1;})
			auto condition_function = [&](){return Data::aiProcChamPressure < Parameters::FastSlowSwitchPressure;};
			float pressure1 = Data::aiProcChamPressure;
			float pressure2 = Parameters::FastSlowSwitchPressure;
			auto event_function = [&, pressure1, pressure2](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
			ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::SlowPumpTimeout, event_function);
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("fast pump process chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doVacFastProcCbVal = 1;
			Data::doVacSlowProcCbVal = 0;})
		auto condition_function = [&](){return Data::aiProcChamPressure < Parameters::PumpDownTargetPressure;};
		float pressure1 = Data::aiProcChamPressure;
		float pressure2 = Parameters::PumpDownTargetPressure;
		auto event_function = [=](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
		ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::FastPumpTimeout, event_function);
	END_UNIT_STEP


	NEW_UNIT_STEP("finish pump", true)
		ADD_STEP_COMMAND([&]()
		{	float pressure_difference = fabs(Data::aiExpChamPressure - Data::aiProcChamPressure);
			if(pressure_difference < Parameters::PressureDiffAllowance)
			{
				Data::doExpCbSupplyCbVal = 1;
				Data::doExpCbVacValve = 0;
				Data::doExpCbVapVacValve = 0;
			}})
	END_UNIT_STEP

	return true;
}

bool ProcessUnit::OnPumpExpChamber()
{
	if(Data::doEnableVPump == 0)
	{
		NEW_UNIT_STEP("turn on pump", true)
			ADD_STEP_COMMAND([&]()
			{	Data::doVacFastProcCbVal = 0;
				Data::doVacSlowProcCbVal = 0;
				Data::doEnableVPump = 1;})
			ADD_STEP_WAIT(5)
			ADD_STEP_WAIT_CONDITION([&]()->bool
			{	return Data::diVPumpWarning == 0 && Data::diVPumpAlarm == 0;},
				1,
				[&](){	EVT::PumpError.Report();})
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("pump expansion chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbSupplyCbVal = 0;
			Data::doExpCbVacIPASupply = 0;
			Data::doExpCbHFInletVal = 0;
			Data::doPurgeN2MFCVal1 = 0;
			Data::doPurgeN2MFCVal2 = 0;
			Data::aoPurgeN2FlowSetpoint = 0;
			Data::doVaVapValve = 0;
			Data::doVaHFValve = 0;
			Data::doExpCbVacValve = 1;
			Data::doExpCbVapVacValve = 1;})
		auto condition_function = [&](){return Data::aiExpChamPressure < Parameters::PumpDownTargetPressure;};
		float pressure1 = Data::aiExpChamPressure;
		float pressure2 = Parameters::PumpDownTargetPressure;
		auto event_function = [&, pressure1, pressure2](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
		ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::PumpExpTimeout, event_function);
	END_UNIT_STEP

	return true;
}

void ProcessUnit::OnVent(unsigned param)
{
	switch(param)
	{
	case 0:
		OnVentProcChamber();
		break;
	case 1:
		OnVentExpChamber();
		break;
	default:
		;
	}
}

bool ProcessUnit::OnVentExpChamber()
{
	if(Data::aiExpChamPressure > Parameters::ATMPressure)
	{
		return true;
	}

	if(Data::ExpChamberDirty == 1)
	{
		//OnPurgeProcChamber();
		EVT::ChamberNotClean.Report("Expansion");
		return false;
	}

	NEW_UNIT_STEP("vent expansion chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbVapVacValve = 0;
			Data::doExpCbSupplyCbVal = 0;
			Data::doExpCbVacIPASupply = 0;
			Data::doExpCbHFInletVal = 0;
			Data::doPurgeN2MFCVal1 = 1;
			Data::doPurgeN2MFCVal2 = 1;
			Data::aoPurgeN2FlowSetpoint = Parameters::ExpVentFlowrate;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::aiExpChamPressure > Parameters::ATMPressure;},
			Parameters::VentExpTimeout,
			[&](){	EVT::VentTimeout.Report("expansion");})
		ADD_STEP_COMMAND([&]()
		{	Data::doPurgeN2MFCVal1 = 0;
			Data::doPurgeN2MFCVal2 = 0;
			Data::aoPurgeN2FlowSetpoint = 0;})
	END_UNIT_STEP

	return true;
}

bool ProcessUnit::OnVentProcChamber()
{
	if(Data::aiProcChamPressure > Parameters::ATMPressure)
	{
		return true;
	}

	if(Data::ProcChamberDirty == 1)
	{
		//OnPurgeProcChamber();
		EVT::ChamberNotClean.Report("Process");
		return false;
	}

	NEW_UNIT_STEP("vent process chamber", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doVacFastProcCbVal = 0;
			Data::doVacSlowProcCbVal = 0;
			Data::doN2SupplyProcVal = 1;
			Data::doExpCbSupplyCbVal = 0;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::aiProcChamPressure > Parameters::ATMPressure;},
			Parameters::VentProcTimeout,
			[&](){	EVT::VentTimeout.Report("process");})
		ADD_STEP_WAIT(2)
		ADD_STEP_COMMAND([&](){	Data::doN2SupplyProcVal = 0;})
	END_UNIT_STEP

	return true;
}

void ProcessUnit::OnPurge(unsigned param)
{
	switch(param)
	{
	case 0:
		OnPurgeProcChamber();
		break;
	case 1:
		OnPurgeExpChamber();
		break;
	case 2:
		OnPurgeHF();
		break;
	case 3:
		OnPurgeEtOH();
		break;
	default:
		;
	}
}

void ProcessUnit::OnPurgeHF()
{
	NEW_UNIT_STEP("prepare purge HF", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doHFFacSupplyVal = 0;
			Data::doHFMFCVal1 = 1;
			Data::doHFMFCVal2 = 1;
			Data::doHFMFCVal3 = 1;
			Data::doExpCbHFInletVal = 0;
			Data::doVaHFValve = 1;
			Data::doExpCbVapVacValve = 0;
			Data::doVaVapValve = 0;
			Data::doExpCbVacValve = 1;})
	END_UNIT_STEP

	for(unsigned i=0; i<Parameters::PurgeRepeatTimes; i++)
	{
		NEW_UNIT_STEP("purge HF", false)
			ADD_STEP_COMMAND([&]()
			{	Data::doN2PurgeHFVal = 1;
				Data::aoHFFlowSetpoint = Parameters::HFPurgeFlow;})
			ADD_STEP_WAIT(Parameters::PurgeHoldTime)
			ADD_STEP_COMMAND([&]()
			{	Data::doN2PurgeHFVal = 0;
				Data::aoHFFlowSetpoint = 0;})
			ADD_STEP_WAIT(Parameters::PumpHoldTime)
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("finish purge HF", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doHFMFCVal1 = 0;
			Data::doHFMFCVal2 = 0;
			Data::doHFMFCVal3 = 0;
			Data::doVaHFValve = 0;
			Data::doExpCbVacValve = 0;})
	END_UNIT_STEP
}

void ProcessUnit::OnPurgeEtOH()
{
	if(Data::diAlcTkLow == 0)
	{
		EVT::GenericWarning.Report("Alcohol tank isn't empty, please make it empty before purge.");
		return;
	}

	NEW_UNIT_STEP("prepare purge EtOH", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbVapVacValve = 0;
			Data::doExpCbVacIPASupply = 0;
			Data::doVaHFValve = 0;
			Data::doN2PurgeHFVal = 0;
			Data::doVaVapValve = 1;
			Data::doExpCbVacValve = 1;
			Data::doVapSupplyN2Valve = 0;
			Data::doVapBypassValve = 0;
			Data::doVaSupplyIPAValve = 1;
			Data::doAlcTankOpen = 1;
			Data::doPurgeAlcTank = 1;
			Data::doAlcMFCVal1 = 1;
			Data::doAlcMFCVal2 = 1;
			Data::doAlcMFCVal3 = 1;
			Data::doVapInletVal = 1;})
	END_UNIT_STEP

	for(unsigned i=0; i<Parameters::PurgeRepeatTimes; i++)
	{
		NEW_UNIT_STEP("purge EtOH", false)
			ADD_STEP_COMMAND([&]()
			{	Data::doPurgeAlcTank = 1;
				Data::aoEtOHFlowSetpoint = Parameters::EtOHPurgeFlow;})
			ADD_STEP_WAIT(Parameters::PurgeHoldTime)
			ADD_STEP_COMMAND([&]()
			{	Data::doPurgeAlcTank = 0;
				Data::aoEtOHFlowSetpoint = 0;})
			ADD_STEP_WAIT(Parameters::PumpHoldTime)
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("finish purge EtOH", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doAlcMFCVal1 = 0;
			Data::doAlcMFCVal2 = 0;
			Data::doAlcMFCVal3 = 0;
			Data::doVaVapValve = 0;
			Data::doExpCbVacValve = 0;
			Data::doVaSupplyIPAValve = 0;
			Data::doAlcTankOpen = 0;
			Data::doPurgeAlcTank = 1;
			Data::doVapInletVal = 0;})
	END_UNIT_STEP
}

void ProcessUnit::OnPurgeExpChamber()
{
	NEW_UNIT_STEP("prepare purge expansion chamber", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbHFInletVal = 0;
			Data::doExpCbVacIPASupply = 0;
			Data::doExpCbSupplyCbVal = 0;
			Data::doVaHFValve = 0;
			Data::doVaVapValve = 0;
			Data::doExpCbVapVacValve = 1;
			Data::doExpCbVacValve = 1;
			Data::doPurgeN2MFCVal1 = 0;
			Data::doPurgeN2MFCVal2 = 1;})
	END_UNIT_STEP

	for(unsigned i=0; i<Parameters::PurgeRepeatTimes; i++)
	{
		NEW_UNIT_STEP("purge expansion chamber", false)
			ADD_STEP_COMMAND([&]()
			{	Data::doPurgeN2MFCVal1 = 1;
				Data::aoPurgeN2FlowSetpoint = Parameters::N2PurgeFlow;})
			ADD_STEP_WAIT(Parameters::PurgeHoldTime)
			ADD_STEP_COMMAND([&]()
			{	Data::doPurgeN2MFCVal1 = 0;
				Data::aoPurgeN2FlowSetpoint = 0;})
			ADD_STEP_WAIT(Parameters::PumpHoldTime)
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("finish purge expansion chamber", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doPurgeN2MFCVal2 = 0;
			Data::ExpChamberDirty = 0;})
	END_UNIT_STEP
}

void ProcessUnit::OnPurgeProcChamber()
{
	if(Data::aiProcChamPressure > Parameters::FastSlowSwitchPressure)
	{
		EVT::GenericWarning.Report("Chamber pressure is too high, please pump process chamber.");
		return;
	}

	NEW_UNIT_STEP("prepare purge process chamber", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doExpCbSupplyCbVal = 0;
			Data::doN2SupplyProcVal = 0;
			Data::aoAPCPosition = 100;
			Data::aoAPCControlMode = APCControl_Position;
			Data::doVacFastProcCbVal = 1;
			Data::doVacSlowProcCbVal = 0;})
		auto condition_function = [&](){return Data::aiProcChamPressure < Parameters::PumpDownTargetPressure;};
		float pressure1 = Data::aiProcChamPressure;
		float pressure2 = Parameters::PumpDownTargetPressure;
		auto event_function = [=](){EVT::PumpTimeout.Report<float, float>(pressure1, pressure2);};
		ADD_STEP_WAIT_CONDITION(condition_function,	Parameters::FastPumpTimeout, event_function);
	END_UNIT_STEP

	for(unsigned i=0; i<Parameters::PurgeRepeatTimes; i++)
	{
		NEW_UNIT_STEP("purge process chamber", false)
			ADD_STEP_COMMAND([&]()
			{	Data::doN2SupplyProcVal = 1;})
			ADD_STEP_WAIT(Parameters::PurgeHoldTime)
			ADD_STEP_COMMAND([&]()
			{	Data::doN2SupplyProcVal = 0;})
			ADD_STEP_WAIT(Parameters::PumpHoldTime)
		END_UNIT_STEP
	}

	NEW_UNIT_STEP("finish purge process chamber", false)
		ADD_STEP_COMMAND([this]()
		{	Data::ProcChamberDirty = 0;})
	END_UNIT_STEP
}

void ProcessUnit::OnTurnOnHeater()
{
	NEW_UNIT_STEP("turn on heater", true)
		ADD_STEP_COMMAND([&]()
		{	Data::aoBodyHTTempSet = Parameters::BodyTemp;
			Data::aoLidHTTempSet = Parameters::LidTemp;
			Data::aoChuckHTTempSet = Parameters::ChuckTemp;
			Data::doBodyHTEnable = 1;
			Data::doLidHTEnable = 1;
			Data::doChuckHTEnable = 1;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diBodyHTPowRdy == 1 && Data::diLidHTPowRdy == 1 && Data::diChuckHTPowRdy == 1;},
			2,
			[&](){	EVT::TurnOnHeaterTimeout.Report();})
	END_UNIT_STEP
}

void ProcessUnit::OnTurnOffHeater()
{
	NEW_UNIT_STEP("turn on heater", false)
		ADD_STEP_COMMAND([&]()
		{	Data::doBodyHTEnable = 0;
			Data::doLidHTEnable = 0;
			Data::doChuckHTEnable = 0;})
	END_UNIT_STEP
}

bool ProcessUnit::OnOpenDoor()
{
//	if(Data::diPrcCbDoorOpen == 1 && Data::diPrcCbDoorClose == 0)
//	{
//		return true;
//	}

	if(Data::aiProcChamPressure < Parameters::ATMPressure)
	{
		EVT::GenericWarning.Report("Chamber is not ATM, gate valve can't be opened.");
		return false;
	}

	if(Data::ProcChamberDirty == 1)
	{
		EVT::GenericWarning.Report("Chamber is not clean, gate valve can't be opened.");
		return false;
	}

	NEW_UNIT_STEP("open door", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doCbGateOpen = 1;
			Data::doCbGateClose = 0;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diPrcCbDoorOpen == 1 && Data::diPrcCbDoorClose == 0;},
			Parameters::GateValveTimeout,
			[&](){	EVT::GateValveTimeout.Report();})
	END_UNIT_STEP

	return true;
}

void ProcessUnit::OnCloseDoor()
{
//	if(Data::diPrcCbDoorOpen == 0 && Data::diPrcCbDoorClose == 1)
//	{
//		return true;
//	}

	NEW_UNIT_STEP("close door", true)
		ADD_STEP_COMMAND([&]()
		{	Data::doCbGateOpen = 0;
			Data::doCbGateClose = 1;})
		ADD_STEP_WAIT_CONDITION([&]()->bool
		{	return Data::diPrcCbDoorOpen == 0 && Data::diPrcCbDoorClose == 1;},
			Parameters::GateValveTimeout,
			[&](){	EVT::GateValveTimeout.Report();})
	END_UNIT_STEP
}

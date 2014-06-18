/*
 * RecipeStep.h
 *
 *  Created on: 2014Äê5ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef RECIPESTEP_H_
#define RECIPESTEP_H_

#include "boost/property_tree/ptree.hpp"

enum APCMode : unsigned short
{
	APCMode_Position,
	APCMode_Pressure,
};

enum BypassMode
{
	BypassMode_None,
	BypassMode_Bypass,
	BypassMode_Chamber,
};

class RecipeStep
{
public:
	RecipeStep(const std::string& name, unsigned short duration, APCMode apc_mode, unsigned short position,
			unsigned short pressure, unsigned short rotate_speed, BypassMode HF_bypass,
			unsigned short HF_flow, BypassMode EtOH_bypass, unsigned short EtOH_flow, unsigned short N2_flow,
			unsigned short N2_purge_flow)
	: m_name(name), m_duration(duration), m_apc_mode(apc_mode), m_position(position), m_pressure(pressure),
	  m_rotate_speed(rotate_speed), m_HF_bypass(HF_bypass), m_HF_flowrate(HF_flow), m_EtOH_bypass(EtOH_bypass),
	  m_EtOH_flowrate(EtOH_flow), m_N2_flowrate(N2_flow), m_N2_purge_flowrate(N2_purge_flow) {};
	RecipeStep() : RecipeStep("", 0, APCMode_Position, 0, 0, 0, BypassMode_None, 0, BypassMode_None, 0, 0, 0){};
	RecipeStep(const RecipeStep& rhs) : RecipeStep(rhs.m_name, rhs.m_duration, rhs.m_apc_mode,
			rhs.m_position, rhs.m_pressure, rhs.m_rotate_speed, rhs.m_HF_bypass, rhs.m_HF_flowrate,
			rhs.m_EtOH_bypass, rhs.m_EtOH_flowrate, rhs.m_N2_flowrate, rhs.m_N2_purge_flowrate) {};
	RecipeStep& operator = (const RecipeStep& rhs);

	void Load(const boost::property_tree::ptree& pt);
	void Save(boost::property_tree::ptree& pt);

	const std::string& Name() const;
	unsigned short Duration() const;
	APCMode Mode() const;
	unsigned short Position() const;
	unsigned short Pressure() const;
	unsigned short RotateSpeed() const;
	BypassMode HFBypass() const;
	unsigned short HFFlowrate() const;
	BypassMode EtOHBypass() const;
	unsigned short EtOHFlowrate() const;
	unsigned short N2Flowrate() const;
	unsigned short N2PurgeFlowrate() const;

private:
	static BypassMode bypass_from_string(const std::string& mode);
	static std::string bypass_to_string(BypassMode mode);
	static APCMode apc_from_string(const std::string& mode);
	static std::string apc_to_string(APCMode mode);

protected:
	std::string m_name;
	unsigned short m_duration;
	APCMode m_apc_mode;
	unsigned short m_position;
	unsigned short m_pressure;
	unsigned short m_rotate_speed;
	BypassMode m_HF_bypass;
	unsigned short m_HF_flowrate;
	BypassMode m_EtOH_bypass;
	unsigned short m_EtOH_flowrate;
	unsigned short m_N2_flowrate;
	unsigned short m_N2_purge_flowrate;
};


#endif /* RECIPESTEP_H_ */

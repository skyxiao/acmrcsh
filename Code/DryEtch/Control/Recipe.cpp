/*
 * Recipe.cpp
 *
 *  Created on: 2014Äê3ÔÂ19ÈÕ
 *      Author: acm
 */

#include <sstream>

#include "boost/algorithm/string.hpp"
#include "boost/function.hpp"

#include "Recipe.h"
#include "Utility.h"
#include "Event.h"
#include "LogFile.h"

using namespace boost::property_tree;


Recipe::Recipe(const Recipe& rhs)
{
	m_name = rhs.m_name;
	m_steps = rhs.m_steps;
}

Recipe& Recipe::operator = (const Recipe& rhs)
{
	m_name = rhs.m_name;
	m_steps = rhs.m_steps;

	return *this;
}

void Recipe::Load(const std::string& name)
{
	ptree pt;
	try
	{
		std::string recipe_name = "./recipe/" + name + ".rcp";
		json_parser::read_json(recipe_name, pt);
		m_steps.clear();
		for(ptree::value_type &v : pt.get_child("Steps"))
		{
			RecipeStep step;
			step.Load(v.second);
			m_steps.push_back(step);
		}
		m_name = name;
	}
	catch(...)
	{
		Reset();
		EVT::LoadRecipeFailed.Report(name);
	}
}

void Recipe::LoadFromString(const std::string& name, const std::string& recipe)
{
	ptree pt;
	try
	{
		std::istringstream ss(recipe);
		json_parser::read_json(ss, pt);
		m_steps.clear();
		for(ptree::value_type &v : pt.get_child("Steps"))
		{
			RecipeStep step;
			step.Load(v.second);
			m_steps.push_back(step);
		}
		m_name = name;
	}
	catch(...)
	{
		Reset();
		EVT::LoadRecipeFailed.Report(name);
	}
}

void Recipe::Save()
{
	if(m_name.empty() || m_steps.empty())
		return;

	ptree pt;
	for(RecipeStep& step : m_steps)
	{
		ptree step_pt;
		step.Save(step_pt);
		pt.push_back(std::make_pair("", step_pt));
	}

	ptree root;
	root.add_child("Steps", pt);

	json_parser::write_json(recipe_fullname(m_name), root);
}

void Recipe::Reset()
{
	m_name.clear();
	m_steps.clear();
}

std::string Recipe::Name()
{
	return m_name;
}

unsigned short Recipe::Duration()
{
	unsigned short dur = 0;
	for(RecipeStep& step : m_steps)
	{
		dur += step.Duration();
	}

	return dur;
}

unsigned Recipe::TotalStep()
{
	return m_steps.size();
}

const RecipeStep& Recipe::Step(unsigned index)
{
	if(index >= m_steps.size())
	{
		std::stringstream ss;
		ss<<"Wrong index ["<<index<<"] of recipe step.";
		LogError(ss.str());
		throw ss.str();
	}

	return m_steps[index];
}

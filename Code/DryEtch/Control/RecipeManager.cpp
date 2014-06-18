/*
 * RecipeManager.cpp
 *
 *  Created on: 2014Äê3ÔÂ19ÈÕ
 *      Author: acm
 */

#include <sstream>

#include "boost/filesystem.hpp"

#include "RecipeManager.h"
#include "Utility.h"
#include "Data.h"
#include "LogFile.h"

namespace fs = boost::filesystem;


void RecipeManager::Initialize()
{

}

void RecipeManager::Terminate()
{
	LogDebug("RecipeManager is terminated.");
}

void RecipeManager::Load(const std::string& recipe_name)
{
	boost::mutex::scoped_lock lock(m_mtx);
	m_rcp.Load(recipe_name);
	UpdateInformation();
}

void RecipeManager::Unload()
{
	boost::mutex::scoped_lock lock(m_mtx);
	m_rcp.Reset();
	UpdateInformation();
}

void RecipeManager::GetRecipe(Recipe& rcp)
{
	boost::mutex::scoped_lock lock(m_mtx);
	rcp = m_rcp;
}

void RecipeManager::GetRecipeList(std::list<std::string>& recipe_list)
{
	fs::path recipe_path("./recipe");
	if(!fs::exists(recipe_path) || !fs::is_directory(recipe_path))
		return;

	fs::directory_iterator dir_itor(recipe_path), end_itor;
	while(dir_itor != end_itor)
	{
		if(fs::is_regular_file(dir_itor->path()) && (dir_itor->path().extension() == L".rcp"))
		{
			std::wstring wfilename = dir_itor->path().filename().native();
			wfilename = wfilename.substr(0, wfilename.size()-4);
			recipe_list.push_back(wstring_to_utf8(wfilename));
		}
		++dir_itor;
	}
}

std::string RecipeManager::ReadRecipe(const std::string& recipe_name)
{
	std::ifstream ifs(recipe_fullname(recipe_name));
	if(!ifs.is_open())
		return "";

	std::istreambuf_iterator<char> beg(ifs), end;
	std::string strdata(beg, end);
	ifs.close();

	return strdata;
}

void RecipeManager::SaveRecipe(const std::string& recipe_name, const std::string& recipe)
{
	std::ofstream ofs(recipe_fullname(recipe_name));
	ofs<<recipe;
	ofs.flush();
	ofs.close();
}

void RecipeManager::RenameRecipe(const std::string& recipe_name, const std::string& new_name)
{
	std::string old_full_name = recipe_fullname(recipe_name);
	std::string new_full_name = recipe_fullname(new_name);

	if(!fs::exists(fs::path(old_full_name)))
	{
		std::stringstream ss;
		ss<<"Recipe "<<recipe_name<<" doesn't exists, can't be renamed.";
		LogWarning(ss.str());
		throw ss.str();
	}
	fs::rename(fs::path(old_full_name), fs::path(new_full_name));
}

void RecipeManager::DeleteRecipe(const std::string& recipe_name)
{
	std::string old_full_name = recipe_fullname(recipe_name);

	if(!fs::exists(fs::path(old_full_name)))
	{
		return;
	}
	fs::remove(fs::path(old_full_name));
}

void RecipeManager::UpdateInformation()
{
	Data::RecipeName = m_rcp.Name();
	Data::RecipeTotalTime = m_rcp.Duration();
	Data::RecipeElapseTime = 0;
	Data::TotalSteps = m_rcp.TotalStep();
	Data::CurrentStep = 0;
	Data::CurrentStepTime = 0;
	Data::StepElapseTime = 0;
	std::stringstream ss;
	ss<<"Recipe changed, current recipe is ["<<m_rcp.Name()<<"].";
	LogInfo(ss.str());
}

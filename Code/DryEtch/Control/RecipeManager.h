/*
 * RecipeManager.h
 *
 *  Created on: 2014Äê3ÔÂ19ÈÕ
 *      Author: acm
 */

#ifndef RECIPEMANAGER_H_
#define RECIPEMANAGER_H_

#include <list>

#include "Singleton.h"
#include "Recipe.h"

class RecipeManager : public SingletonT<RecipeManager>
{
protected:
	RecipeManager() = default;
	RecipeManager(const RecipeManager&) = delete;
	RecipeManager& operator = (const RecipeManager&) = delete;

public:
	virtual ~RecipeManager() = default;

	void Initialize();
	void Terminate();

	void Load(const std::string& recipe_name);
	void Unload();

	void GetRecipe(Recipe& rcp);

	void GetRecipeList(std::list<std::string>& recipe_list);
	std::string ReadRecipe(const std::string& recipe_name);
	void SaveRecipe(const std::string& recipe_name, const std::string& recipe);
	void RenameRecipe(const std::string& recipe_name, const std::string& new_name);
	void DeleteRecipe(const std::string& recipe_name);

	friend class SingletonT<RecipeManager>;

private:
	void UpdateInformation();

private:
	Recipe m_rcp;
	boost::mutex m_mtx;
};

#endif /* RECIPEMANAGER_H_ */

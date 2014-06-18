/*
 * Recipe.h
 *
 *  Created on: 2014Äê3ÔÂ19ÈÕ
 *      Author: acm
 */

#ifndef RECIPE_H_
#define RECIPE_H_

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include "RecipeStep.h"

class Recipe
{
public:
	Recipe() = default;
	virtual ~Recipe() = default;

	Recipe(const Recipe& rhs);
//	Recipe(Recipe&& rhs);
	Recipe& operator = (const Recipe& rhs);
//	Recipe&& operator = (Recipe&& rhs);

	void Load(const std::string& name);
	void LoadFromString(const std::string& name, const std::string& recipe);
	void Save();
	void Reset();

	std::string Name();
	unsigned short Duration();
	unsigned TotalStep();
	const RecipeStep& Step(unsigned index);

protected:
	std::string m_name;
	std::vector<RecipeStep> m_steps;
};

#endif /* RECIPE_H_ */

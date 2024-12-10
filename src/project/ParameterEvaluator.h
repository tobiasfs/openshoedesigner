///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterEvaluator.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.12.2022
// Copyright          : (C) 2022 Tobias Schaefer <tobiassch@users.sourceforge.net>
// Licence            : GNU General Public License version 3.0 (GPLv3)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef PARAMETERCOLLECTION_H
#define PARAMETERCOLLECTION_H

/*!\class ParameterEvaluator
 * \brief Collection of parameters with value and unit
 *
 * The collection of named values orchestrates the MathParser so that all
 * formulas are evaluated in the correct order.
 *
 * ## Groups / Variants
 *
 * To have multiple evaluation sets in a ParameterEvaluator group numbers
 * can be assigned. These can be thought of as variants for variables.
 *
 * For example:
 *  If there is one variable with the same name but available in two or
 *  more different variants. This variable can be assigned
 *  to a group.
 *
 *  Variable "A" in group 1 with the value 12cm and variable "A" in group 2
 *  with the value 11cm.
 *
 * While all other variables are assigned
 * to group 0. In this case two calculations are done. One calculation within
 * group 1 and one with group 2 for that variable. All other variables are
 * taken from the no-group.
 *
 * ## JSON
 *
 * This lass uses JSON serialization to store and retrieve its contents to
 * a file.
 */

#include "ParameterFormula.h"
#include <memory>
#include <set>
#include <stddef.h>
#include <string>
#include <unordered_map>
#include <vector>

class ParameterFormula;
class ParameterEvaluator {

public:
	ParameterEvaluator();

	void Clear();

	/**\brief Select the group the next Parameters are registered to
	 *
	 * If this function is called with no parameter, the next Parameter%s
	 * registered are assigned to the global none-group.
	 * This group is also the default group after start.
	 *
	 * If there are no groups or only a single group, no special variant
	 * handling is done.
	 */
	void SetGroup(const size_t group = (size_t) -1);

	/**\ Register a Parameter to the collection.
	 *
	 * This function generates a new Parameter object, adds it to the collection
	 * and returns a shared pointer to it.
	 *
	 * \param variablename Name for the new parameter. This is the name other parameter can reference in their formulas.
	 * \param description Lengthy description for the tip windows in GUIs.
	 * \param formula The initial formula to evaluate.
	 * \param id Optional ID. Recommended to use the dialog IDs from wxWidgets.
	 * \param group Optional group. If not set the setting from SetGroup() is used.
	 * \return A shared pointer to the generated Parameter
	 */
	std::shared_ptr<ParameterFormula> Register(const std::string &variablename,
			const std::string &description = "",
			const std::string &formula = "", size_t id = (size_t) -1,
			size_t group = (size_t) -1);

	void Register(std::shared_ptr<ParameterFormula> &parameter, size_t group =
			(size_t) -1);

	void Reset();

	/**\brief Update the structure
	 *
	 * Before calculating, this function evaluates the variants (if set) and
	 * assigns the parameters depending on their group to the Evaluation%s.
	 * The Parameter calculations are linked and an evaluationOrder is
	 * calculated for each variant.
	 */
	void Update();

	/**\brief Calculated the values of the variables.
	 *
	 * Does the actual calculation.
	 */
	void Calculate();

//	double GetValue(const size_t id, const size_t group) const;
//	double GetValue(const std::string &id, const size_t group) const;

	std::shared_ptr<ParameterFormula> GetParameter(const size_t id,
			const size_t group);

	bool HasID(const size_t id, const size_t group = (size_t) -1) const;

private:
	bool ConnectExternal(size_t n, std::set<size_t> &open);

	std::vector<std::shared_ptr<ParameterFormula>> parameter;

//	std::set<std::string> parameterNames;
//	std::unordered_map<std::string, size_t> lookup;
//	std::unordered_map<size_t, size_t> lookupID;

	std::vector<size_t> groupIdx;
	std::unordered_map<size_t, size_t> lookupGroupIdx;

	std::vector<size_t> evaluationOrder;

//	std::vector<Evaluation> evaluations;

	size_t currentGroup = (size_t) -1;
//	size_t maxGroupIdx = (size_t) 0;

};

#endif /* PARAMETERCOLLECTION_H */


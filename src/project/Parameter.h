///////////////////////////////////////////////////////////////////////////////
// Name               : Parameter.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 29.11.2022
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

#ifndef MATH_PARAMETER_H
#define MATH_PARAMETER_H

#include "../math/MathParser.h"

#include <map>
#include <string>

/*!\class Parameter
 * \brief Parameter with the capability of keeping a separate formula and value.
 *
 * Stores formula, value, unit, description and has modification-tracking.
 *
 * As this class is derived from the MathParser::Variable, it gets all
 * functions to access the value and Unit from there.
 *
 * \todo Remove the error mechanism and only rely on Exceptions.
 *
 */
class Parameter: public MathParser::Variable {
public:
	Parameter() = delete;
	explicit Parameter(const std::string &name, const std::string &description =
			std::string(""), const size_t id = (size_t) -1, const size_t group =
			(size_t) -1);
	virtual ~Parameter() = default;

	/**\brief Comparison of the name.
	 *
	 * Used for searching for a given name in a collection.
	 */
	bool operator==(const std::string &name) const;

	/**\brief Comparison operator for the ID.
	 *
	 * Used for searching for a given ID in a collection.
	 */
	bool operator==(const size_t id) const;

	void Modify(bool modified = true); ///< Set the modification flag.
	bool IsModified() const; ///< Retrieve the modification flag.

	/**\brief Getter for the description.
	 *
	 * Returns the description of the parameter. The intention of the value us
	 * to display the description on the tip window of the entry window for this
	 * parameter.
	 */
	std::string GetDescription() const;


protected:
	/**\brief ID for parameter
	 *
	 * ID assigned to this Parameter. It is advisable to add the wxWidgets GUI
	 * ID here. This simplifies referencing to this parameter when displaying
	 * it in a dialog or window.
	 */
	size_t id;

public:
	/**\brief Optional grouping of variables.
	 *
	 * This can be thought of as a namespace for the variables. If the same
	 * variable exists multiple times, the variables from the same namespace
	 * are preferred for calculating the formulas.
	 *
	 * This value might change later on, when parameter are made symmetric
	 * or unsymmetric (i.e. they are split into two).
	 */
	size_t group;

	std::string description; ///< Lengthy description (multiline?)

	/**\brief Indicates a change in value.
	 *
	 * This value is set to true depending on the implementation of the
	 * concrete Parameter. For ParameterFormulas for example, this flag is set
	 * to true, also when the value was changed by a calculation because of
	 * changed referenced variables.
	 *
	 */
	bool modified = true;

};

#endif /* MATH_PARAMETER_H */


///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterValue.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 29.03.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_PROJECT_PARAMETERVALUE_H_
#define SRC_PROJECT_PARAMETERVALUE_H_

/** \class ParameterValue
 * 	\code #include "ParameterValue.h"\endcode
 * 	\ingroup project
 *  \brief Parameter for storing debug values
 *
 * This is only for passing the MIDI values around. For everything else the
 * ParameterFormula should be used.
 */

#include "Parameter.h"

class ParameterValue: public Parameter {
public:
	ParameterValue() = delete;
	explicit ParameterValue(const std::string &name, double value,
			const std::string &description = std::string(""), const size_t id =
					(size_t) -1, const size_t group = (size_t) -1);
	virtual ~ParameterValue() = default;

	void SetValue(double newValue);

	void SetString(const std::string &newString_) override;
	std::string GetString() const override;
};

#endif /* SRC_PROJECT_PARAMETERVALUE_H_ */

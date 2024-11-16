///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterString.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 09.11.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_PROJECT_PARAMETERSTRING_H_
#define SRC_PROJECT_PARAMETERSTRING_H_

/** \class ParameterString
 * 	\code #include "ParameterString.h"\endcode
 * 	\ingroup GroupName
 *  \brief Description
 *
 * Text
 */

#include "Parameter.h"

class ParameterString: public Parameter {
public:
	ParameterString() = delete;
	explicit ParameterString(const std::string &name,
			const std::string &initial_value = std::string(""),
			const std::string &description = std::string(""), const size_t id =
					(size_t) -1, const size_t group = (size_t) -1);
	virtual ~ParameterString() = default;

	void SetString(const std::string &text);
	std::string GetString() const;

	double operator()() const;
	double ToDouble() const;
	bool ToBool() const;

	friend std::ostream& operator<<(std::ostream &out,
			const ParameterString &param);

private:
	std::string text;
};

#endif /* SRC_PROJECT_PARAMETERSTRING_H_ */

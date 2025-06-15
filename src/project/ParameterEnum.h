///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterEnum.h
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
#ifndef SRC_PROJECT_PARAMETERENUM_H_
#define SRC_PROJECT_PARAMETERENUM_H_

/** \class ParameterEnum
 * 	\code #include "ParameterEnum.h"\endcode
 * 	\ingroup project
 *  \brief Parameter containing an selection out of a group of options.
 *
 * For a given group of options one is selected.
 */

#include "Parameter.h"

#include <cstddef>
#include <initializer_list>
#include <string>

class ParameterEnum: public Parameter {
public:
	class Option {
	public:
		Option() = delete;
		Option(const std::string &name_, const std::string &description_ = "",
				const size_t id_ = (size_t) -1);

		std::string name;
		std::string description;
		size_t id;
	};

public:
	ParameterEnum() = delete;
	ParameterEnum(const std::string &name_,
			std::initializer_list<Option> values_,
			const std::string &description_ = "",
			const size_t id_ = (size_t) -1, const size_t group_ = (size_t) -1);
	virtual ~ParameterEnum() = default;

	bool IsValid() const;
	bool IsSelection(const std::string &selection_) const;
	void SetSelection(const size_t selection_);
	size_t GetSelectionIdx() const;

	void SetString(const std::string &selection_) override;
	std::string GetString() const override;

	size_t Size() const;
	std::string GetName(size_t idx) const;
	std::string GetDescription(size_t idx) const;
	size_t GetID(size_t idx) const;

private:
	std::vector<Option> options;
	size_t selection = (size_t) -1;
};

#endif /* SRC_PROJECT_PARAMETERENUM_H_ */

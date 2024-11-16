///////////////////////////////////////////////////////////////////////////////
// Name               : Parameter.cpp
// Purpose            : Store a parameter with a formula
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

#include "Parameter.h"

#include <cfloat>
#include <cmath>

Parameter::Parameter(const std::string &name_, const std::string &description_,
		const size_t id_, const size_t group_) :
		MathParser::Variable(name_), id(id_), group(group_), description(
				description_) {
}

bool Parameter::operator ==(const std::string &name) const {
	return (this->name.compare(name) == 0);
}

bool Parameter::operator ==(const size_t id) const {
	return this->id == id;
}

void Parameter::Modify(bool modified_) {
	this->modified = modified_;
}

bool Parameter::IsModified() const {
	return modified;
}

std::string Parameter::GetDescription() const {
	return description;
}


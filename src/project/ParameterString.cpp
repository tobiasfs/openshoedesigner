///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterString.cpp
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
#include "ParameterString.h"

void ParameterString::SetString(const std::string &text_) {
	modified = (text != text_);
	if (modified)
		text = text_;
}

std::string ParameterString::GetString() const {
	return text;
}

double ParameterString::operator ()() const {
	return std::stod(text);
}

double ParameterString::ToDouble() const {
	return std::stod(text);
}

bool ParameterString::ToBool() const {
	if (text.compare("yes") == 0)
		return true;
	if (text.compare("on") == 0)
		return true;
	if (std::stod(text) > 0.5)
		return true;
	return false;
}

ParameterString::ParameterString(const std::string &name_,
		const std::string &initial_value, const std::string &description_,
		const size_t id_, const size_t group_) :
		Parameter(name_, description_, id_, group_) {
	text = initial_value;
}

std::ostream& operator<<(std::ostream &out, const ParameterString &param) {
	out << param.text;
	return out;
}

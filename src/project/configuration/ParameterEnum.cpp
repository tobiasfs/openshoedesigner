///////////////////////////////////////////////////////////////////////////////
// Name               : ParameterEnum.cpp
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
#include "ParameterEnum.h"

#include <algorithm>
#include <stdexcept>
#include <sstream>

ParameterEnum::Option::Option(const std::string &name_,
		const std::string &description_, const size_t id_) :
		name(name_), description(description_), id(id_) {
}

ParameterEnum::ParameterEnum(const std::string &name_,
		std::initializer_list<Option> values_, const std::string &description_,
		const size_t id_, const size_t group_) :
		Parameter(name_, description_, id_, group_), options(values_) {
}

void ParameterEnum::SetSelection(const size_t selection_) {
	size_t oldSelection = selection;
	if (selection_ >= options.size())
		selection = (size_t) -1;
	else
		selection = selection_;
	modified = (selection != oldSelection);
}

void ParameterEnum::SetString(const std::string &selection_) {
	size_t oldSelection = selection;
	auto cmp = [&selection_](const Option &opt) {
		return selection_ == opt.name;
	};
	auto ref = std::find_if(options.begin(), options.end(), cmp);
	if (ref == options.end()) {
		std::ostringstream err;
		err << "The selection \"" << selection_
				<< "\" is not one of the possible options: ";
		bool first = true;
		for (const auto &opt : options) {
			if (first)
				first = false;
			else
				err << ", ";
			err << "\"" << opt.name << "\"";
		}
		err << ".";
		throw std::runtime_error(err.str());
	} else {
		selection = ref - options.begin();
	}
	modified = (selection != oldSelection);
}

bool ParameterEnum::IsSelection(const std::string &selection_) const {
#ifdef DEBUG
	auto cmp = [&selection_](const Option &opt) {
		return selection_ == opt.name;
	};
	auto ref = std::find_if(options.begin(), options.end(), cmp);
	if (ref == options.end()) {
		std::ostringstream err;
		err << "The selection \"" << selection_
				<< "\" is not one of the possible options: ";
		bool first = true;
		for (const auto &opt : options) {
			if (first)
				first = false;
			else
				err << ", ";
			err << "\"" << opt.name << "\"";
		}
		err << ".";
		throw std::runtime_error(err.str());
	}
#endif
	if (selection >= options.size())
		return false;
	return selection_ == options[selection].name;
}

bool ParameterEnum::IsValid() const {
	return selection < options.size();
}

size_t ParameterEnum::GetSelectionIdx() const {
	return selection;
}

std::string ParameterEnum::GetString() const {
	if (selection >= options.size())
		return "";
	return options[selection].name;
}

size_t ParameterEnum::Size() const {
	return options.size();
}

std::string ParameterEnum::GetName(size_t idx) const {
	if (idx >= options.size())
		return "";
	return options[idx].name;
}

std::string ParameterEnum::GetDescription(size_t idx) const {
	if (idx >= options.size())
		return "";
	return options[idx].description;
}

size_t ParameterEnum::GetID(size_t idx) const {
	if (idx >= options.size())
		return (size_t) -1;
	return options[idx].id;
}


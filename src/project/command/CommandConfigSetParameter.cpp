///////////////////////////////////////////////////////////////////////////////
// Name               : CommandShoeSetParameter.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.05.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "CommandConfigSetParameter.h"

#include "../../gui/gui.h"
#include "../ParameterFormula.h"
#include "../Project.h"

#include <exception>
#include <sstream>

CommandConfigSetParameter::CommandConfigSetParameter(const wxString &name,
		Project *project) :
		wxCommand(true, name), project(project) {
}

CommandConfigSetParameter::CommandConfigSetParameter(const wxString &name,
		Project *project, const size_t key, const std::string &newFormula) :
		wxCommand(true, name), project(project) {
	AddValue(key, newFormula);
}

void CommandConfigSetParameter::AddValue(const size_t key,
		const std::string &newFormula) {
	AddValue(key, (size_t) -1, newFormula);
}

void CommandConfigSetParameter::AddValue(const size_t key, const size_t group,
		const std::string &newFormula) {
	CommandConfigSetParameter::Change temp;
	temp.id = key;
	temp.group = group;
	temp.newFormula = newFormula;
	changes.push_back(temp);
}

bool CommandConfigSetParameter::Do() {
	if (project == NULL)
		return false;

	ParameterEvaluator &params = project->evaluator;
	params.Reset();

	bool modified = false;

	for (auto &change : changes) {
		std::shared_ptr<ParameterFormula> param = params.GetParameter(change.id,
				change.group);
		if (!param) {
			std::ostringstream err;
			err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
			err << "The parameter with the ID = " << change.id;
			if (change.group != (size_t) -1)
				err << " in group = " << change.group;
			err << " does not exist";
			err << " and cannot be set to " << change.newFormula << ".";
			throw std::logic_error(err.str());
		}
		change.oldFormula = param->GetString();
		param->SetString(change.newFormula);
		modified |= param->IsModified();
	}
	project->Update();
	return modified;
}

bool CommandConfigSetParameter::Undo() {
	if (project == NULL)
		return false;

	ParameterEvaluator &params = project->evaluator;
	params.Reset();

	for (auto &change : changes) {
		std::shared_ptr<ParameterFormula> param = params.GetParameter(change.id,
				change.group);
		if (!param) {
			std::ostringstream err;
			err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
			err << "The parameter with the ID = " << change.id;
			if (change.group != (size_t) -1)
				err << " in group = " << change.group;
			err << " does not exist";
			err << " and cannot be set to " << change.oldFormula << ".";
			throw std::logic_error(err.str());
		}
		param->SetString(change.oldFormula);
	}
	project->Update();
	return true;
}

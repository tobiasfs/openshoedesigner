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

#include "CommandShoeSetParameter.h"

#include <exception>

#include "../../gui/gui.h"
#include "../../math/Parameter.h"
#include "../Project.h"

CommandShoeSetParameter::CommandShoeSetParameter(const wxString &name,
		Project *project) :
		wxCommand(true, name), project(project) {
}

CommandShoeSetParameter::CommandShoeSetParameter(const wxString &name,
		Project *project, const size_t key, const std::string &newFormula) :
		wxCommand(true, name), project(project) {
	AddValue(key, newFormula);
}

void CommandShoeSetParameter::AddValue(const size_t key,
		const std::string &newFormula) {
	AddValue(key, (size_t) -1, newFormula);
}

void CommandShoeSetParameter::AddValue(const size_t key, const size_t group,
		const std::string &newFormula) {
	CommandShoeSetParameter::Change temp;
	temp.id = key;
	temp.group = group;
	temp.newFormula = newFormula;
	changes.push_back(temp);
}

bool CommandShoeSetParameter::Do(void) {
	if (project == NULL)
		return false;

	ParameterEvaluator &params = project->parameter;
	params.Reset();

	for (auto &change : changes) {
		std::shared_ptr<Parameter> param = params.GetParameter(change.id,
				change.group);
		change.oldFormula = param->GetFormula();
		param->SetFormula(change.newFormula);
	}

//	bool hasChanged = false;
//	for (auto &kv : newParameter) {
//		Parameter &p = shoe->GetParameter(kv.first);
//		hasChanged |= SetNew(kv, p);
//	}
//	if (hasChanged) {
//		shoe->Modify(true);
	project->Update();
//	}
//	return hasChanged;
	return true;
}

bool CommandShoeSetParameter::Undo(void) {
	if (project == NULL)
		return false;

	ParameterEvaluator &params = project->parameter;
	params.Reset();

	for (auto &change : changes) {
		std::shared_ptr<Parameter> param = params.GetParameter(change.id,
				change.group);
		param->SetFormula(change.oldFormula);
	}

//	for (auto &kv : oldParameter) {
//		Parameter &p = shoe->GetParameter(kv.first);
//		SetOld(kv, p);
//	}
//	shoe->Modify(true);
	project->Update();
	return true;
}

//bool CommandShoeSetParameter::SetNew(const std::pair<int, std::string> &kv,
//		Parameter &pf) {
//	bool changed = pf.formula.compare(kv.second) != 0;
//	oldParameter[kv.first] = pf.formula;
//	pf.formula = kv.second;
//	return changed;
//}
//
//void CommandShoeSetParameter::SetOld(const std::pair<int, std::string> &kv,
//		Parameter &pf) {
//	pf.formula = oldParameter[kv.first];
//}


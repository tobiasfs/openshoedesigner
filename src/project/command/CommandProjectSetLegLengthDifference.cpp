///////////////////////////////////////////////////////////////////////////////
// Name               : CommandProjectSetLegLengthDifference.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 30.04.2018
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

#include "CommandProjectSetLegLengthDifference.h"

CommandProjectSetLegLengthDifference::CommandProjectSetLegLengthDifference(
		const wxString &name, Project *project, std::string value) :
		wxCommand(true, name), project(project), value(value) {
}

bool CommandProjectSetLegLengthDifference::Do(void) {
	if (project == NULL)
		return false;
	oldValue = project->legLengthDifference->GetFormula();
	project->legLengthDifference->SetFormula(value);
//	project->measL.Modify(true);
//	project->measR.Modify(true);
	project->Update();
	return true;
}

bool CommandProjectSetLegLengthDifference::Undo(void) {
	if (project == NULL)
		return false;
	project->legLengthDifference->SetFormula(oldValue);
//	project->measL.Modify(true);
//	project->measR.Modify(true);
	project->Update();
	return true;
}

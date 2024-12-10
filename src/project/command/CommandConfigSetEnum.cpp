///////////////////////////////////////////////////////////////////////////////
// Name               : CommandProjectSetParameter.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.04.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "CommandConfigSetEnum.h"

#include "../../gui/gui.h"

CommandConfigSetEnum::CommandConfigSetEnum(const wxString &name,
		Project *project, int parameter, size_t value) :
		wxCommand(true, name), project(project), parameter(parameter), newValue(
				value) {
	DEBUGOUT << __func__ << ": " << name << "\n";
	oldValue = 0;
}

bool CommandConfigSetEnum::Do() {
	DEBUGOUT << __func__ << ".\n";

	switch (parameter) {
	case ID_MEASUREMENTSOURCE:
		oldValue = (int) project->config.measurementSource->GetSelectionIdx();
		project->config.measurementSource->SetSelection(newValue);
		break;
	case ID_MODELTYPE:
		oldValue = (int) project->config.modelType->GetSelectionIdx();
		project->config.modelType->SetSelection(newValue);
		break;
	case ID_GENERATOR:
		oldValue = (int) project->config.generator->GetSelectionIdx();
		project->config.generator->SetSelection(newValue);
		break;
	default:
		return false;
	}
	project->Update();
	return true;
}

bool CommandConfigSetEnum::Undo() {
	DEBUGOUT << __func__ << ".\n";

	switch (parameter) {

	case ID_MEASUREMENTSOURCE:
		oldValue = (int) project->config.measurementSource->GetSelectionIdx();
		project->config.measurementSource->SetSelection(oldValue);
		break;
	case ID_MODELTYPE:
		oldValue = (int) project->config.modelType->GetSelectionIdx();
		project->config.modelType->SetSelection(oldValue);
		break;
	case ID_GENERATOR:
		oldValue = (int) project->config.generator->GetSelectionIdx();
		project->config.generator->SetSelection(oldValue);
		break;

	default:
		return false;
	}
	project->Update();
	return true;
}

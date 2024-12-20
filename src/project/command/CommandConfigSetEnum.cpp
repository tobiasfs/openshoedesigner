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
	DEBUGOUT << __FUNCTION__ << ": " << name << "\n";
	oldValue = 0;
}

bool CommandConfigSetEnum::Do() {
	DEBUGOUT << __FUNCTION__ << ".\n";

	switch (parameter) {
	case ID_MEASUREMENTSOURCE:
		oldValue = (int) project->config.measurementSource->GetSelectionIdx();
		project->config.measurementSource->SetSelection(newValue);
		break;
	case ID_LASTCONSTRUCTIONTYPE:
		oldValue =
				(int) project->config.lastConstructionType->GetSelectionIdx();
		project->config.lastConstructionType->SetSelection(newValue);
		break;
	case ID_LASTMODIFY:
		oldValue = (int) project->config.lastModify->GetSelectionIdx();
		project->config.lastModify->SetSelection(newValue);
		break;
	case ID_LASTREORIENT:
		oldValue = (int) project->config.lastReorient->GetSelectionIdx();
		project->config.lastReorient->SetSelection(newValue);
		break;
	case ID_HEELCONSTRUCTIONTYPE:
		oldValue =
				(int) project->config.heelConstructionType->GetSelectionIdx();
		project->config.heelConstructionType->SetSelection(newValue);
		break;
	case ID_HEELREORIENT:
		oldValue = (int) project->config.heelReorient->GetSelectionIdx();
		project->config.heelReorient->SetSelection(newValue);
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
	DEBUGOUT << __FUNCTION__ << ".\n";

	switch (parameter) {

	case ID_MEASUREMENTSOURCE:
		project->config.measurementSource->SetSelection(oldValue);
		break;
	case ID_LASTCONSTRUCTIONTYPE:
		project->config.lastConstructionType->SetSelection(oldValue);
		break;
	case ID_LASTMODIFY:
		project->config.lastModify->SetSelection(oldValue);
		break;
	case ID_LASTREORIENT:
		project->config.lastReorient->SetSelection(oldValue);
		break;
	case ID_HEELCONSTRUCTIONTYPE:
		project->config.heelConstructionType->SetSelection(oldValue);
		break;
	case ID_HEELREORIENT:
		project->config.heelReorient->SetSelection(oldValue);
		break;
	case ID_GENERATOR:
		project->config.generator->SetSelection(oldValue);
		break;

	default:
		return false;
	}
	project->Update();
	return true;
}

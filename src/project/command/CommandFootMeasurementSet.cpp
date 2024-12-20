///////////////////////////////////////////////////////////////////////////////
// Name               : CommandFootMeasurementSet.cpp
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

#include "CommandFootMeasurementSet.h"
#include "../../gui/gui.h"

CommandFootMeasurementSet::CommandFootMeasurementSet(const wxString &name,
		Project *project, ProjectView::Side active, int parameter,
		const std::string &value) :
		wxCommand(true, name), project(project), active(active), parameterID(
				parameter), newValue(value) {
}

bool CommandFootMeasurementSet::Do() {
	if (project == NULL)
		return false;

	bool hasChanged = false;

	if (active == ProjectView::Side::Left
			|| active == ProjectView::Side::Both) {
		FootMeasurements &meas = project->footL;
		oldValueLeft = Replace(meas, parameterID, newValue);
		hasChanged |= meas.GetParameter(parameterID)->IsModified();
	}
	if (active == ProjectView::Side::Right
			|| active == ProjectView::Side::Both) {
		FootMeasurements &meas = project->footR;
		oldValueRight = Replace(meas, parameterID, newValue);
		hasChanged |= meas.GetParameter(parameterID)->IsModified();
	}
	if (hasChanged)
		project->Update();

	return hasChanged;
}

bool CommandFootMeasurementSet::Undo() {
	if (project == NULL)
		return false;

	bool hasChanged = false;
	if (active == ProjectView::Side::Left
			|| active == ProjectView::Side::Both) {
		FootMeasurements &meas = project->footL;
		std::string currentValue;
		currentValue = Replace(meas, parameterID, oldValueLeft);
		hasChanged |= meas.GetParameter(parameterID)->IsModified();

	}
	if (active == ProjectView::Side::Right
			|| active == ProjectView::Side::Both) {
		FootMeasurements &meas = project->footR;
		std::string currentValue;
		currentValue = Replace(meas, parameterID, oldValueRight);
		hasChanged |= meas.GetParameter(parameterID)->IsModified();
	}
	if (hasChanged)
		project->Update();

	return true;
}

std::string CommandFootMeasurementSet::Replace(FootMeasurements &meas, int ID,
		std::string newFormula) {
	std::string lastValue;
	std::shared_ptr<ParameterFormula> param = meas.GetParameter(ID);
	lastValue = param->GetString();
	param->SetString(newFormula);
	return lastValue;
}

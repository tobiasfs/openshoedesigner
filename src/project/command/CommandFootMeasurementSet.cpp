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
		wxString value) :
		wxCommand(true, name), project(project), active(active), parameter(
				parameter), value(value) {
}

bool CommandFootMeasurementSet::Do(void) {
	if (project == NULL)
		return false;

	bool hasChanged = false;
	if (active == ProjectView::Side::Left
			|| active == ProjectView::Side::Both) {
		FootMeasurements * meas = &(project->measL);
		oldValueLeft = Replace(meas, parameter, value);
		bool temp = !value.IsSameAs(oldValueLeft);
		if (temp)
			meas->Modify(true);
		hasChanged |= temp;
	}
	if (active == ProjectView::Side::Right
			|| active == ProjectView::Side::Both) {
		FootMeasurements * meas = &(project->measR);
		oldValueRight = Replace(meas, parameter, value);
		bool temp = !value.IsSameAs(oldValueRight);
		if (temp)
			meas->Modify(true);
		hasChanged |= temp;
	}
	if (hasChanged)
		project->Update();

	return hasChanged;
}

bool CommandFootMeasurementSet::Undo(void) {
	if (project == NULL)
		return false;

	bool hasChanged = false;
	if (active == ProjectView::Side::Left
			|| active == ProjectView::Side::Both) {
		FootMeasurements *meas = &(project->measL);
		wxString currentValue;
		currentValue = Replace(meas, parameter, oldValueLeft);
		bool temp = !currentValue.IsSameAs(oldValueLeft);
		if (temp)
			meas->Modify(true);
		hasChanged |= temp;

	}
	if (active == ProjectView::Side::Right
			|| active == ProjectView::Side::Both) {
		FootMeasurements *meas = &(project->measR);
		wxString currentValue;
		currentValue = Replace(meas, parameter, oldValueRight);
		bool temp = !currentValue.IsSameAs(oldValueRight);
		if (temp)
			meas->Modify(true);
		hasChanged |= temp;
	}
	if (hasChanged)
		project->Update();
	return true;
}

wxString CommandFootMeasurementSet::Replace(FootMeasurements *meas,
		int parameter, wxString newValue) {
	wxString lastValue;
//	Parameter &param = meas->GetParameter(parameter);
//	lastValue = param.formula;
//	param.formula = newValue.ToStdString();
	return lastValue;
}

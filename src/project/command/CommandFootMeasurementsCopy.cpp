///////////////////////////////////////////////////////////////////////////////
// Name               : CommandFootMeasurementsCopy.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.04.2019
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

#include "CommandFootMeasurementsCopy.h"

#include <stdexcept>

CommandFootMeasurementsCopy::CommandFootMeasurementsCopy(const wxString &name,
		Project *project, ProjectView::Side toSide_) :
		wxCommand(true, name), project(project), toSide(toSide_) {
	if (toSide_ == ProjectView::Side::Both)
		throw std::logic_error(
				"CommandFootMeasurementsCopy - Cannot copy to both sides.");
}

bool CommandFootMeasurementsCopy::Do() {
	DEBUGOUT << __FUNCTION__ << ": " << __FILE__ << "\n";
	if (project == NULL)
		return false;
	if (toSide == ProjectView::Side::Left) {
		oldValue = project->footL;
		project->footL = project->footR;
		project->footL.Modify(true);
	} else {
		oldValue = project->footR;
		project->footR = project->footL;
		project->footR.Modify(true);
	}
	project->Update();
	return true;
}

bool CommandFootMeasurementsCopy::Undo() {
	DEBUGOUT << __FUNCTION__ << ": " << __FILE__ << "\n";
	if (project == NULL)
		return false;

	if (toSide == ProjectView::Side::Left) {
		project->footL = oldValue;
		project->footL.Modify(true);
	} else {
		project->footR = oldValue;
		project->footR.Modify(true);
	}
	project->Update();
	return true;
}

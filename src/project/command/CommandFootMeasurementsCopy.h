///////////////////////////////////////////////////////////////////////////////
// Name               : CommandFootMeasurementsCopy.h
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

#ifndef COMMANDFOOTMEASUREMENTSCOPY_H
#define COMMANDFOOTMEASUREMENTSCOPY_H

/*!\class CommandFootMeasurementsCopy
 * \ingroup command
 * \brief Command to copy the measurements of the foot
 *
 * Copies the measurements of the foot from lest to right or vice versae.
 */

#include <wx/cmdproc.h>
#include <wx/string.h>

#include "../Parameter.h"
#include "../Project.h"
#include "../ProjectView.h"

class CommandFootMeasurementsCopy:public wxCommand {
public:
	CommandFootMeasurementsCopy(const wxString& name, Project* project,
			bool LeftToRight);

	bool Do(void);
	bool Undo(void);

protected:
	Project* project;
	bool LeftToRight;
	FootMeasurements oldValue;
};

#endif /* COMMANDFOOTMEASUREMENTSCOPY_H */

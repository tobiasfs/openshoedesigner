///////////////////////////////////////////////////////////////////////////////
// Name               : CommandProjectSetParameter.h
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

#ifndef PROJECT_COMMAND_COMMANDPROJECTSETPARAMETER_H
#define PROJECT_COMMAND_COMMANDPROJECTSETPARAMETER_H

/*!\class CommandProjectSetParameter
 * \ingroup command
 * \brief Command to set a parameter for the project itself
 *
 * ...
 */

#include <wx/cmdproc.h>
#include <wx/string.h>

#include "../Project.h"

class CommandConfigSetEnum: public wxCommand {
public:
	CommandConfigSetEnum(const wxString &name, Project *project, int parameter,
			size_t value);

	bool Do();
	bool Undo();

protected:
	Project *project;
	int parameter;
	size_t newValue;
	size_t oldValue;
	FootMeasurements oldMeasurements;

};

#endif /* PROJECT_COMMAND_COMMANDPROJECTSETPARAMETER_H */

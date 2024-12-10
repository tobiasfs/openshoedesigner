///////////////////////////////////////////////////////////////////////////////
// Name               : CommandFootModelSetParameter.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.02.2018
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

#ifndef COMMANDFOOTMODELSETPARAMETER_H
#define COMMANDFOOTMODELSETPARAMETER_H

/*!\class CommandFootModelSetParameter
 * \ingroup command
 * \brief Command to change foot model parameters
 *
 * This command changes the parameters of teh bone / foot model.
 */

#include <wx/cmdproc.h>
#include <wx/string.h>

#include "../Project.h"
#include "../ProjectView.h"

class CommandFootModelSetParameter: public wxCommand {
public:
	enum class Type {
		Length, R1, R2, S1, S2
	};

	CommandFootModelSetParameter(const wxString &name, Project *project,
			size_t boneNr, Type field, std::string newFormula);

	bool Do();
	bool Undo();

protected:
	Project *project;
	ProjectView::Side activeSide;
	size_t boneNr;
	Type field;
	std::string newFormula;
	std::string oldFormulaL;
	std::string oldFormulaR;
};

#endif /* COMMANDFOOTMODELSETPARAMETER_H */

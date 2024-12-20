///////////////////////////////////////////////////////////////////////////////
// Name               : CommandConfigSetString.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 16.12.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef SRC_PROJECT_COMMAND_COMMANDCONFIGSETSTRING_H_
#define SRC_PROJECT_COMMAND_COMMANDCONFIGSETSTRING_H_

/** \class CommandConfigSetString
 * 	\code #include "CommandConfigSetString.h"\endcode
 * 	\ingroup GroupName
 *  \brief Description
 *
 * Text
 */

#include "../Project.h"
#include <wx/cmdproc.h>

#include <string>
class CommandConfigSetString: public wxCommand {
public:
	CommandConfigSetString(const wxString &name, Project *project,
			const size_t id, const std::string &newString);

	bool Do() override;
	bool Undo() override;

protected:
	Project *project;
	int id;
	std::string newValue;
	std::string oldValue;

private:
	void GetParam(std::shared_ptr<ParameterString> &param);
};

#endif /* SRC_PROJECT_COMMAND_COMMANDCONFIGSETSTRING_H_ */

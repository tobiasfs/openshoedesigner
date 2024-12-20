///////////////////////////////////////////////////////////////////////////////
// Name               : CommandConfigSetString.cpp
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
#include "CommandConfigSetString.h"

#include "../../gui/gui.h"

#include <sstream>
#include <stdexcept>

CommandConfigSetString::CommandConfigSetString(const wxString &name,
		Project *project, const size_t id, const std::string &newString) :
		wxCommand(true, name), project(project), id(id), newValue(newString) {
}

bool CommandConfigSetString::Do() {
	if (project == NULL)
		return false;
	bool modified = false;

	std::shared_ptr<ParameterString> param;
	GetParam(param);

	oldValue = param->GetString();
	param->SetString(newValue);
	modified |= param->IsModified();

	project->Update();
	return modified;
}

bool CommandConfigSetString::Undo() {
	if (project == NULL)
		return false;

	std::shared_ptr<ParameterString> param;
	GetParam(param);

	param->SetString(oldValue);

	project->Update();
	return true;
}

void CommandConfigSetString::GetParam(std::shared_ptr<ParameterString> &param) {
	switch (id) {
	case ID_LAST:
		param = project->config.filenameLast;
		break;
	case ID_SCAN:
		param = project->config.filenameScan;
		break;
	default:
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		err << "The parameter with the ID = " << id;
		err << " does not exist";
		err << " and cannot be set to " << newValue << ".";
		throw std::logic_error(err.str());
	}
}


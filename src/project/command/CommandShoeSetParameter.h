///////////////////////////////////////////////////////////////////////////////
// Name               : CommandShoeSetParameter.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.05.2018
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

#ifndef COMMANDSHOESETPARAMETER_H
#define COMMANDSHOESETPARAMETER_H

/*!\class CommandShoeSetParameter
 * \ingroup command
 * \brief Command to set the parameters of the shoe
 *
 * ...
 */

#include <wx/cmdproc.h>
#include <string>
#include <vector>

class Parameter;
class Project;

class CommandShoeSetParameter: public wxCommand {
public:
	CommandShoeSetParameter(const wxString &name, Project *project);
	CommandShoeSetParameter(const wxString &name, Project *project,
			const size_t key, const std::string &newFormula);

	void AddValue(const size_t key, const std::string &newFormula);
	void AddValue(const size_t key, const size_t group,
			const std::string &newFormula);

	bool Do(void);
	bool Undo(void);

//protected:
//	bool SetNew(const std::pair<int, std::string> &kv, Parameter &pf);
//	void SetOld(const std::pair<int, std::string> &kv, Parameter &pf);

//	ParameterFormula* GetParameterByID(Shoe *shoe, int id);

private:
	struct Change {
	public:
		size_t id = (size_t) -1;
		size_t group = (size_t) -1;
		std::string oldFormula;
		std::string newFormula;
	};

	Project *project;

	std::vector<Change> changes;
};

#endif /* COMMANDSHOESETPARAMETER_H */


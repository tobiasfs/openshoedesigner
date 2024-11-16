///////////////////////////////////////////////////////////////////////////////
// Name               : IniFile.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 16.11.2018
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

#ifndef INIFILE_H
#define INIFILE_H

/*!\class IniFile
 * \brief ...
 *
 * ...
 */

#include <wx/string.h>
#include <vector>

class IniFile {
public:
	class Section {
	public:
		class Parameter {
		public:
			Parameter(wxString name, wxString value);
			wxString name;
			wxString value;
		};
	public:
		Section(wxString name);
		wxString name;
		std::vector <Parameter> param;
		wxString GetParameter(wxString name, wxString defaultvalue = _T("")) const;

	};

public:
	IniFile(bool casesensitive = true);
	IniFile(wxString filename, bool casesensitive = true);
	virtual ~IniFile();
	bool ReadFile(wxString filename);

	const Section* FindSection(wxString name) const;
	const Section* NextSection(const IniFile::Section * lastsection) const;

	std::vector <Section> section;
private:
	const bool casesensitive;

	wxString CleanString(wxString text);
};

#endif /* INIFILE_H */

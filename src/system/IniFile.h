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
 * \brief INI-file reader
 *
 * Read an INI configuration file and store the sections and parameters found.
 */

#include <string>
#include <vector>

class IniFile {
public:
	class Section {
	public:
		class Parameter {
		public:
			Parameter(std::string name, std::string value);
			std::string name;
			std::string value;
		};
	public:
		Section(std::string name);
		std::string name;
		std::vector<Parameter> param;
		std::string GetParameter(std::string name,
				std::string defaultvalue = "") const;
	};

public:
	IniFile(bool casesensitive = true);
	IniFile(std::string filename, bool casesensitive = true);
	virtual ~IniFile() = default;

	void ReadFile(std::string filename);

	const Section* FindSection(std::string name) const;
	const Section* NextSection(const IniFile::Section *lastsection) const;

	std::vector<Section> section;
	bool casesensitive;

private:
	static std::string CleanString(std::string text);
	static bool StringCmp(const std::string &lhs, const std::string &rhs);
	static bool StringCmpI(const std::string &lhs, const std::string &rhs);

};

#endif /* INIFILE_H */

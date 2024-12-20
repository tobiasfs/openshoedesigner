///////////////////////////////////////////////////////////////////////////////
// Name               : IniFile.cpp
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

#include "IniFile.h"

#include <stddef.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define ERROR(errortxt) { \
		std::ostringstream err; \
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " -"; \
		err << errortxt; \
		throw std::runtime_error(err.str()); \
	}

IniFile::Section::Parameter::Parameter(std::string name, std::string value) :
		name(name), value(value) {
}

IniFile::Section::Section(std::string name) :
		name(name) {
}

IniFile::IniFile(bool casesensitive) :
		casesensitive(casesensitive) {
}

IniFile::IniFile(std::string filename, bool casesensitive) :
		casesensitive(casesensitive) {
	ReadFile(filename);
}

void IniFile::ReadFile(std::string filename) {
	std::ifstream text(filename);
	if (!text.is_open() || !text.good())
		ERROR("The file " << filename << " could not be opened for reading.");

	std::string sectionName("default");

	section.clear();

	size_t lineCount = 0;
	// NOTE: When changing to std::istream, check https://stackoverflow.com/questions/5605125/why-is-iostreameof-inside-a-loop-condition-i-e-while-stream-eof-cons
	while (!text.eof()) {
		std::string x;
		std::getline(text, x);
		lineCount++;
		if (x.empty())
			continue;
		const size_t posFirstChar = x.find_first_not_of(" \f\n\r\t");
		const size_t posComment = x.find_first_of(";#-%");
		if (posFirstChar == std::string::npos)
			continue;
		if (posComment != std::string::npos && posComment == posFirstChar)
			continue;
		const size_t posLeftBracket = x.find_first_of('[');
		if (posLeftBracket != std::string::npos
				&& posLeftBracket == posFirstChar) {
			size_t posRightBracket = x.find_last_of(']');
			if (posRightBracket == std::string::npos) {
				ERROR(
						"INI-File " << filename << " has a broken section header in line " << lineCount << ".");
				posRightBracket = x.length();
			}
			sectionName = x.substr(posLeftBracket + 1,
					posRightBracket - posLeftBracket - 1);
			IniFile::Section temp(sectionName);
			section.push_back(temp);
			continue;
		}
		size_t posEqual = x.find_first_of('=');
		if (posEqual != std::string::npos) {
			if (posEqual == 0) {
				ERROR(
						"In line " << lineCount << " of the INI file " << filename << " there is no key before the equal sign.");
			}
			if (posEqual == x.length() - 1) {
				ERROR(
						"In line " << lineCount << " of the INI file there is no value after the equal sign.\n");
			}
			const size_t poskey0 = x.find_first_not_of(" \f\n\r\t");
			const size_t poskey1 = x.find_last_not_of(" \f\n\r\t",
					posEqual - 1);
			const size_t posvalue0 = x.find_first_not_of(" \f\n\r\t",
					posEqual + 1);
			const size_t posvalue1 = x.find_last_not_of(" \f\n\r\t");

			std::string key = CleanString(
					x.substr(poskey0, poskey1 - poskey0 + 1));
			std::string value = CleanString(
					x.substr(posvalue0, posvalue1 - posvalue0 + 1));

			if (!section.empty()) {
				IniFile::Section::Parameter param(key, value);
				section.back().param.push_back(param);
			}
		}
	}
}

std::string IniFile::Section::GetParameter(std::string name,
		std::string defaultvalue) const {
	for (size_t n = 0; n < param.size(); ++n) {
		if (StringCmpI(param[n].name, name)) {
			return param[n].value;
		}
	}
	return defaultvalue;
}

const IniFile::Section* IniFile::FindSection(std::string name) const {
	if (IniFile::casesensitive) {
		for (size_t n = 0; n < section.size(); ++n)
			if (StringCmp(section[n].name, name))
				return &(section[n]);
	} else {
		for (size_t n = 0; n < section.size(); ++n)
			if (StringCmpI(section[n].name, name))
				return &(section[n]);
	}
	return nullptr;
}

const IniFile::Section* IniFile::NextSection(
		const IniFile::Section *lastsection) const {
	bool temp = false;
	std::string name;
	for (size_t n = 0; n < section.size(); ++n) {
		if (temp) {
			if (IniFile::casesensitive) {
				if (StringCmp(section[n].name, name))
					return &(section[n]);
			} else {
				if (StringCmpI(section[n].name, name))
					return &(section[n]);
			}
		}
		if (lastsection == &(section[n])) {
			temp = true;
			name = section[n].name;
		}
	}
	return nullptr;
}

std::string IniFile::CleanString(std::string text) {
	const size_t n0 = text.find_first_of("\"");
	const size_t n1 = text.find_last_of("\"");
	if (n0 == std::string::npos || n1 == std::string::npos)
		return text;
	if (n0 == n1)
		return text;
	text = text.substr(n0, n1 - n0);
	return text;
}

bool IniFile::StringCmp(const std::string &lhs, const std::string &rhs) {
	return lhs == rhs;
}

bool IniFile::StringCmpI(const std::string &lhs, const std::string &rhs) {
	auto eqi = [](char lhs, char rhs) {
		return std::tolower(static_cast<unsigned char>(lhs))
				== std::tolower(static_cast<unsigned char>(rhs));
	};
	return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), eqi);
}

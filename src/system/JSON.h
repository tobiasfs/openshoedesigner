///////////////////////////////////////////////////////////////////////////////
// Name               : JSON.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 26.11.2019
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

#ifndef SYSTEM_JSON_H
#define SYSTEM_JSON_H

/*!\class JSON
 * \brief JSON handling class
 *
 * Reads and writes JSON files
 * and stores the data.
 *
 * Uses only standard libraries (stl & the other normal ones, i.e. only what
 * is found on https://en.cppreference.com/w/ )
 */

// https://esprima.org/
#include <stddef.h>
#include <istream>
#include <ostream>
#include <map>
#include <string>
#include <vector>
#include <array>

class JSON {
private:
	enum class Type {
		Null, Boolean, Number, String, Array, Object
	};
public:
	JSON() = default;

	static JSON Load(const std::string &filename);
	static JSON Load(std::istream &in);
	void Save(const std::string &filename, bool usenewline = true,
			size_t indent = 0);
	void Save(std::ostream &out, bool usenewline = true, size_t indent = 0);

	const JSON& Begin() const;

	JSON& operator[](const std::string &key);
	const JSON& operator[](const std::string &key) const;
	JSON& operator[](size_t index);
	const JSON& operator[](size_t index) const;

	bool IsNull() const;
	Type GetType() const;
	bool GetBool(const bool defaultvalue = false) const;
	double GetNumber(const double defaultvalue = 0.0) const;
	std::string GetString(
			const std::string &defaultvalue = std::string("")) const;
	bool IsArray() const;
	bool IsObject() const;
	// A Null has the size 1, because it is not empty.
	size_t Size() const;
	bool HasKey(const std::string &key) const;
	std::string GetKey(size_t index) const;

	void SetNull();
	void SetBool(const bool value);
	void SetNumber(const double value);
	void SetString(const std::string &value);
	void SetArray(size_t size);
	void SetObject(const bool clear = true);

private:
	Type type = Type::Null;
	bool valueBoolean = false;
	double valueNumber = 0.0;
	std::string valueString;
	std::vector<JSON> valueArray;
	std::map<std::string, JSON> valueObject;

	class Token {
	public:
		Token() = default;
		std::string Lower() const;
		enum class Type {
			_Null, _Boolean, _Char, _String, _Number
		} type = Type::_Null;
		char c = 0;
		bool b = false;
		std::string str;
		double num = 0.0;
	};

	class FileTokenizer {
	public:
		explicit FileTokenizer(std::istream *in);
		FileTokenizer(const FileTokenizer&) = delete;
		FileTokenizer& operator=(const FileTokenizer&) = delete;
		virtual ~FileTokenizer() = default;

		void NextToken();
		Token token;

		std::istream *in;
		std::vector<char> buffer;
		char nextc;
		size_t charsread;
		size_t position;
		size_t linenumber;
		size_t column;
		size_t buffersize;
	};

	static JSON Parse(FileTokenizer &ft, int maxRecursion);
	void ToStream(std::ostream &out, bool usenewline, size_t indent) const;
	static std::string EscapeString(const std::string &txt);

private:
	static const size_t stateCount = 27;
	static const size_t charWidth = 256; // Normal 8-bit bytes
	static std::array<unsigned char, stateCount * charWidth * 2> lexerTables;

	/**\brief Generate table for the turbo-lexer.
	 *
	 * This static function generates the (actually two) tables for the
	 * turbo-lexer. The tables are sitting side by side in an 6 kByte array.
	 * The first table is the state transition table. It cuts the text fed to
	 * it up in the lexems for the JSON language. The second table is an action
	 * table. It describes special handling for some incoming chars, like
	 * building up numbers or adding characters to IDs.
	 *
	 * The decision to side both tables is due to the fact, that the action-
	 * table relies on the decisions in the state-table and cannot be created
	 * separately with this approach.
	 *
	 * \todo Change approach to use separate tables. (x<<8 is a little faster than x<<9)
	 */
	static std::array<unsigned char, stateCount * charWidth * 2> InitTables();
};

#endif /* SYSTEM_JSON_H */


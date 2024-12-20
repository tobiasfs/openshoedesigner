///////////////////////////////////////////////////////////////////////////////
// Name               : JSON.cpp
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

#include "JSON.h"

#include <fstream>
#include <stdexcept>
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <cmath>

static JSON nullobject;

JSON JSON::Load(const std::string &filename) {
	std::ifstream in;
	in.open(filename.c_str(), std::ifstream::in | std::ios::binary);
	if (!in.good()) {
		throw(std::runtime_error("JSON::Load(...) - Could not read file."));
	}
	return JSON::Load(in);
}

JSON JSON::Load(std::istream &in) {
	FileTokenizer ft(&in);
	ft.NextToken();
	return Parse(ft, 127);
}

void JSON::Save(const std::string &filename, bool usenewline, size_t indent) {
	std::ofstream out;
	out.open(filename.c_str(), std::ofstream::out | std::ios::binary);
	JSON::Save(out, usenewline, indent);
}

void JSON::Save(std::ostream &out, bool usenewline, size_t indent) {
	ToStream(out, usenewline, indent);
}

const JSON& JSON::Begin() const {
	if (type == Type::Array)
		return *(valueArray.begin());
	if (type == Type::Object)
		return valueObject.begin()->second;
	throw(std::logic_error("Not an array or object."));
}

size_t JSON::Size() const {
	if (type == Type::Array)
		return valueArray.size();
	if (type == Type::Object)
		return valueObject.size();
	return 1;
}

bool JSON::HasKey(const std::string &key) const {
	if (type != Type::Object)
		return false;
	return (valueObject.find(key) != valueObject.end());
}

std::string JSON::GetKey(size_t index) const {
	if (type == Type::Object) {
		auto it = valueObject.cbegin();
		for (size_t n = 0; n < index; ++n)
			++it;
		return it->first;
	}
	throw(std::logic_error("Not an object."));
}

JSON& JSON::operator [](const std::string &key) {
	if (type == Type::Object)
		return valueObject[key];
	throw(std::logic_error("Not an object."));
}

const JSON& JSON::operator [](const std::string &key) const {
	if (type == Type::Object) {
		if (valueObject.find(key) == valueObject.end())
			return nullobject;
		return valueObject.at(key);
	}
	throw(std::logic_error("Not an object."));
}

JSON& JSON::operator [](size_t index) {
	if (type == Type::Array)
		return valueArray.at(index);
	if (type == Type::Object) {
		auto it = valueObject.begin();
		for (size_t n = 0; n < index; ++n)
			++it;
		return it->second;
	}
	throw(std::logic_error("Not an array or object."));
}

const JSON& JSON::operator [](size_t index) const {
	if (type == Type::Array)
		return valueArray.at(index);
	if (type == Type::Object) {
		auto it = valueObject.cbegin();
		for (size_t n = 0; n < index; ++n)
			++it;
		return it->second;
	}
	throw(std::logic_error("Not an array or object."));
}

void JSON::SetNull() {
	type = Type::Null;
}

void JSON::SetArray(size_t size) {
	type = Type::Array;
	valueArray.resize(size);
}

void JSON::SetObject(const bool clear) {
	type = Type::Object;
	if (clear)
		valueObject.clear();
}

void JSON::SetBool(const bool value) {
	type = Type::Boolean;
	valueBoolean = value;
}

void JSON::SetNumber(const double value) {
	type = Type::Number;
	valueNumber = value;
}

void JSON::SetString(const std::string &value) {
	type = Type::String;
	valueString = value;
}

JSON::Type JSON::GetType() const {
	return type;
}

bool JSON::IsNull() const {
	return (type == Type::Null);
}

bool JSON::IsArray() const {
	return (type == Type::Array);
}

bool JSON::IsObject() const {
	return type == Type::Object;
}

bool JSON::GetBool(const bool defaultvalue) const {
	if (type == Type::Boolean)
		return valueBoolean;
	if (type == Type::Number)
		return fabs(valueNumber) > 1e-6;
	if (type == Type::Null)
		return defaultvalue;
	throw(std::logic_error("Not convertable to boolean."));
}

double JSON::GetNumber(const double defaultvalue) const {
	if (type == Type::Number)
		return valueNumber;
	if (type == Type::Boolean)
		return (valueBoolean) ? 1 : 0;
	if (type == Type::String)
		return strtod(valueString.c_str(), nullptr);
	if (type == Type::Null)
		return defaultvalue;
	throw(std::logic_error("Not a number."));
}

std::string JSON::GetString(const std::string &defaultvalue) const {
	if (type == Type::String)
		return valueString;
	if (type == Type::Boolean)
		return valueBoolean ? "true" : "false";
	if (type == Type::Null)
		return defaultvalue;
	throw(std::logic_error("Not a string."));
}

JSON JSON::Parse(FileTokenizer &ft, int maxRecursion) {
	JSON temp;
	if (maxRecursion <= 0)
		throw(std::runtime_error(
				"JSON JSON::Parse(...) - Maximum recursion depth reached."));
	switch (ft.token.type) {
	case Token::Type::_Null:
		temp.type = Type::Null;
		break;
	case Token::Type::_Char:
		if (ft.token.c == '{') {
			temp.type = Type::Object;
			ft.NextToken();
			while (ft.token.type == Token::Type::_String) {
				std::string key = ft.token.str;
				ft.NextToken();
				if (ft.token.type != Token::Type::_Char || ft.token.c != ':') {
					throw(std::logic_error("Expected ':' here."));
				}
				ft.NextToken();
				JSON value = Parse(ft, maxRecursion - 1);
				temp.valueObject[key] = value;
				ft.NextToken();
				if (ft.token.type != Token::Type::_Char
						|| (ft.token.c != ',' && ft.token.c != '}')) {

					throw(std::logic_error(
							"JSON::Parse(...) - Expected ',' or '}' here."));
				}
				if (ft.token.c == '}')
					break;
				ft.NextToken();
			}
		}
		if (ft.token.c == '[') {
			temp.type = Type::Array;
			ft.NextToken();
			while (ft.token.type != Token::Type::_Char || ft.token.c != ']') {
				JSON value = Parse(ft, maxRecursion - 1);
				temp.valueArray.push_back(value);
				ft.NextToken();
				if (ft.token.type != Token::Type::_Char
						|| (ft.token.c != ',' && ft.token.c != ']')) {
					throw(std::logic_error(
							"JSON::Parse(...) - Expected ',' or ']' here."));
				}
				if (ft.token.c == ']')
					break;
				ft.NextToken();
			}
		}
		break;
	case Token::Type::_Boolean:
		temp.type = Type::Boolean;
		temp.valueBoolean = ft.token.b;
		break;
	case Token::Type::_String:
		temp.type = Type::String;
		temp.valueString = ft.token.str;
		break;
	case Token::Type::_Number:
		temp.type = Type::Number;
		temp.valueNumber = ft.token.num;
		break;
	}
	return temp;
}

std::string JSON::Token::Lower() const {
	std::string temp = str;
	for (size_t n = 0; n < temp.size(); ++n)
		if (temp[n] >= 'A' && temp[n] <= 'Z')
			temp[n] = (char) (temp[n] + 32);
	return temp;
}

JSON::FileTokenizer::FileTokenizer(std::istream *in) {
	buffersize = 1e7;

	charsread = 0;
	position = 0;
	linenumber = 1;
	column = 0;
	nextc = 0;

	this->in = in;

	buffer.resize(buffersize + 1);
}

void JSON::FileTokenizer::NextToken() {
	bool negative = false;
	double factor = 0.1;
	bool exponentnegative = false;
	int exponent = 0;

	token.c = 0;
	token.b = false;
	token.num = 0.0;
	token.str.clear();
	token.type = Token::Type::_Null;
	unsigned char state = 0;

//	std::cout << "Line: " << linenumber << " Column: " << column << "\n";

	do {
		if (position >= charsread) {
			in->read(buffer.data(), buffersize);
			charsread = in->gcount();
			if (charsread <= buffersize) {
				// Add a 0 byte to the end
				buffer[charsread] = 0;
				++charsread;
			}
			position = 0;
		}
		do {
			char c = nextc;
			nextc = buffer[position++];
//			std::cout << c;
//			++position;
//			if(c == 10){
//				++linenumber;
//				column = 0;
//			}else{
//				column += (c == (char) 9)? 2 : 1;
//			}
			unsigned char nextstate = JSON::lexerTables[(state << 9)
					+ (unsigned char) c];
			unsigned char action = JSON::lexerTables[(state << 9) + 256
					+ (unsigned char) c];
			switch (action) {
			case 1:
				token.type = Token::Type::_Char;
				token.c = c;
				return;
				break;
			case 7:
				throw(std::runtime_error("Character not expected."));
			case 8:
				throw(std::runtime_error("Number incomplete."));
			case 9:
				token.str += c;
				break;
			case 10:
				token.num = 10 * token.num + (double) (c - '0');
				break;
			case 11:
				token.num += factor * (double) (c - '0');
				factor /= 10.0;
				break;
			case 12:
				exponent = 10 * exponent + (int) (c - '0');
				break;
			case 13:
				negative = true;
				break;
			case 14:
				exponentnegative = true;
				break;
			}
			unsigned char nextaction = lexerTables[(nextstate << 9) + 256
					+ (unsigned char) nextc];
			switch (nextaction) {
			case 2:
				token.type = Token::Type::_String;
				return;
			case 3:
				token.type = Token::Type::_Number;
				if (exponent != 0) {
					if (exponentnegative)
						exponent = -exponent;
					token.num *= exp(M_LN10 * (double) exponent);
				}
				if (negative)
					token.num = -token.num;
				return;
			case 4:
				return;
			case 5:
				token.type = Token::Type::_Boolean;
				token.b = true;
				return;
			case 6:
				token.type = Token::Type::_Boolean;
				return;
			}
			state = nextstate;
		} while (position < charsread);
	} while (charsread == buffersize);
}

void JSON::ToStream(std::ostream &out, bool usenewline, size_t indent) const {
	switch (type) {
	case Type::Null:
		out << "null";
		break;
	case Type::Boolean:
		if (valueBoolean) {
			out << "true";
		} else {
			out << "false";
		}
		break;
	case Type::Number:
		out << valueNumber;
		break;
	case Type::String:
		out << '"' << EscapeString(valueString) << '"';
		break;
	case Type::Array: {
		out << "[";
		bool flag = false;
		for (std::vector<JSON>::const_iterator it = valueArray.begin();
				it != valueArray.end(); ++it) {
			if (flag)
				out << ",";
			if (usenewline) {
				out << '\n';
				for (size_t m = 0; m < indent; ++m)
					out << "  ";
			}
			it->ToStream(out, usenewline, indent + 1);
			flag = true;
		}
		out << "]";
		break;
	}
	case Type::Object: {
		out << "{";
		bool flag = false;
		for (auto it = valueObject.cbegin(); it != valueObject.cend(); ++it) {
			if (flag)
				out << ",";
			if (usenewline) {
				out << '\n';
				for (size_t m = 0; m < indent; ++m)
					out << "  ";
			}
			out << '"' << EscapeString(it->first) << '"';
			if (usenewline)
				out << "\t";
			out << ':';
			if (usenewline)
				out << " ";
			it->second.ToStream(out, usenewline, indent + 1);
			flag = true;
		}
		out << "}";
		break;
	}
	}
}

std::string JSON::EscapeString(const std::string &txt) {
	std::string temp = txt;
	for (size_t index = 0; index < temp.size(); ++index) {
		index = temp.find_first_of("\"\\", index);
		if (index == std::string::npos)
			break;
		temp.insert(index, 1, '\\');
		++index;
	}
	return temp;
}

std::array<unsigned char, JSON::stateCount * JSON::charWidth * 2> JSON::lexerTables =
		JSON::InitTables();

std::array<unsigned char, JSON::stateCount * JSON::charWidth * 2> JSON::InitTables() {
	std::array<unsigned char, JSON::stateCount * JSON::charWidth * 2> table { };

	for (size_t n = 0; n < 27 * 256 * 2; ++n) {
		unsigned int m = (n % 512);
		const unsigned int s = (unsigned char) (n >> 9);
		if (m < 256) {
			table[n] = 1;
			if (s == 0) {
				table[n] = 0;
				if (m == '{' || m == '}' || m == '[' || m == ']' || m == ','
						|| m == ':')
					table[n] = 1;
				if (m >= 'a' && m <= 'z')
					table[n] = 15;
				if (m >= 'A' && m <= 'Z')
					table[n] = 15;
				if (m == '_')
					table[n] = 15;
				if (m == '+' || m == '-')
					table[n] = 19;
				if (m == '"')
					table[n] = 16;
				if (m == 'N' || m == 'n')
					table[n] = 2;
				if (m == 'T' || m == 't')
					table[n] = 6;
				if (m == 'F' || m == 'f')
					table[n] = 10;
			}
			if ((s >= 2 && s <= 15)
					&& ((m >= 'A' && m <= 'Z') || (m >= 'a' && m <= 'z')
							|| (m >= '0' && m <= '9') || m == '-' || m == '_'
							|| m == '.'))
				table[n] = 15;
			if (s == 2 && (m == 'U' || m == 'u'))
				table[n] = 3;
			if (s == 3 && (m == 'L' || m == 'l'))
				table[n] = 4;
			if (s == 4 && (m == 'L' || m == 'l'))
				table[n] = 5;
			if (s == 6 && (m == 'R' || m == 'r'))
				table[n] = 7;
			if (s == 7 && (m == 'U' || m == 'u'))
				table[n] = 8;
			if (s == 8 && (m == 'E' || m == 'e'))
				table[n] = 9;
			if (s == 10 && (m == 'A' || m == 'a'))
				table[n] = 11;
			if (s == 11 && (m == 'L' || m == 'l'))
				table[n] = 12;
			if (s == 12 && (m == 'S' || m == 's'))
				table[n] = 13;
			if (s == 13 && (m == 'E' || m == 'e'))
				table[n] = 14;
			if (s == 16 || s == 17)
				table[n] = 16;
			if (s == 16 && m == '\\')
				table[n] = 17;
			if (s == 16 && m == '"')
				table[n] = 18;
			if (s == 18)
				table[n] = 1;

			if ((s == 0 || s == 19 || s == 20 || s == 21)
					&& (m >= '0' && m <= '9'))
				table[n] = 21;
			if ((s == 18 || s == 20) && m >= 1 && m <= 32)
				table[n] = 20;
			if ((s == 0 || s == 19 || s == 20 || s == 21) && m == '.')
				table[n] = 22;
			if ((s == 22 || s == 23) && (m >= '0' && m <= '9'))
				table[n] = 23;
			if ((s == 21 || s == 22 || s == 23) && (m == 'e' || m == 'E'))
				table[n] = 24;
			if (s == 24 && (m == '+' || m == '-'))
				table[n] = 25;
			if ((s == 24 || s == 25 || s == 26) && (m >= '0' && m <= '9'))
				table[n] = 26;

		} else {
			m -= 256;

			// Setup action table
			// 1: Send char token back
			// 2: Send string token back
			// 3: Send value token back
			// 4: Send 'null' token back
			// 5: Send boolean 'true' token back
			// 6: Send boolean 'false' token back
			// 7: Error: Unexpected character
			// 8: Error: Number incomplete
			// 9: Add character to string
			// 10: Add char to value
			// 11: Add char to value as fraction
			// 12: Add char to exponent
			// 13: Mark value negative
			// 14: Mark exponent negative

			table[n] = 0;
			if (s != 0 && s < 19)
				table[n] = 7;
			if (s == 0
					&& (m == '[' || m == ']' || m == '{' || m == '}' || m == ','
							|| m == ':'))
				table[n] = 1;
			if (table[n - 256] >= 2 && table[n - 256] <= 15)
				table[n] = 9;
			if (table[n - 256] == 1) {
				if ((s >= 2 && s <= 4) || (s >= 6 && s <= 8)
						|| (s >= 10 && s <= 13))
					table[n] = 2;
				if (s == 5)
					table[n] = 4;
				if (s == 9)
					table[n] = 5;
				if (s == 14)
					table[n] = 6;
				if (s == 15)
					table[n] = 2;
			}
			if (s == 16) {
				table[n] = (table[n - 256] == 16) ? 9 : 0;
			}
			if (s == 17)
				table[n] = 9;
			if (s == 18)
				table[n] = 2;
			if (table[n - 256] == 1) {
				if (s >= 19)
					table[n] = 8;
				if (s == 21 || s == 22 || s == 23 || s == 26)
					table[n] = 3;
			}
			if (table[n - 256] == 19 && m == '-')
				table[n] = 13;
			if (table[n - 256] == 21)
				table[n] = 10;
			if (table[n - 256] == 23)
				table[n] = 11;
			if (table[n - 256] == 25 && m == '-')
				table[n] = 14;
			if (table[n - 256] == 26)
				table[n] = 12;
		}
	}
	return table;
}

///////////////////////////////////////////////////////////////////////////////
// Name               : UTF8.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 04.05.2024
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
#include "UTF8.h"

#include <algorithm>

std::vector<uint32_t> UTF8::GetCodepoints(const std::string &str) {
	std::vector<uint32_t> ret;
	uint32_t codepoint = 0;
	uint_fast8_t state = 0;
	for (char c : str) {
		uint8_t b = static_cast<uint8_t>(c);
		if (b >= 128) {
			if (b >= 240) {
				codepoint = b & 0b00000111;
				state = 3;
				continue;
			}
			if (b >= 224) {
				codepoint = b & 0b00001111;
				state = 2;
				continue;
			}
			if (b >= 194) {
				codepoint = b & 0b00011111;
				state = 1;
				continue;
			}
			codepoint = (codepoint << 6) | (b & 0b00111111);
			if (state > 0) {
				state--;
				if (state > 0)
					continue;
			}
		} else {
			codepoint = b;
		}
		ret.push_back(codepoint);
	}
	return ret;
}

size_t UTF8::Length(const std::string &str) {
	return std::count_if(str.begin(), str.end(), [](char c) {
		return c >= -64;
	});
}

size_t UTF8::CountPrintable(const std::string &str) {
	return std::count_if(str.begin(), str.end(), [](char c) {
		return (c >= -64 && c < 0) || (c > 32);
	});
}

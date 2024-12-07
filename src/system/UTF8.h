///////////////////////////////////////////////////////////////////////////////
// Name               : UTF8.h
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
#ifndef SYSTEM_UTF8_H
#define SYSTEM_UTF8_H
/** \class UTF8
 * 	\code #include "UTF8.h"\endcode
 * 	\ingroup system
 *  \brief UTF-8 string processing
 *
 * Functions to work with std::string%s that contain UTF-8 encoded characters.
 */
#include <stdint.h>
#include <string>
#include <vector>

class UTF8 {
public:

	/**\brief Split a string into UTF-8 code-points
	 *
	 * \param str std::string to process
	 * \return std::vector of unsigned int 32 numbers with the code points
	 */
	static std::vector<uint32_t> GetCodepoints(const std::string &str);

	/**\brief Count the code-points in a UTF-8 string
	 *
	 */
	static size_t Length(const std::string &str);

	/**\brief Count the printable code-points in a UTF-8 string
	 *
	 */
	static size_t CountPrintable(const std::string &str);
};

#endif /* SYSTEM_UTF8_H */

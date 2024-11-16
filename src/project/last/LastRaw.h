///////////////////////////////////////////////////////////////////////////////
// Name               : LastRaw.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 10.11.2024
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
#ifndef SRC_PROJECT_LAST_LASTRAW_H_
#define SRC_PROJECT_LAST_LASTRAW_H_

/** \class LastRaw
 * 	\code #include "LastRaw.h"\endcode
 * 	\ingroup project
 *  \brief Stored a geometry together with modification flags
 *
 * Overloaded Geometry with additional modification flag.
 */

#include "../../3D/Geometry.h"

class LastRaw: public Geometry {
public:
	LastRaw() = default;
	LastRaw(const Geometry &other);
	virtual ~LastRaw() = default;

	bool IsModified() const;
	void Modify(bool modified_ = true);

private:
	bool modified = true;
};

#endif /* SRC_PROJECT_LAST_LASTRAW_H_ */

///////////////////////////////////////////////////////////////////////////////
// Name               : RawGeometry.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 16.11.2024
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
#ifndef SRC_PROJECT_RAWGEOMETRY_H_
#define SRC_PROJECT_RAWGEOMETRY_H_

/** \class RawGeometry
 * 	\code #include "RawGeometry.h"\endcode
 * 	\ingroup ObjectOperations
 *  \brief Stored a geometry together with modification flags
 *
 * Overloaded Geometry with additional modification flag.
 */

#include "../../3D/Geometry.h"
#include "Object.h"

class RawGeometry: public Geometry, public Object {
public:
	RawGeometry() = default;
	virtual ~RawGeometry() = default;
	RawGeometry(const Geometry &other);
	RawGeometry(Geometry &&other);
//	RawGeometry& operator=(const Geometry &other);
//	RawGeometry& operator=(Geometry &&other);


};

#endif /* SRC_PROJECT_RAWGEOMETRY_H_ */

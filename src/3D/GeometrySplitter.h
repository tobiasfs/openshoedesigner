///////////////////////////////////////////////////////////////////////////////
// Name               : GeometrySplitter.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 01.05.2024
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
#ifndef L3D_GEOMETRYSPLITTER_H
#define L3D_GEOMETRYSPLITTER_H

/** \class GeometrySplitter
 * 	\code #include "GeometrySplitter.h"\endcode
 * 	\ingroup Base3D
 *  \brief Fast splitting of a geometry into separate entities.
 *
 * Takes a Geometry upon construction and generates a list of Geometries each
 * with one object from the original data.
 */

#include "BoundingBox.h"
#include "Geometry.h"

#include <vector>

class GeometrySplitter: public Geometry {
public:
	void Split();

	std::vector<Geometry> objects;
	std::vector<BoundingBox> bbs;
};

#endif /* L3D_GEOMETRYSPLITTER_H */

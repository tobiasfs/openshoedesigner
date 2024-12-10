///////////////////////////////////////////////////////////////////////////////
// Name               : HeightField.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.03.2016
// Copyright          : (C) 2016 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef HEIGHTFIELD_H
#define HEIGHTFIELD_H

/*!\class HeightField
 * \brief DEPRECATED: Planar 2.5D heightfield
 * \deprecated Unused
 *
 * Grid of points with a height. This class provides some operations to generate some Polygon3 objects from it.
 */

#include "AffineTransformMatrix.h"
#include "Vector3.h"

class Polygon3;

class HeightField {
public:
	HeightField();
	HeightField(const HeightField &other);
	virtual ~HeightField();

	void SetCount(unsigned int nx, unsigned int ny, float resolution);
	void SetSize(float x, float y, float resolution);

	void SetValues(double *v, unsigned int size);

	Polygon3 GetUnderline() const;

	void Clear();

	void Paint() const;

private:

	double *value;

	unsigned int N;
	unsigned int Nx;
	unsigned int Ny;
	float dx;
	float dy;

public:
	Vector3 color;
	AffineTransformMatrix matrix;
};

#endif /* HEIGHTFIELD_H */

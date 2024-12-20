///////////////////////////////////////////////////////////////////////////////
// Name               : Polynom2D.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 18.04.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef POLYNOM2D_H
#define POLYNOM2D_H

/*!\class Polynom2D
 * \brief Polynom in 2 dimensions
 *
 * Calculates and stores a polynom in 2 dimensions. The polynom is defined by
 * four points and four normal vectors.
 *
 * For real 3D surfaces defined by Vertex3 with normal vectors refer to
 * NagataPatch.
 */

struct Vector3;

class Polynom2D {
public:
	double a[16];

public:
	Polynom2D() = default;
	virtual ~Polynom2D() = default;

	double Evaluate(double x, double y);

	void Set44(const double v0, const Vector3 &n0, const double v1,
			const Vector3 &n1, const double v2, const Vector3 &n2,
			const double v3, const Vector3 &n3);

};

#endif /* POLYNOM2D_H */

///////////////////////////////////////////////////////////////////////////////
// Name               : Spline3.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 28.01.2017
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

#ifndef SPLINE3_H
#define SPLINE3_H

/*!\class Spline3
 * \brief Spline interpolation of a number of points in 3D space.
 * \deprecated Superseded by Bezier3 and Surface
 *
 * The spline consists of several points. These points are either sharp corners
 * or rounded off by the spline algorithm. The spline can either be closed or
 * open. The coordinate System of the splines has a dedicated zero that can be
 * assigned to one spline point, or assigned to a connection between two
 * spline points at a certain coordinate. The extent of the coordinate is
 * either cyclic, symmetric. It can also be scaled to unity or in direct or
 * interpolated coordinates.
 */

#include "Vector3.h"
#include "../math/Polynomial.h"

#include <vector>

class Spline3 {
public:
	class Point: public Vector3 {
	public:
		Point() = default;
		Polynomial px;
		Polynomial py;
		Polynomial pz;
		double r0 = 0.0;
		double length = 1.0;
		double dvx = 0.0;
		double dvy = 0.0;
		double dvz = 0.0;
		bool corner = false;
	};

	Spline3();
	virtual ~Spline3() = default;

	std::vector<Point> points;

	Vector3 color;
	bool closed;
	bool normalized;
	bool symmetric;
	unsigned int pointsize;
	double length;

	void Clear();
	void AddVector(const Vector3 &vector, bool corner = false);
	void AddVector(float x, float y, float z = 0.0, bool corner = false);
	void Close(bool close = true);
	void Symmetric(bool symmetric = true);
	void Normalize(bool normalize = true);
	void Calculate();

	Vector3 Evaluate(double r);

	void Paint() const;
};

#endif /* SPLINE3_H */


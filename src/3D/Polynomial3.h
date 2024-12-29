///////////////////////////////////////////////////////////////////////////////
// Name               : Polynomial3.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 27.09.2020
// Copyright          : (C) 2020 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_POLYNOM3_H
#define L3D_POLYNOM3_H

/*!\class Polynomial3
 * \brief Combines three Polynomials into a 3D polynomial
 *
 * Contains three Polynomials to generate a 3D polynomial with a separate x, y and z.
 *
 * Static setup function like the ones found in the Polynomial class are available.
 *
 * The variables r0 and r1 show the validity of the polynomial. These variables are also
 * used for painting the polynomial in OpenGL.
 */

#include <functional>
#include <stddef.h>

#include "../math/Polynomial.h"
#include "Vector3.h"

class Polygon3;

class Polynomial3 {
public:
	Polynomial x;
	Polynomial y;
	Polynomial z;
	double r0 = 0.0; ///< Range start
	double r1 = 1.0; ///< Range end

	static Polynomial3 ByValue(double p0, const Vector3 &v0); ///< Initialize by 1 point with position and value (actually only the value is used, because it is a constant)
	static Polynomial3 ByValue(double p0, const Vector3 &v0, double p1,
			const Vector3 &v1); ///< Initialize by 2 points with position and value
	static Polynomial3 ByValue(double p0, const Vector3 &v0, double p1,
			const Vector3 &v1, double r2, const Vector3 &v2); ///< Initialize by 3 points with position and value
	static Polynomial3 ByValue(double p0, const Vector3 &v0, double p1,
			const Vector3 &v1, double r2, const Vector3 &v2, double r3,
			const Vector3 &v3); ///< Initialize by 4 points with position and value
	static Polynomial3 ByBezier(const Vector3 &v0); ///< Bezier interpolation with constant value
	static Polynomial3 ByBezier(const Vector3 &v0, const Vector3 &v1); ///< Bezier interpolation with start- and end-value
	static Polynomial3 ByBezier(const Vector3 &v0, const Vector3 &v1,
			const Vector3 &v2); ///< Bezier interpolation with start-, support- and end-value
	static Polynomial3 ByBezier(const Vector3 &v0, const Vector3 &v1,
			const Vector3 &v2, const Vector3 &v3); ///< Bezier interpolation with two support values

	static Polynomial3 ByPolygon3(const Polygon3 &p, size_t order,
			size_t idxStart, size_t idxEnd);

	double Length(const size_t N = 20) const; ///< Returns the length of the polynomial for r = r0..r1

	void Transform(std::function<Vector3(Vector3)> func);

	Vector3 operator()(double r) const; ///< Evaluation of polygon

	void Paint() const;
};

#endif /* L3D_POLYNOM3_H */


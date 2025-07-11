///////////////////////////////////////////////////////////////////////////////
// Name               : Bezier3.h
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 14.01.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#ifndef L3D_BEZIER3_H_
#define L3D_BEZIER3_H_

/** \class Bezier3
 * 	\code #include "Bezier3.h"\endcode
 * 	\ingroup Base3D
 *  \brief Bezier polynomial chains in 3D
 *
 * For defining Bezier shapes in up to 3 dimensions.
 *
 * A shape is a open or closed shape in up to 3 dimensions with an arbitrary
 * number of corners. At each corner two directional vectors are defined.
 *
 * These directional vectors are 3 times the distance to the
 * Bezier-control-points:
 * 	* PBezierIn  = PCorner - dirIn  / 3
 * 	* PBezierOut = PCorner + dirOut / 3
 *
 * There are different continuity settings for the corners:
 *
 * (C is mathematical continuity to to the given order, G is geometric
 * continuity to to the given order)
 *
 *  * __Free__ - No handles are used. Just C0 (or G0) continuity.
 *  * __Handles__ - Incoming and outgoing directions are defined and used.
 *  * __Linear__ - The outgoing direction is the same as the incoming one, but can have a different length. (G1-continuity)
 *  * __Symmetric__ - Both directions are the same. (C1-continuity)
 *  * __Auto_KB__ - Automatic handles using Kochanek and Bartels algorithm for C2 continuity
 *  * __Auto_DRB__ - Automatic handles using DeRose and Barsky algorithm for G2 continuity
 *  * __Auto_DRBS__ - Automatic handles using a symmetric modification of DeRose and Barsky algorithm for C2 continuity
 *
 * The modified DeRose/Barsky algorithm is used because the original one is not
 * C2. This might be needed for some applications. (It also looks better than
 * Kochanek/Bartels.)
 *
 * Note: The spline editing buttons in the software _Inkscape_ are
 *       Handles, Linear, Symmetric and Auto_DRB.
 *
 */

#include <vector>

#include "Polynomial3.h"
#include "Vector3.h"

class Bezier3 {
public:
	enum struct Continuity {
		Free = 0, ///< No handles (C0)
		Handles = 1, ///< Independent handles (C0)
		Linear = 2, ///< Handles on the same line (G1)
		Symmetric = 3, ///< Symmetric handles on the same line (C1)
		Auto_KB = 4, ///< Automatic curvature continuity (Kochanek and Bartels) (C2)
		Auto_DRB = 5, ///< Automatic curvature continuity (DeRose and Barsky) (G2)
		Auto_DRBS = 6 ///< Automatic curvature continuity symmetric (DeRose and Barsky) (C2)
	};

	class Point: public Vector3 {
	public:
		Point() = default;
		Point(double x_, double y_ = 0.0, double z_ = 0.0);
		Point& operator=(const Vector3 &v);

		Continuity continuity = Continuity::Free;
		Vector3 dirIn;
		Vector3 dirOut;
	};

	Bezier3() = default;
	virtual ~Bezier3() = default;

	void SetSize(size_t pointCount);
	void CloseLoop(bool closed_ = true);
	size_t Size() const;

	Point& operator[](size_t idx);
	const Point& operator[](size_t idx) const;

	const Polynomial3& GetSegment(size_t idx) const;

	void UpdateSegments();

	Vector3 operator()(double u) const;

	void Paint() const;

private:
	/**\brief Order for length interpolation
	 *
	 * This is under experimentation.
	 *
	 * The running parameter r of a spline is not linear related to the
	 * length on the spline.
	 *
	 * If set to 0 the running parameter is linear from 0 to the number of
	 * segments in the spline. The whole numbers are at the corners.
	 *
	 * If set to a value > 0 the running parameter r is linear to the length of
	 * the spline. The value indicates the order of the interpolation.
	 *
	 * An order of 1 is reasonable. Everything above is experimental.
	 */
	size_t maxUMapOrder = 0;

	/**\brief Closed loop.
	 *
	 * If true, the last point is connected to the first. The same number of
	 * segments are generated as there are points.
	 *
	 * If false only one less segment than there are points are generated.
	 *
	 */
	bool closed = false;
	std::vector<Point> points;
	std::vector<Polynomial3> segments;

private:
	mutable size_t searchidx = 0; ///< Speedup variable.
};

#endif /* L3D_BEZIER3_H_ */

///////////////////////////////////////////////////////////////////////////////
// Name               : Insole.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.09.2020
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

#ifndef OBJECT_INSOLE_H
#define OBJECT_INSOLE_H

/*!\class Insole
 * \brief Insole of a shoe
 * \ingroup ObjectOperations
 *
 * \image html Insole.svg
 *
 * The insole is defined in X,Y,Z. The outline of the insole also in X,Y,Z but
 * additionally with an U that is 0 in point B (tip of shoe) and ranges
 * to +/- M_PI around the perimeter to point A (heel). Positive values on the
 * outside, negative values on the inside. (Therefore is the left shoe the
 * default shoe as X is pointing to the front, Y to the outside and Z up.)
 * V (height above insole) is zero on the outline.
 *
 * The coordinate system of the upper in the heel section can be oriented in
 * the N-M axis (e.g. Lloyd shoes, Apollo shoes) or in the A-C axis (e.g. Ecco
 * shoes).
 *
 * The insole is normally oriented in the A-B axis.
 *
 * This orientation is switchable. It results in shifted U values on the
 * outline and thus shifted U values on the last and an shifted symmetry.
 * This is a design choice.
 */

#include <functional>
#include <vector>

#include "../../3D/Polygon3.h"
#include "../../3D/Polynomial3.h"
#include "../../3D/Vector3.h"
#include "../../math/Polynomial.h"
#include "Object.h"

class Insole: public Geometry, public Object {
	friend class InsoleConstruct;
	friend class InsoleTransform;
	friend class InsoleAnalyze;

protected:
	class Point: public Geometry::Vertex {
	public:
		Point() = default;
		Point(const Geometry::Vertex &other);
		static Point FromXY(double x_, double y_) {
			Point temp(Geometry::Vertex(x_, y_, 0.0, x_, y_));
			temp.n.Set(0.0, 0.0, 1.0);
			return temp;
		}
		Vector3 dir; ///< Direction of the incoming and outgoing vector.
		void SetDirection(const Point &p0, const Point &p1);
		void Transform(std::function<Vector3(Vector3)> func);

		Point& operator+=(const Point &rhs);
		friend Point operator+(Point lhs, const Point &rhs) {
			lhs += rhs;
			return lhs;
		}
		Point& operator-=(const Point &rhs);
		friend Point operator-(Point lhs, const Point &rhs) {
			lhs -= rhs;
			return lhs;
		}

	};

	class Line: public Polynomial3 {
	public:
		// Bezier-Circle: sqrt(2) * f = 0.551915024494 --> f = 0.39
		void Setup(const Point &p0, const Point &p1, double f0 = 0.39,
				double f1 = 0.39);
		void Paint() const;
		bool IsInside(const double r) const;
	};

public:
	/**\brief Transform the insole using a transform matrix.
	 *
	 * Has the advantage, that the normal vectors are transformed correctly.
	 */
	void Transform(const AffineTransformMatrix &m);

	/**\brief Transform X,Y,Z values of insole.
	 *
	 * This transforms the X,Y,Z values of the insole while keeping U and V.
	 *
	 * The transformations applies to the geometry, the outline and the
	 * construction points.
	 *
	 * This function uses a transform function. This can be an
	 * AffineTransformMatrix or another function providing the bending of
	 * the insole.
	 */
	void Transform(std::function<Vector3(Vector3)> func);

	/**\brief 3D Painting of insole.
	 *
	 * 2D Painting is happening inside CanvasInsole.
	 */
	void Paint() const;

private:
	void ToOpenGL(const Point &p, const std::string &label) const;

public:
	Polygon3 outline;
	std::vector<Line> lines;

	Insole::Point A; ///< Heel
	Insole::Point B; ///< Tip of last
	Insole::Point C; ///< Middle of ball area
	Insole::Point D; ///< Tip of foot
	Insole::Point E; ///< Big toe ball
	Insole::Point F; ///< Little toe ball
	Insole::Point G; ///< Big toe tip
	Insole::Point H; ///< Little toe direction (construction point)
	Insole::Point J; ///< Center point (below os tibia)
	Insole::Point K; ///< Heel half circle inside
	Insole::Point L; ///< Heel half circle outside
	Insole::Point M; ///< End of the middle line in the back of the foot
	Insole::Point N; ///< Start of the middle line in the back of the foot
	Insole::Point P; ///< Construction point for the heel
	Insole::Point Z; ///< Little toe tip
};

#endif /* OBJECT_INSOLE_H */

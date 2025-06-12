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

#ifndef PROJECT_INSOLE_H
#define PROJECT_INSOLE_H

/*!\class Insole
 * \brief Construct, deform and export the insole of a shoe
 * \ingroup ObjectOperations
 *
 * The heel is normally oriented in the N-M axis.
 *
 * The coordinate system of the upper in the heel section can be oriented in
 * the N-M axis (e.g. Lloyd shoes, Apollo shoes) or in the A-C axis (e.g. Ecco
 * shoes).
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
		static Point FromUV(double u, double v) {
			return Point(Geometry::Vertex(u, v, 0.0, u, v));
		}

//		double rx = 0.0;
		Vector3 dir;
		void SetNormal(const Point &p0, const Point &p1);
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
	void Transform(std::function<Vector3(Vector3)> func);
	void Paint() const;

public:

	std::vector<Line> lines;

	Polygon3 outline;
//	Polygon3 inside;
//	Polygon3 outside;

	Insole::Point A; ///< Heel
	Insole::Point B; ///< Shoe tip
	Insole::Point C; ///< Insole center (middle of ball area)
	Insole::Point D; ///< Center of toes
	Insole::Point E; ///< Big toe ball
	Insole::Point F; ///< Little toe ball
	Insole::Point G; ///< Big toe tip
	Insole::Point H; ///< Little toe direction
	Insole::Point J; ///< Center point (below os tibia)
	Insole::Point K; ///< Heel half circle inside
	Insole::Point L; ///< Heel half circle outside
	Insole::Point M; ///< Middle of heel half-circle
	Insole::Point N; ///< Middle of heel half-circle
	Insole::Point P; ///< Middle of heel
	Insole::Point Z; ///< Little toe tip
};

#endif /* PROJECT_INSOLE_H */

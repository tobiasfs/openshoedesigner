///////////////////////////////////////////////////////////////////////////////
// Name               : Design.h
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 04.02.2025
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
#ifndef OBJECT_DESIGN_H
#define OBJECT_DESIGN_H

/** \class Design
 *  \brief Design for the upper of a shoe
 * 	\ingroup shoe
 * 	\code #include "Design.h"\endcode
 *
 * Describes multiple patches in a UV coordinate system. The design itself is
 * flat. The lines describes are the lines seen from the outside. Overlap and
 * depth order are set as an attribute of the patches.
 *
 * # Coordinate system
 *
 * This class exclusively works only in UV space. For convenience reasons
 * the UV is mapped to XY (with Z=0) in the generated geometry. The final
 * projection to 3D has to be done by applying a CoordinateSystem.
 *
 * The UV coordinate system is cylindric.
 *
 *  * U from -pi to pi, with 0 in front, positive angles to the outside,
 *  	negative angles to the inside of the shoe.
 *  * V height from the sole
 *  	-1 is the floor,
 *  	 0 is the sole,
 *  	 1 the ankle,
 *  	 2 the knee,
 *  	 3 the hip.
 *
 * # Class Design
 *
 * This class has to fulfill two main requirements: editable and open to
 * extension.
 *
 * For the approach is experimental (throw everything at it and see what sicks)
 * this class duplicates features from Bezier3, Polynomial3, Spline3 classes.
 *
 * It uses the Polynomial class for the interpolation tasks.
 *
 * The solver uses Eigens CompleteOrthogonalDecomposition for solving a linear
 * equations system. This comes with some limitations for conditions regarding
 * the Polynomials with an order > 1. Here the conditions are only one-way,
 * i.e. a vertex on the polynomial (not the end-points or the handles).
 *
 * The design is self contained and does not use any data from
 * foot-measurements or the shoe.
 *
 */

#include "Object.h"
#include "../../3D/Geometry.h"
#include "../../3D/Polygon3.h"
#include "../../math/Polynomial.h"

#include <initializer_list>
#include <string>
#include <vector>

class Design: public Object {

public:
	class Vector2 {
	public:
		Vector2() = default;
		Vector2(double u_, double v_) :
				u(u_), v(v_) {
		}
		Vector2 Interp(const Vector2 &b, const double mix) const {
			return {u + (b.u - u) * mix, v + (b.v - v) * mix};
		}
		double Abs() const {
			return std::sqrt(u * u + v * v);
		}
		Vector2 Normal() const;
		void Normalize();
		Vector2 Orthogonal() const {
			return Vector2(-v, u);
		}
		Vector2 RotatedBy(double angle) const;
		bool IsZero() const;
		Vector2& operator+=(const Vector2 &a) {
			this->u += a.u;
			this->v += a.v;
			return *this;
		}
		friend Vector2 operator+(Vector2 a, const Vector2 &b) {
			a += b;
			return a;
		}
		Vector2& operator-=(const Vector2 &a) {
			this->u -= a.u;
			this->v -= a.v;
			return *this;
		}
		friend Vector2 operator-(Vector2 a, const Vector2 &b) {
			a -= b;
			return a;
		}
		friend double operator*(const Vector2 &a, const Vector2 &b) {
			return a.u * b.u + a.v * b.v;
		}
		friend Vector2 operator*(double a, const Vector2 &b) {
			return Vector2(a * b.u, a * b.v);
		}

		double u = 0.0;
		double v = 0.0;
	};
	class Vertex: public Vector2 {
	public:
		Vertex(double u_, double v_) :
				Vector2(u_, v_) {
		}

		std::string name;

//		bool constructed = false; ///< Indicates, that a vertex is constructed.

	};
	class Edge {
	public:
		Edge(std::initializer_list<size_t> idx) :
				vidx(idx) {
		}

		void UpdatePolynomials(const std::vector<Vertex> &v);

		/**\brief Return the 2D point at one position.
		 * \param r Variable from 0 .. 1 to cover the length of the edge.
		 */
		Vector2 operator()(double r) const;
		size_t Size() const {
			return iu.size();
		}
		std::string name;
		bool construction = false;

		std::vector<size_t> vidx;
		Polygon3 geo;

	protected:
		/**\name Interpolation polynomials
		 *
		 * The data for the position interpolation is stored as an polynomial
		 * up to an order of 3 (= 4 coefficients). If more points are
		 * interpolated the data is still stored in these polynomials, but
		 * it is interpolated by deCasteljau's algorithm.
		 *
		 * Polynomials become unstable for large number of points in one single
		 * edge (5 or more points). But deCasteljau is more expensive.
		 * Here (n-1)*n multiplications are needed, while the polynomial
		 * approach only needs (n-1)*2 multiplications (and also less
		 * algorithmic complexity).
		 */
		Polynomial iu; ///< Interpolation polynomials for U
		Polynomial iv; ///< Interpolation polynomials for V
		/**\}
		 */
	};

	/**\brief Boundary conditions for points
	 *
	 * The boundary conditions act upon the vertices of the design.
	 * For each vertex involved, a suggested position (p), a degree of
	 * freedom (a) and a group degree of freedom (b) is defined, so that
	 * new position = p + a*t + b*n. t is the tangential along the line and
	 * n is the normal to the lines tangential vector.
	 *
	 * a and b are later solved for. a is individual for each vertex and b is
	 * the same for all vertices in the condition. (Example: all vertices are
	 * confined to a line with a given angle, each vertex can freely slide
	 * along the line, but all vertices can only shift by the same amount
	 * orthogonal to the line.)
	 *
	 * For symmetry conditions (line symmetry and point symmetry), the vertices
	 * pairwise share mirrored version of t and n. (point symmetry: (t,n) and
	 * (-t,-n), line symmetry: (t,n) and (t,-n) with t parallel to the line).
	 *
	 * The condition is constructed as follows:
	 *
	 *  1. Construct the line
	 *  	If only a vertex is given, a line from this vertex in the direction
	 *  	of global 0 deg (u=1, v=0) is used.
	 *
	 *  	If an edge is given an additional r can be used to determine the
	 *  	starting point on the line. A search algorithm can find the point
	 *  	closest to a given vertex to determine r automatically.
	 *
	 *  	The line can point in the direction of 0 global, along the Edge
	 *  	in the point at r or towards the given vertex.
	 *
	 *  2. Rotate the line
	 *  	Rotate the line round the first vertex by the given angle.
	 *  3. Calculate t, n, and p0 for the line and p for each individual vertex
	 *     as the initial solution.
	 */
	class Condition {
	public:

		static Condition VertexOnEdge(size_t vidx, size_t eidx);
		static Condition VertexOnEdge(size_t vidx, size_t eidx, double r);
		static Condition VerticesOnHorizontalLine(
				std::initializer_list<size_t> idx, double v = 0.0);
		static Condition VerticesOnVerticalLine(
				std::initializer_list<size_t> idx, double u = 0.0);
		static Condition VerticesOnLine(std::initializer_list<size_t> idx,
				double angle);
		static Condition VerticesSymmetricOnLineThroughPoint(
				std::initializer_list<size_t> idx, size_t vidx, double angle);

		size_t CountEquations() const;
		size_t CountFreeParameter() const;

		std::string name;
		size_t vidx = (size_t) -1; ///< Reference Vertex
		size_t eidx = (size_t) -1; ///< Reference edge
		double r = 0.0; ///< Relative position along edge
		bool rSearch = false; ///< Search for closest point on edge to ref. vertex
		double rmin = 0.0; ///< Search r from
		double rmax = 1.0; ///< Search r to
		double angle = 0.0; ///< Angle to rotate the construction line found
		enum struct Angle {
			GLOBAL, VERTEX, EDGE
		} angleOrigin = Angle::GLOBAL; ///< Reference of the angle
		bool inPoint = false; ///< Collapse into a single point (for vertex on vertex or vertex on line solution)
		bool moveParallel = false; ///< Allow the construction line to move parallel
		bool symmetric = false; ///< Make condition pairwise symmetric

		std::vector<size_t> v; ///< Vector of all vertex indices to optimize
		std::vector<Vector2> vinit; ///< Initial solution for the vertices
		Vector2 p0; ///< Origin of construction line
		Vector2 t; ///< Direction of construction line
		Vector2 n; ///< Normal on the construction line
		std::vector<Vector2> p; ///< Initial solutions for all vertices
	};

	class Patch: public Geometry {
	public:
		Patch() = default;
		virtual ~Patch() = default;
		void Update(double edgeLength);

		std::string name;
	};

public:
	Design();

	void PrepareBoundaryConditions();

	void UpdateEdges();
	void UpdatePatches();

	void Paint() const;

private:
	size_t ConditionRow(size_t cidx, size_t vidx = 0) const;
	size_t ConditionCol(size_t cidx, size_t vidx = 0) const;

public:
	std::vector<Vertex> vertices;
	std::vector<Edge> edges;
	std::vector<Condition> conditions;
	std::vector<Patch> patches;
};

#endif /* OBJECT_DESIGN_H */

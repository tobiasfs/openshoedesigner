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
 * Describes multiple patches in a UV-coordinate system. The design itself is
 * flat. The lines describes are the lines seen from the outside. Overlap and
 * depth order are set as an attribute of the patches.
 *
 * The UV coordinate system is cylindric in U. The vertices of the design can
 * be placed on arbitrary coordinates, but are normalized to the
 * range -M_PI < u <= M_PI and and clamped to -1 <= v <= 4.
 *
 * To determine, what way two points are warped around the cylinder, the
 * closest distance in u is considered. E.g. -3.0 is closer to 3.0 than to -2.0
 * (3.0 wraps to 3.0 - 2*M_PI = -3.2832 which has a distance of is 0.2832
 * to -3.0)
 *
 *
 * # Patches
 *
 * Each patch is created from a set of edges, if a loop can be formed using the
 * given edges. Note that there is no "automatic" edge at v=0.0.
 *
 * All edges are evenly (in UV space or in XYZ space) sampled, while checking
 * for edge/edge and edge/vertex intersections.
 *
 * Starting from one edge a connection graph is constructed. Only if one
 * branch touches all selected edges and returns the the first edge, this is
 * considered a valid patch border.
 *
 *
 *
 *
 * # Coordinate system
 *
 * This class exclusively works only in UV space. For convenience reasons
 * the UV is mapped to XY (with Z=0) in the generated geometry. The final
 * projection to 3D has to be done by applying a CoordinateSystem.
 *
 *
 *  * U from -pi to pi, with 0 in front, positive angles to the outside,
 *  	negative angles to the inside of the shoe.
 *  * V height from the sole
 *  	-1 is the floor,
 *  	 0 is the sole,
 *  	 1 the ankle,
 *  	 2 the knee,
 *  	 3 the hip,
 *  	 4 the waist.
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
 * i.e. for a vertex on the polynomial (not the end-points or the handles).
 *
 * The design is self contained and does not use any data from
 * foot-measurements or the shoe.
 *
 */

#include "Object.h"
#include "../../3D/Geometry.h"
#include "../../3D/Polygon3.h"
#include "../../math/Polynomial.h"
#include "../../3D/Vector2.h"

#include <initializer_list>
#include <string>
#include <vector>

class Design: public Object {

public:
	/**\class Vector2
	 * \brief Stores a vector in UV space.
	 *
	 * Storage class for two values: u and v. Additional functions for
	 * manipulating the vector. Only manipulations needed by the Design class
	 * were added.
	 *
	 *   * +, -, +=, -= : Addition and subtraction
	 *   * vector * vector : Dot-product between two vectors
	 *   * double * vector : Scaling a vector
	 *   * IsZero() : Check it U and V are zero
	 *   * RotateBy(), Orthogonal() : Rotation of vector
	 *   * Normal(), Normalize() : Normalize length
	 *   * Abs() : Absolute length of a vector
	 *   * Interp() : Interpolate between two vectors
	 */

	//TODO: Remove the Vertex as a separate concept to the Vector2 class
	class PatchVertex: public Vector2 {
	public:
		PatchVertex(double u_, double v_, const std::string &name = "") :
				Vector2(u_, v_), name(name) {
		}
		std::string name;
	};

	/**\brief Interpolated edge through two or more vertices
	 *
	 * Uses Bezier interpolation to interpolate a bend line through some
	 * vertices. For low number (up to 4) vertices a Bernstein/polynomial
	 * interpolation is done. For higher number of vertices deCasteljaus
	 * algorithm is used.
	 *
	 * Edges can be marked as construction edges. These types of edges
	 * do not interact with the patches or intersect lines. The are purely to
	 * enforce constraints and interact with point during solving.
	 */
	class PatchEdge {
	public:
		PatchEdge(std::initializer_list<size_t> idx, const std::string &name =
				"") :
				name(name), vidx(idx) {
		}
		/**\brief Update the polynomials in the edge
		 *
		 * This is a separate function, because the positions of the vertices
		 * are not available to the edge class per se.
		 *
		 * \param v const reference to the vector with the vertices.
		 */
		void UpdatePolynomials(const std::vector<PatchVertex> &v);

		/**\brief Return the 2D point at one position.
		 * \param r Variable from 0 .. 1 to cover the length of the edge.
		 */
		Vector2 operator()(double r) const;
		Vector2 Slope(double r, unsigned int order = 1) const;
		void ShiftU(double shift);
		size_t Size() const {
			return iu.size();
		}

		/**\brief Find the closest point on an edge to a vertex
		 *
		 * Find the closest point of a vertex to an edge. The distance is
		 * minimized using the Newton-Raphson algorithm. The position r is
		 * returned. The position r is limited between (inclusive) 0 .. 1.
		 *
		 * If requested the distance to the found point is returned.
		 *
		 * \param v Vertex to find smallest distance to. Possibly the vertex is
		 * 			on the edge.
		 * \param returnDist (optional) pointer to a double variable. If given,
		 *                   the smallest found distance is put into this
		 *                   double.
		 * \return The Running variable r of the edge. Clamped to 0..1.
		 *         The distance returned above is also relative to the clamped
		 *         found point.
		 */
		double FindR(const Vector2 &v, double *returnDist = nullptr) const;

		std::string name;
		std::vector<size_t> vidx; ///< Vector of vertex indices.
		bool construction = false; ///< Mark edge as construction edge.
		Polygon3 geo; ///< Sampled geometry of edge (for drawing the edge).

		/**\name Bounding box
		 * \{
		 */
		double u0 = -M_PI;
		double u1 = M_PI;
		double v0 = -1.0;
		double v1 = 4.0;
		/**\}
		 */
#ifdef DEBUG
	public:
#else
	protected:
#endif
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
		Polynomial diu; ///< Interpolation polynomials for dU
		Polynomial div; ///< Interpolation polynomials for dV
		Polynomial ddiu; ///< Interpolation polynomials for ddU
		Polynomial ddiv; ///< Interpolation polynomials for ddV

		/**\}
		 */
	};

	/**\brief Constraints for vertices
	 *
	 * The constraints act upon the vertices of the design.
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
	 * The constraint is constructed as follows:
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
	class Constraint {
	public:
		/**\name Creation functions
		 *
		 * Some common boundary conditions.
		 * \{
		 */
		static Constraint VertexOnEdge(size_t vidx, size_t eidx);
		static Constraint VertexOnEdge(size_t vidx, size_t eidx, double r);
		static Constraint VerticesOnHorizontalLine(
				std::initializer_list<size_t> idx, double v = 0.0);
		static Constraint VerticesOnVerticalLine(
				std::initializer_list<size_t> idx, double u = 0.0);
		static Constraint VerticesOnLine(std::initializer_list<size_t> idx,
				double angle);
		static Constraint VerticesSymmetricOnLineThroughPoint(
				std::initializer_list<size_t> idx, size_t vidx, double angle);
		/**\}
		 */

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

public:
	class SplitVertex: public Vector2 {
	public:
		SplitVertex(double u_, double v_, size_t eidx_, double r_) :
				Vector2(u_, v_), eidx(eidx_), r(r_) {
		}
		bool operator==(const SplitVertex &other) const;
		bool operator<(const SplitVertex &other) const;
		size_t eidx = (size_t) -1;
		double r = 0.0;
	};
	class SplitEdge {
	public:
		SplitEdge() = default;
		size_t eidx = (size_t) -1;
		size_t vidx0 = (size_t) -1;
		double r0 = 0.0;
		size_t vidx1 = (size_t) -1;
		double r1 = 1.0;
		bool operator==(const SplitEdge &other) const;
		bool operator<(const SplitEdge &other) const;
		void Flip();
	};

public:

	/**\brief Container for a patch
	 *
	 *
	 *
	 */
	class Patch: public Polygon3 {
	public:
		Patch(std::initializer_list<size_t> eidx, const std::string &name =
				std::string(""));

		virtual ~Patch() = default;

		void AddSplitEdge(const Design::PatchEdge &ed, double p0, double p1,
				const Polynomial &scaleU, const Polynomial &scaleV,
				double maxErr, double maxDist);

		void Update(double edgeLength, const Polynomial &scaleU,
				const Polynomial &scaleV);
		void UpdateBoundingBox();

		std::string name;
		std::set<size_t> eidx;

		double Umin;
		double Umax;
		double Vmin;
		double Vmax;

	private:
		double SplitAddEdge(const Design::PatchEdge &ed, double p0, double p2,
				const Vector2 &v0, const Vector2 &v2, double d02,
				const Polynomial &scaleU, const Polynomial &scaleV,
				double maxErr2, double maxDist);
	};

public:
	Design();

	void Update();

	/**\brief Apply all constraints and recalculate the edge interpolations
	 */
	void UpdateEdges();

	void UpdateSplits();

	/**\brief Use the edges to fill the patches with a grid of points.
	 */
	void UpdatePatches(double res, const Polynomial &scaleU =
			Polynomial::ByValue(-M_PI, -0.28, M_PI, 0.28),
			const Polynomial &scaleV = Polynomial::ByValue(0, 0, 1, 0.12));

	/**\brief Paint the edges and patches in OpenGL
	 *
	 * The displaying of the pattern in 2D is done in CanvasPattern.
	 */
	void Paint() const;

private:
	/**\brief Prepare the constraints
	 *
	 * Update the inner structure of the constraints by calculation the
	 * construction line and calculating initial solutions for the vertex
	 * positions. (These initial solutions only solve the current constraint.)
	 */
	void PrepareConstraints();
	/**\brief Find the row in the Matrix A and the vector b for a Constraint
	 * \param cidx index of the constraint
	 * \param vidx index of the vertex inside the constraint.
	 */
	size_t ConstraintRow(size_t cidx, size_t vidx = 0) const;
	/**\brief Find the column in the Matrix A for a Constraint
	 * \param cidx index of the constraint
	 * \param vidx index of the vertex inside the constraint.
	 */
	size_t ConstraintCol(size_t cidx, size_t vidx = 0) const;

	/**\brief Construct a loop using the split-edges list
	 *
	 * This function always returns a loop, if there is at least one edge and
	 * the starting edge is within the edges of the splitedge vector.
	 */
	std::vector<SplitEdge> FindLoop(const SplitEdge &begin,
			const std::set<size_t> &eidx) const;

public:
	std::vector<PatchVertex> vertices;
	std::vector<PatchEdge> edges;
	std::vector<Constraint> constraints;
	std::vector<Patch> patches;

public:
	std::vector<Vector2> splitV;
	std::vector<SplitEdge> splitE;

};

#endif /* OBJECT_DESIGN_H */

///////////////////////////////////////////////////////////////////////////////
// Name               : Surface.h
// Purpose            : Quadratic surface
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.10.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_SURFACE_H
#define L3D_SURFACE_H

/*!\class Surface
 * \ingroup Base3D
 * \brief Surface description and coordinate system from quadratic patches
 *
 * A surface is described by patches of 2D polynomials. These polynomials map
 * from an UV coordinate system into a XYZ coordinate system. For each mapped
 * point also a normal vector is generated. This can be used for the
 * W-coordinate and for OpenGL painting.
 */

// Initialization for Maxima:
//
// fx(u,v):=cx0 + cx1*u + cx2*u^2 + cx3*u^3 + (cx4 + cx5*u + cx6*u^2 + cx7*u^3)*v + (cx8 + cx9*u + cx10*u^2 + cx11*u^3)*v^2 + (cx12 + cx13*u + cx14*u^2 + cx15*u^3)*v^3;
// fy(u,v):=cy0 + cy1*u + cy2*u^2 + cy3*u^3 + (cy4 + cy5*u + cy6*u^2 + cy7*u^3)*v + (cy8 + cy9*u + cy10*u^2 + cy11*u^3)*v^2 + (cy12 + cy13*u + cy14*u^2 + cy15*u^3)*v^3;
// fz(u,v):=cz0 + cz1*u + cz2*u^2 + cz3*u^3 + (cz4 + cz5*u + cz6*u^2 + cz7*u^3)*v + (cz8 + cz9*u + cz10*u^2 + cz11*u^3)*v^2 + (cz12 + cz13*u + cz14*u^2 + cz15*u^3)*v^3;
#include "../math/Dependencies.h"
#include "../math/Matrix.h"
#include "../math/Polynomial.h"
#include "Geometry.h"
#include "Polynomial3.h"

#include <array>
#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <vector>

class Surface: protected Geometry {
private:
	/**\brief Patch on the surface.
	 *
	 * Describes a patch in the surface by mapping from UV coordinates to XYZ
	 * coordinates and normal vectors.
	 *
	 * Patches are described as 2D polynomials with different order
	 * of the polynomial in U and V direction. The patches are positioned
	 * in an rectangular region in UV space.
	 *
	 * After modifying the u0, u1, v0, v1, Nu, or Nv value, the function
	 * UpdatePostSetup() has to be called to update all inner values as well.
	 *
	 * After inserting/updating the cx, cy, and cz values the UpdatePostSolve()
	 * function has to be called to make all the function of a patch
	 * operational.
	 */
	class Patch {
	public:
		Patch() = default;

		/**\brief Calculate a vertex point with normal
		 *
		 * At a position u, v on this patch, return a vertex with position
		 * and normal. u and v of the vertex are set the the requested u and v.
		 */
		Geometry::Vertex operator()(double u, double v) const;

		/**\brief Return a full local coordinate system at a position
		 *
		 * Calculates a full coordinate system at the position u, v. The
		 * X axis is pointing in U-, the Y axis in V-, and the Z axis in normal-
		 * direction.
		 *
		 * The origin of the coordinate system is normally at the position
		 * given by U, V. If originAtPos is set to false, the origin is set
		 * as such, that the current position is at u, v mapped into the
		 * returned coordinate system.
		 *
		 * \note This operation is only one Normalize operation more expensive
		 * 		 than the operator(). The bulk of the calculation are
		 * 		 the approx. Nu*Nv*10 multiplications for the matrices.
		 */
		AffineTransformMatrix GetMatrix(double u, double v, bool originAtPos =
				true) const;

		/**\brief Check, if a position u, v is inside this patch.
		 *
		 * One position can be part of multiple patches as an overlap of
		 * DBL_EPSILON is considered. This is needed for the calculation of
		 * the surface, when edges and points have to shared by two patches
		 * to give a continuous surface.
		 */
		bool IsInside(double u, double v) const;

		/**\brief Prepares a matrix for 2D interpolation.
		 *
		 * Fills a matrix with the Nu*Nv values, that are later multiplied with
		 * the coefficients to calculate the position and the normals.
		 *
		 * This matrix is also used for point interpolation during solving.
		 */
		void Fill(std::vector<double> &vect, double u, double v) const;

		/**\brief Prepares a matrix derived in U direction.
		 *
		 * Matrix for setup derivatives during the solving of the surface.
		 *
		 * The matrix is derived diff-times.
		 */
		void FilldU(std::vector<double> &vect, double u, double v,
				uint8_t diff) const;

		/**\brief Prepares a matrix derived in V direction.
		 *
		 * Matrix for setup derivatives during the solving of the surface.
		 *
		 * The matrix is derived diff-times.
		 */
		void FilldV(std::vector<double> &vect, double u, double v,
				uint8_t diff) const;

		/**\brief Update the internal variables post setup
		 *
		 * This function is called during the setup of the shape of the
		 * surface. It updates the internal polynomials and sizes.
		 */
		void UpdatePostSetup();

		/**\brief UpdatePostSolve the internal variables post solve
		 *
		 * Internally some matrices and polynomials are used to speed up the
		 * calculation of points. After solving the surface, this function has
		 * to be called on every patch.
		 */
		void UpdatePostSolve();

		/**\brief Add this patch to the geometry of the whole surface.
		 */
		void AddToGeometry(Geometry &geo);

		size_t Nu = 0; ///< Number of coefficients in U-direction.
		size_t Nv = 0; ///< Number of coefficients in V-direction.

		double u0 = 0.0; ///< Start of patch in U
		double u1 = 1.0; ///< End of patch in U
		double v0 = 0.0; ///< Start of patch in V
		double v1 = 1.0; ///< End of patch in V

		/**\name Patch coefficients
		 * \{
		 * In the direction U this has a stride of 1 and in the direction V a
		 * stride of Nu. In total all coefficient-vectors have Nu*Nv entries.
		 * The ordering of the coefficients is column first (mathematical
		 * ordering).
		 *
		 * cx, cy, and cz are calculated by solving the surface. The rest
		 * is calculated by the UpdatePostSolve() function.
		 */
		std::vector<double> cx; ///< Coefficients to calculate position X
		std::vector<double> cy; ///< Coefficients to calculate position Y
		std::vector<double> cz; ///< Coefficients to calculate position Z

	private:
		std::vector<double> cdxdu; ///< Coefficients to calculate the change of X for a change of U at a given position
		std::vector<double> cdydu; ///< Coefficients to calculate the change of Y for a change of U at a given position
		std::vector<double> cdzdu; ///< Coefficients to calculate the change of Z for a change of U at a given position

		std::vector<double> cdxdv; ///< Coefficients to calculate the change of X for a change of V at a given position
		std::vector<double> cdydv; ///< Coefficients to calculate the change of Y for a change of V at a given position
		std::vector<double> cdzdv; ///< Coefficients to calculate the change of Z for a change of V at a given position
		/**\}
		 */

		size_t group = 0; ///< Part of concept for non-developable surfaces.

		Polynomial mapu; ///< Internal mapping from global U to a local U for better numeric stability (-1..1)
		Polynomial mapv; ///< Internal mapping from global V to a local V for better numeric stability (-1..1)
	};

	/**\brief Boundary condition that applies to one or more patches.
	 *
	 * Boundaries can be points and the derivatives thereof, continuity
	 * conditions (C0, C1, ...), Axis parallel polynomials of the same
	 * order as the underlying patch.
	 *
	 * All boundaries can be of level 0 or level 1.
	 * The level indicates the order of execution of the boundary. Low level
	 * are applied first and later the higher level to the remaining degrees
	 * of freedom while solving.
	 * (As of now only two level are supported. I have to rewrite the solving
	 * algorithm for an arbitrary number of level.)
	 *
	 * Boundaries have a ignore flag to skip some of them, for example if
	 * derived boundaries were generated (partial solutions, for example along
	 * a line).
	 */
	class Boundary {
	public:
		Boundary() = default;

		/**\brief Check for continuity boundary condition
		 *
		 * A continuity condition is one that has no points attached.
		 * This condition only establishes, that in all places where patches
		 * touch all derivatives up to and including the given order are
		 * continuous.
		 */
		bool IsContinuity() const;

		/**\brief Check for a point boundary condition
		 *
		 * These conditions fix one or more points at a given u, v coordinates
		 * to given values. These values can also be the ones of the derivative
		 * of a given order in a point.
		 */
		bool IsPoints() const;

		/**\brief Check for polynomial boundary condition
		 *
		 * These boundary conditions are already solved values, that are
		 * directly injected into the coefficients. This condition should have
		 * as many as or less values as the edge these values are applied to.
		 */
		bool IsPolynomial() const;

		/**\brief Check if this boundary condition applies to a patch.
		 *
		 * The allowPartialPatches flag is use in this function.
		 */
		bool IsRelevantFor(const Patch &patch) const;

		bool active = true;
		int level = 0; ///< Level of execution. Lower values are solved for first.

		/**\name Region
		 *
		 * Region in which the boundary condition applies.
		 *
		 * For the axis parallel polynomials, two of the coordinates are
		 * identical. (u0 == u1 or v0 == v1).
		 *
		 * Partial patches are patches that are only partly overlapping the
		 * rectangle described.
		 *
		 * For cyclic regions, the cyclic values have to be correct, i.e.
		 * for cyclucU u0 = -M_PI and u1=-M_PI to get the cycle-length correct.
		 * \{
		 */
		double u0 = -DBL_MAX; ///< Start in U
		double u1 = DBL_MAX; ///< End in U
		double v0 = -DBL_MAX; ///< Start in V
		double v1 = DBL_MAX; ///< End in V
		bool allowPartialPatches = false; ///< Apply to patches not completely inside the rectangle as well.
		bool cyclicU = false; ///< Cyclic in U so that u0 == u1
		bool cyclicV = false; ///< Cyclic in V so that v0 == v1
		/**\}
		 */

		std::vector<double> u; ///< Array of U positions
		std::vector<double> v; ///< Array of V positions
		std::vector<Vector3> values; ///< Array of XYZ - values
		int order = 0; ///< Order for derivatives
		enum struct Direction {
			U, V
		} direction = Direction::U; ///< Direction in which derivatives apply.
	};

public:
	Surface() = default;
	virtual ~Surface() = default;

	/**\name Setup of basic structure
	 *
	 * The basic structure of the surface is setup by first calling Clear()
	 * to empty out everything and then adding Patches by calling one
	 * or more times the Setup function. Each time new patches are generated
	 * and added to the surface.
	 *
	 * Patches are connected, if they share a common edge in U or in V.
	 *
	 * The order of one or more patches can be set with SetOrder().
	 *
	 * Setup() and SetOrder() call the Patch::UpdatePostSetup() function
	 * internally.
	 *
	 * No additional steps are needed to initialize the structure.
	 * \{
	 */

	void Clear();

	/**\brief Setup the basic patch structure that makes up the surface
	 *
	 * \param Nu Number of patches in U-direction
	 * \param Nv Number of patches in V-direction
	 * \param u0 Minimum U-coordinate
	 * \param u1 Maximum U-coordinate
	 * \param v0 Minimum V-coordinate
	 * \param v1 Maximum V-coordinate
	 */
	void Setup(size_t Nu, size_t Nv = 1, double u0 = 0.0, double u1 = 1.0,
			double v0 = 0.0, double v1 = 1.0);

	/**\brief Set the order of the patches inside a rectangle.
	 *
	 * Sets the order of all patches that are completely inside (or exactly on
	 * the boundary of the rectangle (u0..u1, v0..v1). The boundaries are
	 * DBL_EPSILON bigger to include the patches on the boundaries.
	 *
	 * Sets the number of coefficients in U and in V. The values have to
	 * be > 0, but should be >= 2 for any meaningful interpolation.
	 */
	void SetOrder(size_t Nu, size_t Nv, double u0 = -DBL_MAX, double u1 =
	DBL_MAX, double v0 = -DBL_MAX, double v1 = DBL_MAX);

	/**\}
	 * \name Boundary conditions
	 *
	 * Boundary conditions are stored internally and applied on solving the
	 * surface. The process is two-step, because the solving is more efficient,
	 * if done piece by piece.
	 *
	 */

	void ClearBoundaries();

	void HardBoundaries();
	void SoftBoundaries();

	/**\brief Stitch the patches together to be continuous.
	 *
	 * \param u0
	 * \param u1
	 * \param v0
	 * \param v1
	 * \param order The Added continuity condition are up to and including this
	 * 				order. If the order is greater than the order of the
	 * 				polynomial, differential up to polynomial order minus 1 are
	 * 			    made continuous. (--> Spline interpolation)
	 * \param cyclicU AddPoint a cyclic boundary at the outside U seam.
	 * \param cyclicV AddPoint a cyclic boundary at the outside V seam.
	 * \param allowPartial Connection lines are considered relevant even if
	 * 					   only the end-point is inside the boundary condition.
	 */
	void AddStitching(double u0, double u1, double v0, double v1, size_t order,
			bool cyclicU = false, bool cyclicV = false, bool allowPartial = false);
	void AddPoint(const Geometry::Vertex &p);
	void AddPoint(double u, double v, const Vector3 &p);
	void AdddPdU(double u, double v, const Vector3 &dPdU);
	void AdddPdV(double u, double v, const Vector3 &dPdV);
	void AddPolynomial(double u0, double u1, double v0, double v1,
			const Polynomial3 &p);

//	void AddPatch(const Surface::Patch &other);
//	void AddSurface(const Surface &other);

	/**\}
	 */

	/**\brief Calculate the coefficients of the patches
	 *
	 * Solve the boundary conditions and update the coefficients in the
	 * patches.
	 */
	void Calculate();

	Geometry::Vertex operator()(double u, double v) const;

	/**\brief Apply the surface to a geometry
	 *
	 * Reads the UV values of all vertices and maps the XYZ and normals onto
	 * the surface.
	 *
	 * Additionally the triangle and edge-normals are updated.
	 */
	void Apply(Geometry &geo);

	/**\brief Update the patches to the base-Geometry object
	 *
	 * Calculating the points in the patches is expensive. This function
	 * calculates the surface and updates the Geometry object underneath.
	 *
	 * The Geometry object can then paint the 3D data using old- or new-school
	 * OpenGL.
	 */
	void Update();

	void Paint() const;

	Geometry ExtractByUVPlane(double u, double v, double d = 1.0);

private:

	/**\brief Increase the A and B matrices for an additional number of rows.
	 */
	size_t IncreaseAB(size_t additional);

	/**\brief Central place the A matrix is filled.
	 *
	 * Centralized to make it easier to switch solvers / matrix systems.
	 */
	void SetA(const std::vector<double> &values, size_t posFrom, size_t posTo,
			size_t count, size_t strideFrom = 1, size_t strideTo = 1,
			double factor = 1.0);

	/**\brief Central place the B matrix is filled.
	 *
	 * Centralized to make it easier to switch solvers / matrix systems.
	 *
	 * Since the B matrix is only a nx3 matrix for n cases to solve for, only
	 * the row number is needed as a parameter.
	 */
	void SetB(const Vector3 value, size_t row);

	/**\brief Mapping function into the column number of the A-matrix.
	 *
	 * Since the patches can have different sizes, this function organizes
	 * the columns belonging to each patch.
	 *
	 * As a side-effect: Called with the number of patches, it will return the
	 * number of coefficients needed and thus the width of the A-matrix.
	 */
	size_t Pos(size_t nPatch, uint8_t idxU = 0, uint8_t idxV = 0) const;

	void PrintProblem(const Matrix &A, const Matrix &b) const;

public:
	std::vector<Patch> patches;
	std::vector<Boundary> boundaries;

	double eps = 1e-9;

	//TODO Remove variables below after changing boundary handling strategy.
	int softBoundaries = true;
	bool boundariedFixed = false;
	std::vector<Geometry::Vertex> debug;
	Matrix Ahard;
	Matrix bhard;
	Matrix Asoft;
	Matrix bsoft;

private:

	/** \brief Speedup variable for patch-search
	 *
	 * It is assumed, that subsequent searches are most likely in the same
	 * patch.
	 */
	mutable size_t lastSearchedPatch = 0;
};

#endif /* L3D_SURFACE_H */

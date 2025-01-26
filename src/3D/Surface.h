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
 * \brief DEPRECATED: Surface description and coordinate system from quadratic patches
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
public:
	/**\brief Patch in the surface.
	 *
	 * Describes a patch in the surface by mapping from UV coordinates to XYZ
	 * coordinates and normal vectors.
	 *
	 * # Points in a patch:
	 *
	 * \htmlonly
	 * <svg width="92" height="85"><g transform ="translate(0,-967.32306)">
	 * <path d="m 19.189407,1029.9748 47.973516,10.7729 1.851608,-60.09305 -50.83509,8.75305 z"  stroke="black"  stroke-width="2" fill="none" />
	 * <text x="70" y="1049">1</text><text x="72" y="984.5">2</text><text x="6" y="990">3</text><text x="6" y="1042">0</text>
	 * </g></svg>
	 * \endhtmlonly
	 */
	class Patch {
	public:
		Patch() = default;
		Geometry::Vertex operator()(double u, double v) const;
		void Init();
		void Update(Geometry &geo);
		bool IsInside(double u, double v) const;
		void Fill(std::vector<double> &vect, double u, double v) const;
		void FilldU(std::vector<double> &vect, double u, double v,
				uint8_t diff) const;
		void FilldV(std::vector<double> &vect, double u, double v,
				uint8_t diff) const;

		size_t Nu = 0; ///< Number of coefficients in U-direction.
		size_t Nv = 0; ///< Number of coefficients in V-direction.

		/**\brief Coefficients for the surface.
		 *
		 * In the direction U this has a stride of 1 and in the direction V a
		 * stride of Nu.
		 */
		std::vector<double> cx;
		std::vector<double> cy;
		std::vector<double> cz;

		std::vector<double> dcxdu;
		std::vector<double> dcydu;
		std::vector<double> dczdu;

		std::vector<double> dcxdv;
		std::vector<double> dcydv;
		std::vector<double> dczdv;

		double u0 = 0.0;
		double u1 = 1.0;
		double v0 = 0.0;
		double v1 = 1.0;

		size_t group = 0; ///< Part of concept for non-developable surfaces.
	private:
		Polynomial mapu;
		Polynomial mapv;

	};

public:
	Surface() = default;
	virtual ~Surface() = default;

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

	void SetOrder(size_t Nu_, size_t Nv_ = 1, double u0 = -DBL_MAX, double u1 =
	DBL_MAX, double v0 = -DBL_MAX, double v1 = DBL_MAX);

	void ClearBoundaries();

	void HardBoundaries();
	void SoftBoundaries();

	/**\brief Stitch the patches together to be continuous.
	 *
	 * \param order The Added continuity condition are up to this order. If the
	 * 			    order is greater than the order of the polynomial,
	 * 			    differential up to polynomial order minus 1 are made
	 * 			    continuous. (--> Spline interpolation)
	 * \param cyclicU Add a cyclic boundary at the outside U seam.
	 * \param cyclicV Add a cyclic boundary at the outside V seam.
	 */
	void Stitch(size_t order, bool cyclicU = false, bool cyclicV = false);

	void Add(const Geometry::Vertex &vertex, double scale = 1.0);
	void AdddU(const Geometry::Vertex &vertex, double scale = 1.0);
	void AdddV(const Geometry::Vertex &vertex, double scale = 1.0);
	void AddPolynomial(double u0, double u1, double v0, double v1,
			const Polynomial3 &p);
//	void AddPatch(const Surface::Patch &other);
//	void AddSurface(const Surface &other);

	/**\brief Calculate the coefficients of the patches
	 *
	 * Solve the hard and soft boundary conditions and update the
	 * coefficients in the patches.
	 *
	 * This function call should be followed by Update(), if the surface should
	 * be painted directly. If only Apply() is used, the Update() is
	 * unnecessary.
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
	 */
	void SetB(const Vector3 value, size_t posTo);

	size_t Pos(size_t nPatch, uint8_t idxU = 0, uint8_t idxV = 0) const;

	void PrintProblem(const Matrix &A, const Matrix &b) const;

public:
	bool softBoundaries = true;

	std::vector<Patch> patches;

	std::vector<Geometry::Vertex> debug;

	Matrix Ahard;
	Matrix bhard;
	Matrix Asoft;
	Matrix bsoft;

	double eps = 1e-9;

	bool boundariedFixed = false;
private:
	/** \brief Speedup variable for patch-search
	 *
	 * It is assumed, that subsequent searches are most likely in the same
	 * patch.
	 */
	mutable size_t lastSearchedPatch = 0;
};

#endif /* L3D_SURFACE_H */

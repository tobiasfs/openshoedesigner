///////////////////////////////////////////////////////////////////////////////
// Name               : EnergyRelease.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -std=c++17 or greater
// Author             : Tobias Schaefer
// Created            : 21.04.2025
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

#ifndef MATH_ENERGYRELEASE_H_
#define MATH_ENERGYRELEASE_H_

/**\class EnergyRelease
 * \code #include "EnergyRelease.h"\endcode
 * \brief Calculate a flattening by energy release
 *
 * Work in progress, because it is still unstable for not evenly sized
 * triangle grids. InitByUniformDimension(9 does the trick for insole
 * flattening from a given model for now.
 *
 * Release energy from a 3D grid to produce a flat 2D grid with minimal energy
 * content.
 * Use energy release to position the u and v coordinates to try to make the
 * length of the edges identical in 3D and 2D.
 *
 * This class provides functions to set up a
 * [spring-mass-damper system](https://en.wikipedia.org/wiki/Mass-spring-damper_model)
 * and release the energy contained in the system build up by naive flattening.
 * This results in a 2D surface, that has a minimal energy content. (It only
 * reaches 0 for a perfectly developable surface.
 *
 * # Stability
 *
 * The original paper [1] uses a mass-spring-system without damper. The
 * dampening is indirectly done by the penalty function to keep the surface
 * from wrinkling (flipped triangles).
 *
 * The original approach only works, when the areas of the triangles are within
 * a small range of sizes, because the area is used to calculate the weight of
 * the triangles and the vertices. Small triangles result in light vertices,
 * that can accelerate in high-speed maneuvers. This necessitates a small
 * sampling rate. That results in a long time until all energy has dissipated
 * or the simulation would be unstable. ([5])
 *
 * This class reintroduces the damper element to the vertices to dissipate
 * the energy stored in the grid after initial flattening.
 *
 * Given a sampling rate dt, a max acceleration is determined. This results
 * in a max quotient from the weight of a vertex and the springs attached to
 * the vertex. [2]
 * \f[
 * \frac{dt^2}{\pi^2} \ll \frac{m}{K}
 * \f]
 *
 * Given a constant dt the quotient either the masses or the stiffness of the
 * springs can be freely fixed.
 *
 * Having constant quotient \f$\frac{K}{m}\f$ results in an even acceleration.
 *
 * ...
 *
 *
 * # Literature and Links
 *
 * [1] Charlie C.L. Wang, Shana S.F. Smith, and Matthew M.F. Yuen,
 *     "Surface flattening based on energy model," Computer-Aided Design,
 *     vol.34, no.11, pp.823-833, 2002.
 *     [Paper](https://mewangcl.github.io/pubs/CADFlatten.pdf)
 *
 * [2] Xavier Provot, "Deformation Constraints in a Mass-Spring Model to
 *     Describe Rigid Cloth Behavior," Proceedings of Graphics Interface '95,
 *     1995.
 *     [Paper](https://graphicsinterface.org/wp-content/uploads/gi1995-17.pdf)
 *     [Conference homepage](https://graphicsinterface.org/proceedings/gi1995/)
 *
 * [3] Yogendra Bhasin and Alan Liu, "Bounds for Damping that Guarantee
 *     Stability in Mass-Spring Systems," Medicine Meets Virtual Reality, 2006
 *     [Paper](https://simcen.org/pdf/bhasin%20mmvr%202006.pdf)
 *
 * [4] Rohan Sawhney and Keenan Crane, "Boundary First Flattening," ACM
 *     Transactions on Graphics, 2020
 *     [Paper](https://www.cs.cmu.edu/~kmcrane/Projects/BoundaryFirstFlattening/paper.pdf)
 *
 * [5] Klaus-Jürgen Bathe and E.L. Wilson, "Stability and Accuracy Analysis of
 *     Direct Integration Methods," Earthquake Engineering and Structural
 *     Dynamics, Vol. 1, 283-291, 1973
 *     [Paper](https://web.mit.edu/kjb/www/Publications_Prior_to_1998/Stability_and_Accuracy_Analysis_of_Direct_Integration_Methods.pdf)
 *     [Homepage](https://meche.mit.edu/people/faculty/kjb@mit.edu)
 */

#include <stddef.h>
#include <cfloat>

#include "../3D/Geometry.h"

class EnergyRelease {
public:
	EnergyRelease() = default;
	EnergyRelease(Geometry &other);
	virtual ~EnergyRelease() = default;

	/**\brief Use a PCA to init the UV values
	 *
	 * Use a PCA to calculate UV coordinates close to the correctly
	 * flattened insole.
	 */
	static void InitByPCA(Geometry &geo);

	/**\brief Static construction algorithm for UV values.
	 *
	 * Starts from the triangle with the largest area.
	 * Triangles are added to keep the resulting area convex.
	 * Triangles are added from large to small.
	 */
	static void InitByConstruction(Geometry &geo);

	/**\brief Set V along the uniform dimension and U by distance.
	 *
	 *  Reduces the UV-mapping to a 2D problem.
	 */
	static void InitByUniformDimension(Geometry &geo,
			const Vector3 &uniformDimension);

	void Calculate(Geometry &other);

	size_t Nmax = 250; ///< Max number of simulation steps
	double dt = 0.001; ///< Simulation timestep

	double rho = 1.0; // kg/m^2
	double delta = 1.0;
	double accel = 1.0; // N/m
	double accel2 = 1.0; // N/m
	double accel3 = 1.0; // N/m

	double xEdge = 0.0;
	double xNormal = 0.0;

	double visForce = 0.0;

	double epsMax = 5e-9;
	double sigmaMax = 1e-9;

	double ES = DBL_MAX;
	double EC = DBL_MAX;
	double Ephi = DBL_MAX;
	double dEphi = DBL_MAX;

	Geometry debug;

private:
	static double CalculateTriangleArea(double Au, double Av, double Bu,
			double Bv, double Cu, double Cv);

	/**\brief Calculate the angle in the corner of a triangle in 3D
	 *
	 * Calculates the change of direction in vb in radians.
	 * Uses the dot-product between the normalized directions from va to vb
	 * and from vb to vc.
	 *
	 * Assumes, that the change in direction if between 0 .. M_PI.
	 */
	static double CalculateAngle3D(const Geometry::Vertex &va,
			const Geometry::Vertex &vb, const Geometry::Vertex &vc);

	/**\brief Calculate the angle in the corner of a triangle in 2D
	 *
	 * Calculates the change of direction in vb in radians in 2D space,
	 * assuming Z is 0.0.
	 *
	 * The angle calculated swings the full range from -M_PI .. M_PI using
	 * atan2 to calculate.
	 *
	 * \note Take care, when calculating the compensation movements as the jump
	 * happens, when a slim triangles flips, i.e. the middle vertex crosses
	 * the opposite edge.
	 */
	static double CalculateAngle2D(const Geometry::Vertex &va,
			const Geometry::Vertex &vb, const Geometry::Vertex &vc);

	/**\brief Modify an given angle to move the continuity gap away.
	 *
	 * Modify an given angle, so that the gap in the continuity of the
	 * angles (-M_PI jumps to M_PI) is on the opposite side of the reference
	 * angle. This means, that the difference between a and ref is always the
	 * shortest connection between these two angles.
	 *
	 * Modifies a by adding n*2*M_PI for an integer n.
	 */
	static double FlipCompensate(double a, double ref);

	/**\brief Calculate the time of the triangle collapse
	 *
	 * Numerically stable. Order of A, B, and C is not important.
	 */
	static double TCollapse(double Au, double Av, double Bu, double Bv,
			double Cu, double Cv, double dAu, double dAv, double dBu,
			double dBv, double dCu, double dCv, bool collapseAlreadyHappened);

	/**\brief Calculate the time of the triangle collapse
	 * \deprecated Numerically unstable.
	 */
	double FlipT(double det, double Au, double Av, double Bu, double Bv,
			double Cu, double Cv, double dAu, double dAv, double dBu,
			double dBv, double dCu, double dCv) const;

	/**\brief Calculate the determinant of the triangle collapse.
	 * \deprecated Not used except by FlipT().
	 */
	static double FlipDet(double dAu, double dAv, double dBu, double dBv,
			double dCu, double dCv);
};

#endif /* MATH_ENERGYRELEASE_H_ */

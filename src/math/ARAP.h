///////////////////////////////////////////////////////////////////////////////
// Name               : ARAP.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.09.2026
// Copyright          : (C) 2026 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MATH_ARAP_H_
#define MATH_ARAP_H_

/*!\class ARAP
 * \brief ARAP - As Rigid As Possible
 *
 * Implementation of the paper "A Local/Global Approach to Mesh Parameterization"
 */

#include "../3D/Geometry.h"
#include "Polynomial.h"
#include <complex>

class ARAP: public Geometry {
public:
	ARAP() = default;
	virtual ~ARAP() = default;
	explicit ARAP(Geometry &other);
	ARAP& operator=(Geometry &other);

	/**\brief Record distances in 3D space.
	 */
	void MeasureDistances();

	/**\brief Set the initial positions of the output mesh based on the UV corrdinates.
	 *
	 * \TODO Check if this is really needed. In the lastest modification of this algorithm the initial positions are calculated from the rotated triangles directly.
	 */
	void InitByUV();
	/**\brief Rescale the triangle mesh in x and y to create a initial fit.
	 *
	 * \TODO Check if this is really needed. In the lastest modification of this algorithm the initial positions are calculated from the rotated triangles directly.
	 */
	void RelaxUniform();

	/**\brief Run the ARAP/ASAP algorithm.
	 */
	void Calculate();

	size_t Nmax = 15; ///< Max. number of optimization steps

#ifdef DEBUG
	Geometry debug; ///< Intermediate mesh only available during debugging.
#endif

private:
	std::vector<double> distances;
	Polynomial errorToColor;

	struct Triangle2D {
		double xc;
		double yc;
		double x0;
		double y0;

		double x1;
		double x2;
		double y2;
		std::array<double, 3> omega;
		double A;
		std::complex<double> L;

		/**\brief Return an edge of the triangle as a complex number.
		 *
		 * In 2D space complex arithmetics can be leveraged for the rotation
		 * of vectors.
		 */
		std::complex<double> getEdgeVector(std::int_fast8_t idx) const {
			if (idx == 1)
				return std::complex<double>(x2 - x1, y2);
			if (idx == 2)
				return std::complex<double>(-x2, -y2);
			return std::complex<double>(x1, 0.0);
		}

	};

	/**\brief Class for storing additional info for the ARAP++ algorithm.
	 *
	 * \todo Probably not needed as the modified ARAP algorithm seems to be very stable.
	 */
//	struct Edge2D {
//		double sumTheta;
//		int countTheta;
//	};

	/**\brief Vector of raw triangles.
	 */
	std::vector<Triangle2D> t2ds;

	/**\brief Initialize a transfer polynominal for the residual errors.
	 */
	void ColorInitScale();

	/**\brief Color the mesh edges showing the residual errors.
	 *
	 * red = compressed, shorter than required
	 * blue = expanded, longer than required
	 *
	 * (The mental color-model is air being compressed/expanded and
	 * getting hot/cold.)
	 */
	void ColorErrors();

	/**\brief Calculate the initialization for the locally flattened triangles.
	 *
	 * 3D -> 2D Vertices: (0,0), (x1,0), (x2,x2)
	 */
	void InitLocalTriangles();

	/**\brief Global angle alignment
	 *
	 */
	void PhaseAlignRotations();

	/**\brief Calculation the rotation matrices L.
	 *
	 */
	void UpdateLocalRotations(const double sigma);

	/**\brief Solve Procrustes problem for a 2x2 matrix
	 *
	 * Does a Singular Value Decomposition
	 *
	 * A,B,C = svd(S)
	 *
	 * Replaces the Eigenvalues in B with
	 *
	 * B = eye(2) // for ARAP if sigma = 1.0
	 *
	 * or
	 *
	 * B = eye(2) * mean(B) // for ASAP if sigma = 0.0
	 *
	 * mean(B) is the mean of both eigenvalues of S represented on the diagonal
	 * of B. The variable sigma interpolates between both.
	 *
	 * Then it recombines everything into a matrix R again.
	 *
	 * R = A*B*C
	 *
	 * From that matrix R only the first two elements are returned as a complex
	 * number as R = [cos, -sin; sin, cos] returning cos + i * sin is enough
	 * (and also more convenient for the next calculations).
	 *
	 * \\param S 2x2 column major matrix S
	 * \\param sigma interpolation value 0 for ASAP and 1 for ARAP. Default is 1.
	 *
	 * \return The rotation as a complex number with cos + i * sin
	 */
	[[nodiscard]] static inline std::complex<double> SolveProcrustes(
			const std::array<double, 4> &S, double sigma = 1.0);

	/**\brief Calculate the cot of the angle between two vectors (angle Theta).
	 *
	 */
	[[nodiscard]] static double VectorCotangens(double x0, double y0, double x1,
			double y1);

	[[nodiscard]] static Color IndexToColor(size_t idx);

#ifdef DEBUG
	void Print2x2(const std::string &name, const std::array<double, 4> &matrix);
#endif
};

#endif /* MATH_ARAP_H_ */

///////////////////////////////////////////////////////////////////////////////
// Name               : SVD.h
// Purpose            : Singular Value Decomposition
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 29.11.2023
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

#ifndef MATH_SVD_H
#define MATH_SVD_H

/**\class SVD
 * \brief Singular Value Decomposition
 *
 *
 */

#include "Matrix.h"

#include <limits>
#include <stddef.h>
#include <vector>

class SVD {
public:
	SVD() = default;
	explicit SVD(const Matrix &A);

	/**\brief Decompose a matrix __A__ in __U__, __W__ and __V__
	 *
	 * Decompose a given matrix __A__ into __U__, __W__ and __V__ so
	 * that __U__ * diag(__W__) * transpose(__V__) = __A__.
	 *
	 * __A__ has to be square or have more rows than columns.
	 *
	 * __A__ can have complete rows or complete columns with only 0 values.
	 *
	 * Columns with zeros will create eigenvalues of 0. The condition of the
	 * matrix __A__ is infinity in this case.
	 */
	void Decompose(const Matrix &A);

	/**\brief Solve __A__ * __X__ = __Y__ for __X__
	 *
	 * After a matrix __A__ has been decomposed, this function can be used to
	 * solve the possibly over-determined system __A__ * __X__ = __Y__.
	 *
	 * The parameter maxCond, if set removes all eigenvalues from the solution,
	 * that would make the condition worse than _maxCond_.
	 *
	 * The parameter minAllowed does set a minimum value for the eigenvalues
	 * used for the solution. Smaller eigenvalues are filtered out.
	 *
	 * The filtering of eigenvalues stabilizes the solution.
	 *
	 * \param Y The result of __A__ * __X__
	 * \param maxCond (optional) Maximum condition of the solution.
	 * \param minAllowed (optional) Minimal allowed value for a eigenvalue.
	 * \return The matrix with __X__ so that __A__ * __X__ = __Y__
	 */
	Matrix Solve(const Matrix &Y, double maxCond =
			std::numeric_limits<double>::infinity(), double minAllowed =
			std::numeric_limits<double>::epsilon()) const;

	/**\brief Condition of the decomposed matrix __A__.
	 *
	 * The condition is the biggest eigenvalue divided by the smallest.
	 */
	double Cond() const;

	/**\brief Maximum number of iterations for the stabilization of the
	 * eigenvalues.
	 *
	 * This is only needed, if the matrix has extreme values in the end of the
	 * double value range.
	 */
	size_t maxIterations = 30;

	/**\brief Flag to enable eigenvalue sorting
	 *
	 * Sort the singular values by size, also change __U__ and __V__ to keep
	 * __U__ * diag(__W__) * transpose(__V__) == __A__ intact.
	 *
	 * (This works even if __U__ is not of full width, because there were
	 * empty columns.)
	 *
	 * \note Sorted eigenvalues are not necessary for the Solve() function to
	 * work.
	 */
	bool sortW = false;

	Matrix U; ///< 2-D column-orthonormal matrix. (Can be interpreted as the normalized axes of a coordinate system.)
	Matrix W; ///< 1-D vector of eigenvalues. (This matrix is sometimes named S.)
	Matrix V; ///< 2-D orthogonal matrix. (Controls the deviation from a symmetric matrix. If __A__ was symmetric, this would be equal to transpose(__U__).)

private:
	std::vector<size_t> empty;
};

#endif /* MATH_SVD_H */

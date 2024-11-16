///////////////////////////////////////////////////////////////////////////////
// Name               : Dependencies.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 13.10.2020
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

#ifndef MATH_DEPENDENCIES_H
#define MATH_DEPENDENCIES_H

/*!\class Dependencies
 * \brief Introduce dependencies to approximate linear equation solutions
 *
 * Reduces the degrees of freedom in a linear equation system to create a
 * reduced set of degrees to solve for e.g by approximation algorithms.
 * After a solution has been found, this matrix can be used to expand the
 * parameters of the equation systems back to the full set.
 *
 * Each row in the matrix has to be filled with one dependency. The matrix
 * is then calculated. The resulting matrix can be used to reduce a Vandermonde
 * matrix by multiplying. The coefficients can be found for example by a
 * pseudoinverse. The resulting coefficients are multiplied with this matrix to
 * get the full set of coefficients.
 */

#include "Matrix.h"

class Dependencies: public Matrix {
public:
	void Calculate();

private:
	void PlotMatrix() const;

};

#endif /* MATH_DEPENDENCIES_H */


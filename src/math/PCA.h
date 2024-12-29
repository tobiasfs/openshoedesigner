///////////////////////////////////////////////////////////////////////////////
// Name               : PCA.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.05.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef MATH_PCA_H
#define MATH_PCA_H
/*!\class PCA
 * \brief Principal component analysis for Vector3 point-clouds
 *
 */

#include "../3D/Vector3.h"

#include <stddef.h>

class PCA {
public:
	PCA();
	virtual ~PCA() = default;

	void Reset(); //!< Reset all parameters and start a new data collection.

	void SetCenter(const Vector3 &center); //!< Calculated middle of all vertices. Has to be provided externally, because this function does not store the passed vertices.
	void Add(const Vector3 &point, double weight_ = 1.0); //!< Add the points to analyze

	void Calculate(); //!< Calculate the PCA and store the values.
	void Paint() const;

	double maxError = 1e-6;
	size_t maxIterations = 100;

	Vector3 X; //!< Dominant eigenvector
	Vector3 Y; //!< Second eigenvector
	Vector3 Z; //!< Third eigenvector
	double eX; //!< Dominant eigenvalue
	double eY; //!< Second eigenvalue
	double eZ; //!< Third eigenvalue

	Vector3 center; //!< Center of dataset used for adding points and for painting the found coordinates. Not used in the Calculate function.

	double weight; //!< Combined weight of recorded values
	double xx; //!< Sum of X*X values
	double xy; //!< Sum of X*Y values
	double xz; //!< Sum of X*Z values
	double yy; //!< Sum of Y*Y values
	double yz; //!< Sum of Y*Z values
	double zz; //!< Sum of Z*Z values
};

#endif /* MATH_PCA_H */

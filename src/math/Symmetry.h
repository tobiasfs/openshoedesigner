///////////////////////////////////////////////////////////////////////////////
// Name               : Symmetry.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 27.05.2019
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

#ifndef MATH_SYMMETRY_H
#define MATH_SYMMETRY_H

/*!\class Symmetry
 * \brief Fourier-transform-based symmetry search
 *
 * Uses the result of the FourierTransform class to find symmetries in polygons.
 * Allows for adding multiple testing Polygon3%s and combining the result.
 *
 * The symmetry axes are found by calculating a core desity estimation on the
 * symmetry axes provided at each frequency.
 */

#include <stddef.h>

#include "KernelDensityEstimator.h"

class FourierTransform;

class Symmetry: public KernelDensityEstimator {
public:
	Symmetry();

	void Init(size_t N);
	void AddTransform(const FourierTransform &transform);
	void Normalize(void);

	void Paint(void) const;

public:
	double sigma = 0.2;
};

#endif /* MATH_SYMMETRY_H */

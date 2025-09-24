///////////////////////////////////////////////////////////////////////////////
// Name               : KernelDensityEstimator.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.06.2019
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

#ifndef KERNELDENSITYESTIMATOR_H
#define KERNELDENSITYESTIMATOR_H

/*!\class KernelDensityEstimator
 * \brief Kernel density estimation
 *
 * This Class does a (cyclic) kernel density estimation.
 *
 * [Wikipedia: Kernel density estimation](https://en.wikipedia.org/wiki/Kernel_density_estimation)
 */

#include <cstddef>

#include "DependentVector.h"

class KernelDensityEstimator: public DependentVector {
public:
	KernelDensityEstimator() = default;

	void Clear();
	void Resize(size_t N);
	void XLinspace(double x0, double x1, size_t N = (size_t) -1);
	void YInit(double value = 0.0);

	void Insert(double pos, double kernel(double), double weight = 1.0,
			double sigma = 1.0);
	void Attenuate(double pos, double kernel(double), double weight = 1.0,
			double sigma = 1.0);

	void Normalize();
	void NormalizeByCoverage();

private:
	size_t count = 0;
	double weightsum = 0.0;
	std::vector<double> coverage;
};

#endif /* KERNELDENSITYESTIMATOR_H */

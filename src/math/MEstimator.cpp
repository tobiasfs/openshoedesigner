///////////////////////////////////////////////////////////////////////////////
// Name               : MEstimator.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.07.2020
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

#include "MEstimator.h"

#include <cfloat>
#include <cmath>
#include <numeric>

double MEstimator::LeastSquares::Rho(double z) const {
	return z * z / 2.0;
}

double MEstimator::LeastSquares::Psi(double z) const {
	return z;
}

double MEstimator::LeastSquares::W(double z) const {
	return 1.0;
}

double MEstimator::HuberK::Rho(double z) const {
	if (fabs(z) <= k)
		return z * z / 2.0;
	return k * fabs(z) - k * k / 2.0;
}

double MEstimator::HuberK::Psi(double z) const {
	if (fabs(z) <= k)
		return z;
	return (z >= 0) ? k : -k;
}

double MEstimator::HuberK::W(double z) const {
	if (fabs(z) <= k)
		return 1;
	return k / fabs(z);
}

double MEstimator::Hampel::Rho(double z) const {
	if (fabs(z) <= a)
		return z * z / 2.0;
	if (fabs(z) <= b)
		return a * fabs(z) - a * a / 2.0;
	if (fabs(z) <= c) {
		const double h = (c - fabs(z)) / (c - b);
		return a * b - a * a / 2.0 + (c - b) * a / 2.0 * (1 - h * h);
	}
	return a * b - a * a / 2.0 + (c - b) * a / 2.0;
}

double MEstimator::Hampel::Psi(double z) const {
	if (fabs(z) <= a)
		return z;
	if (fabs(z) <= b)
		return (z >= 0) ? a : -a;
	if (fabs(z) <= c)
		return ((z >= 0.0) ? 1.0 : -1.0) * a * (c - fabs(z)) / (c - b);
	return 0.0;
}

double MEstimator::Hampel::W(double z) const {
	if (fabs(z) <= a)
		return 1.0;
	if (fabs(z) <= b)
		return a / fabs(z);
	if (fabs(z) <= c)
		return a * (c - fabs(z)) / (c - b) / fabs(z);
	return 0.0;
}

double MEstimator::AndrewWave::Rho(double z) const {
	if (fabs(z) <= a)
		return a * a / (M_PI * M_PI) * (1 - cos(M_PI * z / a));
	return 2 * a * a / (M_PI * M_PI);
}

double MEstimator::AndrewWave::Psi(double z) const {
	if (fabs(z) <= a)
		return a / M_PI * sin(M_PI * z / a);
	return 0.0;
}

double MEstimator::AndrewWave::W(double z) const {
	if (fabs(z) <= a)
		return a / (M_PI * z) * sin(M_PI * z / a);
	return 0.0;
}

double MEstimator::TukeysBiweight::Rho(double z) const {
	if (fabs(z) <= a) {
		const double h = (1 - (z * z) / (a * a));
		return a * a / 6.0 * (1 - h * h * h);
	}
	return a * a / 6.0;
}

double MEstimator::TukeysBiweight::Psi(double z) const {
	if (fabs(z) <= a) {
		const double h = (1 - (z * z) / (a * a));
		return z * h * h;
	}
	return 0.0;
}

double MEstimator::TukeysBiweight::W(double z) const {
	if (fabs(z) <= a) {
		const double h = (1 - (z * z) / (a * a));
		return h * h;
	}
	return 0.0;
}

void MEstimator::EstimateY(const DependentVector &data,
		const Estimator &estimator, const double sigma, size_t xstart,
		size_t xend, std::function<double(double)> weighting) {
	if (xstart >= data.Length())
		return;
	if (xend >= data.Length())
		xend = data.Length() - 1;
	if (xstart > xend)
		return;
	if (xend - xstart < 2)
		return;
	std::vector<double> width;
	width.assign(data.Length(), 0.0);
	for (size_t m = xstart; m <= xend; ++m) {
		if (m == 0) {
			width[m] = data.At(m + 1) - data.At(0);
		} else {
			if (m + 1 == data.Length()) {
				width[m] = data.At(m) - data.At(m - 1);
			} else {
				width[m] = (data.At(m + 1) - data.At(m - 1)) / 2.0;
			}
		}
	}
	const double L0 = std::accumulate(width.begin(), width.end(), 1e-9);
//	std::vector <double> weight;
	weight.assign(data.Length(), 0.0);
	double L = 0.0;
	for (size_t m = xstart; m <= xend; ++m) {
		weight[m] = width[m] * weighting(((2.0 * L + width[m]) / L0) - 1.0);
		L += width[m];
	}
	for (size_t n = 0; n < Length(); ++n) {
		double temp = 0.0;
		for (size_t m = xstart; m <= xend; ++m) {
			const double estimation = estimator.Rho(
					(data.At(m, 1) - At(n)) / sigma);
			temp += estimation * weight[m];
		}
		Y()[n] = temp;
	}
	for (size_t m = xstart; m <= xend; ++m)
		if (width[m] > 1e-9)
			weight[m] /= width[m];
}

void MEstimator::EstimateX(const DependentVector &data,
		const Estimator &estimator, const double sigma, const double yValue,
		size_t xstart, size_t xend) {
	if (xstart >= data.Length())
		return;
	if (xend >= data.Length())
		xend = data.Length() - 1;
	if (xstart > xend)
		return;
	if (xend - xstart < 2)
		return;

//	std::vector <double> width;
//	width.resize(data.Length());
//	for(size_t m = xstart; m <= xend; ++m){
//		if(m == 0){
//			width[m] = data.X(m + 1) - data.X(0);
//		}else{
//			if(m + 1 == data.Length()){
//				width[m] = data.X(m) - data.X(m - 1);
//			}else{
//				width[m] = (data.X(m + 1) - data.X(m - 1)) / 2.0;
//			}
//		}
//	}

	const size_t N = data.Length();
	SetSize(N, 2);
	for (size_t n = 0; n < N; ++n)
		At(n) = data.At(n);

	for (size_t m = 0; m < xstart; ++m)
		Y()[m] = estimator.Rho(-DBL_MAX);
	for (size_t m = xstart; m <= xend; ++m)
		Y()[m] = estimator.Rho((data.At(m, 1) - yValue) / sigma);
	for (size_t m = xend + 1; m < N; ++m)
		Y()[m] = estimator.Rho(DBL_MAX);
}

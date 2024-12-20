///////////////////////////////////////////////////////////////////////////////
// Name               : KernelDensityEstimator.cpp
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

#include "KernelDensityEstimator.h"

#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

void KernelDensityEstimator::Clear() {
	DependentVector::Clear();
	coverage.clear();
	count = 0;
	weightsum = 0.0;
}

void KernelDensityEstimator::Resize(size_t N) {
	DependentVector::Resize(N);
	coverage.assign(N, 0.0);
	count = 0;
	weightsum = 0.0;
}

void KernelDensityEstimator::XLinspace(double x0, double x1, size_t N) {
	DependentVector::XLinspace(x0, x1, N);
	coverage.assign(N, 0.0);
	count = 0;
	weightsum = 0.0;
}

void KernelDensityEstimator::YInit(double value) {
	DependentVector::YInit(value);
	coverage.assign(Size(), 0.0);
	weightsum = 0.0;
	count = 0;
}

void KernelDensityEstimator::Insert(double pos, double kernel(double),
		double weight, double sigma) {
	const size_t N = Size();
	const double x0 = X(0);
	const double x1 = X(N - 1);

	count++;
	weightsum += weight;

	double px = 0.0;
	double p = pos;
	if (IsCyclic()) {
		if (x1 > x0) {
			while (p > x1)
				p -= CycleLength();
			while (p < x0)
				p += CycleLength();
		} else {
			while (p > x0)
				p -= CycleLength();
			while (p < x1)
				p += CycleLength();
		}
		if (p > (x0 + CycleLength() / 2.0))
			p = p - CycleLength();
		px = p + CycleLength() / 2.0;
	}
	for (size_t n = 0; n < N; ++n) {
		double v;
		if (IsCyclic()) {
			v = X(n) - (p + ((X(n) >= px) ? CycleLength() : 0));
		} else {
			v = X(n) - p;
		}
		double f = kernel(v / sigma) / sigma;
		coverage[n] += f;
		Y(n) += f * weight;
	}
}

void KernelDensityEstimator::Attenuate(double pos, double kernel(double),
		double weight, double sigma) {
	const size_t N = Size();
	const double x0 = X(0);
	const double x1 = X(N - 1);
	double px = 0.0;
	double p = pos;
	if (IsCyclic()) {
		if (x1 > x0) {
			while (p > x1)
				p -= CycleLength();
			while (p < x0)
				p += CycleLength();
		} else {
			while (p > x0)
				p -= CycleLength();
			while (p < x1)
				p += CycleLength();
		}
		if (p > (x0 + CycleLength() / 2.0))
			p = p - CycleLength();
		px = p + CycleLength() / 2.0;
	}
	for (size_t n = 0; n < N; ++n) {
		double v;
		if (IsCyclic()) {
			v = X(n) - (p + ((X(n) >= px) ? CycleLength() : 0));
		} else {
			v = X(n) - p;
		}

		Y(n) *= 1.0 - (kernel(v / sigma) / kernel(0) * weight);
	}
}

void KernelDensityEstimator::Normalize() {
	operator/=(Area());
	count = 0;
	weightsum = 1.0;
}

void KernelDensityEstimator::NormalizeByCoverage() {
	if (count == 0) {
		std::ostringstream out;
		out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
		out << "Function called, before kernel were inserted.";
		throw std::logic_error(out.str());
	}
	const size_t N = Size();
	for (size_t n = 0; n < N; ++n)
		if (coverage[n] > FLT_EPSILON)
			Y(n) /= coverage[n];
	coverage.assign(N, 0.0);
	count = 0;
	weightsum = 1.0;
}

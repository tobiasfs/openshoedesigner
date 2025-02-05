///////////////////////////////////////////////////////////////////////////////
// Name               : Dependencies.cpp
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

#include "Dependencies.h"

#include <cfloat>
#include <cmath>
#include <iostream>
#include <stddef.h>
#include <vector>

void Dependencies::Calculate() {
	const size_t N = Size(0);
	const size_t M = Size(1);
	const size_t NM = (N < M) ? N : M;
	std::vector<size_t> Q;
	Q.reserve(M);
	size_t kj = (size_t) -1;
	size_t ki;
//	size_t L = 0;
	for (ki = 0; ki < N; ++ki) {
		double max;
		size_t p;
		do {
			++kj;
			if (kj >= M)
				break;
			p = 0;
			max = 0.0;
			for (size_t i = ki; i < N; ++i) {
				double s = (double) 0.0;
				for (size_t j = kj; j < M; ++j)
					s += fabs(operator[](i + j * N));
				if (s < DBL_EPSILON) {
					continue;
				}
				const double q = fabs(operator[](i + kj * N)) / s;
				if (q > max) {
					max = q;
					p = i;
				}
			}
			if (max < DBL_EPSILON)
				Q.push_back(kj);
		} while (max < DBL_EPSILON);
		if (kj >= M)
			break;
		if (max < DBL_EPSILON)
			continue;
		if (p != ki) {
			for (size_t j = 0; j < M; ++j)
				std::swap(operator[](ki + j * N), operator[](p + j * N)); // @suppress("Invalid arguments")
		}
//		L = ki;
		const double pivot = operator[](ki + kj * N);
		for (size_t j = 0; j < M; ++j)
			operator[](ki + j * N) /= pivot;
		for (size_t j = 0; j < M; ++j) {
			if (j != kj) {
				for (size_t i = 0; i < N; ++i)
					if (i != ki)
						operator[](i + j * N) -= operator[](i + kj * N)
								* operator[](ki + j * N);
			}
		}
		for (size_t i = 0; i < N; ++i)
			if (i != ki)
				operator[](i + kj * N) -= operator[](ki + kj * N)
						* operator[](i + kj * N);
//		PlotMatrix();
	}

	for (size_t j = kj + 1; j < M; ++j)
		Q.push_back(j);

	Dependencies temp = *this;
	const size_t W = Q.size();
	this->SetSize(M, W);
	size_t k = 0;
	for (size_t i = 0; i < M; ++i) {
		if (Q[k] == i) {
			for (size_t g = 0; g < W; ++g)
				operator[](i + g * M) = (g == k) ? 1.0 : 0.0;
			++k;
		} else {
			for (size_t g = 0; g < W; ++g)
				operator[](i + g * M) = -temp[i - k + Q[g] * N];
		}
	}
//	PlotMatrix();
}

void Dependencies::PlotMatrix() const {
	const size_t N = Size(0);
	const size_t M = Size(1);
	std::cout << "---\n";
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < M; ++j) {
			if (j > 0)
				std::cout << '\t';
			std::cout << operator[](i + j * N);
		}
		std::cout << '\n';
	}
	std::cout << "---\n";
}


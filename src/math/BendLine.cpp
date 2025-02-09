///////////////////////////////////////////////////////////////////////////////
// Name               : BendLine.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.03.2019
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

#include "BendLine.h"

#include <cmath>

void BendLine::AngleToPos() {
	const size_t N = Length();
	auto x = X();
	auto y = Y();
	x[0] = 0.0;
	y[0] = 0.0;
	double dx = x[1] - x[0];
	double a = (y[1] + y[0]) / 2.0;
	for (size_t n = 2; n < N; ++n) {
		double xn = x[n - 2] + cos(a) * dx;
		double yn = y[n - 2] + sin(a) * dx;
		dx = x[n] - x[n - 1];
		a = (y[n] + y[n - 1]) / 2.0;
		x[n - 1] = xn;
		y[n - 1] = yn;
	}
	x[N - 1] = x[N - 2] + cos(a) * dx;
	y[N - 1] = y[N - 2] + sin(a) * dx;
}

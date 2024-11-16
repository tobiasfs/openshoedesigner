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
	const size_t N = Size();
	X(0) = 0.0;
	Y(0) = 0.0;
	double dx = X(1) - X(0);
	double a = (Y(1) + Y(0)) / 2.0;
	for (size_t n = 2; n < N; ++n) {
		double xn = X(n - 2) + cos(a) * dx;
		double yn = Y(n - 2) + sin(a) * dx;
		dx = X(n) - X(n - 1);
		a = (Y(n) + Y(n - 1)) / 2.0;
		X(n - 1) = xn;
		Y(n - 1) = yn;
	}
	X(N - 1) = X(N - 2) + cos(a) * dx;
	Y(N - 1) = Y(N - 2) + sin(a) * dx;
}

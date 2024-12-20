///////////////////////////////////////////////////////////////////////////////
// Name               : Polynom2D.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 18.04.2017
// Copyright          : (C) 2017 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "Polynom2D.h"

#include "../3D/Vector3.h"

double Polynom2D::Evaluate(double x, double y) {
	const double T0 = ((a[3] * x + a[2]) * x + a[1]) * x + a[0];
	const double T1 = ((a[7] * x + a[6]) * x + a[5]) * x + a[4];
	const double T2 = ((a[11] * x + a[10]) * x + a[9]) * x + a[8];
	const double T3 = ((a[15] * x + a[14]) * x + a[13]) * x + a[12];
	return ((T3 * y + T2) * y + T1) * y + T0;
}

void Polynom2D::Set44(const double v0, const Vector3 &n0, const double v1,
		const Vector3 &n1, const double v2, const Vector3 &n2, const double v3,
		const Vector3 &n3) {
	const double T2 = -3 * v0;
	const double T3 = 2 * n0.y;
	const double T4 = 3 * n3.x;
	const double T5 = 3 * n2.x;
	const double T6 = 3 * n1.x;
	const double T7 = -3 * n0.x;
	const double T8 = 2 * v0;
	const double T9 = 2 * n0.x;
	const double T10 = -6 * v3;
	const double T11 = 6 * v2;
	const double T12 = 6 * v1;
	const double T13 = -6 * v0;
	const double T14 = 3 * n3.y;
	const double T15 = 3 * n2.y;
	const double T16 = 3 * n1.y;
	const double T17 = -3 * n0.y;
	const double T18 = 2 * n3.y;
	const double T19 = 2 * n3.x;
	const double T20 = 2 * n2.y;
	const double T21 = -2 * n2.x;
	const double T22 = -2 * n1.y;
	const double T23 = 2 * n1.x;
	a[0] = v0;
	a[1] = n0.x;
	a[2] = 3 * v1 + T2 - n1.x - T9;
	a[3] = (-2 * v1) + T8 + n1.x + n0.x;
	a[4] = n0.y;
	a[5] = 0;
	a[6] = T16 + T17;
	a[7] = T22 + T3;
	a[8] = 3 * v2 + T2 - n2.y - T3;
	a[9] = T5 + T7;
	a[10] = 9 * v3 + (-9 * v2) + (-9 * v1) + 9 * v0 - T14 - T4 + T15
			+ (-6 * n2.x) + (-6 * n1.y) + T6 + 6 * n0.y + 6 * n0.x;
	a[11] = T10 + T11 + T12 + T13 + T18 + T4 - T20 + T5 + 4 * n1.y - T6
			+ (-4 * n0.y) + T7;
	a[12] = (-2 * v2) + T8 + n2.y + n0.y;
	a[13] = T21 + T9;
	a[14] = T10 + T11 + T12 + T13 + T14 + T19 - T15 + 4 * n2.x + T16 - T23 + T17
			+ (-4 * n0.x);
	a[15] = 4 * v3 + (-4 * v2) + (-4 * v1) + 4 * v0 - T18 - T19 + T20 + T21
			+ T22 + T23 + T3 + T9;
}

///////////////////////////////////////////////////////////////////////////////
// Name               : Ellipse.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 03.10.2020
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

#include "Ellipse.h"

#include <cmath>
#include "../3D/OpenGL.h"

double Ellipse::Excentricity(void) const {
	const double a = (h > w) ? h : w;
	const double b = (h > w) ? w : h;
	return sqrt(1 - (a * a) / (b * b));
}

double Ellipse::E(const double e) const {
	const double e2 = e * e;
	double h = 1.0;
	double k = e2;
	double v = 1.0;
	for (size_t n = 1; n < 10; n += 2) {
		v *= (double) n / (double) (n + 1);
		h -= v * v * k;
		k *= e2;
	}
	return h * M_PI / 2.0;
}

double Ellipse::Circumfence(void) const {
	const double a = (h > w) ? h : w;
	const double e = Excentricity();
	return 4.0 * a * E(e);
}

std::vector<Ellipse::Point> Ellipse::SteinerConstruction(
		size_t nrOfPoints) const {
	std::vector<Point> temp;
	const double dd = 4.0 / (double) (nrOfPoints);
	double d = 0.0;
	for (; d < 1.0; d += dd) {
		const double r = d;
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		temp.push_back(Point(w * (r2 - 1.0) * rd, h * 2.0 * d * r));
	}
	for (; d < 3.0; d += dd) {
		const double r = d - 1.0;
		const double r2 = d * d;
		const double rd = 1.0 / (1 + r2);
		temp.push_back(Point(-w * (r2 - 1.0) * rd, -h * 2.0 * r * rd));
	}
	for (; d < (4.0 - dd / 2.0); d += dd) {
		const double r = d - 3.0;
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		temp.push_back(Point(w * (r2 - 1.0) * rd, h * 2.0 * d * r));
	}
	return temp;
}

void Ellipse::Paint(void) const {
	const double dr = 0.1;
	glBegin(GL_LINE_STRIP);
	for (double r = -1.0; r <= (1.0 + dr / 2); r += dr) {
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		glVertex2d(w * (r2 - 1.0) * rd, h * 2.0 * r * rd);
	}
	for (double r = -1.0; r <= (1.0 + dr / 2); r += dr) {
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		glVertex2d(-w * (r2 - 1.0) * rd, -h * 2.0 * r * rd);
	}

	glEnd();
}


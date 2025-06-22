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

double Ellipse::Excentricity() const {
	const double w = GetEx().Abs();
	const double h = GetEy().Abs();
	const double a = (h > w) ? h : w;
	const double b = (h > w) ? w : h;
	return sqrt(1 - (b * b) / (a * a));
}

double Ellipse::E(const double e) {
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

double Ellipse::Circumfence() const {
	const double w = GetEx().Abs();
	const double h = GetEy().Abs();
	const double a_ = (h > w) ? h : w;
	const double e = Excentricity();
	return 4.0 * a_ * E(e);
}

Polygon3 Ellipse::SteinerConstruction(size_t nrOfPoints) const {
	Polygon3 ret;
	const double dd = 4.0 / (double) (nrOfPoints);
	double d = 0.0;
	for (; d < 1.0; d += dd) {
		const double r = d;
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		Geometry::Vertex vert = Transform((r2 - 1.0) * rd, 2.0 * d * r);
		ret.AddEdgeToVertex(vert);
	}
	for (; d < 3.0; d += dd) {
		const double r = d - 1.0;
		const double r2 = d * d;
		const double rd = 1.0 / (1 + r2);
		Geometry::Vertex vert = Transform(-(r2 - 1.0) * rd, -2.0 * r * rd);
		ret.AddEdgeToVertex(vert);
	}
	for (; d < (4.0 - dd / 2.0); d += dd) {
		const double r = d - 3.0;
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		Geometry::Vertex vert = Transform((r2 - 1.0) * rd, 2.0 * d * r);
		ret.AddEdgeToVertex(vert);
	}
	ret.CloseLoopNextGroup();
	return ret;
}

Bezier3 Ellipse::GetBezierSpline(size_t N) const {
	// Polynomial for the offsetting of the inner Bezier-handles.
	const Polynomial shift( { -5.6457e-01, 7.6449e-01, -6.7089e-02, 7.3566e-03,
			-3.0604e-04 });
	const Polynomial ang = Polynomial::ByValue(0, -M_PI, N, M_PI);

	Bezier3 ret;
	ret.SetSize(N);
	ret.CloseLoop(true);

	for (size_t n = 0; n < N; ++n) {
		const double a0 = ang(n);
		const double s0 = sin(a0);
		const double c0 = cos(a0);
		const double sval = 3.0 / shift(N);

		ret[n] = Transform(c0, s0);
		ret[n].dirIn = (Transform(c0 - s0, s0 + c0) - ret[n]) * sval;
		ret[n].continuity = Bezier3::Continuity::Symmetric;
	}
	ret.UpdateSegments();
	return ret;
}

void Ellipse::Paint() const {
	const double dr = 0.1;
	glBegin(GL_LINE_STRIP);
	for (double r = -1.0; r <= (1.0 + dr / 2); r += dr) {
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		Vector3 v = Transform((r2 - 1.0) * rd, 2.0 * r * rd);
		glVertex3d(v.x, v.y, v.z);
	}
	for (double r = -1.0; r <= (1.0 + dr / 2); r += dr) {
		const double r2 = r * r;
		const double rd = 1.0 / (1 + r2);
		Vector3 v = Transform(-(r2 - 1.0) * rd, -2.0 * r * rd);
		glVertex3d(v.x, v.y, v.z);
	}
	glEnd();
}


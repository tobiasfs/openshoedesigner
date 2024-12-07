///////////////////////////////////////////////////////////////////////////////
// Name               : Polynom3.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 27.09.2020
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

#include "Polynomial3.h"

#include "../math/DependentVector.h"
#include "Vector3.h"
#include "OpenGL.h"
#include "Polygon3.h"

Polynomial3 Polynomial3::ByValue(double p0, const Vector3& v0) {
	Polynomial3 temp;
	temp.x = Polynomial::ByValue(p0, v0.x);
	temp.y = Polynomial::ByValue(p0, v0.y);
	temp.z = Polynomial::ByValue(p0, v0.z);
	return temp;
}

Polynomial3 Polynomial3::ByValue(double p0, const Vector3& v0, double p1,
		const Vector3& v1) {
	Polynomial3 temp;
	temp.x = Polynomial::ByValue(p0, v0.x, p1, v1.x);
	temp.y = Polynomial::ByValue(p0, v0.y, p1, v1.y);
	temp.z = Polynomial::ByValue(p0, v0.z, p1, v1.z);
	return temp;
}

Polynomial3 Polynomial3::ByValue(double p0, const Vector3& v0, double p1,
		const Vector3& v1, double p2, const Vector3& v2) {
	Polynomial3 temp;
	temp.x = Polynomial::ByValue(p0, v0.x, p1, v1.x, p2, v2.x);
	temp.y = Polynomial::ByValue(p0, v0.y, p1, v1.y, p2, v2.y);
	temp.z = Polynomial::ByValue(p0, v0.z, p1, v1.z, p2, v2.z);
	return temp;
}

Polynomial3 Polynomial3::ByValue(double p0, const Vector3& v0, double p1,
		const Vector3& v1, double p2, const Vector3& v2, double p3,
		const Vector3& v3) {
	Polynomial3 temp;
	temp.x = Polynomial::ByValue(p0, v0.x, p1, v1.x, p2, v2.x, p3, v3.x);
	temp.y = Polynomial::ByValue(p0, v0.y, p1, v1.y, p2, v2.y, p3, v3.y);
	temp.z = Polynomial::ByValue(p0, v0.z, p1, v1.z, p2, v2.z, p3, v3.z);
	return temp;
}

Polynomial3 Polynomial3::ByBezier(const Vector3& v0) {
	Polynomial3 temp;
	temp.x = Polynomial::ByBezier(v0.x);
	temp.y = Polynomial::ByBezier(v0.y);
	temp.z = Polynomial::ByBezier(v0.z);
	return temp;
}

Polynomial3 Polynomial3::ByBezier(const Vector3& v0, const Vector3& v1) {
	Polynomial3 temp;
	temp.x = Polynomial::ByBezier(v0.x, v1.x);
	temp.y = Polynomial::ByBezier(v0.y, v1.y);
	temp.z = Polynomial::ByBezier(v0.z, v1.z);
	return temp;
}

Polynomial3 Polynomial3::ByBezier(const Vector3& v0, const Vector3& v1,
		const Vector3& v2) {
	Polynomial3 temp;
	temp.x = Polynomial::ByBezier(v0.x, v1.x, v2.x);
	temp.y = Polynomial::ByBezier(v0.y, v1.y, v2.y);
	temp.z = Polynomial::ByBezier(v0.z, v1.z, v2.z);
	return temp;
}

Polynomial3 Polynomial3::ByBezier(const Vector3& v0, const Vector3& v1,
		const Vector3& v2, const Vector3& v3) {
	Polynomial3 temp;
	temp.x = Polynomial::ByBezier(v0.x, v1.x, v2.x, v3.x);
	temp.y = Polynomial::ByBezier(v0.y, v1.y, v2.y, v3.y);
	temp.z = Polynomial::ByBezier(v0.z, v1.z, v2.z, v3.z);
	return temp;
}

Polynomial3 Polynomial3::ByPolygon3(const Polygon3& p, size_t order, size_t idxStart,
		size_t idxEnd) {
	if (idxEnd >= p.Size())
		idxEnd = p.Size() - 1;
	if (idxStart >= idxEnd)
		return Polynomial3();

	const size_t N = idxEnd - idxStart + 1;
	std::vector<double> r(N, 0.0);
	std::vector<double> vx(N, 0.0);
	std::vector<double> vy(N, 0.0);
	std::vector<double> vz(N, 0.0);
	double L = 0.0;
	for (size_t n = 0; n < idxStart; ++n)
		L += (p[n + 1] - p[n]).Abs();
	for (size_t n = 0; n < N; ++n) {
		r[n] = L;
		vx[n] = p[n + idxStart].x;
		vy[n] = p[n + idxStart].y;
		vz[n] = p[n + idxStart].z;
		if ((n + 1) < N)
			L += (p[n + 1] - p[n]).Abs();
	}
	// Normalize
	const double b = r[0];
	const double a = 1.0 / (r[N - 1] - b);
	for (size_t n = 0; n < N; ++n)
		r[n] = (r[n] - b) * a;

	Polynomial3 temp;
	temp.x = Polynomial::ByVector(r, vx, order);
	temp.y = Polynomial::ByVector(r, vy, order);
	temp.z = Polynomial::ByVector(r, vz, order);
	return temp;
}

double Polynomial3::Length(const size_t N) const {
	Polynomial rn = Polynomial::ByValue(0, r0, N - 1, r1);
	double d = 0;
	Vector3 p0(x(r0), y(r0), z(r0));
	for (size_t n = 1; n < N; ++n) {
		const double r = rn(n);
		Vector3 p1(x(r), y(r), z(r));
		d += (p1 - p0).Abs();
		p0 = p1;
	}
	return d;
}

Vector3 Polynomial3::operator ()(double r) const {
	return Vector3(x(r), y(r), z(r));
}

void Polynomial3::Paint() const {
	glPointSize(3);
	glBegin(GL_POINTS);
	glVertex3d(x(0), y(0), z(0));
	glVertex3d(x(1), y(1), z(1));
	glEnd();
	glBegin(GL_LINE_STRIP);
	const double N = 100;
	const Polynomial i = Polynomial::ByValue(0, r0, (N - 1), r1);
	for (size_t n = 0; n < N; ++n) {
		const double r = i(n);
		glVertex3d(x(r), y(r), z(r));
	}
	glEnd();
	glPointSize(1);
}

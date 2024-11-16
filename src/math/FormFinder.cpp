///////////////////////////////////////////////////////////////////////////////
// Name               : FormFinder.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 31.05.2019
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

#include "FormFinder.h"

#include <iostream>
#include <GL/gl.h>

#include "../Config.h"

void FormFinder::AddPolygon(const Polygon3 &poly, size_t Nsections) {
	pf.Init(2, Nsections);

	std::vector<double> x = poly.GetXVectorD();
	std::vector<double> y = poly.GetYVectorD();
	std::vector<double> z = poly.GetZVectorD();
/*
	const size_t N = poly.IsClosed() ? poly.Size() : (poly.Size() - Nsections);
	a.resize(N);
	b.resize(N);
	c.resize(N);
	d.resize(N);
	for (size_t n = 0; n < N; ++n) {
		a[n].x = pf.Filter(x, n);
		a[n].y = pf.Filter(y, n);
		a[n].z = pf.Filter(z, n);
		b[n].x = pf.Filter(x, n);
		b[n].y = pf.Filter(y, n);
		b[n].z = pf.Filter(z, n);
		c[n].x = pf.Filter(x, n);
		c[n].y = pf.Filter(y, n);
		c[n].z = pf.Filter(z, n);
		d[n].x = pf.Filter(x, n);
		d[n].y = pf.Filter(y, n);
		d[n].z = pf.Filter(z, n);
	}
*/
	double pos;
//	for (size_t n = 0; n < N; n += 1) {
//		DEBUGOUT << n << ": ("
//				<< pf.GetExtremum(pos, a[n].x, b[n].x, c[n].x, d[n].x) << ") ";
//		DEBUGOUT << (double) n + pos << " - (";
//		DEBUGOUT << pf.GetExtremum2(pos, a[n].x, b[n].x, c[n].x, d[n].x)
//				<< ") ";
//		DEBUGOUT << (double) n + pos << "\n";
//	}

}

void FormFinder::Paint(void) const {
	const double eps = 1e-9;
	const size_t N = a.size();
	for (size_t n = 0; n < N; n += pf.Size()) {
//	size_t n = 58;
//		const double x = c[n].x
//				- ((fabs(a[n].x) < eps)? 0 : ((b[n].x * b[n].x + 1)
//						/ (4 * a[n].x)));
//		const double y = c[n].y
//				- ((fabs(a[n].y) < eps)? 0 : ((b[n].y * b[n].y + 1)
//						/ (4 * a[n].y)));
//		const double z = c[n].z
//				- ((fabs(a[n].z) < eps)? 0 : ((b[n].z * b[n].z + 1)
//						/ (4 * a[n].z)));
//		glVertex3f(x, y, z);

		glBegin(GL_LINE_STRIP);
		for (double r = 0; r <= pf.Size(); r += 0.5) {
			Vector3 temp = ((a[n] * r + b[n]) * r + c[n]) * r + d[n];
			glVertex3f(temp.x, temp.y, temp.z);
		}

		glEnd();
	}

	glPointSize(10);
	glBegin(GL_POINTS);

	double pos;
	for (size_t n = 0; n < N; n += 1) {
//		if(pf.GetExtremum(pos, a[n].x, b[n].x, c[n].x, d[n].x)){
//			if(pos < 0 || pos > pf.GetSize()) continue;
//			Vector3 temp = ((a[n] * pos + b[n]) * pos + c[n]) * pos + d[n];
//			glVertex3f(temp.x, temp.y, temp.z);
//		}
	}
	glEnd();

	glPointSize(1);
}

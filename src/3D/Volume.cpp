///////////////////////////////////////////////////////////////////////////////
// Name               : Volume.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.08.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Volume.h"

#include "../StdInclude.h"

#include "Triangle.h"

#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstdint>

Volume& Volume::operator =(const Volume &other) {
	if (&other == this)
		return *this;
	OrientedMatrix::operator=(other);
	color = other.color;
	geometry = other.geometry;
	return *this;
}

Volume::Volume(const Volume &other) {
	if (&other == this)
		return;
	OrientedMatrix::operator=(other);
	color = other.color;
	geometry = other.geometry;
}

Volume::~Volume() {
	if (m_gllist != 0)
		glDeleteLists(m_gllist, 1);
}

void Volume::Clear(void) {
	values.assign(values.size(), 0.0);
}

void Volume::AddHalfplane(const Vector3 &p1, float d0, float k0) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);
	Vector3 p(0, 0, 0);
	const float kh = -log(2 * M_E - 1) / k0;
	unsigned int c = 0;
	for (size_t nz = 0; nz < Nz; nz++) {
		for (size_t ny = 0; ny < Ny; ny++) {
			for (size_t nx = 0; nx < Nx; nx++) {
				const float d = (p + origin).Dot(p1);
				values[c] += 1 - 1 / (1 + exp(kh * (d - d0)));
				c++;
				p.x += dx;
			}
			p.x = 0.0;
			p.y += dy;
		}
		p.y = 0.0;
		p.z += dz;
	}
}

void Volume::AddSphere(const Vector3 &p1, float r1, float k1) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);
	const Vector3 h1 = p1 - origin;
	Vector3 p(0, 0, 0);
	unsigned int c = 0;
	const float kh = -log(2 * M_E - 1) / k1;
	for (size_t nz = 0; nz < Nz; nz++) {
		for (size_t ny = 0; ny < Ny; ny++) {
			for (size_t nx = 0; nx < Nx; nx++) {
				const float d = 1 - 1 / (1 + exp(kh * ((p - h1).Abs() - r1)));
				values[c] += d;
				c++;
				p.x += dx;
			}
			p.x = 0.0;
			p.y += dy;
		}
		p.y = 0.0;
		p.z += dz;
	}
}

void Volume::AddCylinder(const Vector3 &p1, const Vector3 &p2, const float r1,
		const float k1) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);
	const Vector3 h1 = p1 - origin;
	const Vector3 h2 = p2 - origin;
	Vector3 n = h2 - h1;
	const float nd = n.Abs();
	const float kh = -log(2 * M_E - 1) / k1;
	n.Normalize();
	Vector3 p(0, 0, 0);
	unsigned int c = 0;
	for (size_t nz = 0; nz < Nz; nz++) {
		for (size_t ny = 0; ny < Ny; ny++) {
			for (size_t nx = 0; nx < Nx; nx++) {
				float r = n.Dot(p - h1);
				if (r < 0)
					r = 0;
				if (r > nd)
					r = nd;
				const Vector3 h = h1 + n * r;
				const float d = (p - h).Abs() - r1;

				values[c] += 1 - 1 / (1 + exp(kh * d));
				c++;
				p.x += dx;
			}
			p.x = 0.0;
			p.y += dy;
		}
		p.y = 0.0;
		p.z += dz;
	}
}

void Volume::AddCylinder(const Vector3 &p1, const Vector3 &p2, const float r1,
		const float r2, const float k1) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);

	const Vector3 h1 = p1 - origin;
	const Vector3 h2 = p2 - origin;
	Vector3 n = h2 - h1;
	const double nd = n.Abs();
	if (nd == 0) {
		AddSphere(p1, fmax(r1, r2), k1);
		return;
	}
	if (nd + r2 <= r1) {
		AddSphere(p1, r1, k1);
		return;
	}
	if (nd + r1 <= r2) {
		AddSphere(p2, r2, k1);
		return;
	}

	n.Normalize();

//	float a = 1 / (1 + (2 * r1 * r2 - r1 * r1 - r2 * r2) / (nd * nd));
//	float b = (r1 * r2 - r1 * r1) / nd;

// x2 = x1*a
// y2 = x1*b

	const double s0 = (r1 * r1 - r1 * r2) / nd;
	const double h0 = r1 * sqrt(2 * r1 * r2 + nd * nd - r1 * r1 - r2 * r2) / nd;
	// Coordinatesystemtransformation:
	const double a11 = 1 / nd;
	const double a12 = -s0 / (h0 * nd);
//	const float a21 = (r1 - r2) / nd;
//	const float a22 = (r2 * s0 - r1 * s0 + nd * r1) / (h0 * nd);
	const double a21 = 0;
	const double a22 = r1 / h0;

	const double kh = -log(2 * M_E - 1) / k1;
	Vector3 p(0, 0, 0);
	Vector3 h;
	unsigned int c = 0;
	for (size_t nz = 0; nz < Nz; nz++) {
		for (size_t ny = 0; ny < Ny; ny++) {
			for (size_t nx = 0; nx < Nx; nx++) {
				const double x = n.Dot(p - h1);
				h = n * x + h1;
				const double y = (p - h).Abs();
				const double x2 = fma(x, a11, y * a12);
				const double y2 = fma(x, a21, y * a22);
				double d = y2;

				if (x2 < 0)
					d = (p - h1).Abs();
				if (x2 > 1)
					d = (p - h2).Abs();
//				if(x2 >= 0) d = sqrt(
//						(x2 - nd * a) * (x2 - nd * a) + y2 * y2);
				double r;
				if (x2 < 0) {
					r = r1;
				} else {
					if (x2 < 1) {
						r = fma(x2, r2 - r1, r1); // = r1 + (r2 - r1) * x2;
					} else {
						r = r2;
					}
				}
				d -= r;
				d *= kh;

				if (d > -2) {
					if (d > 2) {
						d = 1.0;
					} else {
						d = d * 0.25 + 0.5;
					}
					values[c] += d;
				}
				//d = 1 - 1 / (1 + exp(-k1 * d));
//				if(d > -5){
//					if(d > 5){
//						d = 1.0;
//					}else{
//						d = 1 - 1 / (1 + exp(d));
//					}
//					values[c] += d;
//				}
				c++;
				p.x += dx;
			}
			p.x = 0.0;
			p.y += dy;
		}
		p.y = 0.0;
		p.z += dz;
	}
}

void Volume::AddCylinder(const Vector3 &p1, const Vector3 &p2, const float r1,
		const float r2, const float k1, const float k2) {
	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);
	const Vector3 h1 = p1 - origin;
	const Vector3 h2 = p2 - origin;
	Vector3 n = h2 - h1;
	const float nd = n.Abs();
	if (nd == 0) {
		AddSphere(p1, fmax(r1, r2), k1);
		return;
	}
	if (nd + r2 <= r1) {
		AddSphere(p1, r1, k1);
		return;
	}
	if (nd + r1 <= r2) {
		AddSphere(p2, r2, k1);
		return;
	}

	n.Normalize();

//	float a = 1 / (1 + (2 * r1 * r2 - r1 * r1 - r2 * r2) / (nd * nd));
//	float b = (r1 * r2 - r1 * r1) / nd;

// x2 = x1*a
// y2 = x1*b

	const float dlimit = fmax(fmax(dx, dy), dz);

	const float s0 = (r1 * r1 - r1 * r2) / nd;
	const float h0 = r1 * sqrt(2 * r1 * r2 + nd * nd - r1 * r1 - r2 * r2) / nd;
// Coordinatesystemtransformation:
	const float a11 = 1 / nd;
	const float a12 = -s0 / (h0 * nd);
//	const float a21 = (r1 - r2) / nd;
//	const float a22 = (r2 * s0 - r1 * s0 + nd * r1) / (h0 * nd);
	const float a21 = 0;
	const float a22 = r1 / h0;

	const float kh = -log(2 * M_E - 1);
	Vector3 p(0, 0, 0);
	size_t c = 0;
	for (size_t nz = 0; nz < Nz; nz++) {
		for (size_t ny = 0; ny < Ny; ny++) {
			for (size_t nx = 0; nx < Nx; nx++) {
				float x = n.Dot(p - h1);
				const Vector3 h = h1 + n * x;
				const float y = (p - h).Abs();
				const float x2 = x * a11 + y * a12;
				const float y2 = x * a21 + y * a22;
				float d = y2;

				if (x2 < 0)
					d = (p - h1).Abs();
				if (x2 > 1)
					d = (p - h2).Abs();
				//				if(x2 >= 0) d = sqrt(
				//						(x2 - nd * a) * (x2 - nd * a) + y2 * y2);
				float r;
				if (x2 < 0) {
					r = r1;
				} else {
					if (x2 < 1) {
						r = r1 + (r2 - r1) * x2;
					} else {
						r = r2;
					}
				}
				float k;
//				if(x2 < 0){
//					k = k1;
//				}else{
//					if(x2 < 1){
				k = k1 + (k2 - k1) * x2;
//					}else{
//						k = k2;
//					}
//				}
				if (k < dlimit)
					k = dlimit;

				d -= r;
				//d = 1 - 1 / (1 + exp(-k1 * d));
				d = 1 - 1 / (1 + exp(kh * d / k));
				values[c] += d;
				c++;
				p.x += dx;
			}
			p.x = 0.0;
			p.y += dy;
		}
		p.y = 0.0;
		p.z += dz;
	}
}

const static int8_t tri[3072] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, 0, 1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, 8, 3, 1, 8, 1, 9, -1, -1, -1, -1, -1, -1, 11, 2,
		3, -1, -1, -1, -1, -1, -1, -1, -1, -1, 11, 2, 0, 11, 0, 8, -1, -1, -1,
		-1, -1, -1, 11, 2, 3, 0, 1, 9, -1, -1, -1, -1, -1, -1, 2, 1, 11, 1, 9,
		11, 11, 9, 8, -1, -1, -1, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		8, 3, 0, 1, 2, 10, -1, -1, -1, -1, -1, -1, 9, 0, 2, 9, 2, 10, -1, -1,
		-1, -1, -1, -1, 3, 2, 8, 2, 10, 8, 8, 10, 9, -1, -1, -1, 10, 1, 3, 10,
		3, 11, -1, -1, -1, -1, -1, -1, 1, 0, 10, 0, 8, 10, 10, 8, 11, -1, -1,
		-1, 0, 3, 9, 3, 11, 9, 9, 11, 10, -1, -1, -1, 8, 10, 9, 8, 11, 10, -1,
		-1, -1, -1, -1, -1, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, 3, 0,
		4, 3, 4, 7, -1, -1, -1, -1, -1, -1, 1, 9, 0, 8, 4, 7, -1, -1, -1, -1,
		-1, -1, 9, 4, 1, 4, 7, 1, 1, 7, 3, -1, -1, -1, 2, 3, 11, 7, 8, 4, -1,
		-1, -1, -1, -1, -1, 7, 11, 4, 11, 2, 4, 4, 2, 0, -1, -1, -1, 3, 11, 2,
		4, 7, 8, 9, 0, 1, -1, -1, -1, 2, 7, 11, 2, 1, 7, 1, 4, 7, 1, 9, 4, 10,
		1, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, 2, 10, 1, 0, 4, 7, 0, 7, 3, -1,
		-1, -1, 4, 7, 8, 0, 2, 10, 0, 10, 9, -1, -1, -1, 2, 7, 3, 2, 9, 7, 7, 9,
		4, 2, 10, 9, 8, 4, 7, 11, 10, 1, 11, 1, 3, -1, -1, -1, 11, 4, 7, 1, 4,
		11, 1, 11, 10, 1, 0, 4, 3, 8, 0, 7, 11, 4, 11, 9, 4, 11, 10, 9, 7, 11,
		4, 4, 11, 9, 11, 10, 9, -1, -1, -1, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, 3, 0, 8, 4, 9, 5, -1, -1, -1, -1, -1, -1, 5, 4, 0, 5, 0, 1, -1,
		-1, -1, -1, -1, -1, 4, 8, 5, 8, 3, 5, 5, 3, 1, -1, -1, -1, 11, 2, 3, 9,
		5, 4, -1, -1, -1, -1, -1, -1, 9, 5, 4, 8, 11, 2, 8, 2, 0, -1, -1, -1, 3,
		11, 2, 1, 5, 4, 1, 4, 0, -1, -1, -1, 8, 5, 4, 2, 5, 8, 2, 8, 11, 2, 1,
		5, 2, 10, 1, 9, 5, 4, -1, -1, -1, -1, -1, -1, 0, 8, 3, 5, 4, 9, 10, 1,
		2, -1, -1, -1, 10, 5, 2, 5, 4, 2, 2, 4, 0, -1, -1, -1, 3, 4, 8, 3, 2, 4,
		2, 5, 4, 2, 10, 5, 5, 4, 9, 1, 3, 11, 1, 11, 10, -1, -1, -1, 0, 9, 1, 4,
		8, 5, 8, 10, 5, 8, 11, 10, 3, 4, 0, 3, 10, 4, 4, 10, 5, 3, 11, 10, 4, 8,
		5, 5, 8, 10, 8, 11, 10, -1, -1, -1, 9, 5, 7, 9, 7, 8, -1, -1, -1, -1,
		-1, -1, 0, 9, 3, 9, 5, 3, 3, 5, 7, -1, -1, -1, 8, 0, 7, 0, 1, 7, 7, 1,
		5, -1, -1, -1, 1, 7, 3, 1, 5, 7, -1, -1, -1, -1, -1, -1, 11, 2, 3, 8, 9,
		5, 8, 5, 7, -1, -1, -1, 9, 2, 0, 9, 7, 2, 2, 7, 11, 9, 5, 7, 0, 3, 8, 2,
		1, 11, 1, 7, 11, 1, 5, 7, 2, 1, 11, 11, 1, 7, 1, 5, 7, -1, -1, -1, 1, 2,
		10, 5, 7, 8, 5, 8, 9, -1, -1, -1, 9, 1, 0, 10, 5, 2, 5, 3, 2, 5, 7, 3,
		5, 2, 10, 8, 2, 5, 8, 5, 7, 8, 0, 2, 10, 5, 2, 2, 5, 3, 5, 7, 3, -1, -1,
		-1, 3, 9, 1, 3, 8, 9, 7, 11, 10, 7, 10, 5, 9, 1, 0, 10, 7, 11, 10, 5, 7,
		-1, -1, -1, 3, 8, 0, 7, 10, 5, 7, 11, 10, -1, -1, -1, 11, 5, 7, 11, 10,
		5, -1, -1, -1, -1, -1, -1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		0, 8, 3, 11, 7, 6, -1, -1, -1, -1, -1, -1, 9, 0, 1, 11, 7, 6, -1, -1,
		-1, -1, -1, -1, 7, 6, 11, 3, 1, 9, 3, 9, 8, -1, -1, -1, 2, 3, 7, 2, 7,
		6, -1, -1, -1, -1, -1, -1, 8, 7, 0, 7, 6, 0, 0, 6, 2, -1, -1, -1, 1, 9,
		0, 3, 7, 6, 3, 6, 2, -1, -1, -1, 7, 6, 2, 7, 2, 9, 2, 1, 9, 7, 9, 8, 1,
		2, 10, 6, 11, 7, -1, -1, -1, -1, -1, -1, 2, 10, 1, 7, 6, 11, 8, 3, 0,
		-1, -1, -1, 11, 7, 6, 10, 9, 0, 10, 0, 2, -1, -1, -1, 7, 6, 11, 3, 2, 8,
		8, 2, 10, 8, 10, 9, 6, 10, 7, 10, 1, 7, 7, 1, 3, -1, -1, -1, 6, 10, 1,
		6, 1, 7, 7, 1, 0, 7, 0, 8, 9, 0, 3, 6, 9, 3, 6, 10, 9, 6, 3, 7, 6, 10,
		7, 7, 10, 8, 10, 9, 8, -1, -1, -1, 8, 4, 6, 8, 6, 11, -1, -1, -1, -1,
		-1, -1, 11, 3, 6, 3, 0, 6, 6, 0, 4, -1, -1, -1, 0, 1, 9, 4, 6, 11, 4,
		11, 8, -1, -1, -1, 1, 9, 4, 11, 1, 4, 11, 3, 1, 11, 4, 6, 3, 8, 2, 8, 4,
		2, 2, 4, 6, -1, -1, -1, 2, 0, 4, 2, 4, 6, -1, -1, -1, -1, -1, -1, 1, 9,
		0, 3, 8, 2, 2, 8, 4, 2, 4, 6, 9, 4, 1, 1, 4, 2, 4, 6, 2, -1, -1, -1, 10,
		1, 2, 11, 8, 4, 11, 4, 6, -1, -1, -1, 10, 1, 2, 11, 3, 6, 6, 3, 0, 6, 0,
		4, 0, 2, 10, 0, 10, 9, 4, 11, 8, 4, 6, 11, 2, 11, 3, 6, 9, 4, 6, 10, 9,
		-1, -1, -1, 8, 4, 6, 8, 6, 1, 6, 10, 1, 8, 1, 3, 1, 0, 10, 10, 0, 6, 0,
		4, 6, -1, -1, -1, 8, 0, 3, 9, 6, 10, 9, 4, 6, -1, -1, -1, 10, 4, 6, 10,
		9, 4, -1, -1, -1, -1, -1, -1, 9, 5, 4, 7, 6, 11, -1, -1, -1, -1, -1, -1,
		4, 9, 5, 3, 0, 8, 11, 7, 6, -1, -1, -1, 6, 11, 7, 4, 0, 1, 4, 1, 5, -1,
		-1, -1, 6, 11, 7, 4, 8, 5, 5, 8, 3, 5, 3, 1, 4, 9, 5, 6, 2, 3, 6, 3, 7,
		-1, -1, -1, 9, 5, 4, 8, 7, 0, 0, 7, 6, 0, 6, 2, 4, 0, 1, 4, 1, 5, 6, 3,
		7, 6, 2, 3, 7, 4, 8, 5, 2, 1, 5, 6, 2, -1, -1, -1, 6, 11, 7, 1, 2, 10,
		9, 5, 4, -1, -1, -1, 11, 7, 6, 8, 3, 0, 1, 2, 10, 9, 5, 4, 11, 7, 6, 10,
		5, 2, 2, 5, 4, 2, 4, 0, 7, 4, 8, 2, 11, 3, 10, 5, 6, -1, -1, -1, 4, 9,
		5, 6, 10, 7, 7, 10, 1, 7, 1, 3, 5, 6, 10, 0, 9, 1, 8, 7, 4, -1, -1, -1,
		5, 6, 10, 7, 0, 3, 7, 4, 0, -1, -1, -1, 10, 5, 6, 4, 8, 7, -1, -1, -1,
		-1, -1, -1, 5, 6, 9, 6, 11, 9, 9, 11, 8, -1, -1, -1, 0, 9, 5, 0, 5, 3,
		3, 5, 6, 3, 6, 11, 0, 1, 5, 0, 5, 11, 5, 6, 11, 0, 11, 8, 11, 3, 6, 6,
		3, 5, 3, 1, 5, -1, -1, -1, 9, 5, 6, 3, 9, 6, 3, 8, 9, 3, 6, 2, 5, 6, 9,
		9, 6, 0, 6, 2, 0, -1, -1, -1, 0, 3, 8, 2, 5, 6, 2, 1, 5, -1, -1, -1, 1,
		6, 2, 1, 5, 6, -1, -1, -1, -1, -1, -1, 1, 2, 10, 5, 6, 9, 9, 6, 11, 9,
		11, 8, 1, 0, 9, 6, 10, 5, 11, 3, 2, -1, -1, -1, 6, 10, 5, 2, 8, 0, 2,
		11, 8, -1, -1, -1, 3, 2, 11, 10, 5, 6, -1, -1, -1, -1, -1, -1, 10, 5, 6,
		9, 3, 8, 9, 1, 3, -1, -1, -1, 0, 9, 1, 5, 6, 10, -1, -1, -1, -1, -1, -1,
		8, 0, 3, 10, 5, 6, -1, -1, -1, -1, -1, -1, 10, 5, 6, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, 3, 0,
		10, 6, 5, -1, -1, -1, -1, -1, -1, 0, 1, 9, 5, 10, 6, -1, -1, -1, -1, -1,
		-1, 10, 6, 5, 9, 8, 3, 9, 3, 1, -1, -1, -1, 3, 11, 2, 10, 6, 5, -1, -1,
		-1, -1, -1, -1, 6, 5, 10, 2, 0, 8, 2, 8, 11, -1, -1, -1, 1, 9, 0, 6, 5,
		10, 11, 2, 3, -1, -1, -1, 1, 10, 2, 5, 9, 6, 9, 11, 6, 9, 8, 11, 1, 2,
		6, 1, 6, 5, -1, -1, -1, -1, -1, -1, 0, 8, 3, 2, 6, 5, 2, 5, 1, -1, -1,
		-1, 5, 9, 6, 9, 0, 6, 6, 0, 2, -1, -1, -1, 9, 6, 5, 3, 6, 9, 3, 9, 8, 3,
		2, 6, 11, 6, 3, 6, 5, 3, 3, 5, 1, -1, -1, -1, 0, 5, 1, 0, 11, 5, 5, 11,
		6, 0, 8, 11, 0, 5, 9, 0, 3, 5, 3, 6, 5, 3, 11, 6, 5, 9, 6, 6, 9, 11, 9,
		8, 11, -1, -1, -1, 10, 6, 5, 4, 7, 8, -1, -1, -1, -1, -1, -1, 5, 10, 6,
		7, 3, 0, 7, 0, 4, -1, -1, -1, 5, 10, 6, 0, 1, 9, 8, 4, 7, -1, -1, -1, 4,
		5, 9, 6, 7, 10, 7, 1, 10, 7, 3, 1, 7, 8, 4, 2, 3, 11, 10, 6, 5, -1, -1,
		-1, 11, 6, 7, 10, 2, 5, 2, 4, 5, 2, 0, 4, 11, 6, 7, 8, 0, 3, 1, 10, 2,
		9, 4, 5, 6, 7, 11, 1, 10, 2, 9, 4, 5, -1, -1, -1, 7, 8, 4, 5, 1, 2, 5,
		2, 6, -1, -1, -1, 4, 1, 0, 4, 5, 1, 6, 7, 3, 6, 3, 2, 9, 4, 5, 8, 0, 7,
		0, 6, 7, 0, 2, 6, 4, 5, 9, 6, 3, 2, 6, 7, 3, -1, -1, -1, 6, 7, 11, 4, 5,
		8, 5, 3, 8, 5, 1, 3, 6, 7, 11, 4, 1, 0, 4, 5, 1, -1, -1, -1, 4, 5, 9, 3,
		8, 0, 11, 6, 7, -1, -1, -1, 9, 4, 5, 7, 11, 6, -1, -1, -1, -1, -1, -1,
		10, 6, 4, 10, 4, 9, -1, -1, -1, -1, -1, -1, 8, 3, 0, 9, 10, 6, 9, 6, 4,
		-1, -1, -1, 1, 10, 0, 10, 6, 0, 0, 6, 4, -1, -1, -1, 8, 6, 4, 8, 1, 6,
		6, 1, 10, 8, 3, 1, 2, 3, 11, 6, 4, 9, 6, 9, 10, -1, -1, -1, 0, 10, 2, 0,
		9, 10, 4, 8, 11, 4, 11, 6, 10, 2, 1, 11, 6, 3, 6, 0, 3, 6, 4, 0, 10, 2,
		1, 11, 4, 8, 11, 6, 4, -1, -1, -1, 9, 1, 4, 1, 2, 4, 4, 2, 6, -1, -1,
		-1, 1, 0, 9, 3, 2, 8, 2, 4, 8, 2, 6, 4, 2, 4, 0, 2, 6, 4, -1, -1, -1,
		-1, -1, -1, 3, 2, 8, 8, 2, 4, 2, 6, 4, -1, -1, -1, 1, 4, 9, 11, 4, 1,
		11, 1, 3, 11, 6, 4, 0, 9, 1, 4, 11, 6, 4, 8, 11, -1, -1, -1, 11, 6, 3,
		3, 6, 0, 6, 4, 0, -1, -1, -1, 8, 6, 4, 8, 11, 6, -1, -1, -1, -1, -1, -1,
		6, 7, 10, 7, 8, 10, 10, 8, 9, -1, -1, -1, 9, 3, 0, 6, 3, 9, 6, 9, 10, 6,
		7, 3, 6, 1, 10, 6, 7, 1, 7, 0, 1, 7, 8, 0, 6, 7, 10, 10, 7, 1, 7, 3, 1,
		-1, -1, -1, 7, 11, 6, 3, 8, 2, 8, 10, 2, 8, 9, 10, 11, 6, 7, 10, 0, 9,
		10, 2, 0, -1, -1, -1, 2, 1, 10, 7, 11, 6, 8, 0, 3, -1, -1, -1, 1, 10, 2,
		6, 7, 11, -1, -1, -1, -1, -1, -1, 7, 2, 6, 7, 9, 2, 2, 9, 1, 7, 8, 9, 1,
		0, 9, 3, 6, 7, 3, 2, 6, -1, -1, -1, 8, 0, 7, 7, 0, 6, 0, 2, 6, -1, -1,
		-1, 2, 7, 3, 2, 6, 7, -1, -1, -1, -1, -1, -1, 7, 11, 6, 3, 9, 1, 3, 8,
		9, -1, -1, -1, 9, 1, 0, 11, 6, 7, -1, -1, -1, -1, -1, -1, 0, 3, 8, 11,
		6, 7, -1, -1, -1, -1, -1, -1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, 11, 7, 5, 11, 5, 10, -1, -1, -1, -1, -1, -1, 3, 0, 8, 7, 5, 10, 7,
		10, 11, -1, -1, -1, 9, 0, 1, 10, 11, 7, 10, 7, 5, -1, -1, -1, 3, 1, 9,
		3, 9, 8, 7, 10, 11, 7, 5, 10, 10, 2, 5, 2, 3, 5, 5, 3, 7, -1, -1, -1, 5,
		10, 2, 8, 5, 2, 8, 7, 5, 8, 2, 0, 9, 0, 1, 10, 2, 5, 5, 2, 3, 5, 3, 7,
		1, 10, 2, 5, 8, 7, 5, 9, 8, -1, -1, -1, 2, 11, 1, 11, 7, 1, 1, 7, 5, -1,
		-1, -1, 0, 8, 3, 2, 11, 1, 1, 11, 7, 1, 7, 5, 9, 0, 2, 9, 2, 7, 2, 11,
		7, 9, 7, 5, 11, 3, 2, 8, 5, 9, 8, 7, 5, -1, -1, -1, 1, 3, 7, 1, 7, 5,
		-1, -1, -1, -1, -1, -1, 8, 7, 0, 0, 7, 1, 7, 5, 1, -1, -1, -1, 0, 3, 9,
		9, 3, 5, 3, 7, 5, -1, -1, -1, 9, 7, 5, 9, 8, 7, -1, -1, -1, -1, -1, -1,
		4, 5, 8, 5, 10, 8, 8, 10, 11, -1, -1, -1, 3, 0, 4, 3, 4, 10, 4, 5, 10,
		3, 10, 11, 0, 1, 9, 4, 5, 8, 8, 5, 10, 8, 10, 11, 5, 9, 4, 1, 11, 3, 1,
		10, 11, -1, -1, -1, 3, 8, 4, 3, 4, 2, 2, 4, 5, 2, 5, 10, 10, 2, 5, 5, 2,
		4, 2, 0, 4, -1, -1, -1, 0, 3, 8, 5, 9, 4, 10, 2, 1, -1, -1, -1, 2, 1,
		10, 9, 4, 5, -1, -1, -1, -1, -1, -1, 8, 4, 5, 2, 8, 5, 2, 11, 8, 2, 5,
		1, 3, 2, 11, 1, 4, 5, 1, 0, 4, -1, -1, -1, 9, 4, 5, 8, 2, 11, 8, 0, 2,
		-1, -1, -1, 11, 3, 2, 9, 4, 5, -1, -1, -1, -1, -1, -1, 4, 5, 8, 8, 5, 3,
		5, 1, 3, -1, -1, -1, 5, 0, 4, 5, 1, 0, -1, -1, -1, -1, -1, -1, 3, 8, 0,
		4, 5, 9, -1, -1, -1, -1, -1, -1, 9, 4, 5, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, 7, 4, 11, 4, 9, 11, 11, 9, 10, -1, -1, -1, 3, 0, 8, 7, 4, 11,
		11, 4, 9, 11, 9, 10, 11, 7, 4, 1, 11, 4, 1, 10, 11, 1, 4, 0, 8, 7, 4,
		11, 1, 10, 11, 3, 1, -1, -1, -1, 2, 3, 7, 2, 7, 9, 7, 4, 9, 2, 9, 10, 4,
		8, 7, 0, 10, 2, 0, 9, 10, -1, -1, -1, 2, 1, 10, 0, 7, 4, 0, 3, 7, -1,
		-1, -1, 10, 2, 1, 8, 7, 4, -1, -1, -1, -1, -1, -1, 2, 11, 7, 2, 7, 1, 1,
		7, 4, 1, 4, 9, 3, 2, 11, 4, 8, 7, 9, 1, 0, -1, -1, -1, 7, 4, 11, 11, 4,
		2, 4, 0, 2, -1, -1, -1, 2, 11, 3, 7, 4, 8, -1, -1, -1, -1, -1, -1, 9, 1,
		4, 4, 1, 7, 1, 3, 7, -1, -1, -1, 1, 0, 9, 8, 7, 4, -1, -1, -1, -1, -1,
		-1, 3, 4, 0, 3, 7, 4, -1, -1, -1, -1, -1, -1, 8, 7, 4, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, 8, 9, 10, 8, 10, 11, -1, -1, -1, -1, -1, -1, 0, 9,
		3, 3, 9, 11, 9, 10, 11, -1, -1, -1, 1, 10, 0, 0, 10, 8, 10, 11, 8, -1,
		-1, -1, 10, 3, 1, 10, 11, 3, -1, -1, -1, -1, -1, -1, 3, 8, 2, 2, 8, 10,
		8, 9, 10, -1, -1, -1, 9, 2, 0, 9, 10, 2, -1, -1, -1, -1, -1, -1, 8, 0,
		3, 1, 10, 2, -1, -1, -1, -1, -1, -1, 10, 2, 1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, 2, 11, 1, 1, 11, 9, 11, 8, 9, -1, -1, -1, 11, 3, 2, 0, 9, 1,
		-1, -1, -1, -1, -1, -1, 11, 0, 2, 11, 8, 0, -1, -1, -1, -1, -1, -1, 11,
		3, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, 1, 3, 8, 9, 1, -1, -1, -1,
		-1, -1, -1, 9, 1, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, 8, 0, 3, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1 };

const static uint16_t edge[256] = { 0, 265, 515, 778, 2060, 2309, 2575, 2822,
		1030, 1295, 1541, 1804, 3082, 3331, 3593, 3840, 400, 153, 915, 666,
		2460, 2197, 2975, 2710, 1430, 1183, 1941, 1692, 3482, 3219, 3993, 3728,
		560, 825, 51, 314, 2620, 2869, 2111, 2358, 1590, 1855, 1077, 1340, 3642,
		3891, 3129, 3376, 928, 681, 419, 170, 2988, 2725, 2479, 2214, 1958,
		1711, 1445, 1196, 4010, 3747, 3497, 3232, 2240, 2505, 2755, 3018, 204,
		453, 719, 966, 3270, 3535, 3781, 4044, 1226, 1475, 1737, 1984, 2384,
		2137, 2899, 2650, 348, 85, 863, 598, 3414, 3167, 3925, 3676, 1370, 1107,
		1881, 1616, 2800, 3065, 2291, 2554, 764, 1013, 255, 502, 3830, 4095,
		3317, 3580, 1786, 2035, 1273, 1520, 2912, 2665, 2403, 2154, 876, 613,
		367, 102, 3942, 3695, 3429, 3180, 1898, 1635, 1385, 1120, 1120, 1385,
		1635, 1898, 3180, 3429, 3695, 3942, 102, 367, 613, 876, 2154, 2403,
		2665, 2912, 1520, 1273, 2035, 1786, 3580, 3317, 4095, 3830, 502, 255,
		1013, 764, 2554, 2291, 3065, 2800, 1616, 1881, 1107, 1370, 3676, 3925,
		3167, 3414, 598, 863, 85, 348, 2650, 2899, 2137, 2384, 1984, 1737, 1475,
		1226, 4044, 3781, 3535, 3270, 966, 719, 453, 204, 3018, 2755, 2505,
		2240, 3232, 3497, 3747, 4010, 1196, 1445, 1711, 1958, 2214, 2479, 2725,
		2988, 170, 419, 681, 928, 3376, 3129, 3891, 3642, 1340, 1077, 1855,
		1590, 2358, 2111, 2869, 2620, 314, 51, 825, 560, 3728, 3993, 3219, 3482,
		1692, 1941, 1183, 1430, 2710, 2975, 2197, 2460, 666, 915, 153, 400,
		3840, 3593, 3331, 3082, 1804, 1541, 1295, 1030, 2822, 2575, 2309, 2060,
		778, 515, 265, 0 };

const static int8_t cap[144] = { -1, -1, -1, -1, -1, -1, -1, -1, -1, 7, 1, 0,
		-1, -1, -1, -1, -1, -1, 3, 2, 1, -1, -1, -1, -1, -1, -1, 3, 2, 0, 7, 3,
		0, -1, -1, -1, 5, 4, 3, -1, -1, -1, -1, -1, -1, 7, 1, 0, 5, 4, 3, -1,
		-1, -1, 5, 2, 1, 5, 4, 2, -1, -1, -1, 0, 7, 2, 5, 4, 2, 7, 5, 2, 6, 5,
		7, -1, -1, -1, -1, -1, -1, 6, 1, 0, 6, 5, 1, -1, -1, -1, 3, 2, 1, 7, 6,
		5, -1, -1, -1, 3, 2, 0, 5, 3, 0, 6, 5, 0, 4, 3, 7, 6, 4, 7, -1, -1, -1,
		1, 0, 6, 3, 1, 6, 4, 3, 6, 2, 1, 4, 1, 7, 4, 7, 6, 4, 4, 2, 0, 6, 4, 0,
		-1, -1, -1 };

void Volume::CalcSurface(void) {
//	setlocale(LC_ALL, "C"); // To get a 3.1415 instead 3,1415 or else on every computer.
//	wxTextFile temp(_T("~/octave/oneslice.m"));
//	if(!temp.Exists())
//		temp.Create();
//	else
//		temp.Open();
//	temp.Clear();
//	temp.AddLine(_T("M=[ ..."));

	const size_t Nx = Size(0);
	const size_t Ny = Size(1);
	const size_t Nz = Size(2);

	geometry.Clear();
	Vector3 p(0, 0, 0);
	Vector3 p0;
	Vector3 p1;
	Vector3 p2;
	Vector3 p3;
	Vector3 p4;
	Vector3 p5;
	Vector3 p6;
	Vector3 p7;
	Vector3 p8;
	Vector3 p9;
	Vector3 p10;
	Vector3 p11;
	Triangle t;
	unsigned int c = 0;
	for (size_t k = 0; k < Nz - 1; ++k) {
		for (size_t j = 0; j < Ny - 1; ++j) {
			for (size_t i = 0; i < Nx - 1; ++i) {
//				if(k == 60) temp.AddLine(
//						wxString::Format(_T("%.3f,..."), values[c]));

				const double v0 = values[c];
				const double v1 = values[c + 1];
				const double v2 = values[c + Nx];
				const double v3 = values[c + 1 + Nx];
				const double v4 = values[c + Nx * Ny];
				const double v5 = values[c + 1 + Nx * Ny];
				const double v6 = values[c + Nx * (1 + Ny)];
				const double v7 = values[c + 1 + Nx * (1 + Ny)];
				uint8_t v = 0;
				if (v0 > surface)
					v |= 1;
				if (v1 > surface)
					v |= 2;
				if (v2 > surface)
					v |= 4;
				if (v3 > surface)
					v |= 8;
				if (v4 > surface)
					v |= 16;
				if (v5 > surface)
					v |= 32;
				if (v6 > surface)
					v |= 64;
				if (v7 > surface)
					v |= 128;
				if (v != 0 && v != 255) {

					uint16_t h = edge[v];
					if (h & ((uint16_t) 1 << 0)) {
						const float x = (surface - v0) / (v1 - v0);
						p0.x = p.x;
						p0.y = p.y;
						p0.z = p.z;
						p0.x += dx * x;
					}
					if (h & ((uint16_t) 1 << 1)) {
						const float x = (surface - v1) / (v3 - v1);
						p1.x = p.x + dx;
						p1.y = p.y;
						p1.z = p.z;
						p1.y += dy * x;
					}
					if (h & ((uint16_t) 1 << 2)) {
						const float x = (surface - v2) / (v3 - v2);
						p2.x = p.x;
						p2.y = p.y + dy;
						p2.z = p.z;
						p2.x += dx * x;
					}
					if (h & ((uint16_t) 1 << 3)) {
						const float x = (surface - v0) / (v2 - v0);
						p3.x = p.x;
						p3.y = p.y;
						p3.z = p.z;
						p3.y += dy * x;
					}
					if (h & ((uint16_t) 1 << 4)) {
						const float x = (surface - v4) / (v5 - v4);
						p4.x = p.x;
						p4.y = p.y;
						p4.z = p.z + dz;
						p4.x += dx * x;
					}
					if (h & ((uint16_t) 1 << 5)) {
						const float x = (surface - v5) / (v7 - v5);
						p5.x = p.x + dx;
						p5.y = p.y;
						p5.z = p.z + dz;
						p5.y += dy * x;
					}
					if (h & ((uint16_t) 1 << 6)) {
						const float x = (surface - v6) / (v7 - v6);
						p6.x = p.x;
						p6.y = p.y + dy;
						p6.z = p.z + dz;
						p6.x += dx * x;
					}
					if (h & ((uint16_t) 1 << 7)) {
						const float x = (surface - v4) / (v6 - v4);
						p7.x = p.x;
						p7.y = p.y;
						p7.z = p.z + dz;
						p7.y += dy * x;
					}
					if (h & ((uint16_t) 1 << 8)) {
						const float x = (surface - v0) / (v4 - v0);
						p8.x = p.x;
						p8.y = p.y;
						p8.z = p.z;
						p8.z += dz * x;
					}
					if (h & ((uint16_t) 1 << 9)) {
						const float x = (surface - v1) / (v5 - v1);
						p9.x = p.x + dx;
						p9.y = p.y;
						p9.z = p.z;
						p9.z += dz * x;
					}
					if (h & ((uint16_t) 1 << 10)) {
						const float x = (surface - v3) / (v7 - v3);
						p10.x = p.x + dx;
						p10.y = p.y + dy;
						p10.z = p.z;
						p10.z += dz * x;
					}
					if (h & ((uint16_t) 1 << 11)) {
						const float x = (surface - v2) / (v6 - v2);
						p11.x = p.x;
						p11.y = p.y + dy;
						p11.z = p.z;
						p11.z += dz * x;
					}

					uint16_t h2 = v * 12;
					for (uint_fast8_t n = 0; n < 12; n++) {
						if (tri[h2] == -1)
							break;
						switch (tri[h2]) {
						case 0:
							t.p[2 - (n % 3)] = p0;
							break;
						case 1:
							t.p[2 - (n % 3)] = p1;
							break;
						case 2:
							t.p[2 - (n % 3)] = p2;
							break;
						case 3:
							t.p[2 - (n % 3)] = p3;
							break;
						case 4:
							t.p[2 - (n % 3)] = p4;
							break;
						case 5:
							t.p[2 - (n % 3)] = p5;
							break;
						case 6:
							t.p[2 - (n % 3)] = p6;
							break;
						case 7:
							t.p[2 - (n % 3)] = p7;
							break;
						case 8:
							t.p[2 - (n % 3)] = p8;
							break;
						case 9:
							t.p[2 - (n % 3)] = p9;
							break;
						case 10:
							t.p[2 - (n % 3)] = p10;
							break;
						case 11:
							t.p[2 - (n % 3)] = p11;
							break;
						}

//					if(t.p[2 - (n % 3)].Abs() < 0.001){
//						break;
//					}

						if (n % 3 == 2) {
							const size_t idx0 = geometry.VertexCount();
							geometry.AddVertex(t.p[0]);
							const size_t idx1 = geometry.VertexCount();
							geometry.AddVertex(t.p[1]);
							const size_t idx2 = geometry.VertexCount();
							geometry.AddVertex(t.p[2]);
							geometry.AddTriangle(idx0, idx1, idx2);
						}
						h2++;
					}
				}
				/*
				 if(i == 0){
				 uint8_t v = 0;
				 if(v0 > surface) v |= 1;
				 if(v2 > surface) v |= 2;
				 if(v6 > surface) v |= 4;
				 if(v4 > surface) v |= 8;
				 if(v > 0){
				 uint8_t h = v * 9;
				 for(uint_fast8_t n = 0; n < 9; n++){
				 if(cap[h] == -1) break;
				 switch(cap[h]){
				 case 0:
				 t.p[n % 3] = p;
				 break;
				 case 1:
				 t.p[n % 3] = p3;
				 break;
				 case 2:
				 t.p[n % 3].Set(p.x, p.y + dy, p.z);
				 break;
				 case 3:
				 t.p[n % 3] = p11;
				 break;
				 case 4:
				 t.p[n % 3].Set(p.x, p.y + dy, p.z + dz);
				 break;
				 case 5:
				 t.p[n % 3] = p7;
				 break;
				 case 6:
				 t.p[n % 3].Set(p.x, p.y, p.z + dz);
				 break;
				 case 7:
				 t.p[n % 3] = p8;
				 break;
				 }
				 if(n % 3 == 2) geometry.AddTriangle(t, false);
				 h++;
				 }

				 }

				 }

				 if(i + 2 == Nx){
				 uint8_t v = 0;
				 if(v1 > surface) v |= 1;
				 if(v5 > surface) v |= 2;
				 if(v7 > surface) v |= 4;
				 if(v3 > surface) v |= 8;
				 if(v > 0){
				 uint8_t h = v * 9;
				 for(uint_fast8_t n = 0; n < 9; n++){
				 if(cap[h] == -1) break;
				 switch(cap[h]){
				 case 0:
				 t.p[n % 3].Set(p.x + dx, p.y, p.z);
				 break;
				 case 1:
				 t.p[n % 3] = p9;
				 break;
				 case 2:
				 t.p[n % 3].Set(p.x + dx, p.y, p.z + dz);
				 break;
				 case 3:
				 t.p[n % 3] = p5;
				 break;
				 case 4:
				 t.p[n % 3].Set(p.x + dx, p.y + dy, p.z + dz);
				 break;
				 case 5:
				 t.p[n % 3] = p10;
				 break;
				 case 6:
				 t.p[n % 3].Set(p.x + dx, p.y + dy, p.z);
				 break;
				 case 7:
				 t.p[n % 3] = p1;
				 break;
				 }
				 if(n % 3 == 2) geometry.AddTriangle(t, false);
				 h++;
				 }

				 }

				 }

				 if(j == 0){
				 uint8_t v = 0;
				 if(v0 > surface) v |= 1;
				 if(v4 > surface) v |= 2;
				 if(v5 > surface) v |= 4;
				 if(v1 > surface) v |= 8;
				 if(v > 0){
				 uint8_t h = v * 9;
				 for(uint_fast8_t n = 0; n < 9; n++){
				 if(cap[h] == -1) break;
				 switch(cap[h]){
				 case 0:
				 t.p[n % 3] = p;
				 break;
				 case 1:
				 t.p[n % 3] = p8;
				 break;
				 case 2:
				 t.p[n % 3].Set(p.x, p.y, p.z + dz);
				 break;
				 case 3:
				 t.p[n % 3] = p4;
				 break;
				 case 4:
				 t.p[n % 3].Set(p.x + dx, p.y, p.z + dz);
				 break;
				 case 5:
				 t.p[n % 3] = p9;
				 break;
				 case 6:
				 t.p[n % 3].Set(p.x + dx, p.y, p.z);
				 break;
				 case 7:
				 t.p[n % 3] = p0;
				 break;
				 }
				 if(n % 3 == 2) geometry.AddTriangle(t, false);
				 h++;
				 }

				 }

				 }

				 if(j + 2 == Ny){
				 uint8_t v = 0;
				 if(v2 > surface) v |= 1;
				 if(v3 > surface) v |= 2;
				 if(v7 > surface) v |= 4;
				 if(v6 > surface) v |= 8;
				 if(v > 0){
				 uint8_t h = v * 9;
				 for(uint_fast8_t n = 0; n < 9; n++){
				 if(cap[h] == -1) break;
				 switch(cap[h]){
				 case 0:
				 t.p[n % 3].Set(p.x, p.y + dy, p.z);
				 break;
				 case 1:
				 t.p[n % 3] = p2;
				 break;
				 case 2:
				 t.p[n % 3].Set(p.x + dx, p.y + dy, p.z);
				 break;
				 case 3:
				 t.p[n % 3] = p10;
				 break;
				 case 4:
				 t.p[n % 3].Set(p.x + dx, p.y + dy, p.z + dz);
				 break;
				 case 5:
				 t.p[n % 3] = p6;
				 break;
				 case 6:
				 t.p[n % 3].Set(p.x, p.y + dy, p.z + dz);
				 break;
				 case 7:
				 t.p[n % 3] = p11;
				 break;
				 }
				 if(n % 3 == 2) geometry.AddTriangle(t, false);
				 h++;
				 }

				 }

				 }

				 if(k == 0){
				 uint8_t v = 0;
				 if(v0 > surface) v |= 1;
				 if(v1 > surface) v |= 2;
				 if(v3 > surface) v |= 4;
				 if(v2 > surface) v |= 8;
				 if(v > 0){
				 uint8_t h = v * 9;
				 for(uint_fast8_t n = 0; n < 9; n++){
				 if(cap[h] == -1) break;
				 switch(cap[h]){
				 case 0:
				 t.p[n % 3] = p;
				 break;
				 case 1:
				 t.p[n % 3] = p0;
				 break;
				 case 2:
				 t.p[n % 3].Set(p.x + dx, p.y, p.z);
				 break;
				 case 3:
				 t.p[n % 3] = p1;
				 break;
				 case 4:
				 t.p[n % 3].Set(p.x + dx, p.y + dy, p.z);
				 break;
				 case 5:
				 t.p[n % 3] = p2;
				 break;
				 case 6:
				 t.p[n % 3].Set(p.x, p.y + dy, p.z);
				 break;
				 case 7:
				 t.p[n % 3] = p3;
				 break;
				 }
				 if(n % 3 == 2) geometry.AddTriangle(t, false);
				 h++;
				 }

				 }

				 }

				 if(k + 2 == Nz){
				 uint8_t v = 0;
				 if(v4 > surface) v |= 1;
				 if(v6 > surface) v |= 2;
				 if(v7 > surface) v |= 4;
				 if(v5 > surface) v |= 8;
				 if(v > 0){
				 uint8_t h = v * 9;
				 for(uint_fast8_t n = 0; n < 9; n++){
				 if(cap[h] == -1) break;
				 switch(cap[h]){
				 case 0:
				 t.p[n % 3].Set(p.x, p.y, p.z + dz);
				 break;
				 case 1:
				 t.p[n % 3] = p7;
				 break;
				 case 2:
				 t.p[n % 3].Set(p.x, p.y + dy, p.z + dz);
				 break;
				 case 3:
				 t.p[n % 3] = p6;
				 break;
				 case 4:
				 t.p[n % 3].Set(p.x + dx, p.y + dy, p.z + dz);
				 break;
				 case 5:
				 t.p[n % 3] = p5;
				 break;
				 case 6:
				 t.p[n % 3].Set(p.x + dx, p.y, p.z + dz);
				 break;
				 case 7:
				 t.p[n % 3] = p4;
				 break;
				 }
				 if(n % 3 == 2) geometry.AddTriangle(t, false);
				 h++;
				 }

				 }

				 }
				 */
				p.x += dx;
				c++;
			}
			c++;
			p.x = 0;
			p.y += dy;
		}
		c += Nx;
		p.y = 0;
		p.z += dz;
	}
//	for(n = 0; n < triangles.GetCount(); n++)
//		triangles[n].CalculateNormal();
//	temp.AddLine(_T("];"));
//	temp.AddLine(_T("M=reshape(M,149,[]);"));
//	temp.AddLine(_T("%M=M-min(min(M));"));
//	temp.AddLine(_T("%M=M/max(max(M));"));
//	temp.AddLine(
//			_T(
//					"[X,Y] = ndgrid(linspace(0,0.3,149)-0.075,linspace(0,0.2,99)-0.075);"));
//	temp.Write();
//	setlocale(LC_ALL, "");

	update = true;
}

void Volume::PaintSurface(void) const {
	glPushMatrix();
	glTranslated(origin.x, origin.y, origin.z);
	if (m_gllist == 0) {
		m_gllist = glGenLists(1);
		update = true;
	}
	if (update) {
		glNewList(m_gllist, GL_COMPILE_AND_EXECUTE);
		geometry.Paint();
		glEndList();
		update = false;
	} else {
		glCallList(m_gllist);
	}
	glPopMatrix();
}

Vector3 Volume::GetSurface(const Vector3 &p0, const Vector3 &n) const {
	double d0 = 0;
	double d1 = 1;
	double d = 0.5;
	for (uint_fast8_t c = 0; c < 50; c++) {
		const double v0 = GetValue(p0 + n * d0);
		const double v1 = GetValue(p0 + n * d1);
		if (fabs(v0 - v1) < 1e-5)
			break;
		d = (d0 * v1 - d1 * v0 + (d1 - d0) * surface) / (v1 - v0);
		const double v = GetValue(p0 + n * d);
		if (fabs(v - surface) < 1e-5)
			break;
		if (v > surface) {
			d0 = d;
		} else {
			d1 = d;
		}
	}
	return (p0 + n * d);
}


///////////////////////////////////////////////////////////////////////////////
// Name               : Spline3.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : toby
// Created            : 28.01.2017
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
#include "Spline3.h"

#include <math.h>

#include "OpenGL.h"

Spline3::Spline3() {
	color.Set(1.0, 1.0, 1.0);
	closed = false;
	normalized = true;
	symmetric = false;
	pointsize = 5;
	length = 1.0;
}

void Spline3::Clear() {
	points.clear();
}

void Spline3::AddVector(const Vector3 &vector, bool corner) {
	Point temp;
	temp.Set(vector.x, vector.y, vector.z);
	temp.corner = corner;
	points.push_back(temp);
}

void Spline3::AddVector(float x, float y, float z, bool corner) {
	Point temp;
	temp.Set(x, y, z);
	temp.corner = corner;
	points.push_back(temp);
}

void Spline3::Close(bool close) {
	this->closed = close;
}

void Spline3::Symmetric(bool symmetric) {
	this->symmetric = symmetric;
}

void Spline3::Normalize(bool normalize) {
	this->normalized = normalize;
}

void Spline3::Calculate() {
	const size_t N = points.size();
	if (N == 0)
		return;
	points[0].r0 = 0.0;
	if (N == 1) {
		points[0].length = 0.0;
		points[0].px = Polynomial::ByValue(points[0].r0, points[0].x);
		points[0].py = Polynomial::ByValue(points[0].r0, points[0].y);
		points[0].pz = Polynomial::ByValue(points[0].r0, points[0].z);
		return;
	}
	for (size_t n = 0; n < N - 1; n++)
		points[n].length = (points[n + 1] - points[n]).Abs();
	points[N - 1].length = (points[0] - points[N - 1]).Abs();

	if (normalized) {
		double L = 0.0;
		for (size_t n = 0; n < N; n++)
			L += points[n].length;
		if (closed)
			L *= 2 * M_PI;
		for (size_t n = 0; n < N; n++)
			points[n].length /= L;
	}

	if (symmetric && N > 2) {
		for (size_t n = 0; n < ((N - 1) / 2); n++)
			points[n + 1].r0 = points[n].r0 + points[n].length;
		points[N - 1].r0 = points[0].r0 - points[N - 1].length;
		for (size_t n = (N - 2); n > ((N - 1) / 2); n--)
			points[n].r0 = points[n + 1].r0 - points[n].length;
	} else {
		for (size_t n = 0; n < N - 1; n++)
			points[n + 1].r0 = points[n].r0 + points[n].length;
	}
	if (N == 2) {
		points[0].px = Polynomial::ByValue(points[0].r0, points[0].x,
				points[1].r0, points[1].x);
		points[0].py = Polynomial::ByValue(points[0].r0, points[0].y,
				points[1].r0, points[1].y);
		points[0].pz = Polynomial::ByValue(points[0].r0, points[0].z,
				points[1].r0, points[1].z);
		if (closed) {
			if (symmetric) {
				points[1].px = Polynomial::ByValue(
						points[0].r0 - points[1].length, points[1].x,
						points[0].r0, points[0].x);
				points[1].py = Polynomial::ByValue(
						points[0].r0 - points[1].length, points[1].y,
						points[0].r0, points[0].y);
				points[1].pz = Polynomial::ByValue(
						points[0].r0 - points[1].length, points[1].z,
						points[0].r0, points[0].z);
			} else {
				points[1].px = Polynomial::ByValue(points[1].r0, points[1].x,
						points[1].r0 + points[1].length, points[0].x);
				points[1].py = Polynomial::ByValue(points[1].r0, points[1].y,
						points[1].r0 + points[1].length, points[0].y);
				points[1].pz = Polynomial::ByValue(points[1].r0, points[1].z,
						points[1].r0 + points[1].length, points[0].z);
			}
		} else {
			points[1].px = Polynomial::ByValue(points[1].r0, points[1].x);
			points[1].py = Polynomial::ByValue(points[1].r0, points[1].y);
			points[1].pz = Polynomial::ByValue(points[1].r0, points[1].z);
		}
		return;
	}

//	printf("\n");
//	for(size_t n = 0; n < N; n++){
//		printf("%6.3f  - %6.3f\n", points[n].r0, points[n].length);
//	}
//	printf("\n");

//	for(size_t n = 0; n < N; n++){
//		points[n].length = 1.0;
//	}

	{
		const double L0 = points[N - 2].length;
		const double L1 = points[N - 1].length;
		const double L2 = points[0].length;
		points[N - 1].dvx = (points[0].x - points[N - 2].x) / (L0 + L1);
		points[N - 1].dvy = (points[0].y - points[N - 2].y) / (L0 + L1);
		points[N - 1].dvz = (points[0].z - points[N - 2].z) / (L0 + L1);
		points[0].dvx = (points[1].x - points[N - 1].x) / (L1 + L2);
		points[0].dvy = (points[1].y - points[N - 1].y) / (L1 + L2);
		points[0].dvz = (points[1].z - points[N - 1].z) / (L1 + L2);
	}
	for (size_t n = 1; n < (N - 1); n++) {
		const double L0 = points[n - 1].length;
		const double L1 = points[n].length;
		points[n].dvx = (points[n + 1].x - points[n - 1].x) / (L0 + L1);
		points[n].dvy = (points[n + 1].y - points[n - 1].y) / (L0 + L1);
		points[n].dvz = (points[n + 1].z - points[n - 1].z) / (L0 + L1);
	}

	for (size_t n = 0; n < N; n++) {
		const size_t n1 = (n + 1) % N;
		const bool C0 = ((n == 0 || n == (N - 1)) && !closed)
				|| points[n].corner;
		const bool C1 = ((n1 == 0 || n1 == (N - 1)) && !closed)
				|| points[n1].corner;
		const double r0 = points[n].r0;
		const double r1 = points[n].r0 + points[n].length;

		if (C0 && C1) {
			points[n].px = Polynomial::ByValue(r0, points[n].x, r1,
					points[n1].x);
			points[n].py = Polynomial::ByValue(r0, points[n].y, r1,
					points[n1].y);
			points[n].pz = Polynomial::ByValue(r0, points[n].z, r1,
					points[n1].z);
		}
		if (C0 && !C1) {
			points[n].px = Polynomial::ByDerivative(r1, points[n1].x,
					points[n1].dvx, r0, points[n].x);
			points[n].py = Polynomial::ByDerivative(r1, points[n1].y,
					points[n1].dvy, r0, points[n].y);
			points[n].pz = Polynomial::ByDerivative(r1, points[n1].z,
					points[n1].dvz, r0, points[n].z);
		}
		if (!C0 && C1) {
			points[n].px = Polynomial::ByDerivative(r0, points[n].x,
					points[n].dvx, r1, points[n1].x);
			points[n].py = Polynomial::ByDerivative(r0, points[n].y,
					points[n].dvy, r1, points[n1].y);
			points[n].pz = Polynomial::ByDerivative(r0, points[n].z,
					points[n].dvz, r1, points[n1].z);
		}
		if (!C0 && !C1) {
			points[n].px = Polynomial::ByValue(r0, points[n].x, points[n].dvx,
					r1, points[n1].x, points[n1].dvx);
			points[n].py = Polynomial::ByValue(r0, points[n].y, points[n].dvy,
					r1, points[n1].y, points[n1].dvy);
			points[n].pz = Polynomial::ByValue(r0, points[n].z, points[n].dvz,
					r1, points[n1].z, points[n1].dvz);
		}
	}
}

Vector3 Spline3::Evaluate(double r) {
	Vector3 temp;
	const size_t N = points.size();
	size_t m = 0;
	if (symmetric)
		m = ((N - 1) / 2) + 1;
	for (size_t n = 0; n < N; n++) {
		throw(std::logic_error(__FILE__ "Not implemented."));
	}
	return temp;
}

void Spline3::Paint() const {
	{
		if (closed)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_LINE_STRIP);
		glNormal3f(0, 0, 1);
		glColor3f(0.4, 0.4, 0.0);
		size_t N = points.size();
		for (size_t i = 0; i < N; i++) {
			glVertex3f(points[i].x, points[i].y, points[i].z);
		}
		glEnd();
	}

	if (closed)
		glBegin(GL_LINE_LOOP);
	else
		glBegin(GL_LINE_STRIP);
	glNormal3f(0, 0, 1);
	glColor3f(color.x, color.y, color.z);
	const unsigned int Nd = 301;
	const size_t N = points.size() - (closed ? 0 : 1);
	for (size_t n = 0; n < N; n++) {
		const float delta = points[n].length / (float) Nd;
		float r = points[n].r0;
		for (size_t i = 0; i < Nd; i++) {
			glVertex3f(points[n].px(r), points[n].py(r), points[n].pz(r));
			r += delta;
		}
	}
	glEnd();
}

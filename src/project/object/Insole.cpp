///////////////////////////////////////////////////////////////////////////////
// Name               : Insole.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.09.2020
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

#include "Insole.h"

#include <cfloat>
#include <iostream>

#include "../../3D/Bender.h"
#include "../../math/DependentVector.h"
#include "../Configuration.h"
#include "../FootMeasurements.h"

#include "../../3D/OpenGL.h"

void Insole::Point::SetNormal(const Point &p0, const Point &p1) {
	n = (p1 - p0).Normal();
}

Insole::Point::Point(Vector3 p, double r) {
	x = p.x;
	y = p.y;
	z = p.z;
	rx = r;
}

void Insole::Point::Transform(std::function<Vector3(Vector3)> func) {
	// Scale the normals to be closer to the base point p.
	// The transformation function might be non-linear.
	const double scale = 0.001;
	Vector3 h = *this + n * scale;
	Vector3 temp = func(*this);
	x = temp.x;
	y = temp.y;
	z = temp.z;
	h = func(h);
	n = (h - temp).Normal();
}

void Insole::Line::Setup(const Point &p0, const Point &p1, double f0,
		double f1) {
	const double d = (p1 - p0).Abs();
	x = Polynomial::ByBezier(p0.x, p0.x + d * f0 * p0.n.x,
			p1.x - d * f1 * p1.n.x, p1.x);
	y = Polynomial::ByBezier(p0.y, p0.y + d * f0 * p0.n.y,
			p1.y - d * f1 * p1.n.y, p1.y);
	z = Polynomial::ByBezier(p0.z, p0.z + d * f0 * p0.n.z,
			p1.z - d * f1 * p1.n.z, p1.z);
	r0 = 0.0;
	r1 = 1.0;
}

bool Insole::Line::IsInside(const double r) const {
	return (r >= r0 && r <= r1);
}

void Insole::Line::Paint() const {
	glBegin(GL_LINE_STRIP);
	for (uint8_t n = 0; n <= 100; ++n) {
		const double r = (double) n * 0.01;
		glVertex3d(x(r), y(r), z(r));
	}
	glEnd();
}

void Insole::Transform(std::function<Vector3(Vector3)> func) {
	inside.Transform(func);
	inside.CalculateNormals();
	outside.Transform(func);
	outside.CalculateNormals();
	A.Transform(func);
	B.Transform(func);
	C.Transform(func);
	D.Transform(func);
	E.Transform(func);
	F.Transform(func);
	G.Transform(func);
	H.Transform(func);
	J.Transform(func);
	K.Transform(func);
	L.Transform(func);
	N.Transform(func);
	Z.Transform(func);
	Polygon3::Transform(func);
	this->CalculateNormals();
	for (auto &line : lines)
		line.Transform(func);
}

void Insole::Paint() const {
//	for(auto & line : lines)
//		line.Paint();

	size_t count = std::min(inside.Size(), outside.Size());

	glBegin(GL_QUAD_STRIP);
	for (size_t n = 0; n < count; ++n) {
//		if (leftside) {
		glNormal3f(inside.Normal(n).x, inside.Normal(n).y, inside.Normal(n).z);
		glVertex3f(inside[n].x, inside[n].y, inside[n].z);
		glNormal3f(outside.Normal(n).x, outside.Normal(n).y,
				outside.Normal(n).z);
		glVertex3f(outside[n].x, outside[n].y, outside[n].z);
//		} else {
//			glNormal3f(outside.Normal(n).x, outside.Normal(n).y, outside.Normal(n).z);
//			glVertex3f(outside[n].x, outside[n].y, outside[n].z);
//			glNormal3f(inside.Normal(n).x, inside.Normal(n).y, inside.Normal(n).z);
//			glVertex3f(inside[n].x, inside[n].y, inside[n].z);
//		}
	}
	glEnd();
	inside.Paint();
	outside.Paint();
}


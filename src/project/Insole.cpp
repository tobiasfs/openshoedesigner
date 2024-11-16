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
#include "FootMeasurements.h"
#include "Configuration.h"
#include "../3D/Bender.h"

#include "../3D/OpenGL.h"
#include "../math/DependentVector.h"

void Insole::Modify(bool modify) {
	this->modified = modify;
}

bool Insole::IsModified(void) const {
	return modified;
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
	for (auto &p : outline) {
		p.p = func(p.p);
	}
}

void Insole::Mirror(bool isleft) {
	if (leftside == isleft)
		return;
	AffineTransformMatrix m;
	m.ScaleGlobal(1, -1, 1);
	Transform(m);
	leftside = isleft;
}
void Insole::Point::SetNormal(const Point &p0, const Point &p1) {
	n = (p1.p - p0.p).Normal();
}

void Insole::Point::Transform(std::function<Vector3(Vector3)> func) {
	// Scale the normals to be closer to the base point p.
	// The transformation function might be non-linear.
	const double scale = 0.001;
	Vector3 h = p + n * scale;
	p = func(p);
	h = func(h);
	n = (h - p).Normal();
}

void Insole::Line::Setup(const Point &p0, const Point &p1, double f0,
		double f1) {
	const double d = (p1.p - p0.p).Abs();
	x = Polynomial::ByBezier(p0.p.x, p0.p.x + d * f0 * p0.n.x,
			p1.p.x - d * f1 * p1.n.x, p1.p.x);
	y = Polynomial::ByBezier(p0.p.y, p0.p.y + d * f0 * p0.n.y,
			p1.p.y - d * f1 * p1.n.y, p1.p.y);
	z = Polynomial::ByBezier(p0.p.z, p0.p.z + d * f0 * p0.n.z,
			p1.p.z - d * f1 * p1.n.z, p1.p.z);
	r0 = 0.0;
	r1 = 1.0;
}

bool Insole::Line::IsInside(const double r) const {
	return (r >= r0 && r <= r1);
}
void Insole::Line::Paint(void) const {
	glBegin(GL_LINE_STRIP);
	for (double r = -0.0; r < 1.0001; r += 0.01) {
		glVertex3d(x(r), y(r), z(r));
	}
	glEnd();
}

void Insole::Paint(void) const {
//	for(auto & line : lines)
//		line.Paint();

	size_t N = inside.Size();
	if (outside.Size() < N)
		N = outside.Size();

	glBegin(GL_QUAD_STRIP);
	for (size_t n = 0; n < N; ++n) {
		if (leftside) {
			glNormal3f(inside.Normal(n).x, inside.Normal(n).y,
					inside.Normal(n).z);
			glVertex3f(inside[n].x, inside[n].y, inside[n].z);
			glNormal3f(outside.Normal(n).x, outside.Normal(n).y,
					outside.Normal(n).z);
			glVertex3f(outside[n].x, outside[n].y, outside[n].z);
		} else {
			glNormal3f(outside.Normal(n).x, outside.Normal(n).y,
					outside.Normal(n).z);
			glVertex3f(outside[n].x, outside[n].y, outside[n].z);
			glNormal3f(inside.Normal(n).x, inside.Normal(n).y,
					inside.Normal(n).z);
			glVertex3f(inside[n].x, inside[n].y, inside[n].z);
		}
	}
	glEnd();
	inside.Paint();
	outside.Paint();
}


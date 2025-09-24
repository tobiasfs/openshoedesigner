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
#include "../../3D/OpenGLText.h"

void Insole::Point::SetDirection(const Point &p0, const Point &p1) {
	dir = (Vector3(p1.u, p1.v) - Vector3(p0.u, p0.v)).Normal();
}

Insole::Point::Point(const Geometry::Vertex &other) {
	x = other.x;
	y = other.y;
	z = other.z;
	u = other.u;
	v = other.v;
	n = other.n;
	c = other.c;
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

Insole::Point& Insole::Point::operator +=(const Point &rhs) {
	this->x += rhs.x;
	this->y += rhs.y;
	this->z += rhs.z;
	this->u += rhs.u;
	this->v += rhs.v;
	return *this;
}

Insole::Point& Insole::Point::operator -=(const Point &rhs) {
	this->x -= rhs.x;
	this->y -= rhs.y;
	this->z -= rhs.z;
	this->u -= rhs.u;
	this->v -= rhs.v;
	return *this;
}
void Insole::Line::Setup(const Point &p0, const Point &p1, double f0,
		double f1) {
	const double d = (p1 - p0).Abs();
	x = Polynomial::ByBezier(p0.u, p0.u + d * f0 * p0.dir.x,
			p1.u - d * f1 * p1.dir.x, p1.u);
	y = Polynomial::ByBezier(p0.v, p0.v + d * f0 * p0.dir.y,
			p1.v - d * f1 * p1.dir.y, p1.v);
	z = Polynomial::ByValue(0.0, 0.0);
	r0 = 0.0;
	r1 = 1.0;
}

bool Insole::Line::IsInside(const double r) const {
	return (r >= r0 && r <= r1);
}

void Insole::Line::Paint() const {
	glBegin(GL_LINE_STRIP);
	for (uint8_t n = 0; n <= 100; n++) {
		const double r = (double) n * 0.01;
		glVertex3d(x(r), y(r), z(r));
	}
	glEnd();
}

void Insole::Transform(const AffineTransformMatrix &m) {
	A.Transform(m);
	B.Transform(m);
	C.Transform(m);
	D.Transform(m);
	E.Transform(m);
	F.Transform(m);
	G.Transform(m);
	H.Transform(m);
	J.Transform(m);
	K.Transform(m);
	L.Transform(m);
	M.Transform(m);
	N.Transform(m);
	P.Transform(m);
	Z.Transform(m);
	Geometry::Transform(m);
	outline.Transform(m);
	for (auto &line : lines)
		line.Transform(m);
}

void Insole::Transform(std::function<Vector3(Vector3)> func) {
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
	M.Transform(func);
	N.Transform(func);
	P.Transform(func);
	Z.Transform(func);
	Geometry::Transform(func);
	outline.Transform(func);
//	this->CalculateNormals();
	for (auto &line : lines)
		line.Transform(func);
}

void Insole::ToOpenGL(const Point &p, const std::string &label) const {
	glPushMatrix();
	glTranslated(p.x, p.y, p.z);
	glScaled(0.01, 0.01, 0.01);
	glNormal3d(0.0, -1.0, 0.0);
	OpenGLText txt;
	txt.Paint(label);
	glBegin(GL_POINTS);
	glVertex3d(0, 0, 0);
	glEnd();
	glPopMatrix();
}

void Insole::Paint() const {
//	for(auto & line : lines)
//		line.Paint();



	Geometry::Paint();

	Geometry underside = *this;
	underside.FlipInsideOutside();
	underside.FlipNormals();
	underside.Paint();

	outline.Paint();

	glPointSize(5);

	// Only paint the points on the outline and J.
	ToOpenGL(A, "A");
	ToOpenGL(B, "B");
	ToOpenGL(C, "C");
	ToOpenGL(D, "D");
	ToOpenGL(E, "E");
	ToOpenGL(F, "F");
	ToOpenGL(G, "G");
	ToOpenGL(H, "H");
	ToOpenGL(J, "J");
	ToOpenGL(K, "K");
	ToOpenGL(L, "L");
	ToOpenGL(M, "M");
	ToOpenGL(N, "N");
	ToOpenGL(P, "P");
	ToOpenGL(Z, "Z");

	glPointSize(1);
}


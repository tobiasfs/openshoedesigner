///////////////////////////////////////////////////////////////////////////////
// Name               : NagataPatch.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 30.05.2020
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

#include "NagataPatch.h"

#include  "OpenGL.h"

void NagataPatch::Set(const Vector3& x00, const Vector3& n00,
		const Vector3& x10, const Vector3& n10, const Vector3& x11,
		const Vector3& n11, const Vector3& x01, const Vector3& n01)
{
	// Eq. 48:
	const Vector3 d1 = x10 - x00;
	const Vector3 d2 = x11 - x10;
	const Vector3 d3 = x11 - x01;
	const Vector3 d4 = x01 - x00;

	const Vector3 c1 = c(d1, n00, n10);
	const Vector3 c2 = c(d2, n10, n11);
	const Vector3 c3 = c(d3, n01, n11);
	const Vector3 c4 = c(d4, n00, n01);

	// Eq. 51:
	c00 = x00;
	c10 = d1 - c1;
	c01 = d4 - c4;
	c11 = d2 - d4 + c1 - c2 - c3 + c4;
	c20 = c1;
	c02 = c4;
	c12 = c2 - c4;
	c21 = c3 - c1;
}

Vector3 NagataPatch::c(const Vector3& d, const Vector3& n0, const Vector3 n1)
{
	// Eq. 15:
	const Vector3 v_ = (n0 + n1) / 2.0;
	const Vector3 dv = (n0 - n1) / 2.0;
	const float d_ = d.Dot(v_);
	const float dd = d.Dot(dv);
	const float c_ = n0.Dot(n0 - dv * 2.0);
	const float dc = n0.Dot(dv);
	if(c_ == 1.0 || c_ == -1) return Vector3();
	return v_ * (dd / (1 - dc)) + dv * (d_ / dc);
}

Vector3 NagataPatch::operator ()(const float u, const float v) const
{
	// Eq. 46:
	return c00 + c10 * u + c01 * v + c11 * u * v + c20 * u * u + c02 * v * v
			+ c21 * u * u * v + c12 * u * v * v;
}

Vector3 NagataPatch::Normal(const float u, const float v) const
{
	// Derivatives of Eq. 46:
	// d/du
	const Vector3 nu = c12 * v * v + c21 * 2 * u * v + c11 * v + c20 * 2 * u
			+ c10;
	// d/dv
	const Vector3 nv = c12 * 2 * u * v + c02 * 2 * v + c21 * u * u + c11 * u
			+ c01;
	// Normal vector by vector product
	return (nu * nv).Normal();
}

void NagataPatch::Paint(void) const
{
	const float du = 0.1;
	const float dv = 0.1;
	for(float u = 0.0; u < 1.0; u += du){
		glBegin(GL_QUAD_STRIP);
		Vector3 v0 = this->operator()(u, 0.0);
		Vector3 v1 = this->operator()(u + du, 0.0);
		Vector3 n0 = Normal(u, 0.0);
		Vector3 n1 = Normal(u + du, 0.0);
		glNormal3f(n0.x, n0.y, n0.z);
		glVertex3f(v0.x, v0.y, v0.z);
		glNormal3f(n1.x, n1.y, n1.z);
		glVertex3f(v1.x, v1.y, v1.z);
		for(float v = 0.0; v < 1.0; v += dv){
			Vector3 v0 = this->operator()(u, v + dv);
			Vector3 v1 = this->operator()(u + du, v + dv);
			Vector3 n0 = Normal(u, v + dv);
			Vector3 n1 = Normal(u + du, v + dv);
			glNormal3f(n0.x, n0.y, n0.z);
			glVertex3f(v0.x, v0.y, v0.z);
			glNormal3f(n1.x, n1.y, n1.z);
			glVertex3f(v1.x, v1.y, v1.z);
		}
		glEnd();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Name               : CanvasStereoTest.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 25.01.2015
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

#include "CanvasStereoTest.h"

CanvasStereoTest::CanvasStereoTest(wxWindow *parent, wxWindowID id,
		const wxPoint &pos, const wxSize &size, long style,
		const wxString &name) :
		OpenGLCanvas(parent, id, pos, size, style, name){
	scale = 0.1;
}

void CanvasStereoTest::Render() {
	::glPushMatrix();
	::glColor3f(0.8, 0.8, 0.8);

	Vector3 v0;
	Vector3 v1;
	Vector3 v2;
	Vector3 v3;
	const float dt = (double) 2 / 40;
	glBegin(GL_QUADS);
	for (size_t i = 0; i < 40; ++i) {
		for (size_t j = 0; j < 40; ++j) {
			const float x = -1.0 + (double) i * dt;
			const float y = -1.0 + (double) j * dt;
			v0.Set(x, y, CalculateFront(x, y));
			v1.Set(x + dt, y, CalculateFront(x + dt, y));
			v2.Set(x + dt, y + dt, CalculateFront(x + dt, y + dt));
			v3.Set(x, y + dt, CalculateFront(x, y + dt));
			Vector3 n = (v1 - v0 + v2 - v3) * (v3 - v0 + v2 - v1);
			n.Normalize();

			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v0.x, v0.y, v0.z);
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v3.x, v3.y, v3.z);

		}
	}

	for (size_t i = 0; i < 40; ++i) {
		for (size_t j = 0; j < 40; ++j) {
			const float x = -1.0 + (double) i * dt;
			const float y = -1.0 + (double) j * dt;
			v0.Set(x, y, CalculateZBack(x, y));
			v1.Set(x, y + dt, CalculateZBack(x, y + dt));
			v2.Set(x + dt, y + dt, CalculateZBack(x + dt, y + dt));
			v3.Set(x + dt, y, CalculateZBack(x + dt, y));
			Vector3 n = (v1 - v0 + v2 - v3) * (v3 - v0 + v2 - v1);
			n.Normalize();

			glNormal3f(n.x, n.y, n.z);
			glVertex3f(v0.x, v0.y, v0.z);
			glVertex3f(v1.x, v1.y, v1.z);
			glVertex3f(v2.x, v2.y, v2.z);
			glVertex3f(v3.x, v3.y, v3.z);

		}
	}

	glNormal3f(1, 0, 0);
	glVertex3f(1, -1, -0.5);
	glVertex3f(1, 1, -0.5);
	glVertex3f(1, 1, 0);
	glVertex3f(1, -1, 0);

	glNormal3f(-1, 0, 0);
	glVertex3f(-1, -1, -0.5);
	glVertex3f(-1, -1, 0);
	glVertex3f(-1, 1, 0);
	glVertex3f(-1, 1, -0.5);

	glNormal3f(0, 1, 0);
	glVertex3f(1, 1, -0.5);
	glVertex3f(-1, 1, -0.5);
	glVertex3f(-1, 1, 0);
	glVertex3f(1, 1, 0);

	glNormal3f(0, -1, 0);
	glVertex3f(1, -1, -0.5);
	glVertex3f(1, -1, 0);
	glVertex3f(-1, -1, 0);
	glVertex3f(-1, -1, -0.5);

	glEnd();

	::glPopMatrix();
}

float CanvasStereoTest::CalculateFront(float x, float y) {
	return cos(M_PI * x / 2) * cos(M_PI * y / 2);
}
float CanvasStereoTest::CalculateZBack(float x, float y) {
	return -0.5 - (cos(M_PI * x / 2 * 3) * cos(M_PI * y / 2 * 3)) / 2;
}

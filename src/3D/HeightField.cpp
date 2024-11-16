///////////////////////////////////////////////////////////////////////////////
// Name               : HeightField.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.03.2016
// Copyright          : (C) 2016 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "HeightField.h"

#include "../StdInclude.h"
#include <GL/gl.h>
#include <math.h>
#include <float.h>

HeightField::HeightField() {
	dx = 1.0;
	dy = 1.0;
	Nx = 0;
	Ny = 0;
	N = 0;
	value = NULL;
	color.Set(0.5, 0.5, 0.5);
}

HeightField::HeightField(const HeightField &other) :
		N(other.N), Nx(other.Nx), Ny(other.Ny), dx(other.dx), dy(other.dy), color(
				other.color), matrix(other.matrix) {
	if (N == 0) {
		value = NULL;
		return;
	}
	value = new double[N];
	if (value == NULL)
		throw(__FILE__ ":Copy constructor - Not enough memory.");
	for (unsigned int n = 0; n < N; n++)
		value[n] = other.value[n];
}

HeightField::~HeightField() {
	if (value != NULL)
		delete[] value;
}

void HeightField::SetCount(unsigned int nx, unsigned int ny, float resolution) {
	if (value != NULL)
		delete[] value;
	this->Nx = nx;
	this->Ny = ny;
	this->N = nx * ny;
	value = new double[this->N];
	if (value == NULL)
		throw(__FILE__ ":SetCount(...) - Not enough memory.");
	dx = resolution;
	dy = resolution;
	Clear();
}

void HeightField::SetSize(float x, float y, float resolution) {
	if (value != NULL)
		delete[] value;
	Nx = (unsigned int) ceil(x / resolution);
	Ny = (unsigned int) ceil(y / resolution);
	this->N = Nx * Ny;
	value = new double[this->N];
	if (value == NULL)
		throw(__FILE__ ":SetSize(...) - Not enough memory.");
	dx = resolution;
	dy = resolution;
	Clear();
}

void HeightField::Clear(void) {
	if (value == NULL)
		return;
	unsigned int i;
	for (i = 0; i < N; i++)
		value[i] = 0.0;
}

void HeightField::SetValues(double *v, unsigned int size) {
	if (size > N)
		return;
	for (size_t n = 0; n < size; ++n)
		this->value[n] = v[n];
}

Polygon3 HeightField::GetUnderline(void) const {
	Polygon3 temp;

	Vector3 p(0, 0, 0);
	unsigned int i, j, c = 0;
	for (i = 0; i < Nx; i++) {
		p.z = 0.0;
		for (j = 0; j < Ny; j++) {
			double v0 = value[c];
			if (v0 > 0.0) {
				if (p.z > 0.0) {
					if (v0 < p.z)
						p.z = v0;
				} else {
					p.z = v0;
				}
			}
			c += Nx;
		}
		c -= (Nx * Ny - 1);
		temp.AddVertex(p);
		p.x += dx;
	}

	temp.Transform(this->matrix);
	return temp;
}

void HeightField::Paint(void) const {
	if (value == NULL)
		return;
	glPushMatrix();
	matrix.GLMultMatrix();
	glColor4f(color.x, color.y, color.z, 0.8);

	glPointSize(2.0);
	glBegin(GL_POINTS);

	Vector3 p(0, 0, 0);
	unsigned int i, j, c = 0;
	for (j = 0; j < Ny; j++) {
		for (i = 0; i < Nx; i++) {
			double v0 = value[c];

			glColor3f(v0 * 5, v0 * 5, v0 * 5);
			glVertex3f(p.x, p.y, v0);

			p.x += dx;
			c++;
		}
		p.x = 0;
		p.y += dy;
	}

	glEnd();
	glPopMatrix();
}

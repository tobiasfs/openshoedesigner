///////////////////////////////////////////////////////////////////////////////
// Name               : OrientedMatrix.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 05.11.2016
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

#include "OrientedMatrix.h"

#include <cassert>
#include <float.h>
#include <inttypes.h>

#include "OpenGL.h"

using std::swap;

void OrientedMatrix::SetSize(unsigned int nx, unsigned int ny, unsigned int nz,
		float resolution) {
	Matrix::SetSize(nx, ny, nz);
	dx = resolution;
	dy = resolution;
	dz = resolution;
}

void OrientedMatrix::SetExtent(float x, float y, float z, float resolution) {
	size_t nx = (unsigned int) ceil(x / resolution);
	size_t ny = (unsigned int) ceil(y / resolution);
	size_t nz = (unsigned int) ceil(z / resolution);
	Matrix::SetSize(nx, ny, nz);
	dx = resolution;
	dy = resolution;
	dz = resolution;
}

void OrientedMatrix::SetOrigin(Vector3 origin) {
	this->origin = origin;
}

double OrientedMatrix::GetValue(Vector3 p) const {
	return GetValue(p.x, p.y, p.z);
}

double OrientedMatrix::GetValue(double x, double y, double z) const {
	const size_t Nx = Size(1);
	const size_t Ny = Size(2);
	const size_t Nz = Size(3);
	const double hx = (x - origin.x) / dx;
	const double hy = (y - origin.y) / dy;
	const double hz = (z - origin.z) / dz;
	const double px = floor(hx);
	const double py = floor(hy);
	const double pz = floor(hz);
	const double mx = hx - px;
	const double my = hy - py;
	const double mz = hz - pz;
	const int ix = (int) px;
	const int iy = (int) py;
	const int iz = (int) pz;
	if (ix < 0 || iy < 0 || iz < 0 || ix + 2 > Nx || iy + 2 > Ny || iz + 2 > Nz)
		return 0.0;
	const int pos = ix + (iy + iz * Ny) * Nx;
	double v = 0.0;
	v += values[pos] * (1 - mx) * (1 - my) * (1 - mz);
	v += values[pos + 1] * (mx) * (1 - my) * (1 - mz);
	v += values[pos + Nx] * (1 - mx) * (my) * (1 - mz);
	v += values[pos + 1 + Nx] * (mx) * (my) * (1 - mz);
	v += values[pos + Nx * Ny] * (1 - mx) * (1 - my) * (mz);
	v += values[pos + 1 + Nx * Ny] * (mx) * (1 - my) * (mz);
	v += values[pos + Nx + Nx * Ny] * (1 - mx) * (my) * (mz);
	v += values[pos + 1 + Nx + Nx * Ny] * (mx) * (my) * (mz);
	return v;
}

Vector3 OrientedMatrix::GetGrad(Vector3 p) const {
	const size_t Nx = Size(1);
	const size_t Ny = Size(2);
	const size_t Nz = Size(3);
	const double hx = (p.x - origin.x) / dx;
	const double hy = (p.y - origin.y) / dy;
	const double hz = (p.z - origin.z) / dz;
	const double px = floor(hx);
	const double py = floor(hy);
	const double pz = floor(hz);
	const double mx = hx - px;
	const double my = hy - py;
	const double mz = hz - pz;
	const int ix = (int) px;
	const int iy = (int) py;
	const int iz = (int) pz;

	Vector3 temp;
	if (ix < 0 || iy < 0 || iz < 0 || ix + 2 > Nx || iy + 2 > Ny || iz + 2 > Nz)
		return temp;
	const size_t pos = ix + (iy + iz * Ny) * Nx;
	const double v0 = values[pos];
	const double v1 = values[pos + 1];
	const double v2 = values[pos + Nx];
	const double v3 = values[pos + 1 + Nx];
	const double v4 = values[pos + Nx * Ny];
	const double v5 = values[pos + 1 + Nx * Ny];
	const double v6 = values[pos + Nx + Nx * Ny];
	const double v7 = values[pos + 1 + Nx + Nx * Ny];

	//  Maxima:
	//  v(x,y,z):= v0*(1-x)*(1-y)*(1-z)+v1*x*(1-y)*(1-z)+v2*(1-x)*y*(1-z)+v3*x*y*(1-z)+v4*(1-x)*(1-y)*z+v5*x*(1-y)*z+v6*(1-x)*y*z+v7*x*y*z;
	//	diff(v(x,y,z),x);
	//	diff(v(x,y,z),y);
	//	diff(v(x,y,z),z);

	temp.x = v7 * my * mz - v6 * my * mz + v5 * (1 - my) * mz
			- v4 * (1 - my) * mz + v3 * my * (1 - mz) - v2 * my * (1 - mz)
			+ v1 * (1 - my) * (1 - mz) - v0 * (1 - my) * (1 - mz);
	temp.y = v7 * mx * mz - v5 * mx * mz + v6 * (1 - mx) * mz
			- v4 * (1 - mx) * mz + v3 * mx * (1 - mz) - v1 * mx * (1 - mz)
			+ v2 * (1 - mx) * (1 - mz) - v0 * (1 - mx) * (1 - mz);
	temp.z = v7 * mx * my - v3 * mx * my + v6 * (1 - mx) * my
			- v2 * (1 - mx) * my + v5 * mx * (1 - my) - v1 * mx * (1 - my)
			+ v4 * (1 - mx) * (1 - my) - v0 * (1 - mx) * (1 - my);

	return temp;
}

OrientedMatrix OrientedMatrix::SurfaceField(void) const {
	const size_t Nx = Size(1);
	const size_t Ny = Size(2);
	const size_t Nz = Size(3);

	OrientedMatrix temp;
	temp.SetSize(Nx, Ny, 1, dx);
	temp.SetOrigin(Vector3(origin.x, origin.y, origin.z));
	temp.SetInsertPosition(0);

	const size_t h = Nx * Ny;
	for (size_t i = 0; i < h; i++) {
		double v0 = values[i];
		size_t j = i;
		double v = DBL_MAX;
		for (size_t k = 1; k < Nz; k++) {
			j += h;
			if (values[j] > 0.5) {
				if (v0 > 0.5) {
					v = (double) (k - 1) * dz;
				} else {
					const double c = fmin(
							fmax((0.5 - v0) / (values[j] - v0), 0), 1);
					v = ((double) (k - 1) + c) * dz;
				}
				break;
			}
			v0 = values[j];
		}
		temp.Insert(v);
	}
	return temp;
}

void OrientedMatrix::Paint(void) const {
	const size_t Nx = Size(1);
	const size_t Ny = Size(2);
	const size_t Nz = Size(3);

	const uint_fast8_t NDim = ((Nx > 1) ? 1 : 0) + ((Ny > 1) ? 1 : 0)
			+ ((Nz > 1) ? 1 : 0);

	glPushMatrix();
	glTranslatef(origin.x, origin.y, origin.z);

	if (NDim == 2) {
		switch (display2D) {
		case Display2D::Points2D: {
			glPointSize(2.0);
			glBegin(GL_POINTS);
			Vector3 p(0, 0, 0);
			unsigned int c = 0;
			for (size_t k = 0; k < Nz; k++) {
				for (size_t j = 0; j < Ny; j++) {
					for (size_t i = 0; i < Nx; i++) {
						const double v0 = values[c];
						if (v0 > surface) {
							glColor3f(v0 * 5, v0 * 5, v0 * 5);
							glVertex3f(p.x, p.y, p.z);
						}
						p.x += dx;
						c++;
					}
					p.x = 0;
					p.y += dy;
				}
				p.y = 0;
				p.z += dz;
			}
			glEnd();
			break;
		}
		case Display2D::Grid: {
			glDisable(GL_CULL_FACE);
			glBegin(GL_QUADS);
			glColor3f(1, 1, 1);
			if (Ny == 1) {
				for (size_t x = 0; x < (Nx - 1); x++) {
					for (size_t z = 0; z < (Nz - 1); z++) {
						const double v0 = Matrix::operator()(x, 0, z);
						const double v1 = Matrix::operator()(x, 0, z + 1);
						const double v2 = Matrix::operator()(x + 1, 0, z + 1);
						const double v3 = Matrix::operator()(x + 1, 0, z);
						if (v0 == DBL_MAX || v1 == DBL_MAX || v2 == DBL_MAX
								|| v3 == DBL_MAX)
							continue;
						glNormal3f(0, 0, 1);
						glVertex3f((x) * dx, v0, (z) * dz);
						glVertex3f((x) * dx, v3, (z + 1) * dz);
						glVertex3f((x + 1) * dx, v2, (z + 1) * dz);
						glVertex3f((x + 1) * dx, v1, (z) * dz);
					}
				}
			}
			if (Nz == 1) {
				for (size_t x = 0; x < (Nx - 1); x++) {
					for (size_t y = 0; y < (Ny - 1); y++) {
						const double v0 = Matrix::operator()(x, y, 0);
						const double v1 = Matrix::operator()(x + 1, y, 0);
						const double v2 = Matrix::operator()(x + 1, y + 1, 0);
						const double v3 = Matrix::operator()(x, y + 1, 0);
						if (v0 == DBL_MAX || v1 == DBL_MAX || v2 == DBL_MAX
								|| v3 == DBL_MAX)
							continue;
						glNormal3f(0, 0, 1);
						glVertex3f((x) * dx, (y) * dy, v0);
						glVertex3f((x + 1) * dx, (y) * dy, v1);
						glVertex3f((x + 1) * dx, (y + 1) * dy, v2);
						glVertex3f((x) * dx, (y + 1) * dy, v3);
					}
				}
			}
			glEnd();
			glEnable(GL_CULL_FACE);
			break;
		}
		case Display2D::Image: {
			const double min = this->Min();
			const double max = this->Max();
			const double a = (max != min) ? (1 / (max - min)) : 1.0;
			const double b = -min * a;
			glNormal3f(0, 0, 1);
			glBegin(GL_QUADS);
			if (Ny == 1) {
				for (size_t x = 0; x < (Nx - 1); x++) {
					for (size_t z = 0; z < (Nz - 1); z++) {
						const double v0 = Matrix::operator()(x, 0, z);
						const double v1 = Matrix::operator()(x, 0, z + 1);
						const double v2 = Matrix::operator()(x + 1, 0, z + 1);
						const double v3 = Matrix::operator()(x + 1, 0, z);
						if (v0 == DBL_MAX || v1 == DBL_MAX || v2 == DBL_MAX
								|| v3 == DBL_MAX)
							continue;
						const double c0 = a * v0 + b;
						const double c1 = a * v1 + b;
						const double c2 = a * v2 + b;
						const double c3 = a * v3 + b;
						glNormal3f(0, 0, 1);
						glColor3d(c0, c0, c0);
						glVertex3f((x) * dx, 0, (z) * dz);
						glColor3d(c1, c1, c1);
						glVertex3f((x) * dx, 0, (z + 1) * dz);
						glColor3d(c2, c2, c2);
						glVertex3f((x + 1) * dx, 0, (z + 1) * dz);
						glColor3d(c3, c3, c3);
						glVertex3f((x + 1) * dx, 0, (z) * dz);
					}
				}
			}
			if (Nz == 1) {
				for (size_t x = 0; x < (Nx - 1); x++) {
					for (size_t y = 0; y < (Ny - 1); y++) {
						const double v0 = Matrix::operator()(x, y, 0);
						const double v1 = Matrix::operator()(x + 1, y, 0);
						const double v2 = Matrix::operator()(x + 1, y + 1, 0);
						const double v3 = Matrix::operator()(x, y + 1, 0);
						if (v0 == DBL_MAX || v1 == DBL_MAX || v2 == DBL_MAX
								|| v3 == DBL_MAX)
							continue;
						const double c0 = a * v0 + b;
						const double c1 = a * v1 + b;
						const double c2 = a * v2 + b;
						const double c3 = a * v3 + b;
						glColor3d(c0, c0, c0);
						glVertex3f((x) * dx, (y) * dy, 0);
						glColor3d(c1, c1, c1);
						glVertex3f((x + 1) * dx, (y) * dy, 0);
						glColor3d(c2, c2, c2);
						glVertex3f((x + 1) * dx, (y + 1) * dy, 0);
						glColor3d(c3, c3, c3);
						glVertex3f((x) * dx, (y + 1) * dy, 0);
					}
				}
			}
			glEnd();
			break;
		}
		}
	}

	if (NDim == 3) {
		switch (display3D) {
		case Display3D::Points3D: {
			glPointSize(2.0);
			glBegin(GL_POINTS);
			Vector3 p(0, 0, 0);
			size_t c = 0;
			for (size_t k = 0; k < Nz; k++) {
				for (size_t j = 0; j < Ny; j++) {
					for (size_t i = 0; i < Nx; i++) {
						const double v0 = values[c];
						if (v0 > surface) {
							glColor3f(v0 * 5, v0 * 5, v0 * 5);
							glVertex3f(p.x, p.y, p.z);
						}
						p.x += dx;
						c++;
					}
					p.x = 0;
					p.y += dy;
				}
				p.y = 0;
				p.z += dz;
			}
			glEnd();
			break;
		}
		}
	}

	glPopMatrix();
}

void OrientedMatrix::Rotate(Axis a, int quarters) {
	Matrix::Rotate(a, quarters);

	quarters %= 4;
	if (quarters < 0)
		quarters += 4;
	if (quarters == 0)
		return;
	if (quarters == 2)
		return;
	assert(quarters == 1 || quarters == 3);
	switch (a) {
	case Axis::X:
		swap(dy, dz);
		break;
	case Axis::Y:
		swap(dz, dx);
		break;
	case Axis::Z:
		swap(dx, dy);
		break;
	}
}

OrientedMatrix OrientedMatrix::XRay(Method method) const {
	OrientedMatrix temp;

	// The trick in the next line is, that the XRay Method of the MathMatrix class returns a
	// MathMatrix object. To assign this object to an OrientedMatrix object. The MathMatrix
	// part of the object to be returned is extracted and the result assigned to it.
	*(Matrix*) &temp = Matrix::XRay(method);

	// Then the rest is assigned over.
	temp.dx = this->dx;
	temp.dy = this->dy;
	temp.dz = this->dz;
	temp.display2D = this->display2D;
	temp.display3D = this->display3D;
	temp.surface = this->surface;
	temp.SetOrigin(Vector3(this->origin.x, this->origin.y, this->origin.z));

	return temp;
}

///////////////////////////////////////////////////////////////////////////////
// Name               : TestGrid.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 02.06.2020
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

#include "TestGrid.h"

#include "Vector3.h"
#include "AffineTransformMatrix.h"

#include "OpenGL.h"

TestGrid::TestGrid(const Vector3 &v1, const Vector3 &v2) :
		BoundingBox(v1, v2) {
}

void TestGrid::SetCellSize(double d_) {
	U = GetSizeX() / d_ + 1;
	V = GetSizeY() / d_ + 1;
	W = GetSizeZ() / d_ + 1;
	xmax = xmin + d_ * (double) (U - 1);
	ymax = ymin + d_ * (double) (V - 1);
	zmax = zmin + d_ * (double) (W - 1);
	p.resize(U * V * W);
	this->d = d_;
	Reset();
}

void TestGrid::Reset() {
	size_t i = 0;
	for (size_t w = 0; w < W; w++) {
		for (size_t v = 0; v < V; v++) {
			for (size_t u = 0; u < U; u++) {
				p[i] = Vector3(xmin + (double) u * d, ymin + (double) v * d,
						zmin + (double) w * d);
				i++;
			}
		}
	}
}

void TestGrid::PaintPoints() const {
	const Vector3 c = this->GetCoordinateSystem().operator ()(0.5, 0.5, 0.5);

	glPointSize(3);
	glBegin(GL_POINTS);
	for (auto &v : p) {
		const Vector3 n = (v - c).Normal();
		glNormal3f(n.x, n.y, n.z);
		glVertex3f(v.x, v.y, v.z);
	}
	glEnd();

}

void TestGrid::PaintLines() const {
	const Vector3 c = this->GetCoordinateSystem().operator ()(0.5, 0.5, 0.5);

	for (size_t w = 0; w < W; w++) {
		for (size_t v = 0; v < V; v++) {
			glBegin(GL_LINES);
			for (size_t u = 1; u < U; u++) {
				const Vector3 &v0 = p[(u - 1) + v * U + w * U * V];
				const Vector3 &v1 = p[u + v * U + w * U * V];
				Vector3 n =
						(((v1 - v0) * (((v0 + v1) / 2.0) - c)) * (v1 - v0)).Normal();
				glNormal3f(n.x, n.y, n.z);
				glVertex3f(v0.x, v0.y, v0.z);
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}
	}

	for (size_t u = 0; u < U; u++) {
		for (size_t w = 0; w < W; w++) {
			glBegin(GL_LINES);
			for (size_t v = 1; v < V; v++) {
				const Vector3 &v0 = p[u + (v - 1) * U + w * U * V];
				const Vector3 &v1 = p[u + v * U + w * U * V];
				Vector3 n =
						(((v1 - v0) * (((v0 + v1) / 2.0) - c)) * (v1 - v0)).Normal();
				glNormal3f(n.x, n.y, n.z);
				glVertex3f(v0.x, v0.y, v0.z);
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}
	}
	for (size_t v = 0; v < V; v++) {
		for (size_t u = 0; u < U; u++) {
			glBegin(GL_LINES);
			for (size_t w = 1; w < W; w++) {
				const Vector3 &v0 = p[u + v * U + (w - 1) * U * V];
				const Vector3 &v1 = p[u + v * U + w * U * V];
				Vector3 n =
						(((v1 - v0) * (((v0 + v1) / 2.0) - c)) * (v1 - v0)).Normal();
				glNormal3f(n.x, n.y, n.z);
				glVertex3f(v0.x, v0.y, v0.z);
				glVertex3f(v1.x, v1.y, v1.z);
			}
			glEnd();
		}
	}

}

void TestGrid::Transform(std::function<Vector3(Vector3)> func) {
	for (auto &v : p)
		v = func(v);
}

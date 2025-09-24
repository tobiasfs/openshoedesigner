///////////////////////////////////////////////////////////////////////////////
// Name               : PointCloud.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 14.04.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "PointCloud.h"

#include "OpenGL.h"

void PointCloud::InitExample() {
	const size_t N = 10000;
	p.resize(N);
	for (size_t n = 0; n < N; n++) {
		p[n].x = (float) rand() / (float) (RAND_MAX) * 0.3 - 0.1;
		p[n].y = (float) rand() / (float) (RAND_MAX) * 0.2 - 0.1;
		float d = p[n].y * p[n].y;
		if (p[n].x > 0)
			d += (p[n].x * p[n].x) / 4;
		else
			d += p[n].x * p[n].x;
		if (d >= 0.01)
			d = 0;
		else
			d = sqrt(0.01 - d);
		p[n].z = (float) rand() / (float) (RAND_MAX) * 0.01 - 0.005 + d;
	}
}

void PointCloud::Paint() {
	glBegin(GL_POINTS);
	const size_t N = p.size();
	for (size_t n = 0; n < N; n++)
		glVertex3f(p[n].x, p[n].y, p[n].z);
	glEnd();
}

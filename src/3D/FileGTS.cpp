///////////////////////////////////////////////////////////////////////////////
// Name               : FileGTS.cpp
// Purpose            : Reads a GTS File
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 11.06.2011
// Copyright          : (C) 2011 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "FileGTS.h"

#include <array>
#include <fstream>
#include <stddef.h>
#include <stdexcept>
#include <vector>

FileGTS::FileGTS(const std::string &filename_) :
		FileGeometry(filename_) {
}

void FileGTS::ReadStream(Geometry &geo) {

	size_t nv;
	size_t ne;
	size_t nf;

	*inp >> nv >> ne >> nf;

	if (!inp->good())
		throw std::runtime_error("File is not a valid GTS file!");

	std::vector<Vector3> vectors;
	vectors.resize(nv);
	for (size_t i = 0; i < nv; i++) {
		double x;
		double y;
		double z;
		*inp >> x >> y >> z;
		if (inp->eof())
			throw std::runtime_error("Could not read all vectors from file.");
		vectors[i].Set(x, y, z);
//		hull.LLAddVertex(vectors[i]);
	}

	std::vector<size_t> e0;
	std::vector<size_t> e1;
	e0.resize(ne);
	e1.resize(ne);
	for (size_t i = 0; i < ne; i++) {
		size_t v0;
		size_t v1;
		*inp >> v0 >> v1;
		if (inp->eof())
			throw std::runtime_error("Could not read all edges from file.");
		e0[i] = v0;
		e1[i] = v1;
	}

	for (size_t i = 0; i < nf; i++) {
		size_t v0;
		size_t v1;
		size_t v2;
		*inp >> v0 >> v1 >> v2;
		if (inp->eof())
			throw std::runtime_error("Could not read all triangles from file.");

		std::array<size_t, 6> u;
		u[0] = e0[v0 - 1] - 1;
		u[1] = e1[v0 - 1] - 1;
		u[2] = e0[v1 - 1] - 1;
		u[3] = e1[v1 - 1] - 1;
		u[4] = e0[v2 - 1] - 1;
		u[5] = e1[v2 - 1] - 1;

		// Sort the rotation of the triangles. The triangles are sorted so, that
		// the vertices at the ends of the edges are the same.
		size_t t;
		if (u[1] != u[2] && u[1] != u[3]) {
			t = u[0];
			u[0] = u[1];
			u[1] = t;
		}
		if (u[1] != u[2]) {
			t = u[2];
			u[2] = u[3];
			u[3] = t;
		}
		if (u[3] != u[4]) {
			t = u[4];
			u[4] = u[5];
			u[5] = t;
		}

		geo.AddTriangle(vectors[u[0]], vectors[u[2]], vectors[u[4]]);
//		hull.AddTriangle(vectors[u[0]], vectors[u[2]], vectors[u[4]]);
//		hull.LLAddTriangle(u[0], u[2], u[4]);
	}
	geo.Finish();
//	hull.CalculateNormals();
	// For some models the normals need to be flipped for some they do not. Reason?
//	tc.FlipNormals();
//	hull.FlipNormals();

//	hulls.push_back(hull);
}


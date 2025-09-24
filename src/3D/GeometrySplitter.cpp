///////////////////////////////////////////////////////////////////////////////
// Name               : GeometrySplitter.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 01.05.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "GeometrySplitter.h"

void GeometrySplitter::Split() {
	// Find all parts in the geometry
	size_t groupCount = CalculateObjects();

	objects.assign(groupCount, Geometry());
	for (auto &obj : objects)
		obj.CopyPropertiesFrom(*this);

	// Spread all elements to the different objects
	size_t cv = 0;
	for (const auto &vec : v) {
		const size_t gr = vec.group;
		if (gr >= groupCount) {
			throw std::runtime_error(
					"This should not happen. Vertex.group too large.");
		}
		objects[gr].AddVertexWithIndex(vec, cv++);
	}
	size_t ce = 0;
	for (const auto &ed : e) {
		const size_t gr = ed.group;
		if (gr >= groupCount) {
			throw std::runtime_error(
					"This should not happen. Edge.group too large.");
		}
		objects[gr].AddEdgeWithIndex(ed, ce++);
	}
	size_t ct = 0;
	for (const auto &tri : t) {
		const size_t gr = tri.group;
		if (gr >= groupCount) {
			throw std::runtime_error(
					"This should not happen. Triangle.group too large.");
		}
		objects[gr].AddTriangleWithIndex(tri, ct++);
	}

	// Map all elements of all objects.
	for (auto &obj : objects) {
		// Update the references
		obj.Remap(0, 0, 0);
	}

	// For each element calculate the bounding box.
	bbs.assign(groupCount, BoundingBox());
	size_t n = 0;
	for (const auto &obj : objects) {
		for (size_t m = 0; m < obj.CountVertices(); m++) {
			bbs[n].Insert( { obj[m].x, obj[m].y, obj[m].z });
		}
		n++;
	}
}

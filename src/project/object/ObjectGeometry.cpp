///////////////////////////////////////////////////////////////////////////////
// Name               : ObjectGeometry.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 10.11.2024
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

#include "ObjectGeometry.h"

#include <sstream>
#include <stdexcept>

ObjectGeometry::ObjectGeometry(const Geometry &other) :
		Geometry(other) {
}

ObjectGeometry::ObjectGeometry(const Geometry &&other) :
		Geometry(std::move(other)) {
}

void ObjectGeometry::UpdateBoundingBox() {
	BB.Empty();
	for (size_t i = 0; i < CountVertices(); ++i)
		BB.Insert(v[i]);
}

void ObjectGeometry::SelectFacesCloseTo(const Vector3 &vect) {

	std::map<size_t, double> groupdir;
	for (size_t n = 0; n < CountTriangles(); ++n) {
		const Geometry::Triangle &tri = GetTriangle(n);
		auto xi = groupdir.find(tri.group);
		if (xi == groupdir.end()) {
			groupdir[tri.group] = tri.n.Dot(vect) * GetTriangleArea(n);
		} else {
			xi->second += tri.n.Dot(vect) * GetTriangleArea(n);
		}
	}

	auto it = std::max_element(groupdir.begin(), groupdir.end(),
			[](const auto &lhs, const auto &rhs) {
				return lhs.second < rhs.second;
			});
	if (it == groupdir.end()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " -";
		err
				<< "Could not find any surfaces in geometry. Is this geometry devoid of triangles?";
		throw std::runtime_error(err.str());
	}

	UnselectAll();
	SelectByGroup(it->first);
}

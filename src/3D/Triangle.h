///////////////////////////////////////////////////////////////////////////////
// Name               : Triangle.h
// Purpose            :
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

#ifndef L3D_TRIANGLE_H
#define L3D_TRIANGLE_H

/*!\class Triangle
 * \ingroup Base3D
 * \brief DEPRECATED: Simple triangle
 * \deprecated Not used; way to simple; Geometry contains its own Triangle class
 *
 * Holds the data for a simple triangle:
 *   Three vertices with three normal vectors and three colors.
 */

#include "Vector3.h"

#include <string>
#include <array>

class AffineTransformMatrix;
class Triangle {
public:
	Triangle() = default;
	explicit Triangle(const std::string &string);
	virtual ~Triangle() = default;

	void ApplyTransformation(const AffineTransformMatrix &matrix);

	/*!\brief Calculates normals for the corners of a triangle.
	 *
	 *  If no normals can be provided from elsewhere, this function
	 *  can generate a set. The normal vectors n[0] to n[2] are all
	 *  set normal to the plane of the triangle. Orientation is
	 *  right handed.
	 */
	void CalculateNormal();

	bool FromString(const std::string &string);
	std::string ToString() const;

	/*!\brief Puts a triangle in the OpenGL queue.
	 *
	 * This function does not call glBegin(GL_TRIANGLES); and
	 * glEnd();. This has to be done by the calling function.
	 * (Allows to save on OpenGL calls.)
	 */
	void Paint(bool useNormals = true, bool useColors = false) const;

public:
	std::array<Vector3, 3> p; //!< Position of vertices.
	std::array<Vector3, 3> n; //!< Normal vectors.
	std::array<Vector3, 3> c; //!< Color vectors.
};

#endif /* L3D_TRIANGLE_H */

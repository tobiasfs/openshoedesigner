///////////////////////////////////////////////////////////////////////////////
// Name               : Volume.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.08.2015
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

#ifndef VOLUME_H
#define VOLUME_H

#include "AffineTransformMatrix.h"
#include "Geometry.h"
#include "OrientedMatrix.h"
#include "Vector3.h"

#include "OpenGL.h"

/*!\class Volume
 * \brief Marching Cube Volume
 *
 * Sets up a density map by using morph-objects.
 * Applies the marching cubes algorithm to convert the density map into a surface.
 *
 * In the internal data structure X is the first axis, Y the second one, Z the last one.
 *
 * XYZ
 *
 * 000
 * 100
 * 200
 * 010
 * 110
 * 210
 * 020
 * 120
 * 220
 * 001
 * 101
 * 201
 * 011
 * 111
 * 211
 * 021
 * 121
 * 221
 * 002
 * ...
 *
 */

class Volume: public OrientedMatrix {

public:
	Volume() = default;
	Volume(const Volume&);
	Volume& operator=(const Volume &other);
	virtual ~Volume();

	void Clear();
	void AddHalfplane(const Vector3 &p1, float d0, float k0);
	void AddSphere(const Vector3 &p1, float r1, float k1);
	void AddCylinder(const Vector3 &p1, const Vector3 &p2, const float r1,
			const float k1);
	void AddCylinder(const Vector3 &p1, const Vector3 &p2, const float r1,
			const float r2, const float k1);
	void AddCylinder(const Vector3 &p1, const Vector3 &p2, const float r1,
			const float r2, const float k1, const float k2);

	/*! \brief Marching cubes algorithm
	 *
	 * Run the Marching-Cubes algorithm to generate a geometry of the surface
	 * of the data in the volume. The surface is assumed at the level of the
	 * internal variable "surface".
	 */
	void CalcSurface();

	/*! \brief Render the data
	 *
	 * After the Marching-Cubes algorithm has run, the generated Geometry can be
	 * rendered with OpenGL commands.
	 */
	void PaintSurface() const;

	/*! \brief Point on the surface
	 *
	 *
	 * \param p0
	 * \param n
	 * \return
	 */
	Vector3 GetSurface(const Vector3 &p0, const Vector3 &n) const;

public:
	Vector3 color = { 0.5, 0.5, 0.5 }; ///< Color of the volume
	Geometry geometry; ///< Generated geometry

private:
	mutable GLuint m_gllist = 0;
	mutable bool update = true;

};

#endif /* VOLUME_H */

///////////////////////////////////////////////////////////////////////////////
// Name               : NagataPatch.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 30.05.2020
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

#ifndef L3D_NAGATAPATCH_H
#define L3D_NAGATAPATCH_H
/*!\class NagataPatch
 * \brief 3D Quad Patch as devised by Takashi Nagata
 *
 * http://www.cs.uky.edu/~cheng/PUBL/Paper_Nagata.pdf
 *
 * It is a quadratic patch. It is always continuous in position but almost
 * never continuous in the face normals. Except for very well behaved surfaces as
 * on simple geometry or with a very small patch size.
 */

#include "Vector3.h"

class NagataPatch {
public:
	NagataPatch() = default;
	virtual ~NagataPatch() = default;

	void Set(const Vector3 & x00, const Vector3 & n00, const Vector3 & x10,
			const Vector3 & n10, const Vector3 & x11, const Vector3 & n11,
			const Vector3 & x01, const Vector3 & n01);

	Vector3 operator()(const float u, const float v) const;
	Vector3 Normal(const float u, const float v) const;

	void Paint() const;

private:
	Vector3 c00;
	Vector3 c10;
	Vector3 c01;
	Vector3 c11;
	Vector3 c20;
	Vector3 c02;
	Vector3 c21;
	Vector3 c12;

	static Vector3 c(const Vector3 & d, const Vector3 & n0, const Vector3 n1);
};

#endif /* L3D_NAGATAPATCH_H */

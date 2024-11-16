///////////////////////////////////////////////////////////////////////////////
// Name               : TransformationMixer.h
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.06.2020
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

#ifndef L3D_TRANSFORMATIONMIXER_H
#define L3D_TRANSFORMATIONMIXER_H

/*!\class TransformationMixer
 * \brief ...
 *
 * ...
 */

#include <cstddef>
#include <vector>
#include <functional>
#include <valarray>

#include "AffineTransformMatrix.h"
#include "Vector3.h"

class TransformationMixer {
public:
	struct Element {
		enum class Type {
			Sphere, Cylinder, Plane
		};
		Type type = Type::Sphere;
		AffineTransformMatrix m;
		Vector3 v;
		Vector3 n;
		double d = 0.0;
		std::function <double(double)> kernel;
	};

	TransformationMixer() = default;
	virtual ~TransformationMixer() = default;

	void SetBackground(double strength = 1.0, AffineTransformMatrix matrix =
			AffineTransformMatrix::Identity());

	size_t AddSphere(Vector3 center,
			const std::function <double(double)> kernel,
			AffineTransformMatrix matrix = AffineTransformMatrix::Identity());
	size_t AddCylinder(Vector3 center, Vector3 normal,
			std::function <double(double)> kernel,
			AffineTransformMatrix matrix = AffineTransformMatrix::Identity());
	size_t AddPlane(double distance, Vector3 normal,
			std::function <double(double)> kernel,
			AffineTransformMatrix matrix = AffineTransformMatrix::Identity());
	size_t AddPlane(Vector3 pointonplane, Vector3 normal,
			std::function <double(double)> kernel,
			AffineTransformMatrix matrix = AffineTransformMatrix::Identity());

	size_t Size(void) const;
	AffineTransformMatrix & operator[](size_t idx);
	const AffineTransformMatrix & operator[](size_t idx) const;

	Vector3 operator()(const Vector3 & v) const;

private:
	double background = 1e-6;
	AffineTransformMatrix backgroundmatrix;
	std::vector <Element> elements;

	// Preallocated string for speedup
	mutable std::valarray <double> mixing;
};

#endif /* L3D_TRANSFORMATIONMIXER_H */

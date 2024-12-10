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
 * \brief Mixing several simple transformations base on the position in the 3D space
 *
 * Several transformations of Vector3 are mixed together.
 *
 * The transformations are passed in as std::function<Vector3(Vector3)>
 *
 * These are function pointers to functions that have one parameter of Vector3
 * and return a new Vector3. For example AffineTransformMatrix has the
 * overloaded function Vector3 operator()(const Vector3 & other)
 *
 * The mixing is done by defining a distance from a plane, a cylinder or a
 * sphere in 3D space. The distance is then converted into a mixing factor
 * using a ```std::function<double(double)>``` (for example a Kernel).
 *
 * Based in these values, the Vector3%s are transformed. If no transformation
 * applied, the transformation is done using the _background_ transformation.
 * This is by default the unit transformation (= do nothing).
 * This can be modified by using SetBackground.
 *
 * A __sphere__ is defined by the center of the sphere. The double value to the
 * kernel is the distance from the center.
 *
 * A __cylinder__ is defined by a center and a normal axis. The double value to the
 * kernel is the distance from the axis.
 *
 * A __plane__ is defined by the plane normal and the distance of the plane along
 * the normal
 * ([Hesse normal form](https://en.wikipedia.org/wiki/Hesse_normal_form)
 * of a plane). The distance is the positive distance from the plane. Note that
 * in this case also negative distances are possible.
 *
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
		std::function<Vector3(Vector3)> m;
		Vector3 v;
		Vector3 n;
		double d = 0.0;
		std::function<double(double)> kernel;
	};

	TransformationMixer() = default;
	virtual ~TransformationMixer() = default;

	void SetBackground(double strength = 1.0,
			std::function<Vector3(Vector3)> transformation =
					AffineTransformMatrix::Identity());

	size_t AddSphere(Vector3 center, const std::function<double(double)> kernel,
			std::function<Vector3(Vector3)> transformation);
	size_t AddCylinder(Vector3 center, Vector3 normal,
			std::function<double(double)> kernel,
			std::function<Vector3(Vector3)> transformation);
	size_t AddPlane(double distance, Vector3 normal,
			std::function<double(double)> kernel,
			std::function<Vector3(Vector3)> transformation);
	size_t AddPlane(Vector3 pointonplane, Vector3 normal,
			std::function<double(double)> kernel,
			std::function<Vector3(Vector3)> transformation);

	size_t Size() const;

	Vector3 operator()(const Vector3 &v) const;

private:
	double backgroundstrength = 1e-6;
	std::function<Vector3(Vector3)> backgroundtransformation =
			AffineTransformMatrix::Identity();
	std::vector<Element> elements;

	// Preallocated string for speedup
	mutable std::valarray<double> mixing;
};

#endif /* L3D_TRANSFORMATIONMIXER_H */

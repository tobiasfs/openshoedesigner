///////////////////////////////////////////////////////////////////////////////
// Name               : Vector3.h
// Purpose            : A 3D vector class with support functions.
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 28.01.2010
// Copyright          : (C) 2010 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef L3D_VECTOR3_H
#define L3D_VECTOR3_H

/*!\class Vector3
 * \ingroup Base3D
 * \brief Vector in 3D space
 *
 * This class has lots of code in the header for the compiler to directly
 * inline.
 */

// http://www.parashift.com/c++-faq-lite/operator-overloading.html

#include <cmath>
#include <initializer_list>
#include <ostream>
#include <string>

struct Vector3 {
public:
	Vector3() = default;
	Vector3(double x_, double y_, double z_) :
			x(x_), y(y_), z(z_) {
	}
	Vector3(std::initializer_list<double> list) {
		auto it = list.begin();
		if (it != list.end())
			x = *it++;
		if (it != list.end())
			y = *it++;
		if (it != list.end())
			z = *it;
	}
	explicit Vector3(const std::string &string);

public:
	/**\name Setter
	 * \{
	 */

	void Set(double x_, double y_ = 0.0, double z_ = 0.0) {
		this->x = x_;
		this->y = y_;
		this->z = z_;
	}

	//! Zeros the vector.
	void Zero() {
		x = y = z = 0.0;
	}

	//! Normalizes the length of a vector.
	void Normalize();

	/**\}
	 * \name Getter
	 * \{
	 */

	//! Calculate the absolute length of a vector.
	double Abs() const {
		return sqrt(x * x + y * y + z * z);
	}

	//! Calculate the squared absolute length of a vector.
	double Abs2() const {
		return x * x + y * y + z * z;
	}

	//! Sets the vector to the given coordinates.
	//! Returns a normal length vector (without changing the original vector)
	Vector3 Normal() const;

	/*! \brief Generate an orthogonal vector
	 *
	 * Generates a vector that is guaranteed to be orthogonal to this vector.
	 *
	 * This may be used to define a coordinate system in a plane, where only
	 * the normal vector is given.
	 *
	 * \return Orthogonal unit vector
	 */
	Vector3 Orthogonal() const;

	/**\}
	 * \name Operators
	 * \{
	 */

	//! Overloaded operator for vector addition.
	Vector3& operator+=(const Vector3 &a) {
		this->x += a.x;
		this->y += a.y;
		this->z += a.z;
		return *this;
	}

	//! Overloaded operator for vector addition.
	friend Vector3 operator+(Vector3 a, const Vector3 &b) {
		a += b;
		return a;
	}

	//! Overloaded operator for vector subtraction.
	Vector3& operator-=(const Vector3 &a) {
		this->x -= a.x;
		this->y -= a.y;
		this->z -= a.z;
		return *this;
	}

	//! Overloaded operator for vector subtraction.
	friend Vector3 operator-(Vector3 a, const Vector3 &b) {
		a -= b;
		return a;
	}

	//! Overloaded operator for vector negation.
	Vector3 operator-() const {
		return Vector3(-this->x, -this->y, -this->z);
	}

	/*!\brief Overloaded operator for vector product.
	 *
	 * This function calculates the vector product
	 * \f[
	 * \vec{c}=\left\{
	 * \begin{array}{c}
	 * \vec{a}_y \cdot \vec{b}_z - \vec{a}_z \cdot \vec{b}_y \\
	 * \vec{a}_z \cdot \vec{b}_x - \vec{a}_x \cdot \vec{b}_z \\
	 * \vec{a}_x \cdot \vec{b}_y - \vec{a}_y \cdot \vec{b}_x
	 * \end{array}
	 * \right\}
	 * \f].
	 */
	Vector3& operator*=(const Vector3 &b) {
		Vector3 a = *(this);
		this->x = a.y * b.z - a.z * b.y;
		this->y = a.z * b.x - a.x * b.z;
		this->z = a.x * b.y - a.y * b.x;
		return *this;
	}

	//! Overloaded operator for scalar product.
	Vector3& operator*=(const double b) {
		this->x *= b;
		this->y *= b;
		this->z *= b;
		return *this;
	}

	//! Overloaded operator for vector product.
	friend Vector3 operator*(Vector3 a, const Vector3 &b) {
		a *= b;
		return a;
	}

	//! Overloaded operator for scalar product.
	friend Vector3 operator*(Vector3 a, const double b) {
		a *= b;
		return a;
	}
	//! Overloaded operator for scalar product.
	friend Vector3 operator*(const double b, Vector3 a) {
		a *= b;
		return a;
	}

	//! Calculates the dot product (inner product) of two vectors.
	double Dot(const Vector3 &b) const {
		return (x * b.x + y * b.y + z * b.z);
	}

	//! Overloaded operator for scalar division.
	Vector3& operator/=(const double b) {
		this->x /= b;
		this->y /= b;
		this->z /= b;
		return *this;
	}

	//! Overloaded operator for scalar division.
	friend Vector3 operator/(Vector3 a, const double b) {
		a /= b;
		return a;
	}

	/**\brief Comparison operator equality.
	 *
	 * This function checks, if both vectors are closer than DBL_EPSILON
	 * together. The distance is calculated in Manhattan distance (= a tiny
	 * box) not with Pythagoras (= a tiny sphere). The latter would be
	 * unstable.
	 */
	bool operator==(const Vector3 &b) const;

	//! Comparison operator inequality.
	bool operator!=(const Vector3 &b) const {
		return !(*this == b);
	}

	/**\}
	 */

	/**\brief Scales one vector by another.
	 *
	 * Like a dot product, but without summing the products up.
	 *
	 * Multiplies all values together, but keeps them in the elements of a
	 * vector:
	 * \code
	 * res.x = a.x * b.x;
	 * res.y = a.y * b.y;
	 * res.z = a.z * b.z;
	 * \endcode
	 */
	Vector3 Scale(const Vector3 &b) const {
		Vector3 temp = *this;
		temp.x *= b.x;
		temp.y *= b.y;
		temp.z *= b.z;
		return temp;
	}

	/** \brief Interpolate between two vectors
	 *
	 * Called "interp" because of Octave/Matlab.
	 */
	Vector3 Interp(const Vector3 &b, const double mix) const {
		Vector3 temp;
		temp.x = x + (b.x - x) * mix;
		temp.y = y + (b.y - y) * mix;
		temp.z = z + (b.z - z) * mix;
		return temp;
	}

	std::string ToString() const;
	void FromString(const std::string &string);
	friend std::ostream& operator<<(std::ostream &output, const Vector3 &v) {
		output << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return output;
	}

public:
	double x = 0.0;
	double y = 0.0;
	double z = 0.0;
};

#endif // L3D_VECTOR3_H


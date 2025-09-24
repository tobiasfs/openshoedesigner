///////////////////////////////////////////////////////////////////////////////
// Name               : Vector2.h
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.08.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#ifndef VECTOR2_H_
#define VECTOR2_H_

/*!\class Vector2
 * \brief ...
 *
 * ...
 */

#include <cmath>

class Vector2 {
public:
	Vector2() = default;
	Vector2(double u_, double v_) :
			u(u_), v(v_) {
	}

	/**\}
	 * \name Operators
	 * \{
	 */

	Vector2 operator-() const {
		return Vector2(-u, -v);
	}

	Vector2& operator+=(const Vector2 &rhs) {
		this->u += rhs.u;
		this->v += rhs.v;
		return *this;
	}

	Vector2& operator-=(const Vector2 &rhs) {
		this->u -= rhs.u;
		this->v -= rhs.v;
		return *this;
	}
	Vector2& operator*=(const double rhs) {
		this->u *= rhs;
		this->v *= rhs;
		return *this;
	}

	Vector2& operator/=(const double rhs) {
		this->u /= rhs;
		this->v /= rhs;
		return *this;
	}

	friend Vector2 operator+(Vector2 lhs, const Vector2 &rhs) {
		lhs += rhs;
		return lhs;
	}

	friend Vector2 operator-(Vector2 lhs, const Vector2 &rhs) {
		lhs -= rhs;
		return lhs;
	}

	friend Vector2 operator*(Vector2 lhs, const double rhs) {
		lhs *= rhs;
		return lhs;
	}

	friend Vector2 operator*(double lhs, const Vector2 &rhs) {
		return Vector2(lhs * rhs.u, lhs * rhs.v);
	}

	friend Vector2 operator/(Vector2 lhs, const double &rhs) {
		lhs /= rhs;
		return lhs;
	}

	/**\brief Inner product
	 */
	friend double operator*(const Vector2 &lhs, const Vector2 &rhs) {
		return lhs.u * rhs.u + lhs.v * rhs.v;
	}

	/**\}
	 */

	/**\brief Check if U and V as smaller than epsilon.
	 */
	bool IsZero() const;

	/**\brief Absolute length of the vector
	 */
	double Abs() const {
		return std::sqrt(u * u + v * v);
	}

	/**\brief Returns the normalized vector
	 *
	 * The normalized vector has a length (Abs()) of 1.0.
	 */
	Vector2 Normal() const;

	/**\brief Normalize the vector itself
	 */
	void Normalize();

	/**\brief Return a vector rotated by +90 deg.
	 *
	 * This is less expensive than RotateBy() because no cos or sin have
	 * to be evaluated.
	 */
	Vector2 Orthogonal() const {
		return Vector2(-v, u);
	}

	/**\brief Return a vector rotated by an arbitrary angle.
	 */
	Vector2 RotatedBy(double angle) const;

	/**\brief Test if two vector2 are close to each other.
	 */
	bool CloseTo(const Vector2 &other) const;

	/**\brief Interpolate between two vectors
	 *
	 * Mixing between two given vectors. This is a common operation in
	 * deCasteljaus algorithm.
	 *
	 * The interpolation returns the original vector for mix = 0.0 and the
	 * b vector for mix = 1.0. The value for mix is not clamped so that
	 * extrapolation is possible.
	 *
	 * \param b Vector to interpolate to
	 * \param mix Value between 0 and 1 to mix between the original and
	 *            the b vector
	 * \return Vector mixed from the two given vectors.
	 */
	Vector2 Interp(const Vector2 &b, const double mix) const {
		return {u + (b.u - u) * mix, v + (b.v - v) * mix};
	}

public:
	double u = 0.0;
	double v = 0.0;
};

#endif /* VECTOR2_H_ */

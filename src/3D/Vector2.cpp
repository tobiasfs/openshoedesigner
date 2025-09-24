///////////////////////////////////////////////////////////////////////////////
// Name               : Vector2.cpp
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

#include "Vector2.h"

#include <cfloat>

bool Vector2::IsZero() const {
	return std::fabs(u) < FLT_EPSILON && std::fabs(v) < FLT_EPSILON;
}
Vector2 Vector2::Normal() const {
	const double d = Abs();
	if (d > FLT_EPSILON) {
		return Vector2(u / d, v / d);
	}
	return *this;
}

void Vector2::Normalize() {
	*this = Vector2::Normal();
}

Vector2 Vector2::RotatedBy(double angle) const {
	const double c = std::cos(angle);
	const double s = std::sin(angle);
	return Vector2(c * u - s * v, c * v + s * u);
}

bool Vector2::CloseTo(const Vector2 &other) const {
	const double du = std::fmod(other.u - u, 2.0 * M_PI);
	const double dv = other.v - v;
	if (std::fabs(du) > FLT_EPSILON)
		return false;
	if (std::fabs(dv) > FLT_EPSILON)
		return false;
	return true;
}


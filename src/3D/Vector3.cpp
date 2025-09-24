///////////////////////////////////////////////////////////////////////////////
// Name               : Vector3.cpp
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

#include "Vector3.h"

#include <cfloat>
#include <cstdint>
#include <regex>
#include <sstream>

Vector3::Vector3(const std::string &string) {
	this->FromString(string);
}

void Vector3::Normalize() {
	double d = Abs();
	if (d > 0) {
		x /= d;
		y /= d;
		z /= d;
	}
}

Vector3 Vector3::Normal() const {
	double d = Abs();
	if (d > 0) {
		return Vector3(x / d, y / d, z / d);
	} else {
		return Vector3(x, y, z);
	}
}

Vector3 Vector3::Orthogonal() const {
	Vector3 temp;
	const double ax = fabs(x);
	const double ay = fabs(y);
	const double az = fabs(z);

	if (az >= ax && az >= ay)
		temp.Set(-z, 0, x);
	else
		temp.Set(y, -x, 0);
	temp *= (*this);
	return temp.Normal();
}

bool Vector3::operator ==(const Vector3 &b) const {
	if (this->x < b.x - DBL_EPSILON)
		return false;
	if (this->x > b.x + DBL_EPSILON)
		return false;
	if (this->y < b.y - DBL_EPSILON)
		return false;
	if (this->y > b.y + DBL_EPSILON)
		return false;
	if (this->z < b.z - DBL_EPSILON)
		return false;
	if (this->z > b.z + DBL_EPSILON)
		return false;
	return true;

}

std::string Vector3::ToString() const {
	std::ostringstream os;
	os << "{x:" << x << ",y:" << y << ",z:" << z << '}';
	return os.str();
}

void Vector3::FromString(const std::string &string) {
	size_t pos = 0;
	const std::string numchars("+-.ABCDEFINTXYabcdefintxy0123456789");
	for (uint_fast8_t i = 0; i < 3; i++) {
		const auto posStart = string.find_first_of(numchars, pos);
		if (posStart == std::string::npos)
			throw std::invalid_argument(
					"The string passed to FromString() of Vector3 does not contain 3 numbers that can be converted to double-type.");
		const auto posEnd = string.find_first_not_of(numchars, posStart);
		const auto count =
				(posEnd == std::string::npos) ?
						(string.length() - posStart) : (posEnd - posStart);
		const auto numberstring = string.substr(posStart, count);
		const double value = std::strtof(numberstring.c_str(), nullptr);
		switch (i) {
		case 0:
			x = value;
			break;
		case 1:
			y = value;
			break;
		case 2:
			z = value;
			break;
		}
		pos = posEnd;
	}
}


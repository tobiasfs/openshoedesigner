///////////////////////////////////////////////////////////////////////////////
// Name               : Triangle.cpp
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

#include "Triangle.h"

#include "AffineTransformMatrix.h"
#include <sstream>
#include <regex>
#include <cmath>

#include "OpenGL.h"

Triangle::Triangle(const std::string &string) {
	this->FromString(string);
}

void Triangle::ApplyTransformation(const AffineTransformMatrix &matrix) {
	p[0] = matrix.Transform(p[0]);
	p[1] = matrix.Transform(p[1]);
	p[2] = matrix.Transform(p[2]);

	const double sx = sqrt(
			matrix[0] * matrix[0] + matrix[4] * matrix[4]
					+ matrix[8] * matrix[8]);
	const double sy = sqrt(
			matrix[1] * matrix[1] + matrix[5] * matrix[5]
					+ matrix[9] * matrix[9]);
	const double sz = sqrt(
			matrix[2] * matrix[2] + matrix[6] * matrix[6]
					+ matrix[10] * matrix[10]);
	AffineTransformMatrix temp = matrix;
	temp.ScaleGlobal(1.0 / sx, 1.0 / sy, 1.0 / sz);
	n[0] = temp.TransformWithoutShift(n[0]);
	n[1] = temp.TransformWithoutShift(n[1]);
	n[2] = temp.TransformWithoutShift(n[2]);
}

void Triangle::CalculateNormal() {
	n[0] = (p[1] - p[0]) * (p[2] - p[1]);
	n[0].Normalize();
	n[1] = n[0];
	n[2] = n[0];
}

bool Triangle::FromString(const std::string &string) {
	//TODO: Move the regex into a global static variable when this function is really used a lot.
	// Regex for a Vector3: (\\{[^\\{]+\\})
	std::regex e(
			"^\\{p:\\[(\\{[^\\{]+\\}),(\\{[^\\{]+\\}),(\\{[^\\{]+\\})\\],n:\\[(\\{[^\\{]+\\}),(\\{[^\\{]+\\}),(\\{[^\\{]+\\})\\],c:\\[(\\{[^\\{]+\\}),(\\{[^\\{]+\\}),(\\{[^\\{]+\\})\\]\\}$");
	std::smatch sm;
	std::regex_match(string.begin(), string.end(), sm, e);

	if (sm.size() != 9)
		return false;
	p[0] = Vector3(sm[0]);
	p[1] = Vector3(sm[1]);
	p[2] = Vector3(sm[2]);
	n[0] = Vector3(sm[3]);
	n[1] = Vector3(sm[4]);
	n[2] = Vector3(sm[5]);
	c[0] = Vector3(sm[6]);
	c[1] = Vector3(sm[7]);
	c[2] = Vector3(sm[8]);
	return true;
}

std::string Triangle::ToString() const {
	std::ostringstream os;
	os << "{p:[";
	os << p[0].ToString() << ',' << p[1].ToString() << ',' << p[2].ToString();
	os << "],n:[";
	os << n[0].ToString() << ',' << n[1].ToString() << ',' << n[2].ToString();
	os << "],c:[";
	os << c[0].ToString() << ',' << c[1].ToString() << ',' << c[2].ToString();
	os << "]}";
	return os.str();
}

void Triangle::Paint(bool useNormals, bool useColors) const {
	for (uint_fast8_t i = 0; i < 3; i++) {
		if (useNormals)
			::glNormal3f(n[i].x, n[i].y, n[i].z);
		if (useColors)
			::glColor3f(c[i].x, c[i].y, c[i].z);
		::glVertex3f(p[i].x, p[i].y, p[i].z);
	}
}


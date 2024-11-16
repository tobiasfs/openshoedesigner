///////////////////////////////////////////////////////////////////////////////
// Name               : BoundingBox.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 22.06.2011
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

#include "BoundingBox.h"

#include "AffineTransformMatrix.h"
#include "Triangle.h"
#include "Vector3.h"

#include <cstdint>
#include <sstream>
#include <regex>

#include "OpenGL.h"

#define _USE_MATH_DEFINES
#include <cmath>

// TODO: Insert this elsewhere. The BoundingBox class should not know the Geometry class.
//! Insert a Geometry object into the box.
//	void Insert(const Geometry& geometry, const AffineTransformMatrix& matrix = AffineTransformMatrix::Identity());
//#include "Geometry.h"
//void BoundingBox::Insert(const Geometry &geometry,
//		const AffineTransformMatrix &matrix)
//{
//	for(auto tri : geometry.triangles){
//		tri.ApplyTransformation(matrix * geometry.matrix);
//		this->Insert(tri);
//	}
//}

BoundingBox::BoundingBox(const Vector3 &v1, const Vector3 &v2) {
	this->Set(v1, v2);
}

void BoundingBox::Empty() {
	xmax = ymax = zmax = -DBL_MAX;
	xmin = ymin = zmin = DBL_MAX;
}

void BoundingBox::Set(const Vector3 &v1, const Vector3 &v2) {
	if (v1.x < v2.x) {
		xmin = v1.x;
		xmax = v2.x;
	} else {
		xmin = v2.x;
		xmax = v1.x;
	}
	if (v1.y < v2.y) {
		ymin = v1.y;
		ymax = v2.y;
	} else {
		ymin = v2.y;
		ymax = v1.y;
	}
	if (v1.z < v2.z) {
		zmin = v1.z;
		zmax = v2.z;
	} else {
		zmin = v2.z;
		zmax = v1.z;
	}
}

void BoundingBox::Insert(const BoundingBox &bbox) {
	if (bbox.xmin < xmin)
		xmin = bbox.xmin;
	if (bbox.xmax > xmax)
		xmax = bbox.xmax;
	if (bbox.ymin < ymin)
		ymin = bbox.ymin;
	if (bbox.ymax > ymax)
		ymax = bbox.ymax;
	if (bbox.zmin < zmin)
		zmin = bbox.zmin;
	if (bbox.zmax > zmax)
		zmax = bbox.zmax;
}

void BoundingBox::Insert(const Vector3 &point) {
	if (point.x > xmax)
		xmax = point.x;
	if (point.x < xmin)
		xmin = point.x;
	if (point.y > ymax)
		ymax = point.y;
	if (point.y < ymin)
		ymin = point.y;
	if (point.z > zmax)
		zmax = point.z;
	if (point.z < zmin)
		zmin = point.z;
}

void BoundingBox::Insert(const Triangle &tri) {
	for (auto &p : tri.p) {
		this->Insert(p);
	}
}

BoundingBox& BoundingBox::operator +=(const BoundingBox &rhs) {
	this->Insert(rhs);
	return *this;
}

const BoundingBox BoundingBox::operator +(const BoundingBox &rhs) const {
	BoundingBox temp = *this;
	temp += rhs;
	return temp;
}
bool BoundingBox::IsEmpty() const {
	if (xmax < xmin)
		return true;
	if (ymax < ymin)
		return true;
	if (zmax < zmin)
		return true;
	return false;
}

bool BoundingBox::IsVolumeZero() const {
	if (xmax <= xmin)
		return true;
	if (ymax <= ymin)
		return true;
	if (zmax <= zmin)
		return true;
	return false;
}

void BoundingBox::Transform(const AffineTransformMatrix &matrix) {
	const Vector3 p0(this->xmin, this->ymin, this->zmin);
	const Vector3 p1(this->xmax, this->ymin, this->zmin);
	const Vector3 p2(this->xmin, this->ymax, this->zmin);
	const Vector3 p3(this->xmax, this->ymax, this->zmin);
	const Vector3 p4(this->xmin, this->ymin, this->zmax);
	const Vector3 p5(this->xmax, this->ymin, this->zmax);
	const Vector3 p6(this->xmin, this->ymax, this->zmax);
	const Vector3 p7(this->xmax, this->ymax, this->zmax);
	this->Empty();
	this->Insert(matrix.Transform(p0));
	this->Insert(matrix.Transform(p1));
	this->Insert(matrix.Transform(p2));
	this->Insert(matrix.Transform(p3));
	this->Insert(matrix.Transform(p4));
	this->Insert(matrix.Transform(p5));
	this->Insert(matrix.Transform(p6));
	this->Insert(matrix.Transform(p7));
}

void BoundingBox::SetSize(float sx, float sy, float sz, float origx,
		float origy, float origz) {
	xmin = origx;
	ymin = origy;
	zmin = origz;
	xmax = xmin + sx;
	ymax = ymin + sy;
	zmax = zmin + sz;
}

void BoundingBox::SetOrigin(float origx, float origy, float origz) {
	xmax = xmax - xmin + origx;
	ymax = ymax - ymin + origy;
	zmax = zmax - zmin + origz;
	xmin = origx;
	ymin = origy;
	zmin = origz;
}

void BoundingBox::SetOrigin(const Vector3 &orig) {
	SetOrigin(orig.x, orig.y, orig.z);
}

double BoundingBox::GetVolume() const {
	if (xmax <= xmin)
		return 0.0;
	if (ymax <= ymin)
		return 0.0;
	if (zmax <= zmin)
		return 0.0;
	return (xmax - xmin) * (ymax - ymin) * (zmax - zmin);
}

AffineTransformMatrix BoundingBox::GetCoordinateSystem() const {
	AffineTransformMatrix temp;
	temp.SetEx(Vector3(xmax - xmin, 0, 0));
	temp.SetEy(Vector3(0, ymax - ymin, 0));
	temp.SetEz(Vector3(0, 0, zmax - zmin));
	temp.SetOrigin(Vector3(xmin, ymin, zmin));
	return temp;
}

void BoundingBox::Paint(double overlap) const {
	if (xmax < xmin)
		return;
	if (ymax < ymin)
		return;
	if (zmax < zmin)
		return;

	glPushName(0);
	glBegin(GL_QUADS);

	glNormal3f(1, 0, 0);
	glVertex3f(xmax, ymax + overlap, zmax + overlap);
	glVertex3f(xmax, ymin - overlap, zmax + overlap);
	glVertex3f(xmax, ymin - overlap, zmin - overlap);
	glVertex3f(xmax, ymax + overlap, zmin - overlap);

	glNormal3f(-1, 0, 0);
	glVertex3f(xmin, ymax + overlap, zmax + overlap);
	glVertex3f(xmin, ymax + overlap, zmin - overlap);
	glVertex3f(xmin, ymin - overlap, zmin - overlap);
	glVertex3f(xmin, ymin - overlap, zmax + overlap);

	glNormal3f(0, 1, 0);
	glVertex3f(xmax + overlap, ymax, zmax + overlap);
	glVertex3f(xmax + overlap, ymax, zmin - overlap);
	glVertex3f(xmin - overlap, ymax, zmin - overlap);
	glVertex3f(xmin - overlap, ymax, zmax + overlap);

	glNormal3f(0, -1, 0);
	glVertex3f(xmax + overlap, ymin, zmax + overlap);
	glVertex3f(xmin - overlap, ymin, zmax + overlap);
	glVertex3f(xmin - overlap, ymin, zmin - overlap);
	glVertex3f(xmax + overlap, ymin, zmin - overlap);

	glNormal3f(0, 0, 1);
	glVertex3f(xmax + overlap, ymax + overlap, zmax);
	glVertex3f(xmin - overlap, ymax + overlap, zmax);
	glVertex3f(xmin - overlap, ymin - overlap, zmax);
	glVertex3f(xmax + overlap, ymin - overlap, zmax);

	glNormal3f(0, 0, -1);
	glVertex3f(xmax + overlap, ymax + overlap, zmin);
	glVertex3f(xmax + overlap, ymin - overlap, zmin);
	glVertex3f(xmin - overlap, ymin - overlap, zmin);
	glVertex3f(xmin - overlap, ymax + overlap, zmin);

	glEnd();
	glPopName();
}

void BoundingBox::PaintEdges(double cornerLength,
		unsigned int edgewidth) const {
	if (xmax < xmin)
		return;
	if (ymax < ymin)
		return;
	if (zmax < zmin)
		return;

	const double dx = GetSizeX() * cornerLength;
	const double dy = GetSizeY() * cornerLength;
	const double dz = GetSizeZ() * cornerLength;

	glPushName(0);

	glLineWidth(edgewidth);
	glBegin(GL_LINES);

	glNormal3d(0, -M_SQRT1_2, -M_SQRT1_2);
	glVertex3d(xmin, ymin, zmin);
	glVertex3d(xmin + dx, ymin, zmin);
	glVertex3d(xmax, ymin, zmin);
	glVertex3d(xmax - dx, ymin, zmin);
	glNormal3d(0, M_SQRT1_2, -M_SQRT1_2);
	glVertex3d(xmin, ymax, zmin);
	glVertex3d(xmin + dx, ymax, zmin);
	glVertex3d(xmax, ymax, zmin);
	glVertex3d(xmax - dx, ymax, zmin);
	glNormal3d(0, M_SQRT1_2, M_SQRT1_2);
	glVertex3d(xmin, ymax, zmax);
	glVertex3d(xmin + dx, ymax, zmax);
	glVertex3d(xmax, ymax, zmax);
	glVertex3d(xmax - dx, ymax, zmax);
	glNormal3d(0, -M_SQRT1_2, M_SQRT1_2);
	glVertex3d(xmin, ymin, zmax);
	glVertex3d(xmin + dx, ymin, zmax);
	glVertex3d(xmax, ymin, zmax);
	glVertex3d(xmax - dx, ymin, zmax);

	glNormal3d(-M_SQRT1_2, 0, -M_SQRT1_2);
	glVertex3d(xmin, ymin, zmin);
	glVertex3d(xmin, ymin + dy, zmin);
	glVertex3d(xmin, ymax, zmin);
	glVertex3d(xmin, ymax - dy, zmin);
	glNormal3d(M_SQRT1_2, 0, -M_SQRT1_2);
	glVertex3d(xmax, ymin, zmin);
	glVertex3d(xmax, ymin + dy, zmin);
	glVertex3d(xmax, ymax, zmin);
	glVertex3d(xmax, ymax - dy, zmin);
	glNormal3d(M_SQRT1_2, 0, M_SQRT1_2);
	glVertex3d(xmax, ymin, zmax);
	glVertex3d(xmax, ymin + dy, zmax);
	glVertex3d(xmax, ymax, zmax);
	glVertex3d(xmax, ymax - dy, zmax);
	glNormal3d(-M_SQRT1_2, 0, M_SQRT1_2);
	glVertex3d(xmin, ymin, zmax);
	glVertex3d(xmin, ymin + dy, zmax);
	glVertex3d(xmin, ymax, zmax);
	glVertex3d(xmin, ymax - dy, zmax);

	glNormal3d(-M_SQRT1_2, -M_SQRT1_2, 0);
	glVertex3d(xmin, ymin, zmin);
	glVertex3d(xmin, ymin, zmin + dz);
	glVertex3d(xmin, ymin, zmax);
	glVertex3d(xmin, ymin, zmax - dz);
	glNormal3d(M_SQRT1_2, -M_SQRT1_2, 0);
	glVertex3d(xmax, ymin, zmin);
	glVertex3d(xmax, ymin, zmin + dz);
	glVertex3d(xmax, ymin, zmax);
	glVertex3d(xmax, ymin, zmax - dz);
	glNormal3d(M_SQRT1_2, M_SQRT1_2, 0);
	glVertex3d(xmax, ymax, zmin);
	glVertex3d(xmax, ymax, zmin + dz);
	glVertex3d(xmax, ymax, zmax);
	glVertex3d(xmax, ymax, zmax - dz);
	glNormal3d(-M_SQRT1_2, M_SQRT1_2, 0);
	glVertex3d(xmin, ymax, zmin);
	glVertex3d(xmin, ymax, zmin + dz);
	glVertex3d(xmin, ymax, zmax);
	glVertex3d(xmin, ymax, zmax - dz);

	glEnd();
	glLineWidth(1);
	glPopName();
}

Vector3 BoundingBox::GetSize() const {
	return Vector3(GetSizeX(), GetSizeY(), GetSizeZ());
}

double BoundingBox::GetSize(const Vector3 &n) const {

	double dx = fabs(n.x) > FLT_EPSILON ? (GetSizeX() / fabs(n.x)) : 1e8;
	double dy = fabs(n.y) > FLT_EPSILON ? (GetSizeY() / fabs(n.y)) : 1e8;
	double dz = fabs(n.z) > FLT_EPSILON ? (GetSizeZ() / fabs(n.z)) : 1e8;
	return fmin(fmin(dx, dy), dz) / 2.0;
}

void BoundingBox::PaintVertices(unsigned int extrapoints,
		unsigned int pointsize) const {
	if (xmax < xmin)
		return;
	if (ymax < ymin)
		return;
	if (zmax < zmin)
		return;
	const double dx = GetSizeX() / (1 + extrapoints);
	const double dy = GetSizeY() / (1 + extrapoints);
	const double dz = GetSizeZ() / (1 + extrapoints);
	glPointSize(pointsize);

	unsigned int N = 1 + extrapoints;
	unsigned int c = 0;
	glPushName(c);
	double z = zmin;
	for (unsigned int k = 0; k <= N; ++k) {
		double y = ymin;
		for (unsigned int j = 0; j <= N; ++j) {
			double x = xmin;
			for (unsigned int i = 0; i <= N; ++i) {
				if (i == 0 || j == 0 || k == 0 || i == N || j == N || k == N) {
					glLoadName(c);
					glBegin(GL_POINTS);
					glVertex3d(x, y, z);
					glEnd();
				}
				++c;
				x += dx;
			}
			y += dy;
		}
		z += dz;
	}
	glPopName();
	glPointSize(1);
}

std::string BoundingBox::ToString() const {
	std::ostringstream os;
	os << "{x:[" << xmin << ',' << xmax << "],";
	os << "y:[" << ymin << ',' << ymax << "],";
	os << "z:[" << zmin << ',' << zmax << "]}";
	return os.str();
}

bool BoundingBox::FromString(const std::string &string) {
	//TODO: Move the regex into a global static variable when it is really used a lot.
	std::regex e(
			"^\\{x:\\[([^,\\]]+),([^,\\]]+)\\],y:\\[([^,\\]]+),([^,\\]]+)\\],z:\\[([^,\\]]+),([^,\\]]+)\\]\\}");
	std::smatch sm;
	std::regex_match(string.begin(), string.end(), sm, e);
	if (sm.size() != 6)
		return false;
	xmin = std::stof(sm[0]);
	xmax = std::stof(sm[1]);
	ymin = std::stof(sm[2]);
	ymax = std::stof(sm[3]);
	zmin = std::stof(sm[4]);
	zmax = std::stof(sm[5]);
	return true;
}

bool BoundingBox::Overlaps(const BoundingBox &other) const {
	if (other.xmin > xmax)
		return false;
	if (other.xmax < xmin)
		return false;
	if (other.ymin > ymax)
		return false;
	if (other.ymax < ymin)
		return false;
	if (other.zmin > zmax)
		return false;
	if (other.zmax < zmin)
		return false;
	return true;
}

bool BoundingBox::IsInside(const Vector3 &v) const {
	if (v.x < xmin)
		return false;
	if (v.x > xmax)
		return false;
	if (v.y < ymin)
		return false;
	if (v.y > ymax)
		return false;
	if (v.z < zmin)
		return false;
	if (v.z > zmax)
		return false;
	return true;
}

Vector3 BoundingBox::GetCenter() const {
	return Vector3((xmin + xmax) / 2.0, (ymin + ymax) / 2.0,
			(zmin + zmax) / 2.0);
}

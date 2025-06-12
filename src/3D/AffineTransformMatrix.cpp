///////////////////////////////////////////////////////////////////////////////
// Name               : AffineTransformMatrix.cpp
// Purpose            : A class to store a 3D affine transform matrix and provide operations upon.
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 22.07.2009
// Copyright          : (C) 2009 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "AffineTransformMatrix.h"

#include "Vector3.h"

#include <cfloat>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

#ifdef DONT_USE_OPENGL_H
#include <GL/gl.h>
#else
#include "OpenGL.h"
#endif

#define _USE_MATH_DEFINES
#include <cmath>

AffineTransformMatrix::AffineTransformMatrix() {
	SetIdentity();
}

AffineTransformMatrix::AffineTransformMatrix(Orientation orientation_) :
		orientation(orientation_) {
	SetIdentity();
}

AffineTransformMatrix::AffineTransformMatrix(const Vector3 &ex,
		const Vector3 &ey, const Vector3 &ez, const Vector3 &origin) {
	a[0] = ex.x;
	a[1] = ex.y;
	a[2] = ex.z;
	a[3] = 0.0;
	a[4] = ey.x;
	a[5] = ey.y;
	a[6] = ey.z;
	a[7] = 0.0;
	a[8] = ez.x;
	a[9] = ez.y;
	a[10] = ez.z;
	a[11] = 0.0;
	a[12] = origin.x;
	a[13] = origin.y;
	a[14] = origin.z;
	a[15] = 1.0;
	orientation = ((ex * ey).Dot(ez) > 0) ? Orientation::RHS : Orientation::LHS;
}

AffineTransformMatrix::AffineTransformMatrix(const std::string &numbers) {
	size_t pos = 0;
	const std::string numchars("+-.ABCDEFINTXYabcdefintxy0123456789");
	for (uint_fast8_t i = 0; i < 16; ++i) {
		const auto posStart = numbers.find_first_of(numchars, pos);
		if (posStart == std::string::npos)
			throw std::invalid_argument(
					"The string passed to the constructor of the AffineTransformMatrix does not contain 16 numbers that can be converted to double-type.");
		const auto posEnd = numbers.find_first_not_of(numchars, posStart);
		const auto count =
				(posEnd == std::string::npos) ?
						(numbers.length() - posStart) : (posEnd - posStart);
		const auto numberstring = numbers.substr(posStart, count);
		a[i] = std::strtod(numberstring.c_str(), nullptr);
		pos = posEnd;
	}
	UpdateOrientation();
}

void AffineTransformMatrix::SetOrientation(Orientation orientation_) {
	this->orientation = orientation_;
}

AffineTransformMatrix::Orientation AffineTransformMatrix::GetOrientation() const {
	return orientation;
}

AffineTransformMatrix::Orientation AffineTransformMatrix::CheckOrientation() const {
	const double x = a[1] * a[6] - a[2] * a[5];
	const double y = a[2] * a[4] - a[0] * a[6];
	const double z = a[0] * a[5] - a[1] * a[4];
	const double c = x * a[8] + y * a[9] + z * a[10];
	return (c >= 0.0) ? Orientation::RHS : Orientation::LHS;
}

void AffineTransformMatrix::UpdateOrientation() {
	orientation = CheckOrientation();
}

void AffineTransformMatrix::SetIdentity() {
	for (uint_fast8_t i = 0; i < 16; ++i)
		a[i] = 0;
	a[0] = 1.0;
	a[5] = 1.0;
	a[10] = (orientation == Orientation::RHS) ? 1.0 : -1.0;
	a[15] = 1.0;
}

void AffineTransformMatrix::SetZero() {
	a.fill(0.0);
}

void AffineTransformMatrix::ResetRotationAndScale() {
	for (uint_fast8_t i = 0; i < 12; ++i)
		a[i] = 0;
	a[0] = 1.0;
	a[5] = 1.0;
	a[10] = (orientation == Orientation::RHS) ? 1.0 : -1.0;
	a[15] = 1.0;
}

double& AffineTransformMatrix::operator [](unsigned char index) {
	if (index >= 16)
		throw(std::out_of_range(
		__FILE__ " operator[] - Out of range."));
	return a[index];
}

double AffineTransformMatrix::operator [](unsigned char index) const {
	if (index >= 16)
		throw(std::out_of_range(
		__FILE__ " operator[] - Out of range."));
	return a[index];
}

void AffineTransformMatrix::SetOrigin(const Vector3 &origin) {
	a[12] = origin.x;
	a[13] = origin.y;
	a[14] = origin.z;
}

void AffineTransformMatrix::SetEx(const Vector3 &ex) {
	a[0] = ex.x;
	a[1] = ex.y;
	a[2] = ex.z;
}

void AffineTransformMatrix::SetEy(const Vector3 &ey) {
	a[4] = ey.x;
	a[5] = ey.y;
	a[6] = ey.z;
}

void AffineTransformMatrix::SetEz(const Vector3 &ez) {
	a[8] = ez.x;
	a[9] = ez.y;
	a[10] = ez.z;
}

void AffineTransformMatrix::CalculateEx() {
	if (orientation == Orientation::RHS) {
		a[0] = a[5] * a[10] - a[6] * a[9];
		a[1] = a[6] * a[8] - a[4] * a[10];
		a[2] = a[4] * a[9] - a[5] * a[8];
	} else {
		a[0] = a[9] * a[6] - a[10] * a[5];
		a[1] = a[10] * a[4] - a[8] * a[6];
		a[2] = a[8] * a[5] - a[9] * a[4];
	}
}

void AffineTransformMatrix::CalculateEy() {
	if (orientation == Orientation::RHS) {
		a[4] = a[9] * a[2] - a[10] * a[1];
		a[5] = a[10] * a[0] - a[8] * a[2];
		a[6] = a[8] * a[1] - a[9] * a[0];
	} else {
		a[4] = a[1] * a[10] - a[2] * a[9];
		a[5] = a[2] * a[8] - a[0] * a[10];
		a[6] = a[0] * a[9] - a[1] * a[8];
	}
}

void AffineTransformMatrix::CalculateEz() {
	if (orientation == Orientation::RHS) {
		a[8] = a[1] * a[6] - a[2] * a[5];
		a[9] = a[2] * a[4] - a[0] * a[6];
		a[10] = a[0] * a[5] - a[1] * a[4];
	} else {
		a[8] = a[5] * a[2] - a[6] * a[1];
		a[9] = a[6] * a[0] - a[4] * a[2];
		a[10] = a[4] * a[1] - a[5] * a[0];
	}
}

AffineTransformMatrix AffineTransformMatrix::Normal() const {
	const double v0 = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	const double v1 = a[4] * a[4] + a[5] * a[5] + a[6] * a[6];
	const double v2 = a[8] * a[8] + a[9] * a[9] + a[10] * a[10];
	AffineTransformMatrix temp = *this;
	if (v0 > FLT_EPSILON) {
		const double sv0 = sqrt(v0);
		temp.a[0] /= sv0;
		temp.a[1] /= sv0;
		temp.a[2] /= sv0;
	}
	if (v1 > FLT_EPSILON) {
		const double sv1 = sqrt(v1);
		temp.a[4] /= sv1;
		temp.a[5] /= sv1;
		temp.a[6] /= sv1;
	}
	if (v2 > FLT_EPSILON) {
		const double sv2 = sqrt(v2);
		temp.a[8] /= sv2;
		temp.a[9] /= sv2;
		temp.a[10] /= sv2;
	}
	return temp;
}

void AffineTransformMatrix::Normalize() {
	(*this) = (*this).Normal();
}

void AffineTransformMatrix::Orthogonalize() {
	Vector3 ex = GetEx().Normal();
	Vector3 ey = GetEy().Normal();
	Vector3 ez = GetEz().Normal();
	const double hy = ey.Dot(ex);
	ey = (ey - hy * ex).Normal();
	const double hzx = ez.Dot(ex);
	ez = (ez - hzx * ex).Normal();
	const double hzy = ez.Dot(ey);
	ez = (ez - hzy * ey).Normal();
	SetEx(ex);
	SetEy(ey);
	SetEz(ez);
}

Vector3 AffineTransformMatrix::GetOrigin() const {
	return Vector3(a[12], a[13], a[14]);
}

Vector3 AffineTransformMatrix::GetEx() const {
	return Vector3(a[0], a[1], a[2]);
}

Vector3 AffineTransformMatrix::GetEy() const {
	return Vector3(a[4], a[5], a[6]);
}

Vector3 AffineTransformMatrix::GetEz() const {
	return Vector3(a[8], a[9], a[10]);
}

Vector3 AffineTransformMatrix::GetVectorOfRotation() const {
	return Vector3(a[6] - a[9], a[8] - a[2], a[1] - a[4]);
}

AffineTransformMatrix AffineTransformMatrix::GetNormalMatrix() const {
	// Fricas code:
	// )set fortran optlevel 2
	// )set output fortran on
	// R:=matrix([[a[0],a[4],a[8]],[a[1],a[5],a[9]],[a[2],a[6],a[10]]])
	// transpose(inverse(R))
	AffineTransformMatrix ret;
	const double T2 = a[5];
	const double T3 = a[10];
	const double T4 = a[0];
	const double T5 = a[6];
	const double T6 = a[4];
	const double T7 = a[9];
	const double T8 = a[1];
	const double T9 = a[2];
	const double T12 = a[8];
	const double T10 = (T4 * T2 - T8 * T6) * T3 + (-T4 * T5 + T9 * T6) * T7
			+ (T8 * T5 - T9 * T2) * T12;
	if (fabs(T10) < DBL_MIN)
		throw(std::logic_error(
				__FILE__ " GetNormalMatrix() - Matrix is broken can cannot be inverted and transposed."));
	const double T15 = 1.0 / T10;
	const double T11 = T8 * T5 - T9 * T2;
	const double T13 = -T4 * T5 + T9 * T6;
	const double T14 = T4 * T2 - T8 * T6;
	ret.a[0] = (T2 * T3 - T5 * T7) * T15;
	ret.a[1] = (-T6 * T3 + T5 * T12) * T15;
	ret.a[2] = (T6 * T7 - T2 * T12) * T15;
	ret.a[4] = (-T8 * T3 + T9 * T7) * T15;
	ret.a[5] = (T4 * T3 - T9 * T12) * T15;
	ret.a[6] = (-T4 * T7 + T8 * T12) * T15;
	ret.a[8] = T11 * T15;
	ret.a[9] = T13 * T15;
	ret.a[10] = T14 * T15;
	return ret;
}

AffineTransformMatrix& AffineTransformMatrix::operator*=(
		const AffineTransformMatrix &b) {
	//Generated with this code:
	//php -r'for($i=0;$i<4;$i++){for($j=0;$j<4;$j++){printf("this->a[%u]=",$i*4+$j);for($k=0;$k<4;$k++){printf("c[%u]*b.a[%u]%s",$k*4+$j,$i*4+$k,($k==3)?";\r\n":"+");}}}'

	// The php code generates all combinations.
	// The axiom code optimizes it a little more, because b.a[3], b.a[7] and b.a[11] are
	// zero and b.a[15] is one.

	// Fricas:
	// this:=matrix([[c[0],c[4],c[8],c[12]],[c[1],c[5],c[9],c[13]],[c[2],c[6],c[10],c[14]],[0,0,0,1]]);
	// b:=matrix([[ba[0],ba[4],ba[8],ba[12]],[ba[1],ba[5],ba[9],ba[13]],[ba[2],ba[6],ba[10],ba[14]],[0,0,0,1]]);

	double c[16];
	for (uint_fast8_t i = 0; i < 16; i++)
		c[i] = this->a[i];

	this->a[0] = c[0] * b.a[0] + c[4] * b.a[1] + c[8] * b.a[2];
	this->a[1] = c[1] * b.a[0] + c[5] * b.a[1] + c[9] * b.a[2];
	this->a[2] = c[2] * b.a[0] + c[6] * b.a[1] + c[10] * b.a[2];
	this->a[3] = 0;

	this->a[4] = c[0] * b.a[4] + c[4] * b.a[5] + c[8] * b.a[6];
	this->a[5] = c[1] * b.a[4] + c[5] * b.a[5] + c[9] * b.a[6];
	this->a[6] = c[2] * b.a[4] + c[6] * b.a[5] + c[10] * b.a[6];
	this->a[7] = 0;

	this->a[8] = c[0] * b.a[8] + c[4] * b.a[9] + c[8] * b.a[10];
	this->a[9] = c[1] * b.a[8] + c[5] * b.a[9] + c[9] * b.a[10];
	this->a[10] = c[2] * b.a[8] + c[6] * b.a[9] + c[10] * b.a[10];
	this->a[11] = 0;

	this->a[12] = c[0] * b.a[12] + c[4] * b.a[13] + c[8] * b.a[14] + c[12];
	this->a[13] = c[1] * b.a[12] + c[5] * b.a[13] + c[9] * b.a[14] + c[13];
	this->a[14] = c[2] * b.a[12] + c[6] * b.a[13] + c[10] * b.a[14] + c[14];
	this->a[15] = 1;

	return *this;
}

AffineTransformMatrix AffineTransformMatrix::operator*(
		const AffineTransformMatrix &b) const {
	AffineTransformMatrix c = *this;
	c *= b;
	return c;
}

AffineTransformMatrix& AffineTransformMatrix::operator/=(
		const AffineTransformMatrix &b) {
	(*this) = (*this) * (b.Inverse());
	return *this;
}

AffineTransformMatrix AffineTransformMatrix::operator/(
		const AffineTransformMatrix &b) const {
	AffineTransformMatrix c = *this;
	c /= b;
	return c;
}

AffineTransformMatrix AffineTransformMatrix::Inverse() const {
	// Fricas code:
	// )set fortran optlevel 2
	// )set output fortran on
	// R:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]]);
	// inverse(R)

	const double T30 = a[0] * a[5];
	const double T32 = a[0] * a[6];
	const double T34 = a[1] * a[6];
	const double T31 = a[1] * a[4];
	const double T33 = a[2] * a[4];
	const double T35 = a[2] * a[5];

	const double T11 = (T30 - T31) * a[10] + (T33 - T32) * a[9]
			+ (T34 - T35) * a[8];

	// T11 is the determinant of the matrix. This can
	// not be zero for a correct transformation matrix.
	if (fabs(T11) < DBL_MIN)
		throw(std::logic_error(
		__FILE__ " Inverse() - Matrix is broken and cannot be inverted."));

	const double T27 = T34 - T35;
	const double T28 = T33 - T32;
	const double T29 = T30 - T31;

	const double T18 = a[0] * a[9];
	const double T21 = a[0] * a[10];
	const double T24 = a[1] * a[10];
	const double T19 = a[1] * a[8];
	const double T22 = a[2] * a[8];
	const double T25 = a[2] * a[9];
	const double T12 = a[4] * a[9];
	const double T14 = a[4] * a[10];
	const double T16 = a[5] * a[10];
	const double T13 = a[5] * a[8];
	const double T15 = a[6] * a[8];
	const double T17 = a[6] * a[9];

	AffineTransformMatrix b;

	b.a[0] = (T16 - T17) / T11;
	b.a[4] = (-T14 + T15) / T11;
	b.a[8] = (T12 - T13) / T11;
	b.a[12] =
			((-T12 + T13) * a[14] + (T14 - T15) * a[13] + (-T16 + T17) * a[12])
					/ T11;
	b.a[1] = (-T24 + T25) / T11;
	b.a[5] = (T21 - T22) / T11;
	b.a[9] = (-T18 + T19) / T11;
	b.a[13] =
			((T18 - T19) * a[14] + (-T21 + T22) * a[13] + (-T25 + T24) * a[12])
					/ T11;
	b.a[2] = T27 / T11;
	b.a[6] = T28 / T11;
	b.a[10] = T29 / T11;
	b.a[14] =
			((-T30 + T31) * a[14] + (T32 - T33) * a[13] + (-T34 + T35) * a[12])
					/ T11;
	b.a[3] = 0;
	b.a[7] = 0;
	b.a[11] = 0;
	b.a[15] = 1;
	return b;
}

void AffineTransformMatrix::Invert() {
	(*this) = (*this).Inverse();
}

AffineTransformMatrix AffineTransformMatrix::Identity() {
	AffineTransformMatrix ret;
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::Zero() {
	return AffineTransformMatrix( { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
			0 });
}

AffineTransformMatrix AffineTransformMatrix::Translation(double x, double y,
		double z) {
	AffineTransformMatrix ret;
	ret[12] = x;
	ret[13] = y;
	ret[14] = z;
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::Translation(Vector3 translation) {
	AffineTransformMatrix ret;
	ret[12] = translation.x;
	ret[13] = translation.y;
	ret[14] = translation.z;
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::Scaling(double scale) {
	AffineTransformMatrix ret;
	ret[0] = scale;
	ret[5] = scale;
	ret[10] = scale;
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::Scaling(double x, double y,
		double z) {
	AffineTransformMatrix ret;
	ret[0] = x;
	ret[5] = y;
	ret[10] = z;
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::Scaling(Vector3 scale) {
	AffineTransformMatrix ret;
	ret[0] = scale.x;
	ret[5] = scale.y;
	ret[10] = scale.z;
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::RotationAroundVector(
		Vector3 const &vector, double phi) {
	const double c = cos(phi);
	const double s = sin(phi);
	const double t = 1 - c;

	Vector3 v(vector);
	v.Normalize();

	AffineTransformMatrix ret;

	ret.a[0] = t * v.x * v.x + c;
	ret.a[1] = t * v.x * v.y + s * v.z;
	ret.a[2] = t * v.x * v.z - s * v.y;

	ret.a[4] = t * v.x * v.y - s * v.z;
	ret.a[5] = t * v.y * v.y + c;
	ret.a[6] = t * v.y * v.z + s * v.x;

	ret.a[8] = t * v.x * v.z + s * v.y;
	ret.a[9] = t * v.y * v.z - s * v.x;
	ret.a[10] = t * v.z * v.z + c;

	return ret;
}

AffineTransformMatrix AffineTransformMatrix::RotationXY(int x, int y,
		double scale) {

	const double dx = (double) x / scale;
	const double dy = (double) y / scale;

	const double dist = sqrt(dx * dx + dy * dy);

	AffineTransformMatrix ret;

	if (dist > 0.001) {
		const double ang = -atan2(dy, dx);

		const double coy = cos(dist / 100);
		const double siy = sin(dist / 100);
		const double coz = cos(ang);
		const double siz = sin(ang);

		ret.a[0] = coz * coz * coy + siz * siz;
		ret.a[1] = coz * siz * coy - coz * siz;
		ret.a[2] = -coz * siy;
		ret.a[4] = siz * coz * coy - coz * siz;
		ret.a[5] = siz * siz * coy + coz * coz;
		ret.a[6] = -siz * siy;
		ret.a[8] = coz * siy;
		ret.a[9] = siz * siy;
		ret.a[10] = coy;
	}
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::RotationXYZ(double x, double y,
		double z) {
	AffineTransformMatrix ret;

	//Fricas code:
	// Rx := matrix[[1,0,0,0],[0,cox,-six,0],[0,six,cox,0],[0,0,0,1]]
	// Ry := matrix[[coy,0,siy,0],[0,1,0,0],[-siy,0,coy,0],[0,0,0,1]]
	// Rz := matrix[[coz,-siz,0,0],[siz,coz,0,0],[0,0,1,0],[0,0,0,1]]
	// Rz*Ry*Rx

	const double six = sin(x);
	const double siy = sin(y);
	const double siz = sin(z);
	const double cox = cos(x);
	const double coy = cos(y);
	const double coz = cos(z);

	ret.a[0] = coy * coz;
	ret.a[1] = coy * siz;
	ret.a[2] = -siy;

	ret.a[4] = -cox * siz + coz * six * siy;
	ret.a[5] = six * siy * siz + cox * coz;
	ret.a[6] = coy * six;

	ret.a[8] = six * siz + cox * coz * siy;
	ret.a[9] = cox * siy * siz - coz * six;
	ret.a[10] = cox * coy;

	return ret;
}

AffineTransformMatrix AffineTransformMatrix::RotationInterwoven(double x,
		double y, double z) {
	const double alpha = sqrt(x * x + y * y + z * z);
	if (fabs(alpha) < DBL_MIN)
		return AffineTransformMatrix::Identity();
	const Vector3 R(x / alpha, y / alpha, z / alpha);
	return AffineTransformMatrix::RotationAroundVector(R, alpha);
}

AffineTransformMatrix AffineTransformMatrix::RotationTrackball(const double x1,
		const double y1, const double x2, const double y2, const double r) {
	Vector3 r1(x1, y1, 0);
	r1 /= r;
	const double d1 = r1.Abs2();
	if (d1 >= 1.0) {
		r1 /= sqrt(d1);
	} else {
		r1.z = sqrt(1.0 - d1);
	}
	Vector3 r2(x2, y2, 0.0);
	r2 /= r;
	const double d2 = r2.Abs2();
	if (d2 >= 1.0) {
		r2 /= sqrt(d2);
	} else {
		r2.z = sqrt(1.0 - d2);
	}
	const Vector3 A = r1 * r2;
	const double alpha = asin(A.Abs());
	return AffineTransformMatrix::RotationAroundVector(A, alpha);
}

AffineTransformMatrix AffineTransformMatrix::Perspective(double fovy,
		double aspect, double znear, double zfar) {
	AffineTransformMatrix ret;
	const double top = znear * tan(fovy / 2.0);
	const double bottom = -top;
	const double right = top * aspect;
	const double left = -right;

	ret[0] = 2 * znear / (right - left);
	ret[5] = 2 * znear / (top - bottom);
	// 8-11 are negativ for OpenGL, because z towards viewer
	// (RHS coordinate system)
	ret[8] = (right + left) / (right - left);
	ret[9] = (top + bottom) / (top - bottom);
	ret[10] = -(zfar + znear) / (zfar - znear);
	ret[11] = -1;
	ret[14] = -2 * zfar * znear / (zfar - znear);
	ret[15] = 0.0; // 0 for perspective, 1 for orthogonal
	return ret;
}

AffineTransformMatrix AffineTransformMatrix::Orthogonal(double left,
		double right, double bottom, double top, double znear, double zfar) {
	AffineTransformMatrix ret;
	ret[0] = 2.0 / (right - left);
	ret[5] = 2.0 / (top - bottom);
	ret[10] = -2.0 / (zfar - znear); // negativ for OpenGL

	ret[12] = -(right + left) / (right - left);
	ret[13] = -(top + bottom) / (top - bottom);
	ret[14] = -(zfar + znear) / (zfar - znear);
	ret[15] = 1.0;
	return ret;
}

void AffineTransformMatrix::TranslateGlobal(double x, double y, double z) {
	// Fricas code:
	// this:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]]);
	// T:=matrix([[1,0,0,x],[0,1,0,y],[0,0,1,z],[0,0,0,1]]);
	// T*this
	a[12] += x;
	a[13] += y;
	a[14] += z;
}

void AffineTransformMatrix::TranslateLocal(double x, double y, double z) {
	// Fricas code:
	// this:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]]);
	// T:=matrix([[1,0,0,x],[0,1,0,y],[0,0,1,z],[0,0,0,1]]);
	// this*T
	a[12] += x * a[0] + y * a[4] + z * a[8];
	a[13] += x * a[1] + y * a[5] + z * a[9];
	a[14] += x * a[2] + y * a[6] + z * a[10];
}

void AffineTransformMatrix::ScaleGlobal(double x, double y, double z) {
	// Fricas code:
	// this:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]]);
	// S:=matrix([[x,0,0,0],[0,y,0,0],[0,0,z,0],[0,0,0,1]]);
	// S*this

	a[0] *= x;
	a[1] *= y;
	a[2] *= z;
	a[4] *= x;
	a[5] *= y;
	a[6] *= z;
	a[8] *= x;
	a[9] *= y;
	a[10] *= z;
	a[12] *= x;
	a[13] *= y;
	a[14] *= z;
}

void AffineTransformMatrix::ScaleLocal(double x, double y, double z) {
	// Fricas code:
	// this:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]]);
	// S:=matrix([[x,0,0,0],[0,y,0,0],[0,0,z,0],[0,0,0,1]]);
	// this*S

	a[0] *= x;
	a[1] *= x;
	a[2] *= x;
	a[4] *= y;
	a[5] *= y;
	a[6] *= y;
	a[8] *= z;
	a[9] *= z;
	a[10] *= z;
}

void AffineTransformMatrix::ShiftTransformPosition(const Vector3 &p) {
	// Fricas code:
	// this:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]]);
	// T:=matrix([[1,0,0,x],[0,1,0,y],[0,0,1,z],[0,0,0,1]]);
	// U:=matrix([[1,0,0,-x],[0,1,0,-y],[0,0,1,-z],[0,0,0,1]]);
	// T*this*U

	a[12] = (-a[8] * p.z) + (-a[4] * p.y) + (-a[0] + 1.0) * p.x + a[12];
	a[13] = (-a[9] * p.z) + (-a[5] + 1.0) * p.y + (-a[1] * p.x) + a[13];
	a[14] = (-a[10] + 1.0) * p.z + (-a[6] * p.y) + (-a[2] * p.x) + a[14];
}

Vector3 AffineTransformMatrix::Transform(const Vector3 &v) const {
	//Fricas code:
	// R:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]])
	// V:=matrix([[x],[y],[z],[1]])
	// R*V

	Vector3 temp;
	temp.x = a[0] * v.x + a[4] * v.y + a[8] * v.z + a[12];
	temp.y = a[1] * v.x + a[5] * v.y + a[9] * v.z + a[13];
	temp.z = a[2] * v.x + a[6] * v.y + a[10] * v.z + a[14];
	return temp;
}

Vector3 AffineTransformMatrix::Transform(const double x, const double y,
		const double z) const {
	Vector3 temp;
	temp.x = a[0] * x + a[4] * y + a[8] * z + a[12];
	temp.y = a[1] * x + a[5] * y + a[9] * z + a[13];
	temp.z = a[2] * x + a[6] * y + a[10] * z + a[14];
	return temp;
}

Vector3 AffineTransformMatrix::TransformWithoutShift(const Vector3 &v) const {
	//Fricas code:
	// R:=matrix([[a[0],a[4],a[8],0],[a[1],a[5],a[9],0],[a[2],a[6],a[10],0],[0,0,0,1]])
	// V:=matrix([[x],[y],[z],[1]])
	// R*V

	Vector3 temp;
	temp.x = a[0] * v.x + a[4] * v.y + a[8] * v.z;
	temp.y = a[1] * v.x + a[5] * v.y + a[9] * v.z;
	temp.z = a[2] * v.x + a[6] * v.y + a[10] * v.z;
	return temp;
}

Vector3 AffineTransformMatrix::TransformWithoutShift(const double x,
		const double y, const double z) const {
	Vector3 temp;
	temp.x = a[0] * x + a[4] * y + a[8] * z;
	temp.y = a[1] * x + a[5] * y + a[9] * z;
	temp.z = a[2] * x + a[6] * y + a[10] * z;
	return temp;
}

Vector3 AffineTransformMatrix::operator *(const Vector3 &v) const {
	return Transform(v);
}

Vector3 AffineTransformMatrix::operator ()(const Vector3 &v) const {
	return Transform(v);
}

Vector3 AffineTransformMatrix::operator ()(const double x, const double y,
		const double z) const {
	return Transform(x, y, z);
}

double AffineTransformMatrix::LocalX(const Vector3 &v) const {
	const double den = (a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
	return ((v.x - a[12]) * a[0] + (v.y - a[13]) * a[1] + (v.z - a[14]) * a[2])
			/ den;
}

double AffineTransformMatrix::LocalY(const Vector3 &v) const {
	const double den = (a[4] * a[4] + a[5] * a[5] + a[6] * a[6]);
	return ((v.x - a[12]) * a[4] + (v.y - a[13]) * a[5] + (v.z - a[14]) * a[6])
			/ den;
}

double AffineTransformMatrix::LocalZ(const Vector3 &v) const {
	const double den = (a[8] * a[8] + a[9] * a[9] + a[10] * a[10]);
	return ((v.x - a[12]) * a[8] + (v.y - a[13]) * a[9] + (v.z - a[14]) * a[10])
			/ den;
}

double AffineTransformMatrix::GlobalX(double x, double y, double z) const {
	return a[0] * x + a[4] * y + a[8] * z + a[12];
}

double AffineTransformMatrix::GlobalY(double x, double y, double z) const {
	return a[1] * x + a[5] * y + a[9] * z + a[13];
}

double AffineTransformMatrix::GlobalZ(double x, double y, double z) const {
	return a[2] * x + a[6] * y + a[10] * z + a[14];
}

double AffineTransformMatrix::Distance(
		const AffineTransformMatrix &other) const {
	double temp = 0.0;
	for (uint_fast8_t n = 0; n < 16; ++n)
		temp += (a[n] - other.a[n]) * (a[n] - other.a[n]);
	return sqrt(temp);

	// For future reference (did not work as expected):
	// If two matrices are identical, the matrix times the inverse of the other
	// matrix should result in the identity matrix.

	// A:=matrix([[a[0],a[4],a[8],a[12]],[a[1],a[5],a[9],a[13]],[a[2],a[6],a[10],a[14]],[0,0,0,1]])
	// B:=matrix([[b[0],b[4],b[8],b[12]],[b[1],b[5],b[9],b[13]],[b[2],b[6],b[10],b[14]],[0,0,0,1]])
	// I:=matrix([[1,0,0,0],[0,1,0,0],[0,0,1,0],[0,0,0,1]])
	// R:=A*inverse(B)-I
	// trace(R*transpose(R))

	// Result: No improvements (speedup) when used in Inverse Kinematics. -> Kicked out
}

std::string AffineTransformMatrix::ToString() {
	std::ostringstream out;
	out << '[';
	out << a[0];
	for (uint_fast8_t n = 1; n < 16; ++n) {
		out << ',';
		out << a[n];
	}
	out << ']';
	return out.str();
}

void AffineTransformMatrix::GLMultMatrix() const {
	glMultMatrixd(a.data());
}

void AffineTransformMatrix::GLSetUniformMatrix4(int location) const {
	if (location == -1)
		return;
	std::array<GLfloat, 16> mat;
	for (uint_fast8_t idx = 0; idx < 16; ++idx)
		mat[idx] = a[idx];
	glUniformMatrix4fv(location, 1, GL_FALSE, mat.data());
}

void AffineTransformMatrix::GLSetUniformMatrix3(int location) const {
	if (location == -1)
		return;
	std::array<GLfloat, 9> mat;
	for (uint_fast8_t col = 0; col < 3; ++col)
		for (uint_fast8_t row = 0; row < 3; ++row)
			mat[col * 3 + row] = a[col * 4 + row];
	glUniformMatrix3fv(location, 1, GL_FALSE, mat.data());
}

void AffineTransformMatrix::Paint(const Style style, const double param0,
		const double param1, const double param2) const {
	const uint_fast8_t NSides = 4;
	GLboolean colormask[4];
	glGetBooleanv(GL_COLOR_WRITEMASK, colormask);
	const bool useColors = (colormask[0] && colormask[1] && colormask[2]
			&& colormask[3]);

	switch (style) {

	default: { // Style::Lines
		const double len = param0;
		const double rad = 0.133 * param0;

		AffineTransformMatrix c = *this;
		for (uint_fast8_t i = 0; i < 3; ++i) {
			glPushName(i);
			SetColor(useColors, i + 1, param1);

			Vector3 n = c.TransformWithoutShift(1, 0, 0);
			glBegin(GL_LINES);
			GLNormal(-n);
			GLVertex(c(-len, 0, 0));
			GLNormal(n);
			GLVertex(c(len, 0, 0));
			glEnd();
			const double n0 = cos(M_PI_4);
			const double n1 = sin(M_PI_4);
			glBegin(GL_LINES);
			const Vector3 p0 = c.Transform(len, 0, 0);
			for (uint_fast8_t j = 0; j < NSides; ++j) {
				const double ang = 2 * M_PI / NSides * (double) j;
				const double co = cos(ang);
				const double si = sin(ang);
				const Vector3 p = c(len - rad, rad * co, rad * si);
				const Vector3 nj = c.TransformWithoutShift(n0, co * n1,
						si * n1);
				GLNormal(nj);
				GLVertex(p0);
				GLVertex(p);
			}
			glEnd();
			c = c
					* RotationAroundVector(Vector3(1, 1, 1),
							120.0 / 180.0 * M_PI);
			glPopName();
		}
		break;
	}

	case Style::BasePlane: {

		glPushMatrix();
		GLMultMatrix();
		glNormal3f(0, 0, 1);
		glBegin(GL_QUADS);
		SetColor(useColors, 0, param1);
		glVertex3f(0, 0, 0);
		SetColor(useColors, 1, param1);
		glVertex3f(1, 0, 0);
		SetColor(useColors, 4, param1);
		glVertex3f(1, 1, 0);
		SetColor(useColors, 2, param1);
		glVertex3f(0, 1, 0);
		if (param1 > 0.0) {
			SetColor(useColors, 0, param1);
			glVertex3f(-param0, -param0, 0);
			glVertex3f(0, -param0, 0);
			glVertex3f(0, 0, 0);
			glVertex3f(-param0, 0, 0);

			glVertex3f(0, -param0, 0);
			glVertex3f(1, -param0, 0);
			SetColor(useColors, 1, param1);
			glVertex3f(1, 0, 0);
			SetColor(useColors, 0, param1);
			glVertex3f(0, 0, 0);

			glVertex3f(1, -param0, 0);
			glVertex3f(1 + param0, -param0, 0);
			glVertex3f(1 + param0, 0, 0);
			SetColor(useColors, 1, param1);
			glVertex3f(1, 0, 0);

			glVertex3f(1, 0, 0);
			SetColor(useColors, 0, param1);
			glVertex3f(1 + param0, 0, 0);
			glVertex3f(1 + param0, 1, 0);
			SetColor(useColors, 4, param1);
			glVertex3f(1, 1, 0);

			glVertex3f(1, 1, 0);
			SetColor(useColors, 0, param1);
			glVertex3f(1 + param0, 1, 0);
			glVertex3f(1 + param0, 1 + param0, 0);
			glVertex3f(1, 1 + param0, 0);

			SetColor(useColors, 2, param1);
			glVertex3f(0, 1, 0);
			SetColor(useColors, 4, param1);
			glVertex3f(1, 1, 0);
			SetColor(useColors, 0, param1);
			glVertex3f(1, 1 + param0, 0);
			glVertex3f(0, 1 + param0, 0);

			glVertex3f(-param0, 1, 0);
			SetColor(useColors, 2, param1);
			glVertex3f(0, 1, 0);
			SetColor(useColors, 0, param1);
			glVertex3f(0, 1 + param0, 0);
			glVertex3f(-param0, 1 + param0, 0);

			SetColor(useColors, 0, param1);
			glVertex3f(-param0, 0, 0);
			glVertex3f(0, 0, 0);
			SetColor(useColors, 2, param1);
			glVertex3f(0, 1, 0);
			SetColor(useColors, 0, param1);
			glVertex3f(-param0, 1, 0);
		}

		glEnd();
		glNormal3d(M_SQRT1_2, M_SQRT1_2, 0.0);
		glBegin(GL_LINES);
		SetColor(useColors, 0);
		glVertex3f(0, 0, 0);
		SetColor(useColors, 3);
		glVertex3f(0, 0, 1);
		glEnd();
		glPopMatrix();

		break;
	}

	case Style::Cylinder: {
		const uint16_t N = ceil(param2 * 16.0);
		AffineTransformMatrix c = *this;
		for (uint_fast8_t i = 0; i < 3; ++i) {
			glPushName(i);
			SetColor(useColors, i + 1, param2);

			Vector3 n0 = c.TransformWithoutShift(-1, 0, 0);
			glBegin(GL_TRIANGLE_FAN);
			GLNormal(n0);
			GLVertex(c(0, 0, 0));
			for (uint_fast16_t j = 0; j <= N; ++j) {
				const double ang = 2 * M_PI / N * (double) j;
				const double co = cos(ang);
				const double si = sin(ang);
				GLVertex(c.Transform(0, co * param0, -si * param0));
			}
			glEnd();
			glBegin(GL_QUAD_STRIP);
			for (uint_fast16_t j = 0; j <= N; ++j) {
				const double ang = 2 * M_PI / N * (double) j;
				const double co = cos(ang);
				const double si = sin(ang);
				GLNormal(c.TransformWithoutShift(0, co, -si));
				GLVertex(c.Transform(0, co * param0, -si * param0));
				GLVertex(c.Transform(1 - param1, co * param0, -si * param0));
			}
			glEnd();
			GLNormal(n0);
			glBegin(GL_QUAD_STRIP);
			for (uint_fast16_t j = 0; j <= N; ++j) {
				const double ang = 2 * M_PI / N * (double) j;
				const double co = cos(ang);
				const double si = sin(ang);
				GLVertex(c.Transform(1 - param1, co * param0, -si * param0));
				GLVertex(c.Transform(1 - param1, co * param1, -si * param1));
			}
			glEnd();
			glBegin(GL_TRIANGLE_FAN);
			GLNormal(c(1, 0, 0));
			GLVertex(c(1, 0, 0));
			for (uint_fast16_t j = 0; j <= N; ++j) {
				const double ang = 2 * M_PI / N * (double) j;
				const double co = cos(ang);
				const double si = sin(ang);
				GLNormal(
						c.TransformWithoutShift(M_SQRT1_2, co * M_SQRT1_2,
								si * M_SQRT1_2));
				GLVertex(c.Transform(1 - param1, co * param1, si * param1));
			}
			glEnd();

//			glBegin(GL_LINES);
//			GLNormal(-n);
//			GLVertex(c(-len, 0, 0));
//			GLNormal(n);
//			GLVertex(c(len, 0, 0));
//			glEnd();
//			const double n0 = cos(M_PI_4);
//			const double n1 = sin(M_PI_4);
//			glBegin(GL_LINES);
//			const Vector3 p0 = c.Transform(len, 0, 0);
//				const Vector3 p = c(len - rad, rad * co, rad * si);
//				const Vector3 n = c.TransformWithoutShift(n0, co * n1, si * n1);
//				GLNormal(n);
//				GLVertex(p0);
//				GLVertex(p);
//			}
//			glEnd();
			c = c
					* RotationAroundVector(Vector3(1, 1, 1),
							120.0 / 180.0 * M_PI);
			glPopName();
		}
		break;
	}
	case Style::Planes: {

		AffineTransformMatrix c = *this;
		for (uint_fast8_t i = 0; i < 3; ++i) {
			glPushName(i);
			SetColor(useColors, i + 1, param1);
			const Vector3 n0 = c.TransformWithoutShift(-M_SQRT1_2,
					0 - M_SQRT1_2);
			const Vector3 n1 = c.TransformWithoutShift(-M_SQRT1_2,
			M_SQRT1_2, 0);
			const Vector3 n2 = c.TransformWithoutShift(- M_SQRT1_2, 0,
			M_SQRT1_2);
			const Vector3 v0 = c.Transform(0, 0, 0);
			const Vector3 v1 = c.Transform(0, 1, 0);
			const Vector3 v2 = c.Transform(0, 1, 1);
			const Vector3 v3 = c.Transform(0, 0, 1);
			glBegin(GL_LINES);
			SetColor(useColors, 4, 1.0, true);
			GLNormal(n0);
			GLVertex(v0);
			GLVertex(v1);
			SetColor(useColors, i + 1, 1.0, true);
			GLNormal(n1);
			GLVertex(v1);
			GLVertex(v2);
			GLNormal(n2);
			GLVertex(v2);
			GLVertex(v3);
			glEnd();
			c = c
					* RotationAroundVector(Vector3(1, 1, 1),
							120.0 / 180.0 * M_PI);
			glPopName();
		}
		c = *this;
		for (uint_fast8_t i = 0; i < 3; ++i) {
			glPushName(i);
			SetColor(useColors, i + 1, param1);
			const Vector3 n = c.TransformWithoutShift(-1, 0, 0);
			const Vector3 v0 = c.Transform(0, -param0, -param0);
			const Vector3 v1 = c.Transform(0, -param0, 1 + param0);
			const Vector3 v2 = c.Transform(0, 1 + param0, 1 + param0);
			const Vector3 v3 = c.Transform(0, 1 + param0, -param0);
			glBegin(GL_QUADS);
			GLNormal(n);
			GLVertex(v0);
			GLVertex(v1);
			GLVertex(v2);
			GLVertex(v3);
			glEnd();
			c = c
					* RotationAroundVector(Vector3(1, 1, 1),
							120.0 / 180.0 * M_PI);
			glPopName();
		}
		break;
	}

	case Style::Box:
	case Style::BoxWithoutColors: {

		AffineTransformMatrix c = *this;
		for (uint_fast8_t i = 0; i < 3; ++i) {
			glPushName(i);

			const Vector3 n0 = c.TransformWithoutShift(-M_SQRT1_2, -M_SQRT1_2,
					0);
			const Vector3 n1 = c.TransformWithoutShift(-M_SQRT1_2, 0,
			M_SQRT1_2);
			const Vector3 n2 = c.TransformWithoutShift(-M_SQRT1_2, M_SQRT1_2,
					0);
			const Vector3 n3 = c.TransformWithoutShift(-M_SQRT1_2, 0,
					-M_SQRT1_2);

			const Vector3 v0 = c.Transform(0, 0, 0);
			const Vector3 v1 = c.Transform(0, 0, 1);
			const Vector3 v2 = c.Transform(0, 1, 1);
			const Vector3 v3 = c.Transform(0, 1, 0);
			const Vector3 v4 = c.Transform(1, 0, 0);
			const Vector3 v5 = c.Transform(1, 0, 1);
			const Vector3 v6 = c.Transform(1, 1, 1);
			const Vector3 v7 = c.Transform(1, 1, 0);

			if (style != Style::BoxWithoutColors)
				SetColor(useColors, i + 1, 1.0, true);

			glBegin(GL_LINES);
			GLNormal(n0);
			GLVertex(v0);
			GLVertex(v4);
			GLNormal(n1);
			GLVertex(v1);
			GLVertex(v5);
			GLNormal(n2);
			GLVertex(v2);
			GLVertex(v6);
			GLNormal(n3);
			GLVertex(v3);
			GLVertex(v7);
			glEnd();
			c = c
					* RotationAroundVector(Vector3(1, 1, 1),
							120.0 / 180.0 * M_PI);
			glPopName();
		}
		c = *this;
		for (uint_fast8_t i = 0; i < 3; ++i) {
			glPushName(i);
			if (style != Style::BoxWithoutColors)
				SetColor(useColors, i + 1, param1);
			const Vector3 n = c.TransformWithoutShift(0, 0, -1);
			const Vector3 v0 = c.Transform(0, -param0, -param0);
			const Vector3 v1 = c.Transform(0, -param0, 1 + param0);
			const Vector3 v2 = c.Transform(0, 1 + param0, 1 + param0);
			const Vector3 v3 = c.Transform(0, 1 + param0, -param0);
			const Vector3 v4 = c.Transform(1, -param0, -param0);
			const Vector3 v5 = c.Transform(1, -param0, 1 + param0);
			const Vector3 v6 = c.Transform(1, 1 + param0, 1 + param0);
			const Vector3 v7 = c.Transform(1, 1 + param0, -param0);
			glBegin(GL_QUADS);
			GLNormal(n);
			GLVertex(v0);
			GLVertex(v1);
			GLVertex(v2);
			GLVertex(v3);
			GLNormal(-n);
			GLVertex(v4);
			GLVertex(v5);
			GLVertex(v6);
			GLVertex(v7);
			glEnd();
			c = c
					* RotationAroundVector(Vector3(1, 1, 1),
							120.0 / 180.0 * M_PI);
			glPopName();
		}
		break;
	}
	}
	std::array<GLfloat, 4> buffer = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buffer.data());
}

void AffineTransformMatrix::SetColor(const bool useColors,
		const unsigned char color, const double alpha, bool bright) {
	if (!useColors)
		return;
	glEnable(GL_COLOR_MATERIAL);
	if (color >= 5)
		return;
	std::array<GLfloat, 4> buffer = { 0, 0, 0, 1 };
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, buffer.data());
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0);
	switch (color) {
	default:
		buffer[0] = 0.2f;
		buffer[1] = 0.2f;
		buffer[2] = 0.2f;
		break;
	case 1:
		buffer[0] = 0.5f;
		buffer[1] = 0.0f;
		buffer[2] = 0.0f;
		break;
	case 2:
		buffer[0] = 0.0f;
		buffer[1] = 0.5f;
		buffer[2] = 0.0f;
		break;
	case 3:
		buffer[0] = 0.0f;
		buffer[1] = 0.0f;
		buffer[2] = 0.5f;
		break;
	case 4:
		buffer[0] = 0.5f;
		buffer[1] = 0.5f;
		buffer[2] = 0.5f;
		break;
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, buffer.data());
	if (bright)
		for (auto &b : buffer)
			b *= 2.0f;
	buffer[3] = (float) alpha;
	glColor4fv(buffer.data());
}

void AffineTransformMatrix::GLVertex(const Vector3 &v) {
	glVertex3d(v.x, v.y, v.z);
}

void AffineTransformMatrix::GLNormal(const Vector3 &n) {
	glNormal3d(n.x, n.y, n.z);
}


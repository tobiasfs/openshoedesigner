///////////////////////////////////////////////////////////////////////////////
// Name               : Bone.cpp
// Purpose            : Simple bone
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 12.08.2015
// Copyright          : (C) 2015 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Bone.h"

#include <cmath>

#include "../../3D/OpenGL.h"

Bone::Bone(const std::string &name_) :
		name(name_) {
}

double Bone::CalculateAnchorPoint(const Vector3 &p) const {
	if (fabs(r1init - r2init) >= lengthinit || lengthinit < 1e-9) {
		return (r1init >= r2init) ? 0.0 : 1.0;
	}
	// Calculate relative surface angle:
	double a = asin((r1init - r2init) / lengthinit);
	double ar1 = asin(matrixinit.GetEx().Dot((p - p1).Normal()));
	double ar2 = asin(matrixinit.GetEx().Dot((p - p2).Normal()));
	if (ar1 < a)
		return 0.0;
	if (ar2 > a)
		return 1.0;
	return (a - ar1) / (ar2 - ar1);
}

void Bone::Update() {
	if (!initialized) {
//		lengthinit = (p2-p1).Abs();
//		matrixinit.Normalize();
		p1 = matrixinit.GetOrigin();
		p2 = matrixinit.Transform(lengthinit, 0, 0);
		if (!parentTo.expired()) {
			std::shared_ptr<Bone> parent = parentTo.lock();
			anchorDinit = parent->CalculateAnchorPoint(matrixinit.GetOrigin());
			AffineTransformMatrix base = parent->matrixinit;
			base.TranslateLocal(parent->lengthinit * anchorDinit, 0, 0);
			link = base.Inverse() * matrixinit;
			Vector3 move = link.GetOrigin();
			linkLengthInit = move.Abs();
			move.Normalize();
			link.SetOrigin(move);
			rparentinit = parent->r1init
					+ (parent->r2init - parent->r1init) * anchorDinit;
		}
		matrix = matrixinit;
		r1 = r1init;
		r2 = r2init;
		length = lengthinit;
		initialized = true;
	}
	if (parentTo.expired()) {
		matrix = matrixinit
				* AffineTransformMatrix::RotationInterwoven(0, roty, rotz);
		p1 = matrix.GetOrigin();
		p2 = matrix.Transform(length, 0, 0);
		return;
	}
	std::shared_ptr<Bone> parent = parentTo.lock();

	const double rParent = parent->r1 + (parent->r2 - parent->r1) * anchorDinit;

	const double scaleParent = rParent / rparentinit;
	const double scaleR = r1 / r1init;
	mixing = rParent / (r1 + rParent);

	const double interfacethicknessInit = linkLengthInit - rparentinit - r1init;
	const double interfacethickness = interfacethicknessInit
			* fmin(scaleParent, scaleR);

	double linkLength = interfacethickness + rParent + r1;

//	r1 = linkLengthInit - rParentInit - interfacethickness;
//	r1 = r1 / lengthinit * length;
//	const double linkLength = r1 + rParent + interfacethickness;

//	mixing = rParent / linkLength;

	matrix = parent->matrix;
	matrix.TranslateLocal(parent->length * anchorDinit, 0, 0);
	matrix = matrix
			* AffineTransformMatrix::RotationInterwoven(0,
					roty * (1.0 - mixing), rotz * (1.0 - mixing));

//	Vector3 linkScaled = link * linkLength;
//	matrix.TranslateLocal(linkScaled.x, linkScaled.y, linkScaled.z);
	AffineTransformMatrix m = link;
	m.SetOrigin(m.GetOrigin() * linkLength);
	matrix *= m;

	matrix = matrix
			* AffineTransformMatrix::RotationInterwoven(0, roty * mixing,
					rotz * mixing);
	p1 = matrix.GetOrigin();
	p2 = matrix.Transform(length, 0, 0);
}

void Bone::UpdateHierarchy() {
	if (parentTo.expired()) {
		hierarchyLevel = 0;
	} else {
		std::shared_ptr<Bone> parent = parentTo.lock();
		if (parent->hierarchyLevel == 0)
			parent->UpdateHierarchy();
		hierarchyLevel = parent->hierarchyLevel + 1;
	}
}

double Bone::GetXMax() const {
	return fmax(p1.x + r1 + s1, p2.x + r1 + s1);
}

double Bone::GetXMin() const {
	return fmin(p1.x - r1 - s1, p2.x - r1 - s1);
}

double Bone::GetZMin() const {
	return fmin(p1.z - r1 - s1, p2.z - r1 - s1);
}

void Bone::PushRotation() {
	rotyBackup = roty;
	rotzBackup = rotz;
	roty = 0.0;
	rotz = 0.0;
}

void Bone::PopRotation() {
	roty = rotyBackup;
	rotz = rotzBackup;
}

void Bone::Paint() const {
	const size_t N = 40; // Cap segments
	const size_t M = 40; // Segments around the bone

	if (r1 <= 0.0 && r2 <= 0.0)
		return;
	AffineTransformMatrix matrix2 = matrix;
	matrix2.TranslateLocal(length, 0, 0);
	const float L = (p2 - p1).Abs(); // Length of the vector between the points.
	const float dr = fabs(r1 - r2); // Difference between the radii.

	const float dv = 2.0 * M_PI / N;
//	float v = 0.0;
	const float dw = 2.0 * M_PI / M;
//	float w = 0.0;

	auto vertex = [](const Vector3 &v) {
		glVertex3f(v.x, v.y, v.z);
	};
	auto normal = [](const Vector3 &v) {
		glNormal3f(v.x, v.y, v.z);
	};

	if (L <= dr) {
		// Only a sphere needs to be rendered. (The bigger one.)
		auto sphere = [N, M](size_t n, size_t m) -> Vector3 {
			const double v = M_PI / (double) N * (double) n;
			const double w = 2.0 * M_PI / (double) M * (double) m;
			if (n == 0)
				return Vector3(-1, 0, 0);
			if (n == N)
				return Vector3(1, 0, 0);
			return Vector3(-cos(v), sin(v) * cos(w), sin(v) * sin(w));
		};
		if (r1 > r2) {
			for (size_t m = 0; m < M; m++) {
				glBegin(GL_QUAD_STRIP);
				for (size_t n = 0; n <= N; n++) {
					const Vector3 v1 = sphere(n, m + 1);
					const Vector3 v2 = sphere(n, m);
					if (matrix.GetOrientation()
							== AffineTransformMatrix::Orientation::LHS) {
						normal(matrix.TransformWithoutShift(v1));
						vertex(matrix.Transform(r1 * v1));
						normal(matrix.TransformWithoutShift(v2));
						vertex(matrix.Transform(r1 * v2));
					} else {
						normal(matrix.TransformWithoutShift(v2));
						vertex(matrix.Transform(r1 * v2));
						normal(matrix.TransformWithoutShift(v1));
						vertex(matrix.Transform(r1 * v1));
					}
				}
				glEnd();
			}
		} else {
			for (size_t m = 0; m < M; m++) {
				glBegin(GL_QUAD_STRIP);
				for (size_t n = 0; n <= N; n++) {
					const Vector3 v1 = sphere(n, m + 1);
					const Vector3 v2 = sphere(n, m);
					if (matrix.GetOrientation()
							== AffineTransformMatrix::Orientation::LHS) {
						normal(matrix.TransformWithoutShift(v1));
						vertex(matrix.Transform(r2 * v1));
						normal(matrix.TransformWithoutShift(v2));
						vertex(matrix.Transform(r2 * v2));
					} else {
						normal(matrix.TransformWithoutShift(v2));
						vertex(matrix.Transform(r2 * v2));
						normal(matrix.TransformWithoutShift(v1));
						vertex(matrix.Transform(r2 * v1));
					}
				}
				glEnd();
			}
		}
		return;
	}

	// Complete rendering of the bone.

	const float s1 = (r1 * r1 - r1 * r2) / L;
	const float s2 = (r1 * r2 - r2 * r2) / L;
	Vector3 s = p2 - p1;
	Vector3 g;
	if (s.x <= s.y && s.x <= s.z)
		g.Set(0, s.z, -s.y);
	if (s.y <= s.z && s.y <= s.x)
		g.Set(-s.z, 0, s.x);
	if (s.z <= s.x && s.z <= s.y)
		g.Set(s.y, -s.x, 0);
	Vector3 h;
	s.Normalize();
	g.Normalize();
	h = s * g;
	h.Normalize();

	const double h1 = sqrt(r1 * r1 - s1 * s1);
	const double h2 = sqrt(r2 * r2 - s2 * s2);
	//	double f = s1 / r1;

	Vector3 zyl;
	zyl.Set(s1, h1, 0);
	zyl.Normalize();

	::glBegin(GL_TRIANGLE_STRIP);

	float w = 0.0;
	for (size_t m = 0; m <= M; m++) {
		normal(s * zyl.x + g * zyl.y * cos(w) + h * zyl.y * sin(w));
		vertex(p2 + s * s2 + (g * h2 * cos(w)) + (h * h2 * sin(w)));
		vertex(p1 + s * s1 + (g * h1 * cos(w)) + (h * h1 * sin(w)));
		w += dw;
	}
	::glEnd();

	float v = 0.0;
	float vg = atan2(h1, -s1);
	float v2;
	while (v < vg) {
		v2 = fmin(v + dv, vg);
		::glBegin(GL_TRIANGLE_STRIP);
		w = 0.0;
		for (size_t m = 0; m <= M; m++) {
			normal(-s * cos(v2) + g * cos(w) * sin(v2) + h * sin(w) * sin(v2));
			vertex(
					p1 - s * r1 * cos(v2) + (g * r1 * cos(w) * sin(v2))
							+ (h * r1 * sin(w) * sin(v2)));
			normal(-s * cos(v) + g * cos(w) * sin(v) + h * sin(w) * sin(v));
			vertex(
					p1 - s * r1 * cos(v) + (g * r1 * cos(w) * sin(v))
							+ (h * r1 * sin(w) * sin(v)));
			w += dw;
		}
		::glEnd();
		v += dv;
	}

	v = 0.0;
	vg = atan2(h2, s2);
	while (v < vg) {
		v2 = fmin(v + dv, vg);
		::glBegin(GL_TRIANGLE_STRIP);
		w = 0.0;
		for (size_t m = 0; m <= M; m++) {
			normal(s * cos(v) + g * cos(w) * sin(v) + h * sin(w) * sin(v));
			vertex(
					p2 + s * r2 * cos(v) + (g * r2 * cos(w) * sin(v))
							+ (h * r2 * sin(w) * sin(v)));
			normal(s * cos(v2) + g * cos(w) * sin(v2) + h * sin(w) * sin(v2));
			vertex(
					p2 + s * r2 * cos(v2) + (g * r2 * cos(w) * sin(v2))
							+ (h * r2 * sin(w) * sin(v2)));
			w += dw;
		}
		::glEnd();
		v += dv;
	}
}


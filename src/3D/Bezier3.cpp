///////////////////////////////////////////////////////////////////////////////
// Name               : Bezier3.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 14.01.2025
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
#include "Bezier3.h"

#include <cfloat>

Bezier3::Point::Point(double x_, double y_, double z_) :
		Vector3(x_, y_, z_) {
}

Bezier3::Point& Bezier3::Point::operator=(const Vector3 &v) {
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

void Bezier3::SetSize(size_t pointCount) {
	points.resize(pointCount);
}

void Bezier3::CloseLoop(bool closed_) {
	this->closed = closed_;
}

size_t Bezier3::Size() const {
	return points.size();
}

Bezier3::Point& Bezier3::operator [](size_t idx) {
	return points[idx];
}

const Bezier3::Point& Bezier3::operator [](size_t idx) const {
	return points[idx];
}

void Bezier3::UpdateSegments() {
	const size_t Np = points.size();
	const size_t N = Np - (closed ? 0 : 1);

	// Update the continuity in all points.
	for (size_t n = 0; n < N; n++) {
		Point &v1 = points[(n + 1) % Np];
		switch (v1.continuity) {
		case Continuity::Linear: {
			const double d0 = v1.dirIn.Abs();
			const double d1 = v1.dirOut.Abs();
			if (d0 > FLT_EPSILON) {
				v1.dirOut = v1.dirIn * (d1 / d0);
			} else {
				v1.dirOut = v1.dirIn;
			}
			break;
		}
		case Continuity::Symmetric: {
			v1.dirOut = v1.dirIn;
			break;
		}
		case Continuity::Auto_KB: {
			const Point &v0 = points[n];
			const Point &v2 = points[(n + 2) % Np];
			const Vector3 a0 = v1 - v0;
			const Vector3 a1 = v2 - v1;
			v1.dirIn = (a0 + a1) / 2.0;
			v1.dirOut = v1.dirIn;
			break;
		}

		case Continuity::Auto_DRB: {
			const Point &v0 = points[n];
			const Point &v2 = points[(n + 2) % Np];
			const Vector3 a0 = v1 - v0;
			const Vector3 a1 = v2 - v1;
			const double d0 = a0.Abs();
			const double d1 = a1.Abs();

			if (d0 > FLT_EPSILON) {
				const double beta = d1 / d0;
				v1.dirIn = (a0 * beta + a1 / beta) / (beta + 1);
				v1.dirOut = (a0 * beta * beta + a1) / (beta + 1);
			}
			break;
		}

		case Continuity::Auto_DRBS: {
			const Point &v0 = points[n];
			const Point &v2 = points[(n + 2) % Np];
			const Vector3 a0 = v1 - v0;
			const Vector3 a1 = v2 - v1;
			const double d0 = a0.Abs();
			const double d1 = a1.Abs();
			if (d0 > FLT_EPSILON && d1 > FLT_EPSILON) {
				const double beta = (d0 > d1) ? (d1 / d0) : (d0 / d1);
				v1.dirIn = (a0 * beta + a1 / beta) / (beta + 1);
				v1.dirOut = v1.dirIn;
			}
			break;
		}
		default:
			// Nothing to do
			break;
		}
	}
	segments.resize(N);
	for (size_t n = 0; n < N; n++) {
		Polynomial3 &s = segments[n];
		Point &v0 = points[n];
		Point &v1 = points[(n + 1) % Np];
		Vector3 h0;
		Vector3 h1;
		bool useH0 = false;
		bool useH1 = false;

		if (v0.continuity != Continuity::Free) {
			useH0 = true;
			h0 = v0 + v0.dirOut / 3.0;
		}
		if (v1.continuity != Continuity::Free) {
			useH1 = true;
			h1 = v1 - v1.dirIn / 3.0;
		}

		if (useH0) {
			if (useH1) {
				s.x = Polynomial::ByBezier(v0.x, h0.x, h1.x, v1.x);
				s.y = Polynomial::ByBezier(v0.y, h0.y, h1.y, v1.y);
				s.z = Polynomial::ByBezier(v0.z, h0.z, h1.z, v1.z);
			} else {
				s.x = Polynomial::ByBezier(v0.x, h0.x, v1.x);
				s.y = Polynomial::ByBezier(v0.y, h0.y, v1.y);
				s.z = Polynomial::ByBezier(v0.z, h0.z, v1.z);
			}
		} else {
			if (useH1) {
				s.x = Polynomial::ByBezier(v0.x, h1.x, v1.x);
				s.y = Polynomial::ByBezier(v0.y, h1.y, v1.y);
				s.z = Polynomial::ByBezier(v0.z, h1.z, v1.z);
			} else {
				s.x = Polynomial::ByBezier(v0.x, v1.x);
				s.y = Polynomial::ByBezier(v0.y, v1.y);
				s.z = Polynomial::ByBezier(v0.z, v1.z);
			}
		}
		if (maxUMapOrder == 0) {
			s.r0 = (double) n;
			s.r1 = s.r0 + 1.0;
			s.mapR = Polynomial::ByValue(s.r0, 0.0, s.r1, 1.0);
		} else {
			// Algorithm to compensate for the length of the polynomial.
			// If maxUMapOrder > 0, then u is the length on the polynomial.
			throw std::runtime_error("Not implemented.");
		}

	}
}

Vector3 Bezier3::operator ()(double u) const {
	const size_t N = segments.size();
	if (searchidx >= N || u <= segments.front().r1)
		searchidx = 0;
	while (searchidx > 0 && segments[searchidx].r0 >= u)
		searchidx--;
	while ((searchidx + 1) < N && segments[searchidx].r1 <= u)
		searchidx++;
	return segments[searchidx](u);
}

const Polynomial3& Bezier3::GetSegment(size_t idx) const {
	return segments[idx];
}

void Bezier3::Paint() const {
	for (const Polynomial3 &s : segments)
		s.Paint();
}


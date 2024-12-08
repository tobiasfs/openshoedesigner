///////////////////////////////////////////////////////////////////////////////
// Name               : Polygon25.cpp
// Purpose            : Contains a 2.5D polygon.
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 17.07.2011
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

#include "Polygon25.h"

#include <float.h>
#include <stdint.h>
#include "OpenGL.h"

double Polygon25::GetLengthXY() const {
	//FIXME: This function assumes, that there is only one polygon in data.
	if (v.size() <= 1)
		return 0.0;
	double d = 0.0;

	Vertex temp = v[0];
	for (size_t i = 1; i < v.size(); i++) {
		Vector3 temp2 = temp - v[i];
		temp2.z = 0;
		d += temp2.Abs();
		temp = v[i];
	}
	if (IsClosed()) {
		Vector3 temp2 = temp - v[0];
		temp2.z = 0;
		d += temp2.Abs();
	}
	return d;
}

void Polygon25::PolygonFillHoles() {
	//TODO: This is crude! Find a better way.
	double m = 0.0;
	size_t nrp = 0;
	for (size_t i = 0; i < v.size(); i++) {
		if (v[i].z > -0.5) {
			nrp++;
			m += v[i].z;
		}
	}
	if (nrp == 0)
		return;
	m /= (double) nrp;
	for (size_t i = 0; i < v.size(); i++) {
		if (v[i].z < -0.5) {
			v[i].z = m;
		}
	}
}

void Polygon25::PolygonSmooth() {
	auto temp = v;

	for (size_t i = 0; i < v.size(); i++) {
		Vector3 d;
		if (i == 0)
			d = v[v.size() - 1];
		else
			d = v[i - 1];
		d += v[i];
		if (i + 1 < v.size())
			d += v[i + 1];
		else
			d += v[0];
		temp[i] = d / 3;
	}
	v.swap(temp);
}

void Polygon25::PolygonExpand(double r) {
	if (v.size() < 2)
		return;
	size_t i;
	Vector3 o, n, d;
	o = v[0];
	for (i = 1; i < v.size(); i++) {
		n = v[i];
		o = n - o;
		o.Normalize();
		d.x = o.y;
		d.y = -o.x;
		d.z = o.z;
		o = n;
		v[i] = n + d * r;
	}
}

void Polygon25::PolygonDiminish(double r) {
	this->PolygonExpand(-r);
}

bool Polygon25::IsElementInside(const Vector3 &vTest) const {
	// Using the Jordan Polygon Theorem

	// TODO Check the numerical stability of this algorithm. Some suspicious ==s are used.

	int_fast8_t c = 1;
	size_t E = v.size();
	for (size_t i = 0; i < E; i++) {
		Vector3 p0 = v[i];
		Vector3 p1;
		if (i + 1 == E)
			p1 = v[0];
		else
			p1 = v[i + 1];
		if (vTest.y == p0.y && vTest.y == p1.y) {
			if ((p0.x <= vTest.x && vTest.x <= p1.x)
					|| (p0.x >= vTest.x && vTest.x >= p1.x)) {
				c = 0;
				break;
			}
			continue;
		}
		if (p0.y > p1.y) {
			float h = p0.x;
			p0.x = p1.x;
			p1.x = h;
			h = p0.y;
			p0.y = p1.y;
			p1.y = h;
		}
		if (vTest.x == p0.x && vTest.y == p0.y) {
			c = 0;
			break;
		}
		if (vTest.y <= p0.y || vTest.y > p1.y)
			continue;
		const float delta = (p0.x - vTest.x) * (p1.y - vTest.y)
				- (p0.y - vTest.y) * (p1.x - vTest.x);
		if (delta > 0) {
			c = -c;
			continue;
		}
		if (delta < 0)
			continue;
		c = 0;
		break;
	}
	if (c <= 0)
		return true;
	return false;
}

bool Polygon25::IsPolygonInside(const Polygon25 &other) const {
	for (size_t i = 0; i < other.v.size(); i++) {
		if (!IsElementInside(other.v[i])) {
			return false;
		}
	}
	return true;
}

void Polygon25::SortPolygonsFromOutside(std::vector<Polygon25> *array) {
	const int N = array->size();
	if (N == 0)
		return;
	std::vector<Polygon25> temp;
	temp.push_back(array->operator [](0));
	for (int i = 1; i < N; i++) {
		int pos = -1;
		const int M = temp.size();
		for (int j = M; j > 0; j--) {
			if (temp[j - 1].IsPolygonInside(array->operator [](i))) {
				pos = j - 1;
				break;
			}
		}
		if (pos == -1)
			temp.insert(temp.begin(), array->operator [](i));
		if (pos >= 0 && pos < M - 1)
			temp.insert(temp.begin() + pos + 1, array->operator [](i));
		if (pos >= M - 1)
			temp.push_back(array->operator [](i));
	}
	*array = temp;
}

double Polygon25::DistanceToElement(const size_t elementInPolygon,
		const double x, const double y, const double vx,
		const double vy) const {
	double qx, qy, px, py;
	px = v[elementInPolygon].x;
	py = v[elementInPolygon].y;
	if (elementInPolygon + 1 == v.size()) {
		qx = v[0].x;
		qy = v[0].y;
	} else {
		qx = v[elementInPolygon + 1].x;
		qy = v[elementInPolygon + 1].y;
	}

	// From axiom code:
	//	x1 := px+(qx-px)*r
	//	y1 := py+(qy-py)*r
	//	x2 := x+vx*s
	//	y2 := y+vy*s
	//	solve([x1=x2,y1=y2],[r,s])

	double denom = (qx - px) * vy + (-qy + py) * vx;
	if (fabs(denom) <= FLT_EPSILON)
		return DBL_MAX;
	double r = (-vx * y + vy * x - px * vy + py * vx) / denom;
	double s = ((-qx + px) * y + (qy - py) * x - px * qy + py * qx) / denom;

	if ((r < 0.0) || (r > 1.0))
		return DBL_MAX;
	return s;
}

double Polygon25::DistanceToPolygon(const Polygon25 &polygon, double vx,
		double vy) const {
	//TODO: Make this faster!
	size_t i, j;
	double dmin = DBL_MAX;
	double d;
	size_t n = polygon.v.size();
	if (!polygon.IsClosed() && (n > 0))
		n--;
	for (i = 0; i < n; i++) {
		for (j = 0; j < this->v.size(); j++) {
			d = polygon.DistanceToElement(i, v[j].x, v[j].y, vx, vy);
			if (d < dmin)
				dmin = d;
		}
	}
	return dmin;
}

void Polygon25::RotatePolygonStart(double x, double y) {

	if (v.size() == 0)
		return;

	size_t i;
	Vector3 t;
	double d;
	double dmin = DBL_MAX;
	size_t n = v.size();

	// Find element with minimal distance to (x,y)
	size_t nshift = 0;
	for (i = 0; i < n; i++) {
		t = v[i];
		d = (t.x - x) * (t.x - x) + (t.y - y) * (t.y - y);
		if (d < dmin) {
			dmin = d;
			nshift = i;
		}
	}

	// Shift by -nshift (so nshift becomes 0)
	//	nshift = n - nshift;
	size_t j;
	std::vector<Geometry::Vertex> temp;
	for (i = 0; i < n; i++) {
		j = (i + nshift) % n;
		temp.push_back(v[j]);
	}
	v.swap(temp);
}

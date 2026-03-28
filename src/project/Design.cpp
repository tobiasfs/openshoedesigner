///////////////////////////////////////////////////////////////////////////////
// Name               : Design.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 04.02.2025
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
#include "Design.h"
#include "../3D/FilePLY.h"
#include "../math/Exporter.h"
#include "../math/Polynomial.h"

#include <iostream>
#include <cfloat>
#include <Eigen/Core>
#include <Eigen/Dense>

static const size_t nothing = (size_t) -1;

#define RUNTIME_ERROR(x) { \
	std::ostringstream err; \
	err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - "; \
	err << x ; \
    throw std::runtime_error(err.str()); \
    }

#define LOGIC_ERROR(x) { \
	std::ostringstream err; \
	err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - "; \
	err << x ; \
    throw std::logic_error(err.str()); \
    }

Design::Design() {

	// Example design to start from

	vertices.emplace_back(-M_PI + 0.1, 0.8);
	vertices.emplace_back(-2, 0.25);
	vertices.emplace_back(-2.1, 0.1);
	vertices.emplace_back(-2, 0.8);
	vertices.emplace_back(-1.5, 0.75);

	vertices.emplace_back(-1.0, 0.0);
	vertices.emplace_back(-1.0, 0.6);
	vertices.emplace_back(-1.0, 0.5);

	vertices.emplace_back(-0.5, 0.0);
	vertices.emplace_back(-0.5, 0.2);

	vertices.emplace_back(0.0, 0.75);

	vertices.emplace_back(0.4, 0.0);
	vertices.emplace_back(0.5, 0.2);

	vertices.emplace_back(1.0, 0.0);
	vertices.emplace_back(1.0, 0.6);
	vertices.emplace_back(1.0, 0.5);

	vertices.emplace_back(1.5, 0.65);
	vertices.emplace_back(2, 0.0);
	vertices.emplace_back(2.3, 0.8);
	vertices.emplace_back(2, 0.25);

	vertices.emplace_back(-M_PI, 0.0);

	vertices.emplace_back(-M_PI, 1.1);
	vertices.emplace_back(-1, 1.1);
	vertices.emplace_back(1, 1.1);

	edges.emplace_back((std::initializer_list<size_t> ) { 0, 3, 2 });
	edges.emplace_back((std::initializer_list<size_t> ) { 5, 6, 14, 13 });
	edges.emplace_back((std::initializer_list<size_t> ) { 8, 9 });
	edges.emplace_back((std::initializer_list<size_t> ) { 11, 12 });
	edges.emplace_back((std::initializer_list<size_t> ) { 1, 7, 4, 10 });
	edges.emplace_back((std::initializer_list<size_t> ) { 10, 16, 15, 19 });
	edges.emplace_back((std::initializer_list<size_t> ) { 17, 18, 0 });
	edges.emplace_back(
			(std::initializer_list<size_t> ) { 20, 2, 5, 13, 17, 20 });

	edges.emplace_back((std::initializer_list<size_t> ) { 0, 21 });
	edges.emplace_back((std::initializer_list<size_t> ) { 21, 22, 23, 21 });

//	edges.emplace_back((std::initializer_list<size_t> ) { 2, 16, 3 });

	patches.emplace_back((std::initializer_list<size_t> ) { 2, 3, 1, 7 },
			"Cap");
	patches.emplace_back((std::initializer_list<size_t> ) { 0, 6, 7, 4, 5, 1 },
			"Vamp");
	patches.emplace_back((std::initializer_list<size_t> ) { 0, 6, 7 },
			"Counter");
	patches.emplace_back((std::initializer_list<size_t> ) { 0, 4, 5, 6, 8, 9 },
			"Shaft");

	constraints.emplace_back(Constraint::VertexOnEdge(1, 0));
	constraints.emplace_back(Constraint::VertexOnEdge(19, 6));

	constraints.emplace_back(Constraint::VerticesOnHorizontalLine( { 2, 5, 8,
			11, 13, 17 }, 0.0));
	constraints.emplace_back(Constraint::VerticesOnVerticalLine( { 0 }, -M_PI));
	constraints.emplace_back(Constraint::VerticesOnVerticalLine( { 10 }, 0.0));

	constraints.emplace_back(Constraint::VerticesOnLine( { 4, 10, 16 }, 0));
	constraints.emplace_back(Constraint::VerticesSymmetricOnLineThroughPoint( {
			2, 17, 3, 18, 1, 19, 4, 16, 5, 13, 6, 14, 7, 15, 8, 11, 9, 12 }, 10,
	M_PI_2));

	constraints.emplace_back(Constraint::VerticesOnLine( { 5, 6 }, M_PI_2));
	constraints.emplace_back(Constraint::VerticesOnLine( { 8, 9 }, M_PI_2));

	constraints.emplace_back(Constraint::VerticesOnLine( { 0, 3 }, 0));
	constraints.emplace_back(Constraint::VerticesOnLine( { 2, 3 }, M_PI_2));

	Update();
}

void Design::Update() {
	UpdateEdges();
	UpdateSplits();
}

void Design::Modify(bool modified_) {
	this->modified = modified_;
}

bool Design::IsModified() const {
	return modified;
}

void Design::Paint() const {
//	for (const Patch &p : patches)
//		p.Paint();
//	for (const Edge &e : edges)
//		e.geo.Paint();
}

void Design::PatchEdge::UpdatePolynomials(const std::vector<PatchVertex> &v) {
	const size_t N = vidx.size();

	u0 = DBL_MAX;
	u1 = -DBL_MAX;
	v0 = DBL_MAX;
	v1 = -DBL_MAX;
	std::vector<Vector2> temp;
	temp.reserve(N);
	for (size_t idx : vidx) {
		const PatchVertex &vert = v[idx];
		u0 = std::fmin(u0, vert.u);
		u1 = std::fmax(u1, vert.u);
		v0 = std::fmin(v0, vert.v);
		v1 = std::fmax(v1, vert.v);
		temp.push_back(vert);
	}

	if (N > 0) {
		while (temp.front().u <= -M_PI)
			temp.front().u += 2.0 * M_PI;
		while (temp.front().u > M_PI)
			temp.front().u -= 2.0 * M_PI;
		double ru = temp.front().u;
		for (size_t idx = 1; idx < N; idx++) {
			while (temp[idx].u - ru <= -M_PI)
				temp[idx].u += 2.0 * M_PI;
			while (temp[idx].u - ru > M_PI)
				temp[idx].u -= 2.0 * M_PI;
			ru = temp[idx].u;
		}
	}
	switch (N) {
	case 0:
		iu = Polynomial::ByBezier(0.0);
		iv = Polynomial::ByBezier(0.0);
		break;
	case 1:
		iu = Polynomial::ByBezier(temp[0].u);
		iv = Polynomial::ByBezier(temp[0].v);
		break;
	case 2:
		iu = Polynomial::ByBezier(temp[0].u, temp[1].u);
		iv = Polynomial::ByBezier(temp[0].v, temp[1].v);
		break;
	case 3:
		iu = Polynomial::ByBezier(temp[0].u, temp[1].u, temp[2].u);
		iv = Polynomial::ByBezier(temp[0].v, temp[1].v, temp[2].v);
		break;
	case 4:
		iu = Polynomial::ByBezier(temp[0].u, temp[1].u, temp[2].u, temp[3].u);
		iv = Polynomial::ByBezier(temp[0].v, temp[1].v, temp[2].v, temp[3].v);
		break;
	default:
		iu.clear();
		iv.clear();
		for (Vector2 &vect : temp) {
			iu.push_back(vect.u);
			iv.push_back(vect.v);
		}
		break;
	}
	diu = iu.Derivative(1);
	div = iv.Derivative(1);
	ddiu = diu.Derivative(1);
	ddiv = div.Derivative(1);
}

Vector2 Design::PatchEdge::operator ()(double r) const {
	const size_t N = iu.size();
	if (N <= 4) {
		double u = iu(r);
		double v = iv(r);
		return {u,v};
	}
	std::vector<Vector2> temp;
	temp.reserve(N - 1);
	Vector2 t(iu.front(), iv.front());
	for (size_t idx = 1; idx < N; idx++) {
		Vector2 t2(iu[idx], iv[idx]);
		temp.push_back(t.Interp(t2, r));
		t = t2;
	}

	for (size_t n = 1; n < (N - 1); n++) {
		for (size_t m = 1; m < (N - n); m++) {
			temp[m - 1] = temp[m - 1].Interp(temp[m], r);
		}
	}

	return temp.front();
}

Vector2 Design::PatchEdge::Slope(double r, unsigned int order) const {
	const size_t N = iu.size();
	if (order >= N)
		return {0,0};
	if (N <= 4) {
		switch (order) {
		case 3:
			return {6.0*iu[3], 6.0*iv[3]};
		case 2:
			return {ddiu(r), ddiv(r)};
		case 1:
			return {diu(r), div(r)};
		}
		return {iu(r), iv(r)};
	}
	std::vector<Vector2> temp;
	temp.reserve(N);
	for (size_t idx = 0; idx < N; idx++)
		temp.emplace_back(iu[idx], iv[idx]);
	double f = 1.0;
	size_t n;
	for (n = 1; n <= order; n++) {
		f *= (double) (N - n);
		for (size_t m = 1; m < (N - n); m++) {
			temp[m - 1] = temp[m] - temp[m - 1];
		}
	}
	for (; n < (N - 1); n++) {
		for (size_t m = 1; m < (N - n); m++) {
			temp[m - 1] = temp[m - 1].Interp(temp[m], r);
		}
	}
	return f * temp.front();
}

void Design::PatchEdge::ShiftU(double shift) {
	const size_t N = iu.size();
	if (N <= 4) {
		iu[0] += shift;
	} else {
		for (size_t idx = 0; idx < N; idx++)
			iu[idx] += shift;
	}
}

double Design::PatchEdge::FindR(const Vector2 &v, double *returnDist) const {
	const double rmin = 0.0;
	const double rmax = 1.0;

	auto dist = [&v](Vector2 p) {
		while (p.u <= -M_PI)
			p.u += 2.0 * M_PI;
		while (p.u > M_PI)
			p.u -= 2.0 * M_PI;
		return (p - v).Abs();
	};

	// Initial search
	double r = 0.0;
	const size_t N = Size() * 2;
	Polynomial rTest = Polynomial::ByValue(0, 0, N - 1, 1);
	double dMin = DBL_MAX;
	for (size_t n = 0; n < N; n++) {
		double rCandidate = rTest(n);
		double d = dist(operator()(rCandidate));
		if (d < dMin) {
			dMin = d;
			r = rCandidate;
		}
	}

	// Newton-Raphson to find the minimum distance
	// Limited to N iterations (= 2 * the number of coefficients)
	// Limited to an improvement of min. FLT_EPSILON per iteration
	double dr = rTest(1.0) - rTest(0.0);
	double d1 = dist(operator()(r));
	size_t iterations = 0;
	while (iterations < N) {
		iterations++;
		const double d0 = dist(operator()(r - dr));
		const double d2 = dist(operator()(r + dr));
		// Parabula-fit and finding the minimum of the parabula
		const double T1 = (d0 - d2) * dr;
		const double T2 = 2.0 * (d2 - 2 * d1 + d0);
		// If T2 < 0 the parabula opens downwards. This would
		// calculate a high point not a low point.
		if (T2 <= FLT_EPSILON)
			break;
		const double rUpdate = r + T1 / T2;
		const double dUpdate = dist(operator()(rUpdate));
		if (dUpdate > d1 - FLT_EPSILON)
			break;
		d1 = dUpdate;
		r = rUpdate;
		if (r < rmin) {
			r = rmin;
			break;
		}
		if (r > rmax) {
			r = rmax;
			break;
		}
		dr /= 2.0;
	}
	if (returnDist != nullptr)
		*returnDist = dist(operator()(r));
	return r;
}

bool Design::SplitVertex::operator==(const SplitVertex &other) const {
	return (eidx == other.eidx) && (std::fabs(r - other.r) < FLT_EPSILON);
}

bool Design::SplitVertex::operator<(const SplitVertex &other) const {
	if (eidx < other.eidx)
		return true;
	if (eidx > other.eidx)
		return false;
	return r < other.r;
}

bool Design::SplitEdge::operator ==(const SplitEdge &other) const {
	if (eidx != other.eidx)
		return false;
	if (vidx0 != other.vidx0)
		return false;
	if (vidx1 != other.vidx1)
		return false;
	return true;
}

bool Design::SplitEdge::operator <(const SplitEdge &other) const {
	if (eidx < other.eidx)
		return true;
	if (eidx > other.eidx)
		return false;
	if (vidx0 < other.vidx0)
		return true;
	if (vidx0 > other.vidx0)
		return false;
	return vidx1 < other.vidx1;
}

void Design::SplitEdge::Flip() {
	std::swap(r0, r1);
	std::swap(vidx0, vidx1);
}

Design::Patch::Patch(std::initializer_list<size_t> eidx,
		const std::string &name) :
		name(name), eidx(eidx) {
}

Design::Constraint Design::Constraint::VertexOnEdge(size_t vidx, size_t eidx) {
	Constraint ret;
	ret.v.push_back(vidx);
	ret.vidx = vidx;
	ret.eidx = eidx;
	ret.inPoint = true;
	ret.rSearch = true;
	return ret;
}

Design::Constraint Design::Constraint::VertexOnEdge(size_t vidx, size_t eidx,
		double r) {
	Constraint ret;
	ret.v.push_back(vidx);
	ret.vidx = vidx;
	ret.eidx = eidx;
	ret.inPoint = true;
	ret.r = r;
	return ret;
}

Design::Constraint Design::Constraint::VerticesOnHorizontalLine(
		std::initializer_list<size_t> idx, double v) {
	Constraint ret;
	ret.v = idx;
	ret.p0.v = v;
	return ret;
}

Design::Constraint Design::Constraint::VerticesOnVerticalLine(
		std::initializer_list<size_t> idx, double u) {
	Constraint ret;
	ret.v = idx;
	ret.p0.u = u;
	ret.angle = M_PI_2;
	return ret;
}

Design::Constraint Design::Constraint::VerticesOnLine(
		std::initializer_list<size_t> idx, double angle) {
	Constraint ret;
	ret.v = idx;
	ret.angle = angle;
	ret.moveParallel = true;
	return ret;
}

Design::Constraint Design::Constraint::VerticesSymmetricOnLineThroughPoint(
		std::initializer_list<size_t> idx, size_t vidx, double angle) {
	Constraint ret;
	ret.symmetric = true;
	ret.v = idx;
	ret.angle = angle;
	ret.vidx = vidx;
	return ret;
}

size_t Design::Constraint::CountEquations() const {
	return v.size() * 2;
}

size_t Design::Constraint::CountFreeParameter() const {
	if (symmetric) {
		return v.size();
	}
	return ((moveParallel && !inPoint) ? 1 : 0) + (inPoint ? 0 : v.size());
}

size_t Design::ConstraintRow(size_t cidx, size_t vidx) const {
	size_t row = 0;
	size_t idx = 0;
	if (idx == cidx)
		return row + vidx * 2;
	for (const Constraint &c : constraints) {
		row += c.CountEquations();
		idx++;
		if (idx == cidx)
			return row + vidx * 2;
	}
	return row;
}

size_t Design::ConstraintCol(size_t cidx, size_t vidx) const {
	size_t col = vertices.size() * 2;
	size_t idx = 0;
	if (idx == cidx)
		return col + vidx;
	for (const Constraint &c : constraints) {
		col += c.CountFreeParameter();
		idx++;
		if (idx == cidx)
			return col + vidx;
	}
	return col;
}

void Design::PrepareConstraints() {

	for (Constraint &c : constraints) {
		// 1. Init construction line
		if (c.eidx == nothing) {
			if (c.vidx != nothing)
				c.p0 = vertices[c.vidx];
			c.t.u = 1;
			c.t.v = 0;
		} else {
			const PatchEdge &e = edges[c.eidx];
			if (c.rSearch) {
				if (c.vidx == (size_t) -1) {
					c.r = e.FindR(c.p0);
				} else {
					c.r = e.FindR(vertices[c.vidx]);
				}
			}
			c.p0 = e(c.r);
			while (c.p0.u <= -M_PI)
				c.p0.u += 2.0 * M_PI;
			while (c.p0.u > M_PI)
				c.p0.u -= 2.0 * M_PI;

			if (c.angleOrigin == Constraint::Angle::VERTEX && c.vidx != nothing
					&& !c.inPoint) {
				// Tangent points from edge to vertex
				c.t = vertices[c.vidx] - c.p0;
				c.t.Normalize();
			} else if (c.angleOrigin == Constraint::Angle::EDGE && !c.inPoint) {
				// Tangent along the edge
				const double dr = 1e-6;
				const Vector2 e0 = e(c.r - dr);
				const Vector2 e1 = e(c.r + dr);
				c.t = (e1 - e0).Normal();
			} else {
				// Global tangent in 0 deg orientation
				c.t.u = 1.0;
				c.t.v = 0.0;
			}
		}

		// 2. Rotate the line by angle
		c.t = c.t.RotatedBy(c.angle);
		c.n = c.t.Orthogonal();

		// 3. Calculate initial solution
		c.vinit.clear();
		c.vinit.reserve(c.v.size());

		if (c.symmetric) {
			if (c.inPoint) {
				// Point symmetry
				for (size_t idx1 = 1; idx1 < c.v.size(); idx1 += 2) {
					size_t idx0 = idx1 - 1;
					Vector2 v0 = vertices[c.v[idx0]] - c.p0;
					Vector2 v1 = vertices[c.v[idx1]] - c.p0;
					Vector2 err = 0.5 * (v0 + v1);
					c.vinit.push_back(v0 - err + c.p0);
					c.vinit.push_back(v1 - err + c.p0);
				}
			} else {
				// Line Symmetry
				for (size_t idx1 = 1; idx1 < c.v.size(); idx1 += 2) {
					size_t idx0 = idx1 - 1;
					Vector2 v0 = vertices[c.v[idx0]] - c.p0;
					Vector2 v1 = vertices[c.v[idx1]] - c.p0;
					double x0 = v0 * c.t;
					double y0 = v0 * c.n;
					double x1 = v1 * c.t;
					double y1 = v1 * c.n;
					x0 = (x0 + x1) / 2.0;
					x1 = x0;
					y0 = (y0 - y1) / 2.0;
					y1 = -y0;
					c.vinit.push_back(Vector2(c.p0 + x0 * c.t + y0 * c.n));
					c.vinit.push_back(Vector2(c.p0 + x1 * c.t + y1 * c.n));
				}
			}
		} else {
			if (c.inPoint) {
				c.vinit.assign(c.v.size(), c.p0);
			} else {
				if (c.moveParallel) {
					// Move the line such that all vertices have the average
					// minimal distance to the line along n.
					double f = 0.0;
					for (size_t idx : c.v) {
						const PatchVertex &v = vertices[idx];
						f += (v - c.p0) * c.n;
					}
					c.p0 += (f / (double) c.v.size()) * c.n;
				}
				// Move all vertices onto the line along n.
				for (size_t idx : c.v) {
					const PatchVertex &v = vertices[idx];
					double f = (v - c.p0) * c.n;
					c.vinit.push_back(v - f * c.n);
				}
			}
		}
	}
}

std::vector<Design::SplitEdge> Design::FindLoop(const SplitEdge &begin,
		const std::set<size_t> &eidx) const {
	std::vector<SplitEdge> ret;
	ret.push_back(begin);

	auto patchContainsEdge = [&eidx=eidx](const SplitEdge &e) {
		return eidx.count(e.eidx) != 0;
	};

	while (true) {
		const SplitEdge &e0 = ret.back();
		size_t v0 = e0.vidx1;
		Vector2 d0 = edges[e0.eidx].Slope(e0.r1, 1);
		if (e0.r1 < e0.r0)
			d0 = -d0;
		// Initialize for a dead-end where the reverse of the incoming edge
		// is the outgoing edge.
		double maxa = -M_PI;
		SplitEdge en = e0;
		en.Flip();
		for (const SplitEdge &e1 : splitE) {
			if (!patchContainsEdge(e1))
				continue;
			// Skip the incoming edge
			if (e1.eidx == e0.eidx
					&& ((e1.vidx0 == e0.vidx0 && e1.vidx1 == e0.vidx1)
							|| (e1.vidx0 == e0.vidx1 && e1.vidx1 == e0.vidx0)))
				continue;

			Vector2 d1;
			if (e1.vidx0 == v0) {
				// Pointing-away from vidx0
				d1 = edges[e1.eidx].Slope(e1.r0, 1);
			} else if (e1.vidx1 == v0) {
				// Pointing away from vidx1
				d1 = -edges[e1.eidx].Slope(e1.r1, 1);
			} else {
				// Not connected to v0
				continue;
			}
			// Offsetting d1 on d0
			double hx = d0.u * d1.u + d0.v * d1.v;
			double hy = d0.u * d1.v - d0.v * d1.u;
			// Largest angle in -M_PI < a < M_PI
			double a = std::atan2(hy, hx);
			if (a > maxa) {
				maxa = a;
				en = e1;
			}
		}
		// Flip the found edge, if it is connected from the end.
		if (en.vidx1 == v0)
			en.Flip();

		// Loop is closed, if the last edge found if the first edge in the list
		if (en == ret.front())
			break;

		ret.push_back(en);

		// If more than 2 times the number of available edges were found,
		// the algorithm is broken. (Maybe a numerical instability when
		// many edges are on top of each other.)
		if (ret.size() > 3 * splitE.size()) {
			for (const SplitEdge &e : ret) {
				std::cout << "[" << e.eidx << "] " << e.vidx0 << " -> "
						<< e.vidx1 << "\n";
			}
			std::ostringstream err;
			err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - ";
			err << "The edge loops for the patches could not be determined.";
			err << " The algorithm assigned " << ret.size() << " edges.";
			err << " There are " << splitE.size() << " edges available.";
			err << " Each edge should be only assigned up to two times.";
			throw std::runtime_error(err.str());
		}
	}
	return ret;
}

void Design::UpdateEdges() {

	std::set<size_t> uncovered;
	std::set<size_t> interacting;
	for (size_t idx = 0; idx < vertices.size(); idx++)
		uncovered.insert(idx);
	for (const Constraint &c : constraints) {
		for (size_t vidx : c.v)
			uncovered.erase(vidx);
		if (c.eidx != nothing)
			for (size_t vidx : edges[c.eidx].vidx)
				interacting.insert(vidx);
	}

	size_t M = ConstraintRow(constraints.size()) + uncovered.size() * 2;
	size_t N = ConstraintCol(constraints.size());
	Eigen::MatrixXd A;
	Eigen::MatrixXd b;

	int iteration = 0;
	while (iteration < 5) {
		iteration++;
		// Update all edges for interpolation
		for (PatchEdge &e : edges)
			e.UpdatePolynomials(vertices);

		PrepareConstraints();

		// Fill conditions into matrices A, b
		A = Eigen::MatrixXd::Zero(M, N);
		b = Eigen::MatrixXd::Zero(M, 1);
		size_t offset = 0;
		size_t offsetcol = vertices.size() * 2;
		for (const Constraint &c : constraints) {

			if (c.symmetric) {
				for (size_t idx1 = 1; idx1 < c.v.size(); idx1 += 2) {
					size_t idx0 = idx1 - 1;
					size_t vidx0 = c.v[idx0];
					size_t vidx1 = c.v[idx1];
					A(offset + 0, vidx0 * 2 + 0) = 1;
					A(offset + 1, vidx0 * 2 + 1) = 1;
					A(offset + 2, vidx1 * 2 + 0) = 1;
					A(offset + 3, vidx1 * 2 + 1) = 1;
					b(offset + 0, 0) = c.vinit[idx0].u;
					b(offset + 1, 0) = c.vinit[idx0].v;
					b(offset + 2, 0) = c.vinit[idx1].u;
					b(offset + 3, 0) = c.vinit[idx1].v;
					A(offset + 0, offsetcol) = -c.t.u;
					A(offset + 1, offsetcol) = -c.t.v;
					if (c.inPoint) {
						A(offset + 2, offsetcol) = c.t.u;
						A(offset + 3, offsetcol) = c.t.v;
					} else {
						A(offset + 2, offsetcol) = -c.t.u;
						A(offset + 3, offsetcol) = -c.t.v;
					}
					offsetcol++;
					A(offset + 0, offsetcol) = -c.n.u;
					A(offset + 1, offsetcol) = -c.n.v;
					A(offset + 2, offsetcol) = c.n.u;
					A(offset + 3, offsetcol) = c.n.v;
					offsetcol++;
					offset += 4;
				}
			} else {
				size_t offsetcol2 = offsetcol
						+ ((c.moveParallel && !c.inPoint) ? 1 : 0);
				for (size_t idx0 = 0; idx0 < c.v.size(); idx0++) {
					size_t vidx = c.v[idx0];
					A(offset + 0, vidx * 2 + 0) = 1;
					A(offset + 1, vidx * 2 + 1) = 1;
					b(offset + 0, 0) = c.vinit[idx0].u;
					b(offset + 1, 0) = c.vinit[idx0].v;
					if (c.moveParallel && !c.inPoint) {
						A(offset + 0, offsetcol) = -c.n.u;
						A(offset + 1, offsetcol) = -c.n.v;
					}
					if (!c.inPoint) {
						A(offset + 0, offsetcol2) = -c.t.u;
						A(offset + 1, offsetcol2) = -c.t.v;
						offsetcol2++;
					}

					offset += 2;
				}
				offsetcol = offsetcol2;
			}
		}
		for (size_t vidx : uncovered) {
			A(offset + 0, vidx * 2 + 0) = 1;
			A(offset + 1, vidx * 2 + 1) = 1;
			b(offset + 0, 0) = vertices[vidx].u;
			b(offset + 1, 0) = vertices[vidx].v;
			offset += 2;
		}

		Eigen::CompleteOrthogonalDecomposition Dec =
				A.completeOrthogonalDecomposition();
		Eigen::MatrixXd x0 = Dec.solve(b);
		Eigen::MatrixXd H = Eigen::MatrixXd::Identity(A.cols(), A.cols())
				- Dec.solve(A);
		Eigen::FullPivLU lu = H.fullPivLu();
		Eigen::MatrixXd U =
				lu.matrixLU().triangularView<Eigen::UpLoType::Upper>();
		Eigen::MatrixXd Q = lu.permutationQ();
		Eigen::MatrixXd S = // @suppress("Invalid arguments")
				(U * Q.transpose()).block(0, 0, lu.rank(), Q.cols()).transpose();
		Eigen::MatrixXd U2 = S.block(vertices.size() * 2, 0, // @suppress("Invalid arguments")
				S.rows() - vertices.size() * 2, S.cols());
		Eigen::CompleteOrthogonalDecomposition Dec2 =
				U2.completeOrthogonalDecomposition();
		Eigen::MatrixXd xend = x0.block(vertices.size() * 2, 0, // @suppress("Invalid arguments")
				x0.rows() - vertices.size() * 2, 1);
		Eigen::MatrixXd diff = Dec2.solve(-xend);
		Eigen::MatrixXd x = x0 + S * diff;

		// Copy the solution back into the vertices and check if one or more
		// of the interacting vertices have changed position.
		bool runAgain = false;
		for (size_t vidx = 0; vidx < vertices.size(); vidx++) {
			PatchVertex &v = vertices[vidx];

			PatchVertex vnew(x(vidx * 2 + 0, 0), x(vidx * 2 + 1, 0));
			if ((v - vnew).Abs() > 1e-6
					&& interacting.find(vidx) != interacting.end())
				runAgain = true;

			v.u = vnew.u;
			v.v = vnew.v;

//			while (v.u <= -M_PI)
//				v.u += 2.0 * M_PI;
//			while (v.u > M_PI)
//				v.u -= 2.0 * M_PI;
//			v.v = std::fmin(std::fmax(vnew.v, 0.0), 4.0);
		}
#ifdef DEBUG
		{
			auto toMatrix = [](const std::string &name,
					const Eigen::MatrixXd &m) {
				Matrix ret(name, { (size_t) m.rows(), (size_t) m.cols() });
				for (int idx1 = 0; idx1 < m.cols(); idx1++)
					for (int idx0 = 0; idx0 < m.rows(); idx0++)
						ret(idx0, idx1) = m(idx0, idx1);
				return ret;
			};

//			Exporter ex("/tmp/lu.mat");
//			ex.WriteMatrix(toMatrix("x0", x0));
//			ex.WriteMatrix(toMatrix("x", x));
//			ex.WriteMatrix(toMatrix("A", A));
//			ex.WriteMatrix(toMatrix("b", b));
//			ex.WriteMatrix(toMatrix("H2", H));
//			ex.WriteMatrix(toMatrix("P2", P));
//			ex.WriteMatrix(toMatrix("L2", L));
//			ex.WriteMatrix(toMatrix("U2", U));
//			ex.WriteMatrix(toMatrix("Q2", Q));
//			ex.WriteMatrix(toMatrix("diff", diff));

		}
#endif
		if (!runAgain)
			break;

	}

	// Update all edges for interpolation
	for (PatchEdge &e : edges)
		e.UpdatePolynomials(vertices);

	// Update the edge geometry
	const size_t Inter = 40;
	Polynomial p = Polynomial::ByValue(0, 0, Inter, 1);
	for (PatchEdge &e : edges) {
		e.geo.Clear();
		for (size_t n = 0; n <= Inter; n++) {
			Geometry::Vertex v;
			Vector2 pos = e(p(n));
			v.u = pos.u;
			v.v = pos.v;
			e.geo.AddEdgeToVertex(v);
		}
		e.geo.paintEdges = true;
	}
}

void Design::UpdateSplits() {
	std::vector<SplitVertex> sv;

	// Initialize list with first and last point of each line.
	for (size_t idx = 0; idx < edges.size(); idx++) {
		const PatchEdge &ed = edges[idx];
		const Vector2 v0 = ed(0.0);
		const Vector2 v1 = ed(1.0);
		SplitVertex spv0(v0.u, v0.v, idx, 0.0);
		SplitVertex spv1(v1.u, v1.v, idx, 1.0);
		if (std::find(sv.begin(), sv.end(), spv0) == sv.end())
			sv.push_back(spv0);
		if (std::find(sv.begin(), sv.end(), spv1) == sv.end())
			sv.push_back(spv1);
	}

	// Calculate list of all other intersections.
	for (size_t idx0 = 0; idx0 < edges.size(); idx0++) {
		const PatchEdge &e0 = edges[idx0];
		size_t N0 = e0.vidx.size() * 2 + 1;
		Polynomial pr0 = Polynomial::ByValue(0, 0, N0 - 1, 1);
		// idx1 starts at idx0 to enable self-intersection of lines.
		for (size_t idx1 = idx0; idx1 < edges.size(); idx1++) {
			const PatchEdge &e1 = edges[idx1];
			// Edges could not possible overlap in v direction.
			if (e0.v0 > e1.v1 + FLT_EPSILON || e0.v1 < e1.v0 - FLT_EPSILON)
				continue;

			size_t N1 = e1.vidx.size() * 2 + 1;
			Polynomial pr1 = Polynomial::ByValue(0, 0, N1 - 1, 1);
#ifdef DEBUG
//			Matrix Md = Matrix::Value(10.0, { N0, N1 });
//			Md.SetVariableName("d");
#endif
			for (size_t n0 = 0; n0 < N0; n0++) {
				for (size_t n1 = 0; n1 < N1; n1++) {
					if (idx0 == idx1 && n0 == n1)
						continue;
					double r0 = pr0(n0);
					double r1 = pr1(n1);
//					if (idx0
//							== 7&& idx1 == 7 && r0 <= FLT_EPSILON && r1 >=1.0-FLT_EPSILON)
//						std::cout << "X\n";

					Vector2 dv0 = e0.Slope(r0, 1);
					Vector2 dv1 = e1.Slope(r1, 1);
					double det = dv0.v * dv1.u - dv0.u * dv1.v;
					if (fabs(det) < FLT_EPSILON) {
						if (idx0 == idx1) {
							Vector2 v0 = e0(r0);
							Vector2 v1 = e1(r1);
							double shift = v1.u - v0.u;
							shift -= std::fmod(shift, 2.0 * M_PI);
							v1.u -= shift;
							double d = (v1 - v0).Abs();
							if (d < FLT_EPSILON) {
								SplitVertex spv0(v0.u, v0.v, idx0, r0);
								SplitVertex spv1(v1.u, v1.v, idx1, r1);
								if (std::find(sv.begin(), sv.end(), spv0)
										== sv.end())
									sv.push_back(spv0);
								if (std::find(sv.begin(), sv.end(), spv1)
										== sv.end())
									sv.push_back(spv1);
							}
						}
						continue;
					}

					Vector2 v0 = e0(r0);
					Vector2 v1 = e1(r1);
					double shift = v1.u - v0.u;
					shift -= std::fmod(shift, 2.0 * M_PI);
//					if (fabs(shift) > FLT_EPSILON)
//						std::cout << "Shift != 0.0\n";

					double nr0 = r0
							+ ((v0.u - v1.u + shift) * dv1.v
									+ (v1.v - v0.v) * dv1.u) / det;
					double nr1 = r1
							+ ((v0.u - v1.u + shift) * dv0.v
									+ (v1.v - v0.v) * dv0.u) / det;

					// Skip trivial self-intersections
					if (idx0 == idx1 && std::fabs(nr0 - nr1) < FLT_EPSILON)
						continue;

					nr0 = std::fmin(std::fmax(nr0, 0.0), 1.0);
					nr1 = std::fmin(std::fmax(nr1, 0.0), 1.0);

					Vector2 nv0 = e0(nr0);
					Vector2 nv1 = e1(nr1);
					nv1.u -= shift;
					double d = (nv1 - nv0).Abs();
#ifdef DEBUG
//					Md(n0, n1) = d;
#endif
					if (d > 0.1)
						continue;
					for (uint8_t iteration = 0; iteration < 10; iteration++) {
						if (d < FLT_EPSILON)
							break;
						const double d0 = d;
						dv0 = e0.Slope(nr0, 1);
						dv1 = e1.Slope(nr1, 1);
						det = dv0.v * dv1.u - dv0.u * dv1.v;
						if (fabs(det) < FLT_EPSILON)
							break;

						v0 = e0(nr0);
						v1 = e1(nr1);
						shift = v1.u - v0.u;
						shift -= std::fmod(shift, 2.0 * M_PI);
//						if (fabs(shift) > FLT_EPSILON)
//							std::cout << "Shift != 0.0\n";

						nr0 = nr0
								+ ((v0.u - v1.u + shift) * dv1.v
										+ (v1.v - v0.v) * dv1.u) / det;
						nr1 = nr1
								+ ((v0.u - v1.u + shift) * dv0.v
										+ (v1.v - v0.v) * dv0.u) / det;
						if (idx0 == idx1 && std::fabs(nr0 - nr1) < FLT_EPSILON)
							break;

						nr0 = std::fmin(std::fmax(nr0, 0.0), 1.0);
						nr1 = std::fmin(std::fmax(nr1, 0.0), 1.0);
						nv0 = e0(nr0);
						nv1 = e1(nr1);
						nv1.u -= shift;
						d = (nv1 - nv0).Abs();
						if (d > d0 - FLT_EPSILON)
							break;
					}
					if (idx0 == idx1 && std::fabs(nr0 - nr1) < FLT_EPSILON)
						continue;
					if (d > FLT_EPSILON)
						continue;

					SplitVertex spv0(nv0.u, nv0.v, idx0, nr0);
					SplitVertex spv1(nv1.u, nv1.v, idx1, nr1);
					if (std::find(sv.begin(), sv.end(), spv0) == sv.end())
						sv.push_back(spv0);
					if (std::find(sv.begin(), sv.end(), spv1) == sv.end())
						sv.push_back(spv1);

				}
			}
#ifdef DEBUG
			{
//				Exporter ex("/tmp/dist.mat");
//				ex.WriteMatrix(Md);
//
//				std::cout << "Px = " << e0.iu << ";\n";
//				std::cout << "Py = " << e0.iv << ";\n";
//				std::cout << "Px2 = " << e1.iu << ";\n";
//				std::cout << "Py2 = " << e1.iv << ";\n";
			}
#endif
		}
	}
	splitV.clear();
	splitE.clear();
	std::sort(sv.begin(), sv.end());
	double r0 = 0.0;
	size_t vidx0 = 0.0;
	size_t eidx = (size_t) -1;
	for (size_t idx = 0; idx < sv.size(); idx++) {
		const SplitVertex &ref = sv[idx];
		auto res = std::find_if(splitV.begin(), splitV.end(),
				[&ref](const Vector2 &other) {
					return ref.CloseTo(other);
				});
		size_t vidx = splitV.size();
		if (res != splitV.end())
			vidx = res - splitV.begin();
		else
			splitV.emplace_back(ref.u, ref.v);
		if (eidx != ref.eidx) {
			eidx = ref.eidx;
			r0 = ref.r;
			vidx0 = vidx;
		} else {
			SplitEdge se;
			se.eidx = eidx;
			se.r0 = r0;
			se.r1 = ref.r;
			r0 = ref.r;
			se.vidx0 = vidx0;
			se.vidx1 = vidx;
			vidx0 = vidx;
			splitE.push_back(se);
		}
	}

#ifdef DEBUG
	std::cout << __FILE__ << ":" << __LINE__ << ":\n";
	std::cout << "Number of intersections: " << sv.size() << "\n";
	std::cout << "sv = {\n";
	for (const auto &p : sv)
		std::cout << p.eidx << " (" << p.r << ") : " << p.u << ", " << p.v
				<< "\n";
	std::cout << "}\nsplitV = {\n";
	size_t c = 0;
	for (const auto &v : splitV)
		std::cout << c++ << " : " << v.u << ", " << v.v << "\n";
	std::cout << "}\nsplitE = {\n";
	c = 0;
	for (const auto &e : splitE)
		std::cout << c++ << "[" << e.eidx << "] : " << e.vidx0 << "(" << e.r0
				<< ") --> " << e.vidx1 << "(" << e.r1 << ")\n";
	std::cout << "}\n----------\n";
#endif
}

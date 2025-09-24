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
#include "../../math/Exporter.h"
#include "../../math/Polynomial.h"

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
}

void Design::Update() {
	UpdateEdges();
	UpdateSplits();
	UpdatePatches(0.05);
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
	Umin = DBL_MAX;
	Umax = -DBL_MAX;
	Vmin = DBL_MAX;
	Vmax = -DBL_MAX;
}

void Design::Patch::UpdateBoundingBox() {
	Umin = DBL_MAX;
	Umax = -DBL_MAX;
	Vmin = DBL_MAX;
	Vmax = -DBL_MAX;

	for (const Geometry::Vertex &vert : v) {
		Umin = std::fmin(Umin, vert.u);
		Umax = std::fmax(Umax, vert.u);
		Vmin = std::fmin(Vmin, vert.v);
		Vmax = std::fmax(Vmax, vert.v);
	}
}

void Design::Patch::Update(double edgeLength, const Polynomial &U2X,
		const Polynomial &V2Y) {
	UpdateBoundingBox();

// Mark vertices and edges as group 0
	for (Vertex &vert : v)
		vert.group = 0;
	for (Edge &ed : e)
		ed.group = 0;

	const double fh = std::sqrt(3.0) / 2.0;

	const double dx = edgeLength;
	const double dy = edgeLength * fh;

	const double Xmin = U2X(Umin);
	const double Xmax = U2X(Umax);
	const double Ymin = V2Y(Vmin);
	const double Ymax = V2Y(Vmax);

	const size_t Nx = (size_t) std::floor((Xmax - Xmin) / dx);
	const size_t Ny = (size_t) std::floor((Ymax - Ymin) / dy);
	if (Ny == 0)
		return;

	const double Xoffset = Xmin + ((Xmax - Xmin) - (double) Nx * dx) / 2.0;
	const double Yoffset = Ymin + ((Ymax - Ymin) - (double) Ny * dy) / 2.0;

	const Polynomial N2Y { Yoffset, dy };
	const Polynomial N2Xeven { Xoffset - dx / 4.0, dx };
	const Polynomial N2Xodd = { Xoffset + dx / 4.0, dx };
	const Polynomial X2Neven = N2Xeven.Inverse();
	const Polynomial X2Nodd = N2Xodd.Inverse();

	auto spherewidth = [](const Vector3 &origin, const double h,
			const double y) {
		const double a = fabs(origin.y - y);
		if (a > h)
			return 0.0;
		return sqrt(h * h - a * a);
	};

	struct Block {
		double x0;
		double x1;
		int dir;
		size_t eidx;
	};

// It is assumed, that all edges are oriented correctly. I.e.
// mathematically positive around the outer perimeter, mathematically
// negative for holes.
	const size_t countOutlineEdges = CountEdges();

	std::map<int, size_t> mLast;
	std::map<int, size_t> eLast;
	std::map<size_t, size_t> Vertex2Edge;

	for (size_t m = 0; m < Ny; m++) {
		const double y = N2Y(m);
		const double v_ = V2Y.InvEval(y, (Vmin + Vmax) / 2.0);

		std::vector<Block> block;

		const double h = dx * (0.5 + FLT_EPSILON);

		// Find all edges intersecting a line.
		for (size_t idx = 0; idx < countOutlineEdges; idx++) {
//				const Geometry::Edge &ed = GetEdge(idx);
			const Geometry::Vertex &v0_ = GetEdgeVertex(idx, 0);
			const Geometry::Vertex &v1_ = GetEdgeVertex(idx, 1);

			const Vector3 v0(U2X(v0_.u), V2Y(v0_.v));
			const Vector3 v1(U2X(v1_.u), V2Y(v1_.v));

			if ((v0.y < y - h && v1.y < y - h)
					|| (v0.y > y + h && v1.y > y + h))
				continue;

			double bx0 = DBL_MAX;
			double bx1 = -DBL_MAX;

			Vector3 dv = v1 - v0;
			const double L = dv.Abs();
			if (fabs(L) > FLT_EPSILON)
				dv /= L;

			// Line capsule intersection
			Vector3 dp(-dv.y, dv.x);
			// Line intersection

			Vector3 corner0 = v0 - dp * h;
			Vector3 corner1 = v0 + dp * h;
			Vector3 corner2 = v1 - dp * h;
			Vector3 corner3 = v1 + dp * h;

			if ((corner0.y > y + FLT_EPSILON && corner2.y < y - FLT_EPSILON)
					|| (corner0.y < y - FLT_EPSILON
							&& corner2.y > y + FLT_EPSILON)) {
				const double x = corner0.x + (y - corner0.y) * (dv.x / dv.y);
				if (x > bx1)
					bx1 = x;
				if (x < bx0)
					bx0 = x;
			}
			if ((corner1.y > y + FLT_EPSILON && corner3.y < y - FLT_EPSILON)
					|| (corner1.y < y - FLT_EPSILON
							&& corner3.y > y + FLT_EPSILON)) {
				const double x = corner1.x + (y - corner1.y) * (dv.x / dv.y);
				if (x > bx1)
					bx1 = x;
				if (x < bx0)
					bx0 = x;
			}

			// Sphere 1
			if (fabs(v0.y - y) < h) {
				const double sw = spherewidth(v0, h, y);
				const double x0 = v0.x - sw;
				const double x1 = v0.x + sw;
				if (x1 > bx1)
					bx1 = x1;
				if (x0 < bx0)
					bx0 = x0;
			}
			// Sphere 2
			if (fabs(v1.y - y) < h) {
				const double sw = spherewidth(v1, h, y);
				const double x0 = v1.x - sw;
				const double x1 = v1.x + sw;
				if (x1 > bx1)
					bx1 = x1;
				if (x0 < bx0)
					bx0 = x0;
			}

			if (bx0 < bx1 + FLT_EPSILON) {
				Block b { bx0, bx1, 0, idx };
				if (v0.y < v1.y) {
					b.dir = 1;
					b.x1 -= FLT_EPSILON;
				} else {
					b.dir = -1;
					b.x0 += FLT_EPSILON;
				}
				block.push_back(b);
			}
		}

		// Sort blocks by block-start.
		auto edgeSplitLess = [](const Block &a, const Block &b) {
			return a.x0 < b.x0;
		};
		std::sort(block.begin(), block.end(), edgeSplitLess);

//			for (const Block &b : block) {
//				if (b.dir == 1)
//					SetAddColor( { 0.5, 0.9, 0.4 });
//				else
//					SetAddColor( { 0.9, 0.5, 0.4 });
//
//				Geometry::Vertex v0;
//				Geometry::Vertex v1;
//				v0.u = U2X.InvEval(b.x0);
//				v0.v = v_ + 1e-4;
//				v1.u = U2X.InvEval(b.x1);
//				v1.v = v_ + 1e-4;
//				AddEdge(v0, v1);
//			}
//continue;
//			SetAddColor( { 0.1, 0.1, 0.4 });
		SetAddColor( { 0.5, 0.5, 0.3 });

		std::map<int, size_t> mNext;
		std::map<int, size_t> eNext;
		double xStart = -DBL_MAX;

		for (size_t idx0 = 0; idx0 + 1 < block.size(); idx0++) {
			// Find gaps between blocks. Vertices are positioned only
			// in these gaps.

			if (block[idx0].dir != -1 && block[idx0 + 1].dir != 1)
				continue;

			if (block[idx0].x1 > xStart)
				xStart = block[idx0].x1;
			if (block[idx0 + 1].x0 <= xStart)
				continue;

//				auto endBlock = [](const Block &b) {
//					return b.dir == 1;
//				};
//				auto idx1 = std::find_if(block.begin() + idx0 + 1, block.end(),
//						endBlock);
//
//				if (idx1 == block.end())
//					continue;

			int nMin;
			int nMax;
			if (m & 1) {
				nMin = (int) std::ceil(X2Neven(xStart));
				nMax = (int) std::floor(X2Neven(block[idx0 + 1].x0));
//					nMin = (int) std::round(X2Neven(xStart));
//					nMax = (int) std::round(X2Neven(block[idx0 + 1].x0));
			} else {
				nMin = (int) std::ceil(X2Nodd(xStart));
				nMax = (int) std::floor(X2Nodd(block[idx0 + 1].x0));
//					nMin = (int) std::round(X2Nodd(xStart));
//					nMax = (int) std::round(X2Nodd(block[idx0 + 1].x0));
			}

			size_t vidxLast = nothing;
			size_t eidxLast = nothing;
			size_t eDiagLast = nothing;
			for (int n = nMin; n <= nMax; n++) {
				Geometry::Vertex vert;
				if (m & 1)
					vert.u = U2X.InvEval(N2Xeven(n), (Umin + Umax) / 2.0);
				else
					vert.u = U2X.InvEval(N2Xodd(n), (Umin + Umax) / 2.0);
				vert.v = v_;
				AddVertex(vert);
				v.back().group = 1;
				if (vidxLast != nothing) {
					AddEdge(vidxLast, CountVertices() - 1);
					e.back().group = 1;
					eidxLast = CountEdges() - 1;
					eNext[n] = eidxLast;
				}
				if (vidxLast == nothing) {
					vidxLast = CountVertices() - 1;
					Vertex2Edge[vidxLast] = block[idx0].eidx;
				} else {
					vidxLast = CountVertices() - 1;
				}
				mNext[n] = vidxLast;

//					std::map<int, size_t>::iterator vo = mNext.find(n - 1);
				std::map<int, size_t>::iterator vb;
				std::map<int, size_t>::iterator vc;
				if (m & 1) {
					vb = mLast.find(n - 1);
					vc = mLast.find(n);
				} else {
					vb = mLast.find(n);
					vc = mLast.find(n + 1);
				}

				size_t eidxDiag0 = nothing;
				size_t eidxDiag1 = nothing;

				if (vb != mLast.end()) {
					AddEdge(vidxLast, vb->second);
					e.back().group = 1;
					eidxDiag0 = CountEdges() - 1;
				}
				if (vc != mLast.end()) {
					AddEdge(vc->second, vidxLast);
					e.back().group = 1;
					eidxDiag1 = CountEdges() - 1;
				}

				if (eidxLast != nothing && eDiagLast != nothing
						&& eidxDiag0 != nothing) {
					AddTriangleFromEdges(eidxLast, eDiagLast, eidxDiag0);
					t.back().group = 1;
				}
				if (eidxDiag0 != nothing && eidxDiag1 != nothing) {
					std::map<int, size_t>::iterator e0;

					if (m & 1) {
						e0 = eLast.find(n);
					} else {
						e0 = eLast.find(n + 1);
					}
					if (e0 != eLast.end()) {
						AddTriangleFromEdges(eidxDiag1, eidxDiag0, e0->second);
						t.back().group = 1;
					}
				}

//					if (vb != mLast.end() && vc != mLast.end()) {
//						AddTriangle(vidxLast, vb->second, vc->second);
//					}
				eDiagLast = eidxDiag1;
			}
			if (vidxLast != nothing)
				Vertex2Edge[vidxLast] = block[idx0 + 1].eidx;
		}
		mLast.swap(mNext);
		eLast.swap(eNext);
//			std::cout << "\n";
	}

	std::vector<int> vleft;
	std::vector<int> vright;
	vleft.resize(v.size());
	vright.resize(v.size());
	for (const Edge &ed : e) {
		if (ed.group != 1)
			continue;
		Vertex &va = v[ed.va];
		Vertex &vb = v[ed.vb];
		if (va.u < vb.u) {
			vleft[ed.va]++;
			vright[ed.vb]++;
		} else {
			vright[ed.va]++;
			vleft[ed.vb]++;
		}
	}

	for (size_t vidx = 0; vidx < CountVertices(); vidx++) {
		if (vleft[vidx] >= 0 && vright[vidx] == 0) {
			auto it = Vertex2Edge.find(vidx);
			if (it != Vertex2Edge.end()) {
				size_t vidx0 = e[it->second].va;
				size_t vidx1 = e[it->second].vb;
				Vertex &v0 = v[vidx];
				Vertex &va = v[vidx0];
				Vertex &vb = v[vidx1];
				if (fabs(va.v - v0.v) < fabs(vb.v - v0.v) || vb.u > v0.u)
					AddEdge(vidx0, vidx);
				else
					AddEdge(vidx1, vidx);
				e.back().group = 2;
			}
		}
		if (vleft[vidx] == 0 && vright[vidx] >= 0) {
			auto it = Vertex2Edge.find(vidx);
			if (it != Vertex2Edge.end()) {
				size_t vidx0 = e[it->second].va;
				size_t vidx1 = e[it->second].vb;
				Vertex &v0 = v[vidx];
				Vertex &va = v[vidx0];
				Vertex &vb = v[vidx1];
				if (fabs(va.v - v0.v) < fabs(vb.v - v0.v) || vb.u < v0.u)
					AddEdge(vidx, vidx0);
				else
					AddEdge(vidx, vidx1);
				e.back().group = 2;
			}
		}
	}

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"after triangle generation: Self check for generated geometry not passed.");

	for (Edge &ed : e) {
		if (ed.trianglecount != 1)
			continue;
		const Triangle &tri = t[ed.ta];
		ed.flip = (tri.GetDirection(ed.va, ed.vb) == 1);
	}

//	t.clear();
//	e.erase(std::remove_if(e.begin(), e.end(), [](const Edge &ed) {
//		return ed.trianglecount >= 2;
//	}), e.end());
//	for (Edge &ed : e) {
//		ed.ta = nothing;
//		ed.tb = nothing;
//		ed.trianglecount = 0;
//	}

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"after triangle removal: Self check for generated geometry not passed.");

//	CleanupVertices();

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"after CleanupVertices(): Self check for generated geometry not passed.");

//	Sort();
//
//	std::set<size_t> openEdges;
//	std::set<size_t> openVertices;
//	for (size_t eidx = 0; eidx < CountEdges(); eidx++) {
//		const Edge &ed = e[eidx];
//		if (ed.group == 0)
//			continue;
//		if (ed.trianglecount >= 2)
//			continue;
//		openEdges.insert(eidx);
//		openVertices.insert(ed.va);
//		openVertices.insert(ed.vb);
//	}
//
//	if (!PassedSelfCheck(false))
//		LOGIC_ERROR("Self check for generated geometry not passed.");

	for (Vertex &vert : v) {
		vert.x = vert.u;
		vert.y = vert.v;
		vert.z = 0.0;
	}
	for (Edge &ed : e)
		ed.sharp = (ed.group == 0 || ed.group == 1);

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"before Triangulate(): Self check for generated geometry not passed.");
	if (!PassedPlaneCheck())
		LOGIC_ERROR(
				"before Triangulate(): Plane check for generated geometry not passed.");

	Triangulate();

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"after Triangulate(): Self check for generated geometry not passed.");
	if (!PassedPlaneCheck())
		LOGIC_ERROR(
				"after Triangulate(): Plane check for generated geometry not passed.");

	Regularize();

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"after Regularize(): Self check for generated geometry not passed.");
	if (!PassedPlaneCheck())
		LOGIC_ERROR(
				"after Regularize(): Plane check for generated geometry not passed.");

	ResetPresets();
}

void Design::Patch::AddSplitEdge(const Design::PatchEdge &ed, double p0,
		double p1, const Polynomial &scaleU, const Polynomial &scaleV,
		double maxErr, double maxDist) {
	size_t N = ed.Size();
	if (N < 2) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__ << " - ";
		err << "The edge passed for interpolation has less than two vertices.";
		err << " It has only " << N << ".";
		throw std::runtime_error(err.str());
	}

	auto dist = [&polyU2X = scaleU, &polyV2Y = scaleV](const Vector2 &a,
			const Vector2 &b) {
		const double dx = polyU2X(a.u) - polyU2X(b.u);
		const double dy = polyV2Y(a.v) - polyV2Y(b.v);
		return dx * dx + dy * dy;
	};
	Vector2 v0 = ed(p0);

	Vertex v;
	v.u = v0.u;
	v.v = v0.v;
	AddVertex(v);

	double L = 0.0;
	const double maxErr2 = maxErr * maxErr;
	Polynomial pol = Polynomial::ByValue(0, p0, N - 1, p1);
	for (size_t n = 0; n < N - 1; n++) {
		Vector2 v1 = ed(pol(n + 1));
		L += SplitAddEdge(ed, pol(n), pol(n + 1), v0, v1, dist(v0, v1), scaleU,
				scaleV, maxErr2, maxDist);
		v0 = v1;
	}
}

double Design::Patch::SplitAddEdge(const Design::PatchEdge &ed, double p0,
		double p2, const Vector2 &v0, const Vector2 &v2, double d02,
		const Polynomial &scaleU, const Polynomial &scaleV, double maxErr2,
		double maxDist) {
	auto dist = [&polyU2X = scaleU, &polyV2Y = scaleV](const Vector2 &a,
			const Vector2 &b) {
		const double dx = polyU2X(a.u) - polyU2X(b.u);
		const double dy = polyV2Y(a.v) - polyV2Y(b.v);
		return dx * dx + dy * dy;
	};
	double p1 = (p0 + p2) / 2.0;
	Vector2 v1 = ed(p1);
	double d01 = dist(v0, v1);
	double d12 = dist(v1, v2);
	double err2 = (2 * d01 - d12 + 2 * d02) * d12 + (2 * d01 - d02) * d02
			- d01 * d01;
	err2 /= 4 * d02;
	if (err2 > maxErr2) {
		d01 = SplitAddEdge(ed, p0, p1, v0, v1, d01, scaleU, scaleV, maxErr2,
				maxDist);
		Vertex v;
		v.u = v1.u;
		v.v = v1.v;
		AddVertex(v);

		d12 = SplitAddEdge(ed, p1, p2, v1, v2, d12, scaleU, scaleV, maxErr2,
				maxDist);
	} else {
		if (d01 > maxDist)
			d01 = SplitAddEdge(ed, p0, p1, v0, v1, d01, scaleU, scaleV, maxErr2,
					maxDist);
		Vertex v;
		v.u = v1.u;
		v.v = v1.v;
		AddVertex(v);

		if (d12 > maxDist)
			d12 = SplitAddEdge(ed, p1, p2, v1, v2, d12, scaleU, scaleV, maxErr2,
					maxDist);
	}
	return d01 + d12;
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
			{
				std::ostringstream err;
				err << __FILE__ << ":" << __LINE__ << "::" << __FUNCTION__
						<< " - ";
				err
						<< "The edge loops for the patches could not be determined.";
				err << " The algorithm assigned " << ret.size() << " edges.";
				err << " There are " << splitE.size() << " edges available.";
				err << " Each edge should be only assigned up to two times.";
				throw std::runtime_error(err.str());
			}
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
//		{
//			auto toMatrix = [](const std::string &name,
//					const Eigen::MatrixXd &m) {
//				Matrix ret(name, { (size_t) m.rows(), (size_t) m.cols() });
//				for (int idx1 = 0; idx1 < m.cols(); idx1++)
//					for (int idx0 = 0; idx0 < m.rows(); idx0++)
//						ret(idx0, idx1) = m(idx0, idx1);
//				return ret;
//			};
//
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
//
//		}
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

	MarkValid(true);
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
//			{
//				Exporter ex("/tmp/dist.mat");
//				ex.WriteMatrix(Md);
//
//				std::cout << "Px = " << e0.iu << ";\n";
//				std::cout << "Py = " << e0.iv << ";\n";
//				std::cout << "Px2 = " << e1.iu << ";\n";
//				std::cout << "Py2 = " << e1.iv << ";\n";
//			}
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
	std::cout << "Number of intersections: " << sv.size() << "\n";
	for (const auto &p : sv)
		std::cout << p.eidx << " (" << p.r << ") : " << p.u << ", " << p.v
				<< "\n";
	std::cout << "----------\n";
	size_t c = 0;
	for (const auto &v : splitV)
		std::cout << c++ << " : " << v.u << ", " << v.v << "\n";
	std::cout << "----------\n";
	c = 0;
	for (const auto &e : splitE)
		std::cout << c++ << "[" << e.eidx << "] : " << e.vidx0 << "(" << e.r0
				<< ") --> " << e.vidx1 << "(" << e.r1 << ")\n";
	std::cout << "----------\n";
#endif
}

void Design::UpdatePatches(double res, const Polynomial &scaleU,
		const Polynomial &scaleV) {
#ifdef DEBUG
	double upShift = 0.00;
#endif
// Search for the outlines of each patch.
	for (Patch &p : patches) {
		p.Clear();
		p.UpdateBoundingBox();
#ifdef DEBUG
		std::cout << "-----------\n";
		std::cout << "p.name = " << p.name << "\n";
#endif

//		std::vector<size_t> eInVCount(splitV.size(), 0);

		auto patchContainsEdge = [&p](const SplitEdge &e) {
			return p.eidx.count(e.eidx) != 0;
		};

		// Generate a set of all possible edges
		std::set<SplitEdge> pe;
		for (const SplitEdge &e : splitE) {
			if (!patchContainsEdge(e))
				continue;
			pe.insert(e);
			// Add reverse of edge as well
			SplitEdge e2 = e;
			e2.Flip();
			pe.insert(e2);
		}
		// Collect all loops
		std::vector<std::vector<SplitEdge>> loops;
		std::vector<int> loopDirection;

		while (!pe.empty()) {
			std::set<SplitEdge>::const_iterator en = pe.begin();
			std::vector<SplitEdge> loop = FindLoop(*en, p.eidx);

			// Remove the edges of the loop found from the list of remaining
			// edges.
#ifdef DEBUG
			std::cout << "pe.size() = " << pe.size() << "\n";
#endif
			std::set<SplitEdge> loopSet(loop.begin(), loop.end());
			std::set<SplitEdge> remaining;
			std::set_difference(pe.begin(), pe.end(), loopSet.begin(),
					loopSet.end(), std::inserter(remaining, remaining.begin()));
#ifdef DEBUG
//			std::cout << "remaining.size() = " << remaining.size() << "\n";
//			std::cout << "------------\n pe:\n";
//			for (const SplitEdge &e : pe)
//				std::cout << e.eidx << ", " << e.vidx0 << ", " << e.vidx1
//						<< "\n";
//			std::cout << "------------\n loopSet:\n";
//			for (const SplitEdge &e : loopSet)
//				std::cout << e.eidx << ", " << e.vidx0 << ", " << e.vidx1
//						<< "\n";
//			std::cout << "------------\n remaining:\n";
//			for (const SplitEdge &e : remaining)
//				std::cout << e.eidx << ", " << e.vidx0 << ", " << e.vidx1
//						<< "\n";
#endif
			pe.swap(remaining);
#ifdef DEBUG

//			const Vector2 &v0 = splitV[loop.front().vidx0];
//			std::cout << "(" << v0.u << ", " << v0.v << ")";
			std::cout << loop.front().vidx0;

			for (const SplitEdge &e : loop) {
				const Vector2 &v1 = splitV[e.vidx1];
//				std::cout << " -> (" << v1.u << ", " << v1.v << ")";
				std::cout << " -> " << e.vidx1;
			}
			std::cout << "\n";
#endif

			// Check orientation of loop
			double s = 0.0;
			int zeroCount = 0;
			Vector2 pLast;
			double uOffset = 0;
			for (const SplitEdge &e : loop) {
				const PatchEdge &ed = edges[e.eidx];
				const Vector2 &v0 = splitV[e.vidx0];
				const Vector2 &v1 = splitV[e.vidx1];
#ifdef DEBUG
				// Up to this point all vertex positions should have been
				// cleaned up.
				if (v0.u <= -M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v0.u below negative boundary. (" << v0.u << " < -M_PI)");
				if (v0.u > M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v0.u above positive boundary. (" << v0.u << " > M_PI)");
				if (v1.u <= -M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v1.u below negative boundary. (" << v1.u << " < -M_PI)");
				if (v1.u > M_PI - FLT_EPSILON)
					RUNTIME_ERROR(
							"v1.u above positive boundary. (" << v1.u << " > M_PI)");
#endif

				const size_t N = ed.Size() * 2 - 1;
				Polynomial pol = Polynomial::ByValue(0, e.r0, N - 1, e.r1);
				Vector2 p0 = ed(pol(0));
				Vector2 p1 = ed(pol(N - 1));

				// Count the crossings at +/- M_PI.
				double du0 = p0.u - v0.u;
				double du1 = p1.u - v1.u;

				zeroCount -= (int) std::round(du0 / 2.0 / M_PI);
				zeroCount += (int) std::round(du1 / 2.0 / M_PI);

				uOffset -= p0.u - pLast.u;
				for (size_t n = 1; n < N; n++) {
					p1 = ed(pol(n));
					s += (p0.u + uOffset) * (p1.v - p0.v)
							- p0.v * (p1.u - p0.u);
					p0 = p1;
				}
				pLast = p1;
			}

#ifdef DEBUG
			std::cout << "zeroCount = " << zeroCount;
			std::cout << "   s = " << s << "\n";
#endif

			if (zeroCount != 0)
				continue;

			loopDirection.push_back((s < -FLT_EPSILON) ? (-1) : (1));
			loops.push_back(loop);
//			if (s < -FLT_EPSILON) {
//				std::cout << "Area of loop is negative.\n";
//				loop.clear();
//				continue;
//			}
		}
#ifdef DEBUG
		std::cout << "pe.size() = " << pe.size() << "\n";
		std::cout << "-----------\n";
#endif

		if (!loops.empty()) {

			// Check if an outer loop completes all edges or if an outer loop
			// plus some of the inner loops complete the edges exactly.

			size_t idxLoopOuter = nothing;
			std::set<size_t> idxLoopInner;

			std::vector<std::set<size_t>> coveredEdges;
			coveredEdges.resize(loops.size());
			for (size_t idx = 0; idx < loops.size(); idx++) {
				for (const SplitEdge &e : loops[idx])
					coveredEdges[idx].insert(e.eidx);
			}

			for (size_t idx = 0; idx < loops.size(); idx++) {
				if (loopDirection[idx] <= 0)
					continue;

				// A valid loop contains all edges, that are specified in the
				// patch.
				if (coveredEdges[idx] == p.eidx) {
					// Perfect fit
					idxLoopOuter = idx;
#ifdef DEBUG
					std::cout << "Valid loop found: " << idx << "\n";
#endif
					break;
				}
				if (std::includes(p.eidx.begin(), p.eidx.end(),
						coveredEdges[idx].begin(), coveredEdges[idx].end())) {

					std::set<size_t> missing;
					std::set_difference(p.eidx.begin(), p.eidx.end(),
							coveredEdges[idx].begin(), coveredEdges[idx].end(),
							std::inserter(missing, missing.begin()));

					// Check if the inner area can be covered by reverse loops.

					for (size_t idx2 = 0; idx2 < loops.size(); idx2++) {
						if (loopDirection[idx2] >= 0)
							continue;
						if (std::includes(missing.begin(), missing.end(),
								coveredEdges[idx2].begin(),
								coveredEdges[idx2].end())) {
							idxLoopInner.insert(idx2);
							for (size_t v : coveredEdges[idx2])
								missing.erase(v);
						}

						if (missing.empty()) {
							idxLoopOuter = idx;
							break;
						}
						idxLoopInner.clear();
					}
				}
			}
			// Tesselate patch
			if (idxLoopOuter != nothing) {
#ifdef DEBUG
				std::cout << "Patch tesselation of " << idxLoopOuter << "\n";
#endif
				const size_t loopBack = p.CountVertices();
				// Some edged are not connected to a loop but are off by +/-
				// 2 * M_PI.

				Vector2 lastU;
				bool first = true;
				for (const SplitEdge &se : loops[idxLoopOuter]) {
					PatchEdge &ed = edges[se.eidx];
					if (first) {
						first = false;
					} else {
						double Ushift = lastU.u - ed(se.r0).u;
						Ushift = std::round(Ushift / (2.0 * M_PI)) * 2.0 * M_PI;
						if (fabs(Ushift) > FLT_EPSILON)
							ed.ShiftU(Ushift);
					}
					p.AddSplitEdge(ed, se.r0, se.r1, scaleU, scaleV, 0.001,
							0.005);
					lastU = ed(se.r1);
				}
				for (size_t n = loopBack + 1; n < p.CountVertices(); n++)
					p.AddEdge(n - 1, n);
				p.AddEdge(p.CountVertices() - 1, loopBack);

				std::cout << "-----------\n";
			}
		}

#ifdef DEBUG
		p.matrix = AffineTransformMatrix::Translation(-upShift * 0.0, 0.0,
				upShift);
		upShift += 0.18;
		if (!p.PassedSelfCheck(false)) {
			std::cout << p.name << " has broken geometry.\n";
		}
#endif
		p.UpdateBoundingBox();
		// Interpolate the patch.
		p.Update(0.005, scaleU, scaleV);
//		p.paintDirection = true;
		p.paintVertices = true;
//		p.paintNormals = true;
		p.paintTriangles = true;
		p.paintEdges = true;
		p.dotSize = 3;
	}

	MarkValid(true);
}


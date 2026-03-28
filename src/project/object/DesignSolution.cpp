///////////////////////////////////////////////////////////////////////////////
// Name               : DesignSolution.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 19.03.2026
// Copyright          : (C) 2026 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "DesignSolution.h"
#include "../../3D/FilePLY.h"

#include <cfloat>
#include <stdexcept>
#include <sstream>
#include "../../Config.h"

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

DesignSolution::DesignSolution() {
	// TODO Auto-generated constructor stub

}

DesignSolution::Patch::Patch(const std::string &name) :
		name(name) {
	Umin = DBL_MAX;
	Umax = -DBL_MAX;
	Vmin = DBL_MAX;
	Vmax = -DBL_MAX;
}

DesignSolution::Patch::Patch(std::initializer_list<size_t> eidx,
		const std::string &name) :
		name(name), eidx(eidx) {
	Umin = DBL_MAX;
	Umax = -DBL_MAX;
	Vmin = DBL_MAX;
	Vmax = -DBL_MAX;
}

void DesignSolution::Patch::UpdateBoundingBox() {
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

void DesignSolution::Patch::Update(double edgeLength, const Polynomial &U2X,
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
			// const Geometry::Edge &ed = GetEdge(idx);
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

	// Add edges connecting the filling of the patch with the outline.

	// Count the left and right edges in each vertex.
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

	// Add extra edges
//	for (size_t vidx = 0; vidx < CountVertices(); vidx++) {
//		if (vleft[vidx] >= 0 && vright[vidx] == 0) {
//			auto it = Vertex2Edge.find(vidx);
//			if (it != Vertex2Edge.end()) {
//				size_t vidx0 = e[it->second].va;
//				size_t vidx1 = e[it->second].vb;
//				Vertex &v0 = v[vidx];
//				Vertex &va = v[vidx0];
//				Vertex &vb = v[vidx1];
//				if (fabs(va.v - v0.v) < fabs(vb.v - v0.v) || vb.u > v0.u)
//					AddEdge(vidx0, vidx);
//				else
//					AddEdge(vidx1, vidx);
//				e.back().group = 2;
//			}
//		}
//		if (vleft[vidx] == 0 && vright[vidx] >= 0) {
//			auto it = Vertex2Edge.find(vidx);
//			if (it != Vertex2Edge.end()) {
//				size_t vidx0 = e[it->second].va;
//				size_t vidx1 = e[it->second].vb;
//				Vertex &v0 = v[vidx];
//				Vertex &va = v[vidx0];
//				Vertex &vb = v[vidx1];
//				if (fabs(va.v - v0.v) < fabs(vb.v - v0.v) || vb.u < v0.u)
//					AddEdge(vidx, vidx0);
//				else
//					AddEdge(vidx, vidx1);
//				e.back().group = 2;
//			}
//		}
//	}

	if (!PassedSelfCheck(false))
		LOGIC_ERROR(
				"after triangle generation: Self check for generated geometry not passed.");

	// Sort the direction of edges connected to exactly one triangle.
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

#ifdef DEBUG
	{
		FilePLY ply("/tmp/problem.ply");
		ply.meshlabCompatibilityMode = true;
		ply.Write(*this);
	}
#endif

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

void DesignSolution::Patch::AddSplitEdge(const Design::PatchEdge &ed, double p0,
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

double DesignSolution::Patch::SplitAddEdge(const Design::PatchEdge &ed,
		double p0, double p2, const Vector2 &v0, const Vector2 &v2, double d02,
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

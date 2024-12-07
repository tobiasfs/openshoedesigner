///////////////////////////////////////////////////////////////////////////////
// Name               : Polygon3.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -std=c++14 or greater
// Author             : Tobias Schaefer
// Created            : 07.07.2011
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

#include "Polygon3.h"

#include <algorithm>
#include <float.h>
#include <sstream>
#include <list>

#include "OpenGL.h"

using std::swap;

static const Vector3 nullvector(0.0, 0.0, 0.0);
static const Vector3 nonnullvector(0.0, 0.0, 1.0);
static const size_t nothing = (size_t) -1;

Polygon3::Polygon3() {
	paintTriangles = false;
	paintEdges = true;
//	paintVertices = false;
//	paintNormals = true;
//	smooth = false;
}
void Polygon3::Clear() {
	Geometry::Clear();
	groupCount = 0;
	firstIndex = 0;
	lastIndex = 0;
}

void Polygon3::AddEdgeToVertex(const Geometry::Vertex &vertex) {
	verticesHaveNormal |=
			(fabs(vertex.n.x) > FLT_EPSILON || fabs(vertex.n.y) > FLT_EPSILON
					|| fabs(vertex.n.z) > FLT_EPSILON);
	verticesHaveColor |= (fabs(vertex.c.r) > FLT_EPSILON
			|| fabs(vertex.c.g) > FLT_EPSILON || fabs(vertex.c.b) > FLT_EPSILON
			|| fabs(vertex.c.a) > FLT_EPSILON);
	const size_t idx = v.size();
	v.push_back(vertex);
	v.back().group = groupCount;
	if (lastIndex < idx) {
		AddEdge(lastIndex, idx);
		e.back().group = groupCount;
		lastIndex = idx;
	}
}

void Polygon3::NextGroup() {
	firstIndex = v.size();
	lastIndex = firstIndex;
	groupCount++;
}

void Polygon3::CloseLoopNextGroup() {
	if (firstIndex < lastIndex) {
		if ((v[firstIndex] - v.back()).Abs() <= FLT_EPSILON) {
			v.pop_back();
			e.back().vb = firstIndex;
		} else {
			AddEdge(lastIndex, firstIndex);
			e.back().group = groupCount;
		}
	}
	NextGroup();
}

void Polygon3::SortLoop() {

	// Un-flip edges
	for (Edge &ed : e)
		if (ed.flip) {
			ed.flip = false;
			swap(ed.va, ed.vb);
		}

	// Sort edges for fast searching.
	auto edge_less = [](const Edge &lhs, const Edge &rhs) {
		if (lhs.va < rhs.va)
			return true;
		if (lhs.va > rhs.va)
			return false;
		return lhs.vb < rhs.vb;
	};
	std::sort(e.begin(), e.end(), edge_less);

	std::vector<size_t> connections(e.size(), 0);
	for (const Edge &ed : e)
		connections[ed.vb]++;
	std::list<size_t> starting_points;
	for (size_t idx = 0; idx < connections.size(); ++idx)
		if (connections[idx] == 0)
			starting_points.push_back(idx);

	vmap.assign(v.size(), nothing);
	emap.assign(e.size(), nothing);
	tmap.clear();
	gmap.clear();

	std::vector<Vertex> tempv;
	tempv.reserve(v.size());
	std::vector<Edge> tempe;
	tempe.reserve(e.size());

	// Form edge-loops.
	while (true) {
		size_t eidx;
		size_t vidx;
		if (!starting_points.empty()) {
			eidx = 0;
			vidx = starting_points.front();
			starting_points.pop_front();
		} else {
			auto eh0 = std::find(emap.begin(), emap.end(), nothing);
			if (eh0 == emap.end())
				break;
			eidx = eh0 - emap.begin();
			vidx = e[eidx].va;
		}
		vmap[vidx] = tempv.size();
		tempv.push_back(v[vidx]);

		while (true) {
			while (e[eidx].va < vidx && (eidx + 1) < e.size())
				eidx++;
			while (e[eidx].va > vidx && eidx > 0)
				eidx--;

			if (e[eidx].va != vidx)
				break;

			emap[eidx] = tempe.size();
			tempe.push_back(e[eidx]);

			vidx = e[eidx].vb;
			if (vmap[vidx] != nothing)
				break;

			vmap[vidx] = tempv.size();
			tempv.push_back(v[vidx]);
		}
	}

//	std::iota(emap.begin(), emap.end(), 0);
//	std::sort(emap.begin(), emap.end(), edge_less);
//	for (size_t idx : emap)
//		temp.push_back(e[idx]);

	v.swap(tempv);
	e.swap(tempe);
	Remap(0, 0, 0);
}

Polygon3& Polygon3::operator+=(const Polygon3 &a) {
	const size_t vfirst = v.size();
	const size_t efirst = e.size();
	Geometry::AddFrom(a);
	for (size_t n = vfirst; n < v.size(); ++n)
		if (v[n].group != nothing)
			v[n].group += groupCount;
	for (size_t n = efirst; n < e.size(); ++n)
		if (e[n].group != nothing)
			e[n].group += groupCount;

	auto maxvgroup = std::max_element(v.begin(), v.end(),
			[](const Vertex &a, const Vertex &b) {
				return a.group < b.group;
			});
	auto maxegroup = std::max_element(e.begin(), e.end(),
			[](const Edge &a, const Edge &b) {
				return a.group < b.group;
			});
	groupCount =
			(maxvgroup->group > maxegroup->group) ?
					(maxvgroup->group + 1) : (maxegroup->group + 1);
	return *this;
}

const Polygon3 Polygon3::operator+(const Polygon3 &a) const {
	Polygon3 temp = *this;
	temp += a;
	return temp;
}

//FIXME: I am sure, the next four function should not exist. There might even
// be functions using these, that do not perform as intended.
// Use Shift() instead.
//
//Polygon3& Polygon3::operator *=(const double val) {
//	for (auto &vect : v)
//		vect *= val;
//	return *this;
//}
//
//const Polygon3 Polygon3::operator *(const double val) {
//	Polygon3 temp = *this;
//	temp *= val;
//	return temp;
//}
//
//Polygon3& Polygon3::operator /=(const double val) {
//	for (auto &vect : v)
//		vect /= val;
//	return *this;
//}
//
//const Polygon3 Polygon3::operator /(const double val) {
//	Polygon3 temp = *this;
//	temp /= val;
//	return temp;
//}

size_t Polygon3::Size() const {
	return v.size();
}

Polygon3::Vertex& Polygon3::operator [](size_t index) {
	return Geometry::operator [](index);
}

const Polygon3::Vertex& Polygon3::operator [](size_t index) const {
	return Geometry::operator [](index);
}

Vector3 Polygon3::Direction(size_t index) const {
	auto checkEdge = [index](const Edge &ed) {
		return (ed.flip) ? (ed.vb == index) : (ed.va == index);
	};
	std::vector<Edge>::const_iterator it = std::find_if(e.begin(), e.end(),
			checkEdge);
	if (it == e.end())
		return Vector3();
	const Vector3 p0 = v[it->VertexIndex(0)];
	const Vector3 p1 = v[it->VertexIndex(1)];
	return p0 - p1;
}

Vector3& Polygon3::Normal(size_t index) {
	return v[index].n;
}

const Vector3& Polygon3::Normal(size_t index) const {
	return v[index].n;
}

void Polygon3::CalculateNormals(const CalculateNormalMethod method) {

	switch (method) {
	case CalculateNormalMethod::ByCenter: {
		Vector3 temp = GetCenter();
		for (auto &vect : v) {
			vect.n = (vect - temp);
			vect.n.Normalize();
		}
		for (auto &ed : e) {
			ed.n = v[ed.va].n + v[ed.vb].n;
			ed.n.Normalize();
		}
		break;
	}
	case CalculateNormalMethod::ByBends: {

		if (e.size() < 2) {
			for (auto &ed : e)
				ed.n = Vector3(0, 0, 1);
			for (auto &vect : v)
				vect.n = Vector3(0, 0, 1);
			return;
		}

		auto eFirst = e.begin();
		auto ePrev = e.begin();
		auto eNext = ePrev + 1;

		while (eNext != e.end()) {
			if (ePrev->group == eNext->group) {
				Vector3 dPrev = v[ePrev->vb] - v[ePrev->va];
				Vector3 dNext = v[eNext->va] - v[eNext->vb];
				dPrev.Normalize();
				dNext.Normalize();
				Vector3 n = dPrev + dNext;
				n.Normalize();
				v[ePrev->vb].n = n;
				if (eNext->vb < eNext->va) {
					Vector3 dNext2 = v[eFirst->vb] - v[eFirst->va];
					dNext2.Normalize();
					Vector3 n2 = dNext + dNext2;
					n2.Normalize();
					v[eNext->vb].n = n2;
				}
			} else {
				eFirst = eNext;
			}
			ePrev = eNext;
			eNext += 1;
		}

		for (auto &ed : e) {
			Vector3 n = v[ed.va].n + v[ed.vb].n;
			n.Normalize();
			ed.n = n;
		}
		break;
	}
	case CalculateNormalMethod::InPlaneXY:
		CalculateNormalsAroundVector( { 0, 0, 1 });
		break;
	case CalculateNormalMethod::InPlaneYZ:
		CalculateNormalsAroundVector( { 1, 0, 0 });
		break;
	case CalculateNormalMethod::InPlaneZX:
		CalculateNormalsAroundVector( { 0, 1, 0 });
		break;
	}
}

void Polygon3::CalculateNormalsAroundVector(const Vector3 &planenormal) {

	for (auto &vect : v)
		vect.n = Vector3(0, 0, 0);

	for (auto &ed : e) {
		size_t idx0 = ed.VertexIndex(0);
		size_t idx1 = ed.VertexIndex(1);
		Vector3 n = (v[idx1] - v[idx0]) * planenormal;
		n.Normalize();
		ed.n = n;
		v[ed.va].n += n;
		v[ed.vb].n += n;
	}

	for (auto &vect : v)
		vect.n.Normalize();
}

void Polygon3::Shift(double distance) {
	for (auto &vert : v)
		vert += vert.n * distance;
}

void Polygon3::RemoveZeroLength() {
	const double threshold = FLT_EPSILON;
	const auto &localv = v;
	auto it = std::remove_if(e.begin(), e.end(),
			[threshold, localv](const Geometry::Edge &edge) {
				return ((localv[edge.va] - localv[edge.vb]).Abs() < threshold);
			});
	e.erase(it, e.end());
	// TODO remove unused vertices in v.
}

void Polygon3::Reverse() {
	for (auto &edge : e)
		edge.flip = !edge.flip;
}

void Polygon3::Reverse(size_t group) {
	for (auto &edge : e)
		if (edge.group == group)
			edge.flip = !edge.flip;
}

void Polygon3::RotateOrigin(const Vector3 &p) {
	size_t minimalIndex;
	size_t group;
	std::tie(minimalIndex, group) = ClosestPoint(p);

	std::rotate(v.begin(), v.begin() + minimalIndex, v.end());
}

void Polygon3::RotateOrigin(const Vector3 &p, size_t group) {
	throw std::runtime_error(
			"Polygon3::RotateOrigin(const Vector3 &p, size_t group) - Not implemented.");
//	const size_t minimalIndex = ClosestPoint(p, group);
}

void Polygon3::Triangulate() {

	// Check that there are no triangles. If it is needed, that some
	// triangles already exist, write some edge-triangle-sorting algorithm.
	// i.e. swapping e[].ta and e[].tb so that from above in the direction
	// of the edge ta is left and tb is right. This is needed to find edges,
	// that already have been processed.

	for (const Edge &ed : e) {
		if (ed.ta != nothing || ed.tb != nothing) {
			throw std::logic_error(
					"Polygon3::Triangulate() - Function called on a polygon, that already has triangles.");
		}
	}

	if (e.empty())
		return;

	// Add edges to generate areas that are x-monotone.
	Sort();

	// Collect some information on the vertices.

	std::vector<size_t> left_count(v.size(), 0);
	std::vector<size_t> right_count(v.size(), 0);
	std::vector<int> dir(v.size(), 0);
	std::vector<double> eslope;
	eslope.reserve(e.size());
	std::vector<double> vslope(v.size(), 0);

	for (const Edge &ed : e) {
		left_count[ed.vb]++;
		right_count[ed.va]++;
		if (ed.flip) {
			dir[ed.va]--;
			dir[ed.vb]--;
		} else {
			dir[ed.va]++;
			dir[ed.vb]++;
		}
		const Vertex &va = v[ed.va];
		const Vertex &vb = v[ed.vb];
		const double dx = vb.x - va.x;
		const double dy = vb.y - va.y;
		double es = 0.0;
		if (fabs(dx) < FLT_EPSILON)
			es = std::numeric_limits<double>::infinity();
		else
			es = dy / dx;
		if (ed.flip)
			es = -es;
		eslope.push_back(es);
		vslope[ed.va] += es;
		vslope[ed.vb] += es;
	}

//	verticesHaveColor = true;
//	paintVertices = true;
//	for (size_t n = 0; n < v.size(); ++n) {
//		v[n].c.r = fmin(fmax(-vslope[n] * 2.0, 0), 1);
//		v[n].c.g = fmin(fmax(vslope[n] * 2.0, 0), 1);
//		v[n].c.b = 0.0;
//	}

	// Mark all existing edges as "sharp" to separate them from the extra edges.

	for (Edge &ed : e)
		ed.sharp = true;

	// Add extra edges

	for (size_t idx = 0; idx < v.size(); ++idx) {
		if (right_count[idx] == 0 && vslope[idx] < 0.0) {

			size_t idx1 = idx;
			bool crossed = false;
			do {
				idx1++;
				//TODO Check if idx1 is within the range of vertices. Will eventually fail for non-simple polygons.
				// Check if some other edge is crossed.
				crossed = false;
				for (Edge &ed : e) {
					if (ed.vb <= idx)
						continue;
					if (ed.va >= idx1)
						break;
					const Vertex &v0a = v[idx];
					const Vertex &v0b = v[idx1];
					const Vertex &v1a = v[ed.va];
					const Vertex &v1b = v[ed.vb];

					const double det = (v0b.x - v0a.x) * v1b.y
							+ (v0a.y - v0b.y) * v1b.x + (v0a.x - v0b.x) * v1a.y
							+ (v0b.y - v0a.y) * v1a.x;
					if (fabs(det) < FLT_EPSILON)
						continue; // Parallel edges

					const double r = ((v1a.x - v0a.x) * v1b.y
							+ (v0a.y - v1a.y) * v1b.x + v0a.x * v1a.y
							- v0a.y * v1a.x) / det;
					const double s = -((v0b.x - v0a.x) * v1a.y
							+ (v0a.y - v0b.y) * v1a.x + v0a.x * v0b.y
							- v0a.y * v0b.x) / det;
					if (r > FLT_EPSILON && r < 1.0 - FLT_EPSILON
							&& s > FLT_EPSILON && s < 1.0 - FLT_EPSILON) {
						crossed = true;
						break;
					}
				}
			} while (crossed);

			AddEdge(idx, idx1);
			e.back().n = (v[idx].n + v[idx1].n) / 2.0;
		}
		if (left_count[idx] == 0 && vslope[idx] > 0.0) {
			size_t idx0 = idx;
			bool crossed = false;
			do {
				idx0--;
				//TODO Check if idx0 is within the range of vertices. Will eventually fail for non-simple polygons.
				// Check if some other edge is crossed.
				crossed = false;
				for (Edge &ed : e) {
					if (ed.vb <= idx0)
						continue;
					if (ed.va >= idx)
						break;
					const Vertex &v0a = v[idx0];
					const Vertex &v0b = v[idx];
					const Vertex &v1a = v[ed.va];
					const Vertex &v1b = v[ed.vb];

					const double det = (v0b.x - v0a.x) * v1b.y
							+ (v0a.y - v0b.y) * v1b.x + (v0a.x - v0b.x) * v1a.y
							+ (v0b.y - v0a.y) * v1a.x;
					if (fabs(det) < FLT_EPSILON)
						continue; // Parallel edges

					const double r = ((v1a.x - v0a.x) * v1b.y
							+ (v0a.y - v1a.y) * v1b.x + v0a.x * v1a.y
							- v0a.y * v1a.x) / det;
					const double s = -((v0b.x - v0a.x) * v1a.y
							+ (v0a.y - v0b.y) * v1a.x + v0a.x * v0b.y
							- v0a.y * v0b.x) / det;
					if (r > FLT_EPSILON && r < 1.0 - FLT_EPSILON
							&& s > FLT_EPSILON && s < 1.0 - FLT_EPSILON) {
						crossed = true;
						break;
					}
				}
			} while (crossed);

			// Check if the edge already exists. Needs only to be done for the
			// left-hand-side edges, because there could already been a
			// right-hand-side edge in the same place. These are always
			// generated first.
			bool edgeexists = false;
			for (const auto &ed : e) {
				// No shortcuts possible, as edges are not sorted here.
				if (ed.va == idx0 && ed.vb == idx) {
					edgeexists = true;
					break;
				}
			}
			if (!edgeexists) {
				AddEdge(idx0, idx);
				e.back().n = (v[idx0].n + v[idx].n) / 2.0;
			}
		}
	}
	Sort();

	// Redo the statistics with the added edges

	left_count.assign(v.size(), 0);
	right_count.assign(v.size(), 0);
	dir.assign(v.size(), 0);
	eslope.clear();
	eslope.reserve(e.size());
	vslope.assign(v.size(), 0);
	for (const Edge &ed : e) {
		left_count[ed.vb]++;
		right_count[ed.va]++;
		if (ed.flip) {
			dir[ed.va]--;
			dir[ed.vb]--;
		} else {
			dir[ed.va]++;
			dir[ed.vb]++;
		}
		const Vertex &va = v[ed.va];
		const Vertex &vb = v[ed.vb];
		const double dx = vb.x - va.x;
		const double dy = vb.y - va.y;
		double es = 0.0;
		if (fabs(dx) < FLT_EPSILON)
			es = std::numeric_limits<double>::infinity();
		else
			es = dy / dx;
		eslope.push_back(es);
		if (ed.flip) {
			vslope[ed.va] -= es;
			vslope[ed.vb] -= es;
		} else {
			vslope[ed.va] += es;
			vslope[ed.vb] += es;
		}
	}

	// Generate triangles

	std::vector<size_t> idx_top;
	std::vector<size_t> idx_bot;

	// Store the current number of edges, as generation of triangles also
	// generates edges, that need to be ignored here.
	const size_t Ne = e.size();

	bool runagain = true;		// For each area in the polygon
	while (runagain) {
		runagain = false;

		// Search for a starting point.
		size_t vidx = (size_t) -1;
		size_t eidx = 0;
		while ((vidx + 1) < v.size() || vidx == (size_t) -1) {
			vidx++;
			// Search for a starting point
			if (right_count[vidx] < 2)
				continue;

			size_t e_top = nothing;
			size_t e_bot = nothing;

			// Search for the first edge for a given vertex.
			while (eidx < Ne && e[eidx].va < vidx)
				eidx++;
			if (eidx >= Ne) {
				throw std::logic_error("Triangulate(): Vertex without edges.");
			}
			size_t e_first = eidx;

			// Find top-most valid edge (e_top)
			for (eidx = e_first; eidx < Ne && e[eidx].va == vidx; ++eidx) {
				// Skip already processed areas.
				if (e[eidx].tb != nothing)
					continue;
				if (e[eidx].sharp) {
					if (e[eidx].flip)
						e_top = eidx;
				} else {
					// A generated edge is always in a valid area on both sides.
					e_top = eidx;
				}
			}
			if (e_top == nothing) {
				continue;
			}

			// Find the highest valid edge below the top edge (=bottom edge of
			// area)

			double esl = -DBL_MAX;
			for (eidx = e_first; eidx < Ne && e[eidx].va == vidx; ++eidx) {
				if (eidx == e_top || e[eidx].ta != nothing)
					continue;

				if (eslope[eidx] > eslope[e_top])
					continue;

				if (eslope[eidx] > esl) {
					e_bot = eidx;
					esl = eslope[eidx];
				}
			}

			if (e_bot == nothing) {
				continue;
			}

			auto check_triangle = [&vref = v](size_t i0, size_t i1, size_t i2) {
				// Cross-product to check, if the triangle is mathematically
				// positive.
				const double dx0 = vref[i1].x - vref[i0].x;
				const double dx1 = vref[i2].x - vref[i1].x;
				const double dy0 = vref[i1].y - vref[i0].y;
				const double dy1 = vref[i2].y - vref[i1].y;
				return dx0 * dy1 - dx1 * dy0;
			};

			idx_top.clear();
			idx_bot.clear();

			idx_top.push_back(e[e_top].va);
			idx_bot.push_back(e[e_bot].va);

			for (; vidx < v.size(); ++vidx) {
				int c = 0;
				if (vidx == e[e_top].vb) {

					// Check if there was a reflexive chain, that could be
					// resolved.
					while (idx_top.size() > 1) {
						const size_t idx0 = idx_top[idx_top.size() - 1];
						const size_t idx1 = idx_top[idx_top.size() - 2];
						const double v = check_triangle(vidx, idx0, idx1);
						if (v > FLT_EPSILON) {
							AddTriangleMinimal(vidx, idx0, idx1);
							idx_top.pop_back();
						} else {
							break;
						}
					}
					if (idx_bot.size() > 1) {
						if (idx_top.size() != 1)
							throw std::logic_error(
									"Triangulate(): The idx_top should contain exactly 1 element here.");
						for (size_t n = 1; n < idx_bot.size(); ++n) {
							AddTriangleMinimal(idx_bot[n - 1], idx_bot[n],
									vidx);
						}
						idx_top.assign(1, idx_bot.back());
						idx_bot.assign(1, idx_bot.back());
					}

					idx_top.push_back(vidx);
					++c;

					// Search for the next top edge
					eidx = e_top;
					e_top = nothing;
					for (; eidx < Ne && e[eidx].va <= vidx; ++eidx) {
						if (e[eidx].va < vidx)
							continue;
						if (e[eidx].tb != nothing)
							continue;
						if (e_top == nothing) {
							e_top = eidx;
						} else {
							if (eslope[eidx] < eslope[e_top])
								e_top = eidx;
						}
					}
				}

				if (vidx == e[e_bot].vb) {
					if (c >= 1)
						break;
					while (idx_bot.size() > 1) {
						const size_t idx0 = idx_bot[idx_bot.size() - 1];
						const size_t idx1 = idx_bot[idx_bot.size() - 2];
						const double v = check_triangle(vidx, idx1, idx0);
						if (v > FLT_EPSILON) {
							AddTriangleMinimal(vidx, idx1, idx0);
							idx_bot.pop_back();
						} else {
							break;
						}
					}
					if (idx_top.size() > 1) {
						if (idx_bot.size() != 1)
							throw std::logic_error(
									"Triangulate(): The idx_bot should contain exactly 1 element here.");
						for (size_t n = 1; n < idx_top.size(); ++n) {
							AddTriangleMinimal(idx_top[n], idx_top[n - 1],
									vidx);
						}
						idx_bot.assign(1, idx_top.back());
						idx_top.assign(1, idx_top.back());
					}
					idx_bot.push_back(vidx);
					// Search for the next bottom edge
					eidx = e_bot;
					e_bot = nothing;
					for (; eidx < Ne && e[eidx].va <= vidx; ++eidx) {
						if (e[eidx].va < vidx)
							continue;
						if (e[eidx].ta != nothing)
							continue;
						if (e_bot == nothing) {
							e_bot = eidx;
						} else {
							if (eslope[eidx] > eslope[e_bot])
								e_bot = eidx;
						}
					}
				}

			}

			runagain = true;

			// Final vertex of this group.
			if ((idx_top.size() + idx_bot.size()) > 3)
				throw std::logic_error(
						"Triangulate(): Additional unprocessed vertices found.");
			paintTriangles = false;

		}
	}

	// Clean up edges
	for (Edge &ed : e) {
		if (ed.ta == nothing)
			swap(ed.ta, ed.tb);
		ed.trianglecount = (ed.ta == nothing) ? 0 : 1;
		if (ed.tb != nothing)
			ed.trianglecount++;
	}

	Finish(); // Sort the triangles into a consistent order
}

void Polygon3::Filter(unsigned int width) {
	//TODO Implement this.
}

void Polygon3::AddTriangleMinimal(size_t idx0, size_t idx1, size_t idx2) {

	Geometry::Triangle tri;
	tri.va = idx0;
	tri.vb = idx1;
	tri.vc = idx2;
	tri.Fix();

	trianglesHaveNormal |= addNormals;
	trianglesHaveColor |= addColors;
	Vector3 n;
	if (addNormals) {
		n = addNormal;
	} else {
		if (verticesHaveNormal) {
			n += v[idx0].n;
			n += v[idx1].n;
			n += v[idx2].n;
		} else {
			Vector3 a = v[idx1] - v[idx0];
			Vector3 b = v[idx2] - v[idx1];
			n = a * b;
		}
		n.Normalize();
	}
	tri.n = n;
	if (addColors)
		tri.c = addColor;

	const size_t tidx = t.size();

	for (size_t idx = 0; idx < e.size(); ++idx) {
		if (e[idx].va == tri.va && e[idx].vb == tri.vb) {
			tri.ea = idx;
			if (tri.flip)
				e[idx].tb = tidx;
			else
				e[idx].ta = tidx;
		}

		if (e[idx].va == tri.vb && e[idx].vb == tri.vc) {
			tri.eb = idx;
			if (tri.flip)
				e[idx].tb = tidx;
			else
				e[idx].ta = tidx;
		}

		if (e[idx].va == tri.va && e[idx].vb == tri.vc) {
			tri.ec = idx;
			if (tri.flip)
				e[idx].ta = tidx;
			else
				e[idx].tb = tidx;
		}
	}
	if (tri.ea == nothing) {
		Geometry::Edge edge0;
		edge0.va = tri.va;
		edge0.vb = tri.vb;
		if (tri.flip)
			edge0.tb = tidx;
		else
			edge0.ta = tidx;
		edge0.n = n;
		edge0.c = addColor;
		edge0.trianglecount = 1;
		tri.ea = e.size();
		e.push_back(edge0);
	}
	if (tri.eb == nothing) {
		Geometry::Edge edge1;
		edge1.va = tri.vb;
		edge1.vb = tri.vc;
		if (tri.flip)
			edge1.tb = tidx;
		else
			edge1.ta = tidx;
		edge1.n = n;
		edge1.c = addColor;
		edge1.trianglecount = 1;
		tri.eb = e.size();
		e.push_back(edge1);
	}

	if (tri.ec == nothing) {
		Geometry::Edge edge2;
		edge2.va = tri.va;
		edge2.vb = tri.vc;
		edge2.flip = true;
		if (tri.flip)
			edge2.ta = tidx;
		else
			edge2.tb = tidx;
		edge2.n = n;
		edge2.c = addColor;
		edge2.trianglecount = 1;
		tri.ec = e.size();
		e.push_back(edge2);
	}
	t.push_back(tri);
	finished = false;
}

double Polygon3::GetLength() const {
	double d = 0.0;
	for (const Edge &edge : e) {
		const auto &v0 = v[edge.va];
		const auto &v1 = v[edge.vb];
		d += (v0 - v1).Abs();
	}
	return d;
}

double Polygon3::GetLength(size_t group) const {
	double d = 0.0;
	for (const Edge &edge : e) {
		if (edge.group == group) {
			const auto &v0 = v[edge.va];
			const auto &v1 = v[edge.vb];
			d += (v0 - v1).Abs();
		}
	}
	return d;
}

std::vector<double> Polygon3::GetXVectorD() const {
	std::vector<double> temp;
	temp.reserve(v.size());
	for (const auto &vec : v)
		temp.push_back((double) vec.x);
	return temp;
}

std::vector<double> Polygon3::GetYVectorD() const {
	std::vector<double> temp;
	temp.reserve(v.size());
	for (const auto &vec : v)
		temp.push_back((double) vec.y);
	return temp;
}

std::vector<double> Polygon3::GetZVectorD() const {
	std::vector<double> temp;
	temp.reserve(v.size());
	for (const auto &vec : v)
		temp.push_back((double) vec.z);
	return temp;
}

Vector3 Polygon3::GetCenter() const {
	Vector3 c;
	double wSum = 0.0;
	for (const Edge &edge : e) {
		const auto &v0 = v[edge.va];
		const auto &v1 = v[edge.vb];
		const double d = (v0 - v1).Abs();
		const auto cw = (v0 + v1) / 2.0 * d;
		wSum += d;
		c += cw;
	}
	if (wSum > FLT_EPSILON)
		c /= wSum;
	return c;
}

Vector3 Polygon3::GetCenter(size_t group) const {
	Vector3 c;
	double wSum = 0.0;
	for (const Edge &edge : e) {
		if (edge.group == group) {
			const auto &v0 = v[edge.va];
			const auto &v1 = v[edge.vb];
			const double d = (v0 - v1).Abs();
			const auto cw = (v0 + v1) / 2.0 * d;
			wSum += d;
			c += cw;
		}
	}
	if (wSum > FLT_EPSILON)
		c /= wSum;
	return c;
}

double Polygon3::GetArea() const {
	Vector3 temp;
	for (const auto &edge : e) {
		const auto &v0 = v[edge.VertexIndex(0)];
		const auto &v1 = v[edge.VertexIndex(1)];
		temp += (v0 * v1);
	}
	return temp.Abs() / 2.0;
}

Vector3 Polygon3::GetRotationalAxis() const {
	if (e.size() < 2)
		return Vector3();
	const Vector3 center = GetCenter();
	Vector3 temp;
	for (const auto &edge : e) {
		const auto &v0 = v[edge.VertexIndex(0)];
		const auto &v1 = v[edge.VertexIndex(1)];
		temp += (v0 - center) * (v1 - v0);
	}
	temp.Normalize();
	return temp;
}

Polygon3::Result Polygon3::At(double L) const {
	const size_t N = v.size();
	Result res;
	if (N == 0)
		return res;
	if (N == 1) {
		res.pos = v[0];
		res.normal = v[0].n;
		return res;
	}
	size_t n = 0;
	double sL = 0.0;
	double dL = (v[1] - v[0]).Abs();
	while (n < (N - 2) && (sL + dL) < L) {
		++n;
		sL += dL;
		dL = (v[n + 1] - v[n]).Abs();
	}
	res.rel = (L - sL);
	res.idx = n;
	res.dir = (v[n + 1] - v[n]).Normal();
	res.pos = v[n] + res.dir * res.rel;
	res.normal = (v[n].n + (v[n + 1].n - v[n].n) * res.rel / dL);
	return res;
}

std::tuple<size_t, size_t> Polygon3::ClosestPoint(const Vector3 &p) const {
	size_t minimalIndex = 0;
	double minimalDistance = DBL_MAX;
	for (size_t n = 0; n < v.size(); ++n) {
		const double distance = (v[n] - p).Abs();
		if (distance < minimalDistance) {
			minimalDistance = distance;
			minimalIndex = n;
		}
	}
	return std::make_tuple(minimalIndex, v[minimalIndex].group);
}

size_t Polygon3::ClosestPoint(const Vector3 &p, size_t group) const {
	size_t minimalIndex = 0;
	double minimalDistance = DBL_MAX;
	for (size_t n = 0; n < v.size(); ++n) {
		if (v[n].group != group)
			continue;
		const double distance = (v[n] - p).Abs();
		if (distance < minimalDistance) {
			minimalDistance = distance;
			minimalIndex = n;
		}
	}
	return minimalIndex;
}

std::string Polygon3::ToString() const {
	std::ostringstream s;
	s << '[';
	bool first = true;
	for (const auto &vert : v) {
		if (first)
			first = false;
		else
			s << ';';
		s << vert.x << ',' << vert.y << ',' << vert.z;
	}
	s << ']';
	return s.str();
}


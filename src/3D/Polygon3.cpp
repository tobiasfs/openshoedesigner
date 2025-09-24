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

#include "../math/MatlabFile.h"

#include <algorithm>
#include <float.h>
#include <iostream>
#include <list>
#include <queue>
#include <sstream>

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

void Polygon3::ExtractOutline(const Geometry &other) {
	const Vector3 center = other.GetCenterOfVertices();
	ResetAddNormal();
	for (size_t n = 0; n < other.CountEdges(); n++) {
		const auto &ed = other.GetEdge(n);
		if (ed.trianglecount >= 2)
			continue;
		const Geometry::Vertex &v0 = other.GetEdgeVertex(n, 0);
		const Geometry::Vertex &v1 = other.GetEdgeVertex(n, 1);
		Vector3 rot = (v0 - center) * (v1 - v0);
		if (rot.z > 0.0)
			AddEdge(v0, v1);
		else
			AddEdge(v1, v0);
		e.back().n = ed.n;
		e.back().c = ed.c;
	}
	CopyPropertiesFrom(other);
	paintEdges = true;
	Join();
	SortLoop();
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

	// Count how many edges connect to a vertex.
	std::vector<size_t> connections(v.size(), 0);
	for (const Edge &ed : e)
		connections[ed.vb]++;
	// Vertices without connections to them are starting points.
	std::list<size_t> starting_points;
	for (size_t idx = 0; idx < connections.size(); idx++)
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
	for (size_t n = vfirst; n < v.size(); n++)
		if (v[n].group != nothing)
			v[n].group += groupCount;
	for (size_t n = efirst; n < e.size(); n++)
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
	const Vector3 p0 = v[it->GetVertexIndex(0)];
	const Vector3 p1 = v[it->GetVertexIndex(1)];
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
		size_t idx0 = ed.GetVertexIndex(0);
		size_t idx1 = ed.GetVertexIndex(1);
		Vector3 n = (v[idx1] - v[idx0]) * planenormal;
		n.Normalize();
		ed.n = n;
		v[ed.va].n += n;
		v[ed.vb].n += n;
	}

	for (auto &vect : v)
		vect.n.Normalize();
}

double Polygon3::MapU(bool fixFirstVertexToZero) {
	if (v.empty())
		return 0.0;
	if (e.empty())
		return 0.0;
	v.front().u = 0.0;
	for (Edge ed : e) {
		auto &v0 = v[ed.va];
		auto &v1 = v[ed.vb];
		const double d = (v1 - v0).Abs();
		v1.u = v0.u + d;
	}
	const double L = v[e.back().vb].u;
	if (fixFirstVertexToZero)
		v[e.front().va].u = 0.0;
	return L;
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

void Polygon3::Filter(unsigned int width) {
//TODO Implement this.
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
		const auto &v0 = v[edge.GetVertexIndex(0)];
		const auto &v1 = v[edge.GetVertexIndex(1)];
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
		const auto &v0 = v[edge.GetVertexIndex(0)];
		const auto &v1 = v[edge.GetVertexIndex(1)];
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
		n++;
		sL += dL;
		dL = (v[n + 1] - v[n]).Abs();
	}
	res.rel = (L - sL);
	res.idx = n;
	res.dir = (v[n + 1] - v[n]).Normal();
	res.pos = v[n].Interp(v[n + 1], res.rel / dL);
	res.normal = v[n].n.Interp(v[n + 1].n, res.rel / dL);
	return res;
}

Polygon3::Result Polygon3::AtU(double u) const {
	const size_t N = v.size();
	if (N <= 1)
		return At(u);
	Result res;
	double L = v.back().u;
	if (fabs(v.front().u) > DBL_EPSILON)
		L = v.front().u;
	while (u < 0.0)
		u += L;
	while (u > L)
		u -= L;
	size_t idx = 0;
	while (u > v[e[idx].vb].u && (idx + 1) < N)
		idx++;
	if (idx >= N)
		return res;
	const Edge &ed = e[idx];
	const Vertex &va = v[ed.va];
	const Vertex &vb = v[ed.vb];

	const double u0 = (ed.va == 0) ? 0.0 : va.u;
	const double u1 = (ed.vb == 0) ? 0.0 : vb.u;
	const double d = u1 - u0;
	if (fabs(d) > DBL_EPSILON)
		res.rel = u - u0;
	else
		res.rel = 0.0;
	res.idx = idx;
	res.dir = (vb - va).Normal();
	res.pos = va.Interp(vb, res.rel / d);
	res.normal = va.n.Interp(vb.n, res.rel / d);
	return res;
}

std::tuple<size_t, size_t> Polygon3::ClosestPoint(const Vector3 &p) const {
	size_t minimalIndex = 0;
	double minimalDistance = DBL_MAX;
	for (size_t n = 0; n < v.size(); n++) {
		const double distance = (v[n] - p).Abs();
		if (distance < minimalDistance) {
			minimalDistance = distance;
			minimalIndex = n;
		}
	}
	return std::make_tuple(minimalIndex, v[minimalIndex].group); // @suppress("Function cannot be instantiated")
}

size_t Polygon3::ClosestPoint(const Vector3 &p, size_t group) const {
	size_t minimalIndex = 0;
	double minimalDistance = DBL_MAX;
	for (size_t n = 0; n < v.size(); n++) {
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

Polygon3::Intersections Polygon3::Intersect(Vector3 n, double d) const {
	Polygon3::Intersections ret;
	for (size_t idx = 0; idx < e.size(); idx++) {
		const auto &v0 = GetEdgeVertex(idx, 0);
		const auto &v1 = GetEdgeVertex(idx, 1);
		const double h0 = v0.Dot(n);
		const double h1 = v1.Dot(n);
		if (h0 <= d && h1 > d)
			ret.positive.push_back(v0.Interp(v1, (d - h0) / (h1 - h0)));
		if (h0 >= d && h1 < d)
			ret.negative.push_back(v0.Interp(v1, (d - h0) / (h1 - h0)));
	}
	return ret;
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

void Polygon3::Triangulate() {
	if (e.empty())
		return;
	Sort();

	// If some triangles already exist, correct the triangle edge assignment.
	// i.e. swapping e[].ta and e[].tb so that from above in the direction
	// of the edge ta is left and tb is right. This is needed to find edges,
	// that already have been processed.

	for (Edge &ed : e) {
		if (ed.trianglecount != 1)
			continue;
		// After sorting, trianglecount is correct and ta always contains the
		// triangle if trianglecount == 1.
//		Triangle &tri = t[ed.ta];
		// Orient the edges, so that they point mathematically negative. The
		// existing triangles look like holes this way.
//		ed.flip = (tri.GetDirection(ed.va, ed.vb) == 1);
		if (!ed.flip)
			std::swap(ed.ta, ed.tb);
	}

	// Add edges to generate areas that are x-monotone.

	// Collect some information on the vertices.

	std::vector<size_t> left_count(v.size(), 0);
	std::vector<size_t> right_count(v.size(), 0);
	std::vector<int> dir(v.size(), 0);
//	std::vector<double> eslope;
//	eslope.reserve(e.size());
	std::vector<double> vslope(v.size(), 0);

	for (const Edge &ed : e) {
		if (ed.trianglecount >= 2) {
//			eslope.push_back(0.0);
			continue;
		}
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
//		eslope.push_back(es);
		vslope[ed.va] += es;
		vslope[ed.vb] += es;
	}

//	verticesHaveColor = true;
//	paintVertices = true;
//	for (size_t n = 0; n < v.size(); n++) {
//		v[n].c.r = fmin(fmax(-vslope[n] * 2.0, 0), 1);
//		v[n].c.g = fmin(fmax(vslope[n] * 2.0, 0), 1);
//		v[n].c.b = 0.0;
//	}

	// Mark all existing edges as "sharp" to separate them from the extra edges.

	for (Edge &ed : e)
		ed.sharp |= (ed.group == nothing || ed.group == 0);

	// Add extra edges

	for (size_t idx = 0; idx < v.size(); idx++) {
		if (right_count[idx] == 0 && vslope[idx] < -DBL_EPSILON) {
			//TODO Check if the vertex found is inside or outside using the .sharp flag.

			size_t idx1 = idx;
			bool crossed = false;
			do {
				idx1++;
				if (idx1 >= v.size()) {
					throw std::logic_error(
							"Triangulate(): The vertex search algorithm returned a vertex outside the number of available vertices.");
				}

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

			// The extra edges are added pointing from left to right by
			// construction.
			AddEdge(idx, idx1);
			e.back().n = (v[idx].n + v[idx1].n) / 2.0;
		}
		if (left_count[idx] == 0 && vslope[idx] > DBL_EPSILON) {
			//TODO Check if the vertex found is inside or outside using the .sharp flag.
			size_t idx0 = idx;
			bool crossed = false;
			do {
				if (idx0 == 0) {
					throw std::logic_error(
							"Triangulate(): The vertex search algorithm returned a vertex smaller than 0.");
				}
				idx0--;

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
	// Reposition existing triangles to the outside of single-triangle-edges.
	for (Edge &ed : e)
		if (ed.trianglecount == 1 && !ed.flip)
			std::swap(ed.ta, ed.tb);

	// Redo the statistics with the added edges

	left_count.assign(v.size(), 0);
	right_count.assign(v.size(), 0);
	dir.assign(v.size(), 0);
	std::vector<double> eslope;
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

	// Store the current number of edges, as the generation of triangles also
	// generates edges, that need to be ignored here.
	const size_t Ne = e.size();

	bool runagain = true; // For each area in the polygon
	while (runagain) {
		runagain = false;

		// Search for a starting point.
		size_t vidx = (size_t) -1;
		size_t eidx = 0;
		while ((vidx + 1) < v.size() || vidx == (size_t) -1) {
			vidx++;

//			if (vidx == 2 || vidx == 3) {
//				std::cout << "x\n";
//			}

			// Search for a starting point
			if (right_count[vidx] < 2)
				continue;

			size_t e_top = nothing;
			size_t e_bot = nothing;

			// Search for the first edge for a given vertex.
			while (eidx < Ne && e[eidx].va < vidx)
				eidx++;
			if (eidx >= Ne) {
				throw std::logic_error(
						"Triangulate(): Free-floating vertex without edges connected found.");
			}
			size_t e_first = eidx;

			// Find top-most valid edge (e_top)
			for (eidx = e_first; eidx < Ne && e[eidx].va == vidx; eidx++) {

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
			for (eidx = e_first; eidx < Ne && e[eidx].va == vidx; eidx++) {
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

			for (; vidx < v.size(); vidx++) {
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
						for (size_t n = 1; n < idx_bot.size(); n++) {
							AddTriangleMinimal(idx_bot[n - 1], idx_bot[n],
									vidx);
						}
						idx_top.assign(1, idx_bot.back());
						idx_bot.assign(1, idx_bot.back());
					}

					idx_top.push_back(vidx);
					c++;

					// Search for the next top edge
					eidx = e_top;
					e_top = nothing;
					for (; eidx < Ne && e[eidx].va <= vidx; eidx++) {
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
						for (size_t n = 1; n < idx_top.size(); n++) {
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
					for (; eidx < Ne && e[eidx].va <= vidx; eidx++) {
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
	for (Edge &ed : e)
		ed.Fix();
	for (Triangle &tri : t)
		tri.Fix();

	Sort();

	PropagateNormals();
	CalculateUVCoordinateSystems();
//	Finish(); // Sort the triangles into a consistent order
}

void Polygon3::AddTriangleMinimal(size_t idx0, size_t idx1, size_t idx2) {
	Geometry::Triangle tri;
	tri.va = idx0;
	tri.vb = idx1;
	tri.vc = idx2;

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

	tri.Fix();
	const size_t tidx = t.size();

	for (size_t idx = 0; idx < e.size(); idx++) {
		Geometry::Edge &ed = e[idx];

		if (ed.va == tri.va && ed.vb == tri.vb) {
			tri.ea = idx;

			if (tri.flip)
				ed.tb = tidx;
			else
				ed.ta = tidx;

			ed.trianglecount = ((ed.ta == nothing) ? 0 : 1)
					+ ((ed.tb == nothing) ? 0 : 1);
		}
		if (ed.va == tri.vb && ed.vb == tri.vc) {
			tri.eb = idx;

			if (tri.flip)
				ed.tb = tidx;
			else
				ed.ta = tidx;
			ed.trianglecount = ((ed.ta == nothing) ? 0 : 1)
					+ ((ed.tb == nothing) ? 0 : 1);
		}
		if (ed.va == tri.va && ed.vb == tri.vc) {
			tri.ec = idx;

			if (tri.flip)
				ed.ta = tidx;
			else
				ed.tb = tidx;
			ed.trianglecount = ((ed.ta == nothing) ? 0 : 1)
					+ ((ed.tb == nothing) ? 0 : 1);
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

Polygon3 Polygon3::Voronoi() const {
	// https://archive.org/details/computational-geometry-algorithms-and-applications-mark-de-berg-otfried-cheong-marc-van-kreveld-etc./page/151/mode/2up
	// https://www.cs.princeton.edu/courses/archive/spring12/cos423/bib/fortune.pdf
	// https://www2.cs.sfu.ca/~binay/813.2011/Fortune.pdf
	// https://www.cs.hmc.edu/~mbrubeck/voronoi.html

	Polygon3 ret;
	if (v.size() < 2)
		return ret; // Nothing to do. At least 2 points are needed to generate a single line.

	// Sort all vertices by y, x (and z).
	Polygon3 Q;
	Q.v = v;
	size_t count = 0;
	for (auto &p : Q.v) {
		p.z = 0.0;
		p.c = { 1, 1, 1 };
		p.group = count++;
	}
	Q.Join();

	auto vertex_less = [eps=epsilon](const Geometry::Vertex &a,
			const Geometry::Vertex &b) {
		if (a.y < (b.y - eps))
			return true;
		if (a.y >= (b.y + eps))
			return false;
		if (a.x < (b.x - eps))
			return true;
		if (a.x >= (b.x + eps))
			return false;
		if (a.z < b.z)
			return true;
		return false;
	};
	std::sort(Q.v.begin(), Q.v.end(), vertex_less);

	class Event;
	class Arc {
	public:
		double x;
		double y;
		double x1; // Arc end
		size_t vidx_remove;
		std::list<Event>::iterator ev;
		Vertex he;
		size_t eidx = nothing;
		void Update(const Arc &pk, const Arc &pj, double by) {
			const double T2 = pk.y - pj.y;
			if (fabs(T2) < DBL_EPSILON) {
				// L'Hôpital's rule applied to the formula below
				x1 = (pk.x + pj.x) / 2.0;
				return;
			}
			const double T0 = pk.y - by;
			const double T1 = pj.y - by;
			const double T3 = pk.x - pj.x;
			const double T4 = pj.x * pk.y - pj.y * pk.x;
			const double T5 = T0 * T1 * (T2 * T2 + T3 * T3);
			const double T6 = sqrt(T5);
			const double T7 = T4 + T3 * by;
			x1 = (T6 + T7) / T2;
		}
	};
	std::list<Arc> T; ///< Beachline

	class Event {
	public:
		double x;
		double y;
		double y0;
		std::list<Arc>::iterator arc;
	};
	auto event_cmp = [](const Event &lhs, const Event &rhs) {
		return lhs.y0 < rhs.y0;
	};
//	std::priority_queue<Event, std::list<Event>, decltype(event_cmp)> Qe {
//			event_cmp };
	std::list<Event> Qe;

	// 1.) If T is empty, insert pi into it (so that T consists of
	// a single leaf storing pi) and return.
	std::vector<size_t> topEdges;
	// Pre-fill T with all arcs on the first line
	auto Qs = Q.v.begin();
	T.push_back( { Qs->x, Qs->y, DBL_MAX, Qs->group, Qe.end() });
	Qs++;
	while (fabs(Qs->y - T.back().y) < DBL_EPSILON) {
		Arc &pi = T.back();
		T.push_back( { Qs->x, Qs->y, DBL_MAX, Qs->group, Qe.end() });
		Arc &pj = T.back();
		pi.he.x = (pj.x + pi.x) / 2.0;
		pi.he.y = (pj.y + pi.y) / 2.0;
		pi.he.z = 0.0;
		pi.he.n.x = -(pj.y - pi.y);
		pi.he.n.y = (pj.x - pi.x);
		pi.he.n.z = 0.0;

		ret.e.emplace_back();
		ret.e.back().va = nothing;
		ret.e.back().vb = nothing;
		ret.e.back().n = pi.he.n;
		ret.e.back().trianglecount = 0;
		pi.eidx = ret.e.size() - 1;
		topEdges.push_back(ret.e.size() - 1);
		Qs++;
	}

	// Main loop
	while (Qs != Q.v.end() || !Qe.empty()) {
		// Horizon
		double hy = 0.0;
//		if (Qs != Q.v.end())
		if (Qs != Q.v.end() && (Qe.empty() || Qs->y < Qe.front().y0))
			hy = Qs->y;
		else
			hy = Qe.front().y0;
//		if (!Qe.empty() && Qs->y > Qe.front().y)
//			hy = Qs->y + 0.1;

		// Calculate all arc-splits for the horizon at hy
		std::list<Arc>::iterator it1 = T.begin();
		std::list<Arc>::iterator it0 = it1++;
		for (; it1 != T.end(); ++it0, it1++) {
			if (fabs(it0->y - it1->y) < DBL_EPSILON) {
				it0->x1 = (it0->x + it1->x) / 2.0;
				continue;
			}
			const double T1 = hy - it0->y;
			const double T2 = hy - it1->y;
			const double T3 = it1->x - it0->x;
			const double T4 = it1->y - it0->y;
			const double T5 = T3 * T3 + T4 * T4;
			const double T6 = T1 * T2 * T5;
			const double T7 = sqrt(T6) / T4; // :W
			const double T8 = (T1 * it1->x - T2 * it0->x) / T4;
//			std::cout << T8 << " + " << T7 << " = " << T8 + T7 << "\n";
//			std::cout << T8 << " - " << T7 << " = " << T8 - T7 << "\n";
			it0->x1 = T8 - T7;
//			std::cout << "---\n";
		}
		it0->x1 = DBL_MAX;

#ifdef DEBUG
//		{
//			std::cout << "Beachline (y=" << hy << "):\n";
//			std::cout << "x0 = " << -DBL_MAX << "\n";
//			for (const auto &b : T) {
//				std::cout << "\tSite: (" << b.x << ", " << b.y << ") \n";
//				std::cout << "x1 = " << b.x1;
//				std::cout << "\t\tHalf-edge: (" << b.he.x << ", " << b.he.y;
//				std::cout << " : " << b.he.n.x << ", " << b.he.n.y << ")\n";
//			}
//		}
//		std::cout << "-------------------------------------------\n";
//		{
//			std::cout << "Lines:\n";
//			size_t c = 0;
//			for (const auto &ed : ret.e) {
//				std::cout << "\t" << c++ << "\t[";
//				if (ed.va == nothing)
//					std::cout << " -";
//				else
//					std::cout << " " << ed.va;
//				std::cout << ",";
//				if (ed.vb == nothing)
//					std::cout << " -";
//				else
//					std::cout << " " << ed.vb;
//				std::cout << "]";
//				if (ed.trianglecount == 0)
//					std::cout << " = line";
//				else if (ed.trianglecount == 1)
//					std::cout << " = half-edge";
//				else if (ed.trianglecount == 2)
//					std::cout << " = edge";
//				else
//					std::cout << " = ? (" << ed.trianglecount << ")";
//				std::cout << '\n';
//			}
//		}
//		{
//			std::cout << "Arcs:\n";
//			for (const auto &a : T) {
//				std::cout << "\teidx = " << a.eidx;
//				std::cout << '\n';
//			}
//		}
//
//		{
//			MatlabFile mf("/tmp/voronoi.mat");
//			Matrix m_hy("hy", 1);
//			Matrix m_points("points", Matrix::Order::TWO_REVERSED);
//			Matrix m_lines("lines", Matrix::Order::TWO_REVERSED);
//			Matrix m_voronoi("voronoi", Matrix::Order::TWO_REVERSED);
//			Matrix m_arcs("arcs", Matrix::Order::TWO_REVERSED);
//			Matrix m_events("events", Matrix::Order::TWO_REVERSED);
//			m_hy.Insert(hy);
//
//			for (const auto &a : T) {
//
//				const double c2 = 1.0 / (2.0 * (a.y - hy));
//				const double c1 = (-2.0 * a.x) * c2;
//				const double c0 = (a.x * a.x + a.y * a.y - hy * hy) * c2;
//
//				m_arcs.push_back(a.x);
//				m_arcs.push_back(a.y);
//				m_arcs.push_back(a.x1);
//				m_arcs.push_back(c2);
//				m_arcs.push_back(c1);
//				m_arcs.push_back(c0);
//			}
//			m_arcs.Reshape(-1, 6);
//
//			for (const auto &q : Qe) {
//				m_events.push_back(q.x);
//				m_events.push_back(q.y);
//			}
//			m_events.Reshape(-1, 2);
//			for (auto it = Q.v.begin(); it != Qs; it++) {
//				m_points.push_back(it->x);
//				m_points.push_back(it->y);
//			}
//			m_points.Reshape(-1, 2);
//			for (const auto &ed : ret.e) {
//				const size_t idxa = ed.va;
//				const size_t idxb = ed.vb;
//				if (idxa == nothing || idxb == nothing)
//					continue;
//				const Vertex &va = ret.v[idxa];
//				const Vertex &vb = ret.v[idxb];
//				m_lines.push_back(va.x);
//				m_lines.push_back(va.y);
//				m_lines.push_back(vb.x);
//				m_lines.push_back(vb.y);
//			}
//			m_lines.Reshape(-1, 4);
//			for (const auto &vert : ret.v) {
//				m_voronoi.push_back(vert.x);
//				m_voronoi.push_back(vert.y);
//			}
//			m_voronoi.Reshape(-1, 2);
//
//			mf.WriteMatrix(m_hy);
//			m_points.ReorderDimensions();
//			mf.WriteMatrix(m_points);
//			m_lines.ReorderDimensions();
//			mf.WriteMatrix(m_lines);
//			m_voronoi.ReorderDimensions();
//			mf.WriteMatrix(m_voronoi);
//			m_arcs.ReorderDimensions();
//			mf.WriteMatrix(m_arcs);
//			m_events.ReorderDimensions();
//			mf.WriteMatrix(m_events);
//		}
#endif
		std::list<Arc>::iterator p0;
		std::list<Arc>::iterator pi;
		std::list<Arc>::iterator pj;
		std::list<Arc>::iterator pk;
		std::list<Arc>::iterator p1;

		bool dosort = false;
		if (Qs != Q.v.end() && (Qe.empty() || Qs->y < Qe.front().y0)) {
			// Handle Site Event

			// 2.) Search in T for the arc α vertically above pi . If the leaf
			// representing α has a pointer to a circle event in Q, then this
			// circle event is a false alarm and it must be deleted from Q.
			pk = std::find_if(T.begin(), T.end(), [&pt = *Qs](const Arc &arc) {
				return arc.x1 > pt.x;
			});
			if (pk == T.end())
				throw std::logic_error("Implementation error.");
			if (fabs(pk->y - Qs->y) < DBL_EPSILON) {
				throw std::logic_error("Implementation error.");
			}

			if (pk->ev != Qe.end()) {
				Qe.erase(pk->ev);
				pk->ev = Qe.end();
			}

			// 3.) Replace the leaf of T that represents α with a subtree
			// having three leaves. The middle leaf stores the new site pi
			// and the other two leaves store the site pj that was originally
			// stored with α. Store the tuples <pj, pi> and <pi, pj>
			// representing the new breakpoints at the two new internal nodes.
			Arc arcsplit = *pk;
			pj = T.insert(pk, { Qs->x, Qs->y, Qs->x, Qs->group, Qe.end() });
			pi = T.insert(pj, arcsplit);
			pi->x1 = Qs->x;
			// pi -> pj -> pk    (pj is new)

			// 4.) Create new half-edge records in the Voronoi diagram
			// structure for the edge separating V(pi) and V(pj), which will
			// be traced out by the two new breakpoints.
			{
				pi->he.x = (pj->x + pi->x) / 2.0;
				pi->he.y = (pj->y + pi->y) / 2.0;
				pi->he.z = 0.0;
				pi->he.n.x = -(pj->y - pi->y);
				pi->he.n.y = (pj->x - pi->x);
				pi->he.n.z = 0.0;
				pj->he.x = (pk->x + pj->x) / 2.0;
				pj->he.y = (pk->y + pj->y) / 2.0;
				pj->he.z = 0.0;
				pj->he.n.x = -(pk->y - pj->y);
				pj->he.n.y = (pk->x - pj->x);
				pj->he.n.z = 0.0;

				// FIXME pi as the leftmost arc keeps the already available line
				// FIXME pj and pk are assigned a new line.
				ret.e.emplace_back();
				ret.e.back().va = nothing;
				ret.e.back().vb = nothing;
				ret.e.back().trianglecount = 0;
				pi->eidx = ret.e.size() - 1;
//				ret.e.emplace_back();
//				ret.e.back().va = nothing;
//				ret.e.back().vb = nothing;
//				ret.e.back().trianglecount = 0;
				pj->eidx = ret.e.size() - 1;
			}

			Qs++;

			p1 = pk;
			if (p1 != T.end())
				p1++;
			if (p1 != T.end()) {
				// Right arc pj -> pk -> p1
				const double div = pj->he.n.x * pk->he.n.y
						- pj->he.n.y * pk->he.n.x;
				if (div > DBL_EPSILON) {

					const double T1 = pj->he.y - pk->he.y;
					const double T2 = pk->he.x - pj->he.x;
					const double g = (T1 * pk->he.n.x + T2 * pk->he.n.y) / div;
					const double h = (T1 * pj->he.n.x + T2 * pj->he.n.y) / div;
					const double xe = pj->he.x + pj->he.n.x * g;
					const double ye = pj->he.y + pj->he.n.y * g;
					const double r = sqrt(
							(xe - pk->x) * (xe - pk->x)
									+ (ye - pk->y) * (ye - pk->y));
					const double yr = ye + r;
					{
						Qe.push_front( { xe, ye, yr, pk });
						pk->ev = Qe.begin();
						dosort = true;
					}
				}
			}

		} else {
			// Handle Circle event
#ifdef DEBUG
//			std::cout << "Circle Event\n";
#endif
			// 1.) Delete the leaf γ that represents the disappearing arc α
			// from T. Update the tuples representing the breakpoints at the
			// internal nodes. Perform rebalancing operations on T if
			// necessary. Delete all circle events involving α from Q; these
			// can be found using the pointers from the predecessor and the
			// successor of γ in T. (The circle event where α is the middle
			// arc is currently being handled, and has already been deleted
			// from Q.)
			Event E = Qe.front();
			Qe.pop_front();
			std::list<Arc>::iterator a1 = E.arc;
			std::list<Arc>::iterator a0 = a1;
			std::list<Arc>::iterator a2 = a1;
			if (a0 != T.begin()) {
				a0--;
				for (std::list<Event>::iterator e = Qe.begin(); e != Qe.end();
						e++) {
					if (e->arc == a0) {
						a0->ev = Qe.end();
						Qe.erase(e);
						break;
					}
				}
//				Qe.remove_if([&a0](const Event &e) {
//					return e.arc == a0;
//				});
			}
			a2++;
			if (a2 != T.end()) {
				for (std::list<Event>::iterator e = Qe.begin(); e != Qe.end();
						e++) {
					if (e->arc == a2) {
						a2->ev = Qe.end();
						Qe.erase(e);
						break;
					}
				}

//				Qe.remove_if([&a2](const Event &e) {
//					return e.arc == a2;
//				});
			}
			const size_t eidx = E.arc->eidx;
			pj = T.erase(E.arc);
			if (pj == T.begin())
				throw std::logic_error("Implementation error.");

			// 2.) Add the center of the circle causing the event as a vertex
			// record to the doubly-connected edge list D storing the Voronoi
			// diagram under construction. Create two half-edge records
			// corresponding to the new breakpoint of the beach line. Set the
			// pointers between them appropriately. Attach the three new
			// records to the half-edge records that end at the vertex.
			const size_t vidx = ret.v.size();
			ret.v.push_back(Vertex(E.x, E.y));
			{
				auto &ed = ret.e[eidx];
				if (ed.trianglecount == 0) {
					ed.va = vidx;
				} else {
					ed.vb = vidx;
				}
				ed.trianglecount++;
			}
			pi = pj;
			pi--;
			{
				auto &ed = ret.e[pi->eidx];
				if (ed.trianglecount == 0) {
					ed.va = vidx;
				} else {
					ed.vb = vidx;
				}
				ed.trianglecount++;
			}

			pi->he.x = (pj->x + pi->x) / 2.0;
			pi->he.y = (pj->y + pi->y) / 2.0;
			pi->he.z = 0.0;
			pi->he.n.x = -(pj->y - pi->y);
			pi->he.n.y = (pj->x - pi->x);
			pi->he.n.z = 0.0;

			// pj is assigned a new line.
			ret.e.emplace_back();
			ret.e.back().va = vidx;
			ret.e.back().vb = nothing;
			ret.e.back().trianglecount = 1;
			pi->eidx = ret.e.size() - 1;

			// 3.) Check the new triple of consecutive arcs that has the
			// former left neighbor of α as its middle arc to see if the two
			// breakpoints of the triple converge. If so, insert the
			// corresponding circle event into Q. and set pointers between
			// the new circle event in Q and the corresponding leaf of T. Do
			// the same for the triple where the former right neighbor is
			// the middle arc.
			// p0 -> pi -> pj -> pk
			pk = pj;
			if (pk != T.end())
				pk++;

			if (pk != T.end()) {
				// Right arc pi -> pj -> pk

				const double div = pi->he.n.x * pj->he.n.y
						- pi->he.n.y * pj->he.n.x;
				if (div > DBL_EPSILON) {

					const double T1 = pi->he.y - pj->he.y;
					const double T2 = pj->he.x - pi->he.x;
					const double g = (T1 * pj->he.n.x + T2 * pj->he.n.y) / div;
					const double h = (T1 * pi->he.n.x + T2 * pi->he.n.y) / div;
					const double xe = pi->he.x + pi->he.n.x * g;
					const double ye = pi->he.y + pi->he.n.y * g;
					const double r = sqrt(
							(xe - pj->x) * (xe - pj->x)
									+ (ye - pj->y) * (ye - pj->y));
					const double yr = ye + r;
					{
						Qe.push_front( { xe, ye, yr, pj });
						pj->ev = Qe.begin();
						dosort = true;
					}
				}
			}

		}

		// 3. & 5.) Check the triple of consecutive arcs where the new arc
		// for pi is the left arc to see if the breakpoints converge. If
		// so, insert the circle event into Q and add pointers between the
		// node in T and the node in Q. Do the same for the triple where
		// the new arc is the right arc.

		if (pi != T.begin()) {
			p0 = pi;
			p0--;
			// Left arc p0 -> pi -> pj
			const double div = p0->he.n.x * pi->he.n.y
					- p0->he.n.y * pi->he.n.x;
			if (div > DBL_EPSILON) {
				const double T1 = p0->he.y - pi->he.y;
				const double T2 = pi->he.x - p0->he.x;
				const double g = (T1 * pi->he.n.x + T2 * pi->he.n.y) / div;
				const double h = (T1 * p0->he.n.x + T2 * p0->he.n.y) / div;
				const double xe = p0->he.x + p0->he.n.x * g;
				const double ye = p0->he.y + p0->he.n.y * g;
				const double r = sqrt(
						(xe - pi->x) * (xe - pi->x)
								+ (ye - pi->y) * (ye - pi->y));
				const double yr = ye + r;
				{
					Qe.push_front( { xe, ye, yr, pi });
					pi->ev = Qe.begin();
					dosort = true;
				}
			}
		}

		if (dosort)
			Qe.sort(event_cmp);
	}

	// Bounding box for not converging edges.

	double xmin = DBL_MAX;
	double xmax = -DBL_MIN;
	double ymin = DBL_MAX;
	double ymax = -DBL_MIN;
	for (Vertex &vert : Q.v) {
		if (vert.x < xmin)
			xmin = vert.x;
		if (vert.x > xmax)
			xmax = vert.x;
		if (vert.y < ymin)
			ymin = vert.y;
		if (vert.y > ymax)
			ymax = vert.y;
	}
	for (Vertex &vert : ret.v) {
		if (vert.x < xmin)
			xmin = vert.x;
		if (vert.x > xmax)
			xmax = vert.x;
		if (vert.y < ymin)
			ymin = vert.y;
		if (vert.y > ymax)
			ymax = vert.y;
	}

	const double sx = fmax(FLT_EPSILON, xmax - xmin);
	const double sy = fmax(FLT_EPSILON, ymax - ymin);
	xmin -= sx;
	xmax += sx;
	ymin -= sy;
	ymax += sy;

	// Connect the remaining edges to the bounding box.

	size_t vstart = nothing;
	size_t v0idx = nothing;
	for (size_t eidx : topEdges) {
		Edge &ed = ret.e[eidx];
		if (ed.trianglecount >= 2)
			throw std::logic_error("Implementation error.");
		const Vertex &v0 = ret.v[ed.va];
		if (ed.trianglecount == 0) {
			ret.v.emplace_back(v0.x, ymax, v0.z);
			ed.va = ret.v.size() - 1;
			ed.trianglecount++;
		}
		ret.v.emplace_back(v0.x, ymin, v0.z);
		const size_t vidx = ret.v.size() - 1;
		ed.vb = vidx;
		ed.trianglecount++;
		if (v0idx == nothing)
			v0idx = vidx;
		vstart = vidx;
	}
	int quad = 0;
	for (const auto &a : T) {
		const size_t eidx = a.eidx;
		if (eidx == nothing)
			continue;
		const auto &he = a.he;
		Vertex v;
		int newquad = 0;
		if (fabs(he.n.x) < DBL_EPSILON) {
			if (he.n.y < 0.0) {
				newquad = 0;
				v.Set(he.x, ymin, he.z);
			} else {
				newquad = 2;
				v.Set(he.x, ymax, he.z);
			}
		} else if (fabs(he.n.y) < DBL_EPSILON) {
			if (he.n.x < 0.0) {
				newquad = 1;
				v.Set(xmin, he.y, he.z);
			} else {
				newquad = 3;
				v.Set(xmax, he.y, he.z);
			}
		} else {
			Vertex vx;
			if (he.n.x > 0.0) {
				vx.Set(xmax, he.y + (xmax - he.x) * he.n.y / he.n.x, he.z);
			} else {
				vx.Set(xmin, he.y + (xmin - he.x) * he.n.y / he.n.x, he.z);
			}
			Vertex vy;
			if (he.n.y > 0.0) {
				vy.Set(he.x + (ymax - he.y) * he.n.x / he.n.y, ymax, he.z);
			} else {
				vy.Set(he.x + (ymin - he.y) * he.n.x / he.n.y, ymin, he.z);
			}
			if (vx.y >= ymin && vx.y <= ymax) {
				v = vx;
				newquad = (vx.x > 0.0) ? 3 : 1;
			} else {
				v = vy;
				newquad = (vx.y > 0.0) ? 2 : 0;
			}
		}
		if (newquad < quad)
			newquad += 4;
		while (newquad > quad) {
			quad++;
			switch (quad) {
			default:
				ret.v.emplace_back(xmax, ymin, 0);
				break;
			case 1:
				ret.v.emplace_back(xmin, ymin, 0);
				break;
			case 2:
				ret.v.emplace_back(xmin, ymax, 0);
				break;
			case 3:
				ret.v.emplace_back(xmax, ymax, 0);
				break;
			}
			if (v0idx != nothing)
				ret.AddEdge(v0idx, ret.v.size() - 1);
			v0idx = ret.v.size() - 1;
			if (vstart == nothing)
				vstart = ret.v.size() - 1;
		}
		ret.v.push_back(v);
		ret.e[eidx].vb = ret.v.size() - 1;

		if (v0idx != nothing)
			ret.AddEdge(v0idx, ret.v.size() - 1);
		v0idx = ret.v.size() - 1;
		if (vstart == nothing)
			vstart = ret.v.size() - 1;
	}

	while (4 > quad) {
		quad++;
		switch (quad) {
		default:
			ret.v.emplace_back(xmax, ymin, 0);
			break;
		case 1:
			ret.v.emplace_back(xmin, ymin, 0);
			break;
		case 2:
			ret.v.emplace_back(xmin, ymax, 0);
			break;
		case 3:
			ret.v.emplace_back(xmax, ymax, 0);
			break;
		}
		if (v0idx != nothing)
			ret.AddEdge(v0idx, ret.v.size() - 1);
		v0idx = ret.v.size() - 1;
		if (vstart == nothing)
			vstart = ret.v.size() - 1;
	}
	if (v0idx != nothing)
		ret.AddEdge(v0idx, vstart);

#ifdef DEBUG
//	double hy = 6.0;
//	std::cout << "-------------------------------------------\n";
//	{
//		std::cout << "Lines:\n";
//		size_t c = 0;
//		for (const auto &ed : ret.e) {
//			std::cout << "\t" << c++ << "\t[";
//			if (ed.va == nothing)
//				std::cout << " -";
//			else
//				std::cout << " " << ed.va;
//			std::cout << ",";
//			if (ed.vb == nothing)
//				std::cout << " -";
//			else
//				std::cout << " " << ed.vb;
//			std::cout << "]";
//			if (ed.trianglecount == 0)
//				std::cout << " = line";
//			else if (ed.trianglecount == 1)
//				std::cout << " = half-edge";
//			else if (ed.trianglecount == 2)
//				std::cout << " = edge";
//			else
//				std::cout << " = ? (" << ed.trianglecount << ")";
//			std::cout << '\n';
//		}
//	}
//	{
//		std::cout << "Arcs:\n";
//		for (const auto &a : T) {
//			std::cout << "\teidx = " << a.eidx;
//			std::cout << " n = {" << a.he.n.x << ", " << a.he.n.y << "}";
//
//			std::cout << '\n';
//		}
//	}
//
//	{
//		MatlabFile mf("/tmp/voronoi.mat");
//		Matrix m_hy("hy", 1);
//		Matrix m_points("points", Matrix::Order::TWO_REVERSED);
//		Matrix m_lines("lines", Matrix::Order::TWO_REVERSED);
//		Matrix m_voronoi("voronoi", Matrix::Order::TWO_REVERSED);
//		Matrix m_arcs("arcs", Matrix::Order::TWO_REVERSED);
//		Matrix m_events("events", Matrix::Order::TWO_REVERSED);
//		m_hy.Insert(hy);
//
//		for (const auto &a : T) {
//
//			const double c2 = 1.0 / (2.0 * (a.y - hy));
//			const double c1 = (-2.0 * a.x) * c2;
//			const double c0 = (a.x * a.x + a.y * a.y - hy * hy) * c2;
//
//			m_arcs.push_back(a.x);
//			m_arcs.push_back(a.y);
//			m_arcs.push_back(a.x1);
//			m_arcs.push_back(c2);
//			m_arcs.push_back(c1);
//			m_arcs.push_back(c0);
//		}
//		m_arcs.Reshape(-1, 6);
//
//		for (const auto &q : Qe) {
//			m_events.push_back(q.x);
//			m_events.push_back(q.y);
//		}
//		m_events.Reshape(-1, 2);
//		for (auto it = Q.v.begin(); it != Qs; it++) {
//			m_points.push_back(it->x);
//			m_points.push_back(it->y);
//		}
//		m_points.Reshape(-1, 2);
//		for (const auto &ed : ret.e) {
//			const size_t idxa = ed.va;
//			const size_t idxb = ed.vb;
//			if (idxa == nothing || idxb == nothing)
//				continue;
//			const Vertex &va = ret.v[idxa];
//			const Vertex &vb = ret.v[idxb];
//			m_lines.push_back(va.x);
//			m_lines.push_back(va.y);
//			m_lines.push_back(vb.x);
//			m_lines.push_back(vb.y);
//		}
//		m_lines.Reshape(-1, 4);
//		for (const auto &vert : ret.v) {
//			m_voronoi.push_back(vert.x);
//			m_voronoi.push_back(vert.y);
//		}
//		m_voronoi.Reshape(-1, 2);
//
//		mf.WriteMatrix(m_hy);
//		m_points.ReorderDimensions();
//		mf.WriteMatrix(m_points);
//		m_lines.ReorderDimensions();
//		mf.WriteMatrix(m_lines);
//		m_voronoi.ReorderDimensions();
//		mf.WriteMatrix(m_voronoi);
//		m_arcs.ReorderDimensions();
//		mf.WriteMatrix(m_arcs);
//		m_events.ReorderDimensions();
//		mf.WriteMatrix(m_events);
//	}
#endif

	ret.verticesHaveColor = true;
	ret.paintVertices = true;
	ret.paintNormals = true;
	ret.dotSize = 2;
	return ret;
}

void Polygon3::Regularize() {

	for (size_t n = 0; n < 10; n++) {

		for (size_t eidx = 0; eidx < e.size(); eidx++) {
			Edge &em = e[eidx];
			if (em.trianglecount != 2)
				continue;
			Triangle &ta = t[em.ta];
			Triangle &tb = t[em.tb];

			int_fast8_t idxeta = ta.GetEdgePosition(eidx);
			int_fast8_t idxetb = tb.GetEdgePosition(eidx);
			if (idxeta < 0 || idxetb < 0)
				continue;

			size_t idxv0 = ta.GetVertexIndex((idxeta + 1) % 3);
			size_t idxv1 = ta.GetVertexIndex((idxeta + 2) % 3);
			size_t idxv2 = tb.GetVertexIndex((idxetb + 1) % 3);
			size_t idxv3 = tb.GetVertexIndex((idxetb + 2) % 3);
			if (idxv0 == idxv2)
				throw std::runtime_error("One of the triangles is flipped.");

			size_t idxe0 = ta.GetEdgeIndex((idxeta + 1) % 3);
			size_t idxe1 = ta.GetEdgeIndex((idxeta + 2) % 3);
			size_t idxe2 = tb.GetEdgeIndex((idxetb + 1) % 3);
			size_t idxe3 = tb.GetEdgeIndex((idxetb + 2) % 3);

			const Vertex &v0 = v[idxv0];
			const Vertex &v1 = v[idxv1];
			const Vertex &v2 = v[idxv2];
			const Vertex &v3 = v[idxv3];

			const Vector3 dv0 = v1 - v0;
			const Vector3 dv1 = v2 - v1;
			const Vector3 dv2 = v3 - v2;
			const Vector3 dv3 = v0 - v3;

			auto negativeAngle = [](const Vector3 &lhs, const Vector3 &rhs) {
				const double h = lhs.x * (lhs.y + rhs.y)
						- lhs.y * (lhs.x + rhs.x);
				return (bool) (h < 0.0);
			};
			const bool co0 = negativeAngle(dv0, dv1);
			const bool co1 = negativeAngle(dv1, dv2);
			const bool co2 = negativeAngle(dv2, dv3);
			const bool co3 = negativeAngle(dv3, dv0);
			if (co0 || co1 || co2 || co3)
				continue;

			// Fricas code:
			// )clear all
			// )set fortran optlevel 2
			// )set output fortran on
			// determinant(matrix([[v0x,v0y,v0x^2+v0y^2,1],[v1x,v1y,v1x^2+v1y^2,1],[v2x,v2y,v2x^2+v2y^2,1],[v3x,v3y,v3x^2+v3y^2,1]]))

			const double T2 = v1.x - v0.x;
			const double T3 = v1.y * v1.y;
			const double T4 = v1.x * v1.x;
			const double T5 = v0.y * v0.y;
			const double T6 = v0.x * v0.x;
			const double T7 = (-v1.x + v0.x) * v2.y + (v1.y - v0.y) * v2.x
					- v0.x * v1.y + v0.y * v1.x;
			const double T8 = v2.y * v2.y;
			const double T9 = -v1.y + v0.y;
			const double T10 = v2.x * v2.x;
			const double T11 = v0.x * v1.y;
			const double T12 = v0.y * v1.x;
			const double T13 = v0.x * T3;
			const double T14 = v0.x * T4;
			const double T15 = T5 + T6;
			const double T16 = T11 - T12;
			const double T17 = v0.y * T3;
			const double T18 = -T5 - T6;
			const double T19 = v0.y * T4;
			const double det = T7 * v3.y * v3.y
					+ (T2 * T8 + T2 * T10 + (-T3 - T4 + T5 + T6) * v2.x + T13
							+ T14 + T18 * v1.x) * v3.y + T7 * v3.x * v3.x
					+ (T9 * T8 + (T3 + T4 - T5 - T6) * v2.y + T9 * T10 - T17
							+ T15 * v1.y - T19) * v3.x + T16 * T8
					+ (-T13 - T14 + T15 * v1.x) * v2.y + T16 * T10
					+ (T17 + T18 * v1.y + T19) * v2.x;

			if (det < 0.0)
				continue;

//			Matrix n("n", { 6, 4 });
//			n.Insert(ta.GetVertexIndex(0));
//			n.Insert(ta.GetVertexIndex(1));
//			n.Insert(ta.GetVertexIndex(2));
//			n.Insert(tb.GetVertexIndex(0));
//			n.Insert(tb.GetVertexIndex(1));
//			n.Insert(tb.GetVertexIndex(2));
//			n.Insert(ta.GetEdgeIndex(0));
//			n.Insert(ta.GetEdgeIndex(1));
//			n.Insert(ta.GetEdgeIndex(2));
//			n.Insert(tb.GetEdgeIndex(0));
//			n.Insert(tb.GetEdgeIndex(1));
//			n.Insert(tb.GetEdgeIndex(2));

			// Flip center edge

			Edge &e0 = e[idxe0];
			Edge &e1 = e[idxe1];
			Edge &e2 = e[idxe2];
			Edge &e3 = e[idxe3];

			const size_t idxt0 = e0.GetOtherTriangle(em.ta);
			const size_t idxt1 = e1.GetOtherTriangle(em.ta);
			const size_t idxt2 = e2.GetOtherTriangle(em.tb);
			const size_t idxt3 = e3.GetOtherTriangle(em.tb);

			em.va = idxv1;
			em.vb = idxv3;

			ta.flip = false;
			ta.va = idxv1;
			ta.vb = idxv2;
			ta.vc = idxv3;
			ta.ea = idxe1;
			ta.eb = idxe2;
			ta.ec = eidx;

			tb.flip = false;
			tb.va = idxv3;
			tb.vb = idxv0;
			tb.vc = idxv1;
			tb.ea = idxe3;
			tb.eb = idxe0;
			tb.ec = eidx;

			e0.ta = em.tb;
			e0.tb = idxt0;

			e2.ta = em.ta;
			e2.tb = idxt2;

//			n.Insert(ta.GetVertexIndex(0));
//			n.Insert(ta.GetVertexIndex(1));
//			n.Insert(ta.GetVertexIndex(2));
//			n.Insert(tb.GetVertexIndex(0));
//			n.Insert(tb.GetVertexIndex(1));
//			n.Insert(tb.GetVertexIndex(2));
//			n.Insert(ta.GetEdgeIndex(0));
//			n.Insert(ta.GetEdgeIndex(1));
//			n.Insert(ta.GetEdgeIndex(2));
//			n.Insert(tb.GetEdgeIndex(0));
//			n.Insert(tb.GetEdgeIndex(1));
//			n.Insert(tb.GetEdgeIndex(2));
//
//			{
//				MatlabFile mf("/tmp/triangles.mat");
//				Matrix p("p", { 4, 3 }, Matrix::Order::TWO_REVERSED);
//				p.Insert(v0.x);
//				p.Insert(v0.y);
//				p.Insert(v0.z);
//				p.Insert(v1.x);
//				p.Insert(v1.y);
//				p.Insert(v1.z);
//				p.Insert(v2.x);
//				p.Insert(v2.y);
//				p.Insert(v2.z);
//				p.Insert(v3.x);
//				p.Insert(v3.y);
//				p.Insert(v3.z);
//				p.ReorderDimensions();
//
//				mf.WriteMatrix(p);
//				mf.WriteMatrix(n);
//			}

//			std::cout << 'x';
//			return;
		}
	}
//	std::cout << "\n";
	Sort();
}

#define SELFCHECK_GREATER_THAN(idx, type, var, rhs) if(var > rhs){ \
	std::cerr << "The " << type << " " << idx << " has in " << #var \
	<< " a value greater than " << rhs << ". (" << #var << " = " << var \
	<< ".\n"; errorCount++; passed = false;}

bool Polygon3::PassedPlaneCheck(size_t maxErrorsPerType) const {
	// All checks are done on the data in the objects (vertex, edge, triangle).
	// None of the methods of the objects are called.

	const size_t vcount = v.size();
	const size_t ecount = e.size();
	const size_t tcount = t.size();

	size_t errorCount = 0;
	bool passed = true;

	for (size_t eidx = 0; eidx < ecount; eidx++) {
		const Edge &edge = e[eidx];
		SELFCHECK_GREATER_THAN(eidx, "edge", edge.trianglecount, 2);

		if (errorCount >= maxErrorsPerType) {
			std::cerr << "...\n";
			break;
		}
	}

	// Check for duplicated edges
	// (without sorting first)

	for (size_t idx0 = 0; idx0 < ecount; idx0++) {
		const Edge &e0 = e[idx0];
		for (size_t idx1 = idx0 + 1; idx1 < ecount; idx1++) {
			const Edge &e1 = e[idx1];

			if (e0.va == e1.va && e0.vb == e1.vb) {
				std::cerr << "The edge " << idx0
						<< " contains the same vertices (" << e0.va << " and "
						<< e0.vb << ") as the edge " << idx1 << ".";
				errorCount++;
				passed = false;
			}
			if (e0.va == e1.vb && e0.vb == e1.va) {
				std::cerr << "The edge " << idx0
						<< " contains the same vertices (" << e0.va << " and "
						<< e0.vb << ") as the edge " << idx1 << " but flipped.";
				errorCount++;
				passed = false;
			}
			if (errorCount >= maxErrorsPerType) {
				std::cerr << "...\n";
				break;
			}
		}
	}

	return passed;
}

///////////////////////////////////////////////////////////////////////////////
// Name               : Geometry.cpp
// Purpose            : Store and manipulate 3D objects
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   : -std=c++14 or greater
// Author             : Tobias Schaefer
// Created            : 22.09.2023
// Copyright          : (C) 2023 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "Geometry.h"

#include "Polygon3.h"

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <iterator>
#include <numeric>
#include <regex>
#include <utility>
#include <string>

#include "OpenGL.h"

using std::swap;

#ifdef NDEBUG
#define RANGE_CHECK(ary,idx)
#else
#define RANGE_CHECK(ary,idx) if(idx>=ary.size()) \
	throw std::range_error(std::string(__FILE__":") + \
			std::to_string(__LINE__) + std::string(": ") + \
			std::string("The index "#idx" is larger than the size of the array "#ary"."))
#endif

static const size_t nothing = (size_t) -1;

Geometry::Vertex::Vertex(double x_, double y_, double z_, double u_,
		double v_) {
	this->x = x_;
	this->y = y_;
	this->z = z_;
	this->u = u_;
	this->v = v_;

}

Geometry::Vertex::Vertex(const Vector3 &vector, const Vector3 &normal) :
		n(normal) {
	x = vector.x;
	y = vector.y;
	z = vector.z;
}

void Geometry::Vertex::Transform(const AffineTransformMatrix &m) {
	n = m.TransformWithoutShift(n);
	Vector3 temp = m.Transform(x, y, z);
	x = temp.x;
	y = temp.y;
	z = temp.z;
}

void Geometry::Vertex::Transform(const AffineTransformMatrix &m,
		const AffineTransformMatrix &mn) {
	n = mn.Transform(n);
	Vector3 temp = m.Transform(x, y, z);
	x = temp.x;
	y = temp.y;
	z = temp.z;
}

void Geometry::Vertex::FlipNormal() {
	n.x = -n.x;
	n.y = -n.y;
	n.z = -n.z;
}

Geometry::Vertex Geometry::Vertex::Interp(const Vertex &b,
		const double mix) const {
	Vertex temp;
	temp.x = x + (b.x - x) * mix;
	temp.y = y + (b.y - y) * mix;
	temp.z = z + (b.z - z) * mix;

	temp.c.r = c.r + (b.c.r - c.r) * mix;
	temp.c.g = c.g + (b.c.g - c.g) * mix;
	temp.c.b = c.b + (b.c.b - c.b) * mix;
	temp.c.a = c.a + (b.c.a - c.a) * mix;

	temp.u = u + (b.u - u) * mix;
	temp.v = v + (b.v - v) * mix;

	return temp;
}

void Geometry::Edge::Fix() {
	if (ta == nothing) {
		if (tb == nothing) {
			trianglecount = 0;
		} else {
			trianglecount = 1;
			swap(ta, tb);
		}
	} else {
		if (tb == nothing) {
			trianglecount = 1;
		} else {
			// If there are more than 2 triangles joined at the edge, the
			// number of triangles is preserved.
			trianglecount = (trianglecount < 2) ? 2 : trianglecount;
			if (ta > tb)
				swap(ta, tb);
		}
	}
	if (va > vb) {
		swap(va, vb);
		flip = !flip;
	}
}

void Geometry::Edge::FlipNormal() {
	n.x = -n.x;
	n.y = -n.y;
	n.z = -n.z;
}

size_t Geometry::Edge::GetOtherVertex(size_t index) const {
	if (va == index)
		return vb;
	return va;
}

size_t Geometry::Edge::GetOtherTriangle(size_t index) const {
	if (ta == index)
		return tb;
	return ta;
}

size_t Geometry::Edge::GetVertexIndex(uint_fast8_t index) const {
	if (flip) {
		switch (index) {
		case 0:
			return vb;
		case 1:
			return va;
		default:
			throw std::range_error(
					"Geometry::Edge::VertexIndex - Index out of range");
		}
	} else {
		switch (index) {
		case 0:
			return va;
		case 1:
			return vb;
		default:
			throw std::range_error(
					"Geometry::Edge::VertexIndex - Index out of range");
		}
	}
}

bool Geometry::Edge::IsCollapsed() const {
	return va == vb;
}

void Geometry::Triangle::Fix() {
	// Note, that the edges opposed to the switched vertices are changed.
	// Rotation is not affected.
	// a < b < c
	if (va <= vb && vb <= vc) {
		// Nothing to do, just return.
		return;
	}
	// a < c < b
	if (va < vc && vc < vb) {
		// Swap b and c
		swap(vb, vc);
		swap(ea, ec);
		flip = !flip;
		return;
	}
	// b < c < a
	if (vb < vc && vc < va) {
		// Rotate right
		size_t temp = va;
		va = vb;
		vb = vc;
		vc = temp;
		temp = ea;
		ea = eb;
		eb = ec;
		ec = temp;
		return;
	}
	// b < a < c
	if (vb < va && va < vc) {
		// Swap a and b
		swap(va, vb);
		swap(ec, eb);
		flip = !flip;
		return;
	}
	// c < a < b
	if (vc < va && va < vb) {
		// Rotate left
		size_t temp = vc;
		vc = vb;
		vb = va;
		va = temp;
		temp = ec;
		ec = eb;
		eb = ea;
		ea = temp;
		return;
	}
	// c < b < a
	if (vc < vb && vb < va) {
		// Swap a and c
		swap(va, vc);
		swap(ea, eb);
		flip = !flip;
		return;
	}
}

void Geometry::Triangle::FlipNormal() {
	n.x = -n.x;
	n.y = -n.y;
	n.z = -n.z;
}

int Geometry::Triangle::GetDirection(size_t idx0, size_t idx1) const {
	if ((idx0 == va && idx1 == vb) || (idx0 == vb && idx1 == vc)
			|| (idx0 == vc && idx1 == va))
		return (flip ? -1 : 1);
	if ((idx0 == vb && idx1 == va) || (idx0 == vc && idx1 == vb)
			|| (idx0 == va && idx1 == vc))
		return (flip ? 1 : -1);
	return 0;
}

size_t Geometry::Triangle::GetVertexIndex(uint_fast8_t index) const {
	if (flip) {
		switch (index) {
		case 0:
			return va;
		case 1:
			return vc;
		case 2:
			return vb;
		default:
			throw std::range_error(
					"Geometry::Triangle::VertexIndex - Index out of range");
		}
	} else {
		switch (index) {
		case 0:
			return va;
		case 1:
			return vb;
		case 2:
			return vc;
		default:
			throw std::range_error(
					"Geometry::Triangle::VertexIndex - Index out of range");
		}
	}
}

size_t Geometry::Triangle::GetEdgeIndex(uint_fast8_t index) const {
	if (flip) {
		switch (index) {
		case 0:
			return ec;
		case 1:
			return eb;
		case 2:
			return ea;
		default:
			throw std::range_error(
					"Geometry::Triangle::EdgeIndex - Index out of range");
		}
	} else {
		switch (index) {
		case 0:
			return ea;
		case 1:
			return eb;
		case 2:
			return ec;
		default:
			throw std::range_error(
					"Geometry::Triangle::EdgeIndex - Index out of range");
		}
	}
}

uint_fast8_t Geometry::Triangle::GetVertexPosition(size_t idx) const {
	if (idx == va)
		return 0;
	if (idx == vb)
		return 1;
	if (idx == vc)
		return 2;
	return (uint_fast8_t) -1;
}

uint_fast8_t Geometry::Triangle::GetEdgePosition(size_t idx) const {
	if (idx == ea)
		return 0;
	if (idx == eb)
		return 1;
	if (idx == ec)
		return 2;
	return (uint_fast8_t) -1;
}

bool Geometry::Triangle::IsCollapsed() const {
	return va == vb || vb == vc || vc == va;
}

Geometry::~Geometry() {
	DeleteGLVertexArray();
}

void Geometry::SetEpsilon(double newEpsilon) {
	epsilon = newEpsilon;
}

void Geometry::CopyPropertiesFrom(const Geometry &other) {
	epsilon = other.epsilon;
	this->verticesHaveNormal = other.verticesHaveNormal;
	this->verticesHaveColor = other.verticesHaveColor;
	this->edgesHaveNormal = other.edgesHaveNormal;
	this->edgesHaveColor = other.edgesHaveColor;
	this->trianglesHaveNormal = other.trianglesHaveNormal;
	this->trianglesHaveColor = other.trianglesHaveColor;
	this->finished = other.finished;
	this->matrix = other.matrix;
	this->smooth = other.smooth;
	this->paintEdges = other.paintEdges;
	this->paintTriangles = other.paintTriangles;
	this->paintVertices = other.paintVertices;
	this->paintNormals = other.paintNormals;
	this->paintDirection = other.paintDirection;
	this->paintSelected = other.paintSelected;
	this->dotSize = other.dotSize;
}

void Geometry::Clear() {
	v.clear();
	e.clear();
	t.clear();
	vmap.clear();
	emap.clear();
	tmap.clear();
	gmap.clear();
	openvertices.clear();
	openedges.clear();
	selected.clear();
	ResetPresets();
	matrix.SetIdentity();
	verticesHaveNormal = false;
	verticesHaveColor = false;
	edgesHaveNormal = false;
	edgesHaveColor = false;
	trianglesHaveNormal = false;
	trianglesHaveColor = false;
	finished = false;
}

bool Geometry::IsEmpty() const {
	return (v.size() == 0);
}

bool Geometry::IsClosed() const {
	return (openedges.empty() && openvertices.empty());
}

bool Geometry::IsFinished() const {
	return finished;
}

void Geometry::SetAddNormal(const Vector3 &n) {
	addNormals = (fabs(n.x) > FLT_EPSILON || fabs(n.y) > FLT_EPSILON
			|| fabs(n.z) > FLT_EPSILON);
	addNormal.x = n.x;
	addNormal.y = n.y;
	addNormal.z = n.z;
}

void Geometry::SetAddNormal(double nx, double ny, double nz) {
	addNormals = (fabs(nx) > FLT_EPSILON || fabs(ny) > FLT_EPSILON
			|| fabs(nz) > FLT_EPSILON);
	addNormal.x = nx;
	addNormal.y = ny;
	addNormal.z = nz;
}

void Geometry::ResetAddNormal() {
	addNormals = false;
}

void Geometry::SetAddColor(const Vector3 &c) {
	addColors = true;
	addColor.r = c.x;
	addColor.g = c.y;
	addColor.b = c.z;
	addColor.a = 1.0;
}

void Geometry::SetAddColor(double r, double g, double b, double a) {
	addColors = true;
	addColor.r = r;
	addColor.g = g;
	addColor.b = b;
	addColor.a = a;
}

void Geometry::ResetAddColor() {
	addColors = false;
}

void Geometry::SetAddMatrix(const AffineTransformMatrix &m) {
	addMatrix = m;
	addNormalMatrix = m.GetNormalMatrix();
	useAddMatrix = true;
}

void Geometry::ResetAddMatrix() {
	addMatrix.SetIdentity();
	addNormalMatrix.SetIdentity();
	useAddMatrix = false;
}

void Geometry::ResetPresets() {
	ResetAddNormal();
	ResetAddColor();
	ResetAddMatrix();
}

void Geometry::AddVertex(const Geometry::Vertex &vertex, size_t sourceIndex) {
	verticesHaveNormal |= addNormals;
	verticesHaveColor |= addColors;
	if (sourceIndex != nothing) {
		if (vmap.size() <= sourceIndex)
			vmap.resize(sourceIndex + 1, nothing);
		vmap[sourceIndex] = v.size();
	}
	v.push_back(vertex);
	if (addNormals)
		v.back().n = addNormal;
	if (addColors)
		v.back().c = addColor;

}

//void Geometry::AddVertex(const Vector3 &vertex) {
//	Vector3 temp = addMatrix(vertex);
//	Vertex temp2;
//	temp2.x = temp.x;
//	temp2.y = temp.y;
//	temp2.z = temp.z;
//	AddVertex(temp2);
//}

void Geometry::AddVertex(const std::vector<Vector3> &vertices) {
	for (const Vector3 &vertex : vertices)
		AddVertex(vertex);
}

void Geometry::AddEdge(const Geometry::Edge &edge, size_t sourceIndex) {
	edgesHaveNormal |= addNormals;
	edgesHaveColor |= addColors;
	if (sourceIndex != nothing) {
		if (emap.size() <= sourceIndex)
			emap.resize(sourceIndex + 1, nothing);
		emap[sourceIndex] = e.size();
	}
	e.push_back(edge);
	if (addNormals)
		e.back().n = addNormal;
	if (addColors)
		e.back().c = addColor;
}

void Geometry::AddEdge(const Geometry::Vertex &v0, const Geometry::Vertex &v1) {
	AddVertex(v0);
	size_t idx0 = v.size() - 1;
	AddVertex(v1);
	size_t idx1 = v.size() - 1;
	AddEdge(idx0, idx1);
}

//void Geometry::AddEdge(const Vector3 &v0, const Vector3 &v1) {
//	AddVertex(v0);
//	size_t idx0 = v.size() - 1;
//	AddVertex(v1);
//	size_t idx1 = v.size() - 1;
//	AddEdge(idx0, idx1);
//}

void Geometry::AddEdge(size_t idx0, size_t idx1) {
	edgesHaveNormal |= addNormals;
	edgesHaveColor |= addColors;
	Geometry::Edge edge;
	edge.va = idx0;
	edge.vb = idx1;
	if (addNormals)
		edge.n = addNormal;
	if (addColors)
		edge.c = addColor;
	e.push_back(edge);
}

void Geometry::AddTriangle(const Geometry::Triangle &triangle,
		size_t sourceIndex) {
	trianglesHaveNormal |= addNormals;
	trianglesHaveColor |= addColors;
	if (sourceIndex != nothing) {
		if (tmap.size() <= sourceIndex)
			tmap.resize(sourceIndex + 1, nothing);
		tmap[sourceIndex] = t.size();
	}
	t.push_back(triangle);
	if (addNormals)
		t.back().n = addNormal;
	if (addColors)
		t.back().c = addColor;
}

void Geometry::AddTriangle(const Geometry::Vertex &v0,
		const Geometry::Vertex &v1, const Geometry::Vertex &v2) {
	AddVertex(v0);
	size_t idx0 = v.size() - 1;
	AddVertex(v1);
	size_t idx1 = v.size() - 1;
	AddVertex(v2);
	size_t idx2 = v.size() - 1;
	AddTriangle(idx0, idx1, idx2);
}

//void Geometry::AddTriangle(const Vector3 &v0, const Vector3 &v1,
//		const Vector3 &v2) {
//	AddVertex(v0);
//	size_t idx0 = v.size() - 1;
//	AddVertex(v1);
//	size_t idx1 = v.size() - 1;
//	AddVertex(v2);
//	size_t idx2 = v.size() - 1;
//	AddTriangle(idx0, idx1, idx2);
//}

void Geometry::AddTriangle(size_t idx0, size_t idx1, size_t idx2) {
	trianglesHaveNormal |= addNormals;
	trianglesHaveColor |= addColors;
	Vector3 n;
	if (addNormals) {
		n = addNormal;
	} else {
		Vector3 a = v[idx1] - v[idx0];
		Vector3 b = v[idx2] - v[idx1];
		n = a * b;
		double L = n.Abs();
		if (fabs(L) > FLT_EPSILON)
			n /= L;
	}
	Geometry::Triangle tri;
	tri.va = idx0;
	tri.vb = idx1;
	tri.vc = idx2;
	tri.n = n;
	tri.c = addColor;

	const size_t tidx = t.size();

	Geometry::Edge edge0;
	edge0.va = idx0;
	edge0.vb = idx1;
	edge0.ta = tidx;
	edge0.n = n;
	edge0.c = addColor;
	edge0.trianglecount = 1;
	tri.ea = e.size();
	e.push_back(edge0);

	Geometry::Edge edge1;
	edge1.va = idx1;
	edge1.vb = idx2;
	edge1.ta = tidx;
	edge1.n = n;
	edge1.c = addColor;
	edge1.trianglecount = 1;
	tri.eb = e.size();
	e.push_back(edge1);

	Geometry::Edge edge2;
	edge2.va = idx2;
	edge2.vb = idx0;
	edge2.ta = tidx;
	edge2.n = n;
	edge2.c = addColor;
	edge2.trianglecount = 1;
	tri.ec = e.size();
	e.push_back(edge2);

	t.push_back(tri);

	finished = false;
}

void Geometry::AddQuad(const Vector3 &va, const Vector3 &vb, const Vector3 &vc,
		const Vector3 &vd) {
	AddVertex(va);
	size_t idx0 = v.size() - 1;
	AddVertex(vb);
	size_t idx1 = v.size() - 1;
	AddVertex(vc);
	size_t idx2 = v.size() - 1;
	AddVertex(vd);
	size_t idx3 = v.size() - 1;
	AddQuad(idx0, idx1, idx2, idx3);
}

void Geometry::AddQuad(size_t idx0, size_t idx1, size_t idx2, size_t idx3) {
	AddTriangle(idx0, idx1, idx2);
	t.back().tua = 0.0;
	t.back().tva = 0.0;
	t.back().tub = 1.0;
	t.back().tvb = 0.0;
	t.back().tuc = 1.0;
	t.back().tvc = 1.0;
	AddTriangle(idx0, idx2, idx3);
	t.back().tua = 0.0;
	t.back().tva = 0.0;
	t.back().tub = 1.0;
	t.back().tvb = 1.0;
	t.back().tuc = 0.0;
	t.back().tvc = 1.0;
	trianglesHaveTexture = true;
	finished = false;
}

void Geometry::AddFrom(const Geometry &other) {
	vmap.clear();
	emap.clear();
	tmap.clear();
	vmap.reserve(other.v.size());
	emap.reserve(other.e.size());
	tmap.reserve(other.t.size());

	const size_t vfirst = v.size();
	const size_t efirst = e.size();
	const size_t tfirst = t.size();

	for (const Vertex &vec : other.v) {
		vmap.push_back(v.size());
		v.push_back(vec);
	}
	for (const Edge &ed : other.e) {
		emap.push_back(e.size());
		e.push_back(ed);
	}
	for (const Triangle &tri : other.t) {
		tmap.push_back(t.size());
		t.push_back(tri);
	}

	// Update the references
	Remap(vfirst, efirst, tfirst);
}

void Geometry::AddSelectedFrom(const Geometry &other) {
	vmap.assign(other.v.size(), nothing);
	emap.assign(other.e.size(), nothing);
	tmap.assign(other.t.size(), nothing);

	const size_t vfirst = v.size();
	const size_t efirst = e.size();
	const size_t tfirst = t.size();

	for (const size_t n : other.selected) {
		vmap[n] = v.size();
		v.push_back(other.v[n]);
	}
	for (size_t n = 0; n < other.e.size(); ++n) {
		if (vmap[other.e[n].va] == nothing)
			continue;
		if (vmap[other.e[n].vb] == nothing)
			continue;
		emap[n] = e.size();
		e.push_back(other.e[n]);
	}
	for (size_t n = 0; n < other.t.size(); ++n) {
		if (vmap[other.t[n].va] == nothing)
			continue;
		if (vmap[other.t[n].vb] == nothing)
			continue;
		if (vmap[other.t[n].vc] == nothing)
			continue;
		tmap[n] = t.size();
		t.push_back(other.t[n]);
	}

	// Update the references
	Remap(vfirst, efirst, tfirst);

	// Cleanup references
	for (Edge &ed : e) {
		if (ed.ta == nothing && ed.tb != nothing) {
			ed.ta = ed.tb;
			ed.tb = nothing;
		}
	}
}

void Geometry::InitMap() {
	vmap.resize(v.size());
	emap.resize(e.size());
	tmap.resize(t.size());
	gmap.clear();
	std::iota(vmap.begin(), vmap.end(), 0);
	std::iota(emap.begin(), emap.end(), 0);
	std::iota(tmap.begin(), tmap.end(), 0);
}

void Geometry::FlipMap() {
	std::vector<size_t> temp;
	if (!vmap.empty()) {
		temp.resize(vmap.size());
		for (size_t n = 0; n < vmap.size(); ++n)
			temp[vmap[n]] = n;
		vmap.swap(temp);
	}
	if (!emap.empty()) {
		temp.resize(emap.size());
		for (size_t n = 0; n < emap.size(); ++n)
			temp[emap[n]] = n;
		emap.swap(temp);
	}
	if (!tmap.empty()) {
		temp.resize(tmap.size());
		for (size_t n = 0; n < tmap.size(); ++n)
			temp[tmap[n]] = n;
		tmap.swap(temp);
	}
}

void Geometry::Remap(int vstart, int estart, int tstart) {
	if (!vmap.empty()) {
		for (std::vector<Edge>::iterator ed = e.begin() + estart; ed != e.end();
				++ed) {
			RANGE_CHECK(vmap, ed->va);
			RANGE_CHECK(vmap, ed->vb);
			ed->va = vmap[ed->va];
			ed->vb = vmap[ed->vb];
		}
		for (std::vector<Triangle>::iterator tri = t.begin() + tstart;
				tri != t.end(); ++tri) {
			RANGE_CHECK(vmap, tri->va);
			RANGE_CHECK(vmap, tri->vb);
			RANGE_CHECK(vmap, tri->vc);
			tri->va = vmap[tri->va];
			tri->vb = vmap[tri->vb];
			tri->vc = vmap[tri->vc];
		}
		vmap.clear();
	}
	if (!emap.empty()) {
		for (std::vector<Triangle>::iterator tri = t.begin() + tstart;
				tri != t.end(); ++tri) {
			RANGE_CHECK(emap, tri->ea);
			RANGE_CHECK(emap, tri->eb);
			RANGE_CHECK(emap, tri->ec);
			tri->ea = emap[tri->ea];
			tri->eb = emap[tri->eb];
			tri->ec = emap[tri->ec];
		}
		emap.clear();
	}
	if (!tmap.empty()) {
		for (std::vector<Edge>::iterator ed = e.begin() + estart; ed != e.end();
				++ed) {
			if (ed->ta != nothing) {
				RANGE_CHECK(tmap, ed->ta);
				ed->ta = tmap[ed->ta];
			}
			if (ed->tb != nothing) {
				RANGE_CHECK(tmap, ed->tb);
				ed->tb = tmap[ed->tb];
			}
			uint_least8_t c = 0;
			c += (ed->ta != nothing) ? 1 : 0;
			c += (ed->tb != nothing) ? 1 : 0;
			ed->trianglecount = c;
		}
		tmap.clear();
	}
	if (!gmap.empty()) {

		for (std::vector<Vertex>::iterator vec = v.begin() + vstart;
				vec != v.end(); ++vec) {
			vec->group = gmap[vec->group];
		}
		for (std::vector<Edge>::iterator ed = e.begin() + estart; ed != e.end();
				++ed) {
			ed->group = gmap[ed->group];
		}
		for (std::vector<Triangle>::iterator tri = t.begin() + tstart;
				tri != t.end(); ++tri) {
			tri->group = gmap[tri->group];
		}
		gmap.clear();
	}
}

void Geometry::Fix() {
	for (Edge &ed : e)
		ed.Fix();
	for (Triangle &tri : t)
		tri.Fix();
}

void Geometry::Sort() {
	// Note, that the lambdas below are different to the lambdas in Join().
	auto vertex_less = [eps=epsilon, &vref=v](const size_t &idxa,
			const size_t &idxb) {
		const auto &a = vref[idxa];
		const auto &b = vref[idxb];
		if (a.x < (b.x - eps))
			return true;
		if (a.x >= (b.x + eps))
			return false;
		if (a.y < (b.y - eps))
			return true;
		if (a.y >= (b.y + eps))
			return false;
		if (a.z < b.z)
			return true;
		return false;
	};

	vmap.resize(v.size());
	std::iota(vmap.begin(), vmap.end(), 0);
	std::sort(vmap.begin(), vmap.end(), vertex_less);
	{
		std::vector<Vertex> temp;
		temp.reserve(v.size());
		for (size_t idx : vmap)
			temp.push_back(v[idx]);
		v.swap(temp);
	}
	FlipMap();
	Remap(0, 0, 0);
	for (auto &ed : e)
		ed.Fix();

	auto edge_less = [&eref = e](const size_t &idxa, const size_t &idxb) {
		const auto &a = eref[idxa];
		const auto &b = eref[idxb];
		if (a.va < b.va)
			return true;
		if (a.va > b.va)
			return false;
		if (a.vb < b.vb)
			return true;
		return false;
	};

	emap.resize(e.size());
	std::iota(emap.begin(), emap.end(), 0);
	std::sort(emap.begin(), emap.end(), edge_less);
	{
		std::vector<Edge> temp;
		temp.reserve(e.size());
		for (size_t idx : emap)
			temp.push_back(e[idx]);
		e.swap(temp);
	}
	FlipMap();
	Remap(0, 0, 0);
	for (auto &tri : t)
		tri.Fix();

	auto triangle_less = [&tref = t](const size_t &idxa, const size_t &idxb) {
		const auto &a = tref[idxa];
		const auto &b = tref[idxb];
		if (a.va < b.va)
			return true;
		if (a.va > b.va)
			return false;
		if (a.vb < b.vb)
			return true;
		if (a.vb > b.vb)
			return false;
		if (a.vc < b.vc)
			return true;
		return false;
	};

	tmap.resize(t.size());
	std::iota(tmap.begin(), tmap.end(), 0);
	std::sort(tmap.begin(), tmap.end(), triangle_less);
	{
		std::vector<Triangle> temp;
		temp.reserve(t.size());
		for (size_t idx : tmap)
			temp.push_back(t[idx]);
		t.swap(temp);
	}
	FlipMap();
	Remap(0, 0, 0);
}

void Geometry::Join() {
	vmap.clear();
	emap.clear();
	tmap.clear();

	if (v.empty())
		return;

	auto vertex_less = [eps = epsilon](const Vector3 &a, const Vector3 &b) {
		if (a.x < (b.x - eps))
			return true;
		if (a.x >= (b.x + eps))
			return false;
		if (a.y < (b.y - eps))
			return true;
		if (a.y >= (b.y + eps))
			return false;
		if (a.z < b.z)
			return true;
		return false;
	};

	auto vertex_equal = [eps = epsilon](const Vector3 &a, const Vector3 &b) {
		if (a.x < (b.x - eps))
			return false;
		if (a.x > (b.x + eps))
			return false;
		if (a.y < (b.y - eps))
			return false;
		if (a.y > (b.y + eps))
			return false;
		if (a.z < (b.z - eps))
			return false;
		if (a.z > (b.z + eps))
			return false;
		return true;
	};

	auto edge_less = [](const Edge &a, const Edge &b) {
		if (a.va < b.va)
			return true;
		if (a.va > b.va)
			return false;
		if (a.vb < b.vb)
			return true;
		return false;
	};

	auto edge_equal = [](const Edge &a, const Edge &b) {
		return (a.va == b.va && a.vb == b.vb);
	};

	auto triangle_less = [](const Triangle &a, const Triangle &b) {
		if (a.va < b.va)
			return true;
		if (a.va > b.va)
			return false;
		if (a.vb < b.vb)
			return true;
		if (a.vb > b.vb)
			return false;
		if (a.vc < b.vc)
			return true;
		return false;
	};

	auto triangle_equal = [](const Triangle &a, const Triangle &b) {
		return (a.va == b.va && a.vb == b.vb && a.vc == b.vc);
	};

	// Enumerate everything
	for (size_t i = 0; i < v.size(); ++i)
		v[i].group = i;
	for (size_t i = 0; i < e.size(); ++i)
		e[i].group = i;
	for (size_t i = 0; i < t.size(); ++i)
		t[i].group = i;

	// Remove duplicated vertices

	if (!v.empty()) {
		vmap.assign(v.size(), nothing);
		std::sort(v.begin(), v.end(), vertex_less);
		size_t j = 0;
		vmap[v[j].group] = j;
		for (size_t i = 1; i < v.size(); ++i) {
			if (vertex_equal(v[j], v[i])) {
				v[j].n += v[i].n;
				vmap[v[i].group] = j;
			} else {
				++j;
				v[j] = v[i];
				vmap[v[j].group] = j;
			}
		}
		v.erase(v.begin() + (int) j + 1, v.end());

		// Normalize normal vectors.
		if (verticesHaveNormal)
			for (size_t i = 0; i < v.size(); ++i)
				v[i].n.Normalize();

		// Map edges and triangles
		Remap(0, 0, 0);
	}

	// Remove duplicate edges

	bool reassignEdges = false;
	if (!e.empty()) {
		// emap has to have the free space before erasing edges
		emap.assign(e.size(), nothing);

		// Remove collapsed edges
		auto edge_collapsed = [](const Edge &ed) {
			return ed.va != nothing && ed.va == ed.vb;
		};
		e.erase(std::remove_if(e.begin(), e.end(), edge_collapsed), e.end());

		for (Edge &ed : e)
			ed.Fix();
		std::sort(e.begin(), e.end(), edge_less);

		std::vector<size_t> ecount;
		ecount.assign(e.size(), 1);
		size_t j = 0;

		emap[e[j].group] = j;
		for (size_t i = 1; i < e.size(); ++i) {
			if (edge_equal(e[j], e[i])) {
				emap[e[i].group] = j;
				if (e[i].trianglecount > 0)
					reassignEdges = true;
				e[j].n += e[i].n;
				e[j].c.r += e[i].c.r;
				e[j].c.g += e[i].c.g;
				e[j].c.b += e[i].c.b;
				e[j].c.a += e[i].c.a;
				ecount[j]++;
			} else {
				++j;
				e[j] = e[i];
				emap[e[j].group] = j;
			}
		}
		e.erase(e.begin() + (int) j + 1, e.end());
		ecount.erase(ecount.begin() + (int) j + 1, ecount.end());

		// Normalize normal vectors
		for (size_t i = 0; i < e.size(); ++i)
			e[i].n /= (double) ecount[i];

		// Map triangles
		Remap(0, 0, 0);
	}

	// Remove duplicated and collapsed triangles

	if (!t.empty()) {
		for (Triangle &tri : t)
			tri.Fix();
		std::sort(t.begin(), t.end(), triangle_less);
		size_t j = 0;
		for (size_t i = 0; i < t.size(); ++i) {
			if (t[i].IsCollapsed()) {
				reassignEdges = true;
				continue;
			}
			if (j > 0 && triangle_equal(t[j - 1], t[i])) {
				reassignEdges = true;
				continue;
			}
			t[j] = t[i];
			j++;
		}
		t.erase(t.begin() + (int) j, t.end());
	}

	if (reassignEdges && !e.empty()) {
		// Assuming that the connection vertex -> edge -> triangle is OK.
		// Zero all edge connections.
		for (Edge &ed : e)
			ed.trianglecount = 0;
		// Add all triangles anew
		for (size_t i = 0; i < t.size(); ++i) {
			const Triangle &tri = t[i];
			if (e[tri.ea].trianglecount == 0)
				e[tri.ea].ta = i;
			if (e[tri.ea].trianglecount == 1)
				e[tri.ea].tb = i;
			e[tri.ea].trianglecount++;
			if (e[tri.eb].trianglecount == 0)
				e[tri.eb].ta = i;
			if (e[tri.eb].trianglecount == 1)
				e[tri.eb].tb = i;
			e[tri.eb].trianglecount++;
			if (e[tri.ec].trianglecount == 0)
				e[tri.ec].ta = i;
			if (e[tri.ec].trianglecount == 1)
				e[tri.ec].tb = i;
			e[tri.ec].trianglecount++;
		}
		for (Edge &ed : e)
			ed.Fix();
	}
}

void Geometry::Finish() {
	PropagateNormals();
	CalculateUVCoordinateSystems();
	Join();
	finished = true;
}

#define SELFCHECK_EQUAL(idxa, idxb, fielda, fieldb, typea, typeb) if(fielda != fieldb){ \
	std::cerr << "The field " << #fielda << " of " << typea << " (" << idxa \
	<< ") and the field " << #fieldb << " of " << typeb << " (" << idxb \
	<< ") should be identical: " << fielda << " == " << fieldb << "\n"; \
	++errorCount; passed = false;}

#define SELFCHECK_NOTEQUAL(idxa, idxb, fielda, fieldb, typea, typeb) if(fielda == fieldb){ \
	std::cerr << "The field " << #fielda << " of " << typea << " (" << idxa \
	<< ") and the field " << #fieldb << " of " << typeb << " (" << idxb \
	<< ") should not be identical: " << fielda << " != " << fieldb << "\n"; \
	++errorCount; passed = false;}

#define SELFCHECK_RANGE(idx, field, max, typea, typeb) if(field >= max){ \
	std::cerr << "The " << typea << " " << idx << " refers in " << #field \
	<< " to the " << typeb << " " << field << " but there are only " << max \
	<< " " << typeb << "s.\n"; ++errorCount; passed = false;}

#define SELFCHECK_RANGE_NG(idx, field, max, typea, typeb) if(field != nothing && field >= max){ \
	std::cerr << "The " << typea << " " << idx << " refers in " << #field \
	<< " to the " << typeb << " " << field << " but there are only " << max \
	<< " " << typeb << "s.\n"; ++errorCount; passed = false;}

#define SELFCHECK_EMPTY(idx, field, typea, typeb) if(field != nothing){ \
	std::cerr << "The " << typea << " " << idx << " has in " << #field \
	<< " a valid " << typeb <<"-index entry. Should be empty.\n"; \
	++errorCount; passed = false;}

#define SELFCHECK_NOT_EMPTY(idx, field, typea, typeb) if(field == nothing){ \
	std::cerr << "The " << typea << " " << idx << " has in " << #field \
	<< " no valid " << typeb <<"-index entry.\n"; ++errorCount; passed = false;}

#define SELFCHECK_LESS_THAN(idx, fielda, fieldb, typea, typeb) if(fielda == fieldb){ \
	std::cerr << "The " << typea << " " << idx << " has in " << #fielda \
	<< " and " << #fieldb << " the same " << typeb <<"-index (" << fielda \
    << ").\n"; ++errorCount; passed = false;}if(fielda > fieldb) { \
    std::cerr << "The " << typea << " " << idx << " has in " << #fielda \
	<< " a greater " << typeb << "-index than in " << #fieldb << ". (" \
	<< fielda << " is not smaller than " << fieldb << ")\n"; ++errorCount; \
	passed = false;}

#define SELFCHECK_EDGE_VERTEX(idx, edge, v0, v1) if(!((e[edge].va == v0 && e[edge].vb == v1) \
	|| (e[edge].va == v1 && e[edge].vb == v0))){std::cerr \
	<< "The triangles (" << idx << ") edge " << #edge << " (" << edge \
	<< ") does not reference the triangles vertices " << #v0 << " (" << v0 \
	<< ") and " << #v1 << " (" << v1 << "). Edge " << edge << " references " \
	<< e[edge].va << " and " << e[edge].vb << ".\n"; \
	++errorCount; passed = false;}

#define SELFCHECK_EDGE_TRIANGLE(idx, edge, idxb) if(idxb != nothing \
		&& t[idxb].ea != edge && t[idxb].eb != edge && t[idxb].ec != edge) \
		{std::cerr << "The triangles (" << idx << ") edge " << #edge << " (" << edge \
	<< ") connects with triangle (" << idxb << "). " \
	<< "This triangle does not  have a reference to the edge. Only to " \
	<< t[idxb].ea << ", " << t[idxb].eb << ", and " << t[idxb].ec << ".\n"; \
	++errorCount; passed = false;}

bool Geometry::PassedSelfCheck(bool checkWellOrdering,
		size_t maxErrorsPerType) const {

	// All checks are done on the data in the objects (vertex, edge, triangle).
	// None of the methods of the objects are called.

	const size_t vcount = v.size();
	const size_t ecount = e.size();
	const size_t tcount = t.size();

	size_t errorCount;
	bool passed = true;
	bool edgeOK = true;
	bool triangleOK = true;

	// Check edges

	errorCount = 0;
	for (size_t idx = 0; idx < ecount; ++idx) {
		// Check if the edges vertex indices and triangle-indices are either
		// empty or less than the number of available vertices / triangles.
		SELFCHECK_RANGE_NG(idx, e[idx].va, vcount, "edge", "vertex");
		SELFCHECK_RANGE_NG(idx, e[idx].vb, vcount, "edge", "vertex");
		SELFCHECK_RANGE_NG(idx, e[idx].ta, tcount, "edge", "triangle");
		SELFCHECK_RANGE_NG(idx, e[idx].tb, tcount, "edge", "triangle");

		// Check that the edge has two valid vertices.
		SELFCHECK_NOT_EMPTY(idx, e[idx].va, "edge", "vertex");
		SELFCHECK_NOT_EMPTY(idx, e[idx].vb, "edge", "vertex");

		// If the trianglecount is 0, ta and tb should be empty.
		if (e[idx].trianglecount == 0) {
			SELFCHECK_EMPTY(idx, e[idx].ta, "edge", "triangle");
			SELFCHECK_EMPTY(idx, e[idx].tb, "edge", "triangle");
		}
		// If the trianglecount of the edge == 1, the triangle ta has to be
		// valid and tb has to be empty.
		if (e[idx].trianglecount == 1) {
			SELFCHECK_NOT_EMPTY(idx, e[idx].ta, "edge", "triangle");
			SELFCHECK_EMPTY(idx, e[idx].tb, "edge", "triangle");
		}
		// Both triangles have to be set, if the triangle count is >=2
		if (e[idx].trianglecount >= 2) {
			SELFCHECK_NOT_EMPTY(idx, e[idx].ta, "edge", "triangle");
			SELFCHECK_NOT_EMPTY(idx, e[idx].tb, "edge", "triangle");
		}

		if (errorCount >= maxErrorsPerType) {
			std::cerr << "...\n";
			break;
		}
	}
	edgeOK = (errorCount == 0);

	// Check triangles

	errorCount = 0;
	for (size_t idx = 0; idx < tcount; ++idx) {
		// A triangle has to have 3 valid vertices and 3 valid edges.
		SELFCHECK_RANGE_NG(idx, t[idx].va, vcount, "triangle", "vertex");
		SELFCHECK_RANGE_NG(idx, t[idx].vb, vcount, "triangle", "vertex");
		SELFCHECK_RANGE_NG(idx, t[idx].vc, vcount, "triangle", "vertex");
		SELFCHECK_RANGE_NG(idx, t[idx].ea, ecount, "triangle", "edge");
		SELFCHECK_RANGE_NG(idx, t[idx].eb, ecount, "triangle", "edge");
		SELFCHECK_RANGE_NG(idx, t[idx].ec, ecount, "triangle", "edge");
		SELFCHECK_NOT_EMPTY(idx, t[idx].va, "triangle", "vertex");
		SELFCHECK_NOT_EMPTY(idx, t[idx].vb, "triangle", "vertex");
		SELFCHECK_NOT_EMPTY(idx, t[idx].vc, "triangle", "vertex");
		SELFCHECK_NOT_EMPTY(idx, t[idx].ea, "triangle", "edge");
		SELFCHECK_NOT_EMPTY(idx, t[idx].eb, "triangle", "edge");
		SELFCHECK_NOT_EMPTY(idx, t[idx].ec, "triangle", "edge");
		if (errorCount >= maxErrorsPerType) {
			std::cerr << "...\n";
			break;
		}
	}
	triangleOK = (errorCount == 0);

	// Check map sizes

	if (!vmap.empty() && vmap.size() != v.size()) {
		std::cerr << "The vmap has a size of " << vmap.size()
				<< " but the v-vector has a size of " << v.size() << ".\n";
		passed = false;
	}
	if (!emap.empty() && emap.size() != e.size()) {
		std::cerr << "The emap has a size of " << emap.size()
				<< " but the e-vector has a size of " << e.size() << ".\n";
		passed = false;
	}
	if (!tmap.empty() && tmap.size() != t.size()) {
		std::cerr << "The tmap has a size of " << tmap.size()
				<< " but the t-vector has a size of " << t.size() << ".\n";
		passed = false;
	}

	// Check selected

	for (const auto idx : selected) {
		if (idx >= v.size()) {
			std::cerr << "In selected the vertex index " << idx
					<< " is greater than the number of vertices in v ("
					<< v.size() << ").\n";
			passed = false;
		}
	}

	if (checkWellOrdering) {

		// Check if all edges are well ordered.

		errorCount = 0;
		if (edgeOK) {
			for (size_t idx = 0; idx < ecount; ++idx) {
				SELFCHECK_LESS_THAN(idx, e[idx].va, e[idx].vb, "edge", "vertex");

				// If an edge has two triangle, tb has to be greater the ta.
				if (e[idx].trianglecount >= 2)
					SELFCHECK_LESS_THAN(idx, e[idx].ta, e[idx].tb, "edge",
							"triangle");

				if (errorCount >= maxErrorsPerType) {
					std::cerr << "...\n";
					break;
				}

			}
			if (errorCount > 0)
				edgeOK = false;
		}

		// Check if all triangles are well ordered

		errorCount = 0;
		if (triangleOK) {
			for (size_t idx = 0; idx < tcount; ++idx) {
				SELFCHECK_LESS_THAN(idx, t[idx].va, t[idx].vb, "triangle",
						"vertex");
				SELFCHECK_LESS_THAN(idx, t[idx].vb, t[idx].vc, "triangle",
						"vertex");
				SELFCHECK_LESS_THAN(idx, t[idx].ea, t[idx].eb, "triangle",
						"edge");
				// Note, that this check is inverted, because of the way the
				// vertices are ordered in a triangle:
				// All vertices are sorted ascending and all edges are sorted
				// by the vertices ascending.
				// Therefor the va of ec is smaller then va of eb and ec is
				// smaller than eb.
				SELFCHECK_LESS_THAN(idx, t[idx].ec, t[idx].eb, "triangle",
						"edge");
				// Because vb of ec is greater than vb of ea: ec is greater
				// than ea.
				SELFCHECK_LESS_THAN(idx, t[idx].ea, t[idx].ec, "triangle",
						"edge");

				if (errorCount >= maxErrorsPerType) {
					std::cerr << "...\n";
					break;
				}
			}
			if (errorCount > 0)
				triangleOK = false;
		}

		// Check triangle edge well ordering. This is only possible for edges,
		// that are connected to one triangle. Otherwise the edge would be
		// reversed for one of the triangles.

		errorCount = 0;
		if (triangleOK && edgeOK) {
			for (size_t idx = 0; idx < tcount; ++idx) {
				const size_t ea = t[idx].ea;
				const size_t eb = t[idx].eb;
				const size_t ec = t[idx].ec;

				if (e[ea].trianglecount <= 1)
					SELFCHECK_EQUAL(idx, ea, t[idx].flip, e[ea].flip,
							"triangle", "edge");
				if (e[eb].trianglecount <= 1)
					SELFCHECK_EQUAL(idx, eb, t[idx].flip, e[eb].flip,
							"triangle", "edge");
				if (e[ec].trianglecount <= 1)
					SELFCHECK_NOTEQUAL(idx, ec, t[idx].flip, e[ec].flip,
							"triangle", "edge");

				if (errorCount >= maxErrorsPerType) {
					std::cerr << "...\n";
					break;
				}
			}
			if (errorCount > 0)
				triangleOK = false;
		}
	}

	// Consistency check triangle -> edge -> vertex

	if (edgeOK && triangleOK) {
		errorCount = 0;
		for (size_t tidx = 0; tidx < tcount; ++tidx) {
			const size_t ea = t[tidx].ea;
			const size_t eb = t[tidx].eb;
			const size_t ec = t[tidx].ec;
			const size_t va = t[tidx].va;
			const size_t vb = t[tidx].vb;
			const size_t vc = t[tidx].vc;

			const size_t t2a = (e[ea].ta == tidx) ? e[ea].tb : e[ea].ta;
			const size_t t2b = (e[eb].ta == tidx) ? e[eb].tb : e[eb].ta;
			const size_t t2c = (e[ec].ta == tidx) ? e[ec].tb : e[ec].ta;

			// An edge.ta should never be zero here, because it belongs to at
			// least on triangle.
			SELFCHECK_NOT_EMPTY(ea, e[ea].ta, "edge", "triangle");
			SELFCHECK_NOT_EMPTY(eb, e[eb].ta, "edge", "triangle");
			SELFCHECK_NOT_EMPTY(ec, e[ec].ta, "edge", "triangle");

			SELFCHECK_EDGE_VERTEX(tidx, ea, va, vb);
			SELFCHECK_EDGE_VERTEX(tidx, eb, vb, vc);
			SELFCHECK_EDGE_VERTEX(tidx, ec, vc, va);

			SELFCHECK_EDGE_TRIANGLE(tidx, ea, t2a);
			SELFCHECK_EDGE_TRIANGLE(tidx, eb, t2b);
			SELFCHECK_EDGE_TRIANGLE(tidx, ec, t2c);

			if (errorCount >= maxErrorsPerType) {
				std::cerr << "...\n";
				break;
			}
		}

	} else {
		std::cerr << "Consistency of geometry is not checked, because";
		if (!triangleOK)
			std::cerr << " triangles";
		if (!triangleOK && !edgeOK)
			std::cerr << " and";
		if (!edgeOK)
			std::cerr << " edges";
		std::cerr << " are not OK.\n";
	}

	return passed;
}

void Geometry::CalculateNormals() {
	// Zero vertex normals
	for (Vertex &ve : v)
		ve.n = { 0, 0, 0 };

	// Start with triangle normals
	for (Triangle &t : this->t) {

		const double x1 = (v[t.vb].x - v[t.va].x);
		const double y1 = (v[t.vb].y - v[t.va].y);
		const double z1 = (v[t.vb].z - v[t.va].z);
		const double x2 = (v[t.vc].x - v[t.vb].x);
		const double y2 = (v[t.vc].y - v[t.vb].y);
		const double z2 = (v[t.vc].z - v[t.vb].z);
		Vector3 temp(y1 * z2 - y2 * z1, z1 * x2 - z2 * x1, x1 * y2 - x2 * y1);
		if (t.flip)
			temp = -temp;
		temp.Normalize();
		t.n = temp;
//		v[t.va].n += temp;
//		v[t.vb].n += temp;
//		v[t.vc].n += temp;
	}
	for (Edge &ed : e) {
		// For edges connected to triangles:
		if (ed.trianglecount == 0)
			continue;
		// Average triangle normals to edge normals
		Vector3 temp = t[ed.ta].n;
		if (ed.trianglecount > 1)
			temp += t[ed.tb].n;
		temp /= (double) ed.trianglecount;
		ed.n = temp;
		// Propagate edge normals to vertex normals
		const size_t a = ed.va;
		const size_t b = ed.vb;
		v[a].n += temp;
		v[b].n += temp;
	}
	// Normalize all vertex normals
	for (Vertex &ve : v)
		ve.n.Normalize();
	// Use vertex normals for edges not belonging to triangles
	for (Edge &ed : e) {
		if (ed.trianglecount != 0)
			continue;
		Vector3 temp = v[ed.va].n + v[ed.vb].n;
		temp.Normalize();
		ed.n = temp;
	}

	verticesHaveNormal = true;
	edgesHaveNormal = true;
	trianglesHaveNormal = true;
}

void Geometry::PropagateNormals() {

	if (!verticesHaveNormal && !edgesHaveNormal && !trianglesHaveNormal)
		CalculateNormals();

	// Spread information from the vertices normals to the edges and triangles,
	// before the information is removed by removing the vertices in the Join()
	// method.
	if (verticesHaveNormal) {
		if (!edgesHaveNormal) {
			for (Edge &ed : e) {
				ed.n = (v[ed.va].n + v[ed.vb].n);
				ed.n.Normalize();
			}
			edgesHaveNormal = true;
		}
		if (!trianglesHaveNormal) {
			for (Triangle &tri : t) {
				tri.n = (v[tri.va].n + v[tri.vb].n + v[tri.vc].n);
				tri.n.Normalize();
			}
			trianglesHaveNormal = true;
		}
	}

	if (edgesHaveNormal) {
		if (!verticesHaveNormal) {
			for (Vertex &vert : v)
				vert.n.Zero();
			for (Edge &ed : e) {
				v[ed.va].n += ed.n;
				v[ed.vb].n += ed.n;
			}
			for (Vertex &vert : v)
				vert.n.Normalize();
			verticesHaveNormal = true;
		}
		if (!trianglesHaveNormal && !t.empty()) {
			for (Triangle &tri : t) {
				tri.n = (e[tri.ea].n + e[tri.eb].n + e[tri.ec].n);
				tri.n.Normalize();
			}
			trianglesHaveNormal = true;
		}
	}

	if (trianglesHaveNormal) {
		if (!verticesHaveNormal) {
			for (Vertex &vert : v)
				vert.n.Zero();

			for (Triangle &tri : t) {
				v[tri.va].n += tri.n;
				v[tri.vb].n += tri.n;
				v[tri.vc].n += tri.n;
			}
			for (Vertex &vert : v)
				vert.n.Normalize();
			verticesHaveNormal = true;
		}
		if (!edgesHaveNormal && !e.empty()) {
			for (Edge &ed : e)
				ed.n.Zero();
			for (Triangle &tri : t) {
				e[tri.ea].n += tri.n;
				e[tri.eb].n += tri.n;
				e[tri.ec].n += tri.n;
			}
			for (Edge &ed : e)
				ed.n.Normalize();
			edgesHaveNormal = true;
		}
	}
}

void Geometry::UpdateNormals(bool updateVertices, bool updateEdges,
		bool updateTriangles) {
	verticesHaveNormal &= (!updateVertices);
	edgesHaveNormal &= (!updateEdges);
	trianglesHaveNormal &= (!updateTriangles);
	PropagateNormals();
}

void Geometry::FlipNormals() {
	for (Vertex &v : this->v)
		v.FlipNormal();
	for (Edge &e : this->e)
		e.FlipNormal();
	for (Triangle &t : this->t)
		t.FlipNormal();
}

void Geometry::FlipInsideOutside() {
	for (Edge &ed : e)
		ed.flip = !ed.flip;
	for (Triangle &tri : t)
		tri.flip = !tri.flip;
}

void Geometry::FlagUV(bool inVertices, bool inTriangles) {
	verticesHaveTextur = inVertices;
	trianglesHaveTexture = inTriangles;
}

void Geometry::CalculateUVFromBox() {
	for (Triangle &tri : t) {
		const Vector3 &n = tri.n;
		const Vertex &va = v[tri.va];
		const Vertex &vb = v[tri.vb];
		const Vertex &vc = v[tri.vc];

		if (fabs(n.x) > fabs(n.y) && fabs(n.x) > fabs(n.z)) {
			if (n.x < 0) {
				tri.tua = -va.y;
				tri.tva = va.z;
				tri.tub = -vb.y;
				tri.tvb = vb.z;
				tri.tuc = -vc.y;
				tri.tvc = vc.z;
			} else {
				tri.tua = va.y;
				tri.tva = va.z;
				tri.tub = vb.y;
				tri.tvb = vb.z;
				tri.tuc = vc.y;
				tri.tvc = vc.z;
			}
		} else if (fabs(n.y) > fabs(n.z) && fabs(n.y) > fabs(n.x)) {
			if (n.y < 0) {
				tri.tua = va.x;
				tri.tva = va.z;
				tri.tub = vb.x;
				tri.tvb = vb.z;
				tri.tuc = vc.x;
				tri.tvc = vc.z;
			} else {
				tri.tua = -va.x;
				tri.tva = va.z;
				tri.tub = -vb.x;
				tri.tvb = vb.z;
				tri.tuc = -vc.x;
				tri.tvc = vc.z;
			}
		} else {
			if (n.z < 0) {
				tri.tua = va.x;
				tri.tva = -va.y;
				tri.tub = vb.x;
				tri.tvb = -vb.y;
				tri.tuc = vc.x;
				tri.tvc = -vc.y;
			} else {
				tri.tua = va.x;
				tri.tva = va.y;
				tri.tub = vb.x;
				tri.tvb = vb.y;
				tri.tuc = vc.x;
				tri.tvc = vc.y;
			}
		}
	}
	trianglesHaveTexture = true;
}

void Geometry::CalculateUVFromAxis(const Vector3 &n, bool symmetric) {
	AffineTransformMatrix uv;
	uv.SetEz(n);
	uv.SetEy(n.Orthogonal());
	uv.CalculateEx();
	uv.Normalize();
	for (Triangle &tri : t) {
		const Vertex &va = v[tri.va];
		const Vertex &vb = v[tri.vb];
		const Vertex &vc = v[tri.vc];
		if (symmetric || tri.n.Dot(n) > 0.0) {
			tri.tua = uv.LocalX(va);
			tri.tub = uv.LocalX(vb);
			tri.tuc = uv.LocalX(vc);
		} else {
			tri.tua = -uv.LocalX(va);
			tri.tub = -uv.LocalX(vb);
			tri.tuc = -uv.LocalX(vc);
		}
		tri.tva = uv.LocalY(va);
		tri.tvb = uv.LocalY(vb);
		tri.tvc = uv.LocalY(vc);
	}
	trianglesHaveTexture = true;
}

void Geometry::CalculateUVFromCylinder(const Vector3 &n) {
	AffineTransformMatrix uv;
	uv.SetEz(n);
	uv.SetEy(n.Orthogonal());
	uv.CalculateEx();
	uv.Normalize();
	for (Triangle &tri : t) {
		const Vertex &va = v[tri.va];
		const Vertex &vb = v[tri.vb];
		const Vertex &vc = v[tri.vc];
		tri.tua = atan2(uv.LocalY(va), uv.LocalX(va)) / M_PI / 2.0;
		tri.tva = uv.LocalZ(va);
		tri.tub = atan2(uv.LocalY(vb), uv.LocalX(vb)) / M_PI / 2.0;
		tri.tvb = uv.LocalZ(vb);
		tri.tuc = atan2(uv.LocalY(vc), uv.LocalX(vc)) / M_PI / 2.0;
		tri.tvc = uv.LocalZ(vc);

		while (tri.tua > 0.25 && tri.tub > 0.25 && tri.tuc < -0.25)
			tri.tuc += 1.0;
		while (tri.tua > 0.25 && tri.tub < -0.25 && tri.tuc > 0.25)
			tri.tub += 1.0;
		while (tri.tua < -0.25 && tri.tub > 0.25 && tri.tuc > 0.25)
			tri.tua += 1.0;
		while (tri.tua < -0.25 && tri.tub < -0.25 && tri.tuc > 0.25)
			tri.tuc -= 1.0;
		while (tri.tua < -0.25 && tri.tub > 0.25 && tri.tuc < -0.25)
			tri.tub -= 1.0;
		while (tri.tua > 0.25 && tri.tub < -0.25 && tri.tuc < -0.25)
			tri.tua -= 1.0;
	}
	trianglesHaveTexture = true;
}

void Geometry::CalculateUVFromSphere(const Vector3 &n) {
	AffineTransformMatrix uv;
	uv.SetEz(n);
	uv.SetEy(n.Orthogonal());
	uv.CalculateEx();
	uv.Normalize();
	for (Triangle &tri : t) {
		const Vector3 &n = tri.n;
		const Vertex &va = v[tri.va];
		const Vertex &vb = v[tri.vb];
		const Vertex &vc = v[tri.vc];
		{
			const double x = uv.LocalX(va);
			const double y = uv.LocalY(va);
			const double z = uv.LocalZ(va);
			tri.tua = atan2(y, x) / M_PI;
			tri.tva = atan2(z, sqrt(x * x + y * y)) / M_PI_2;
		}
		{
			const double x = uv.LocalX(vb);
			const double y = uv.LocalY(vb);
			const double z = uv.LocalZ(vb);
			tri.tub = atan2(y, x) / M_PI;
			tri.tvb = atan2(z, sqrt(x * x + y * y)) / M_PI_2;
		}
		{
			const double x = uv.LocalX(vc);
			const double y = uv.LocalY(vc);
			const double z = uv.LocalZ(vc);
			tri.tuc = atan2(y, x) / M_PI;
			tri.tvc = atan2(z, sqrt(x * x + y * y)) / M_PI_2;
		}
		while (tri.tua > 0.25 && tri.tub > 0.25 && tri.tuc < -0.25)
			tri.tuc += 1.0;
		while (tri.tua > 0.25 && tri.tub < -0.25 && tri.tuc > 0.25)
			tri.tub += 1.0;
		while (tri.tua < -0.25 && tri.tub > 0.25 && tri.tuc > 0.25)
			tri.tua += 1.0;
		while (tri.tua < -0.25 && tri.tub < -0.25 && tri.tuc > 0.25)
			tri.tuc -= 1.0;
		while (tri.tua < -0.25 && tri.tub > 0.25 && tri.tuc < -0.25)
			tri.tub -= 1.0;
		while (tri.tua > 0.25 && tri.tub < -0.25 && tri.tuc < -0.25)
			tri.tua -= 1.0;
	}
	trianglesHaveTexture = true;
}

void Geometry::TransformUV(const AffineTransformMatrix &matrix) {
	for (Vertex &vert : v) {
		Vector3 temp = matrix.Transform(vert.u, vert.v);
		vert.u = temp.x;
		vert.v = temp.y;
	}
	for (Triangle &tri : t) {
		Vector3 tempa = matrix.Transform(tri.tua, tri.tva);
		Vector3 tempb = matrix.Transform(tri.tub, tri.tvb);
		Vector3 tempc = matrix.Transform(tri.tuc, tri.tvc);
		tri.tua = tempa.x;
		tri.tva = tempa.y;
		tri.tub = tempb.x;
		tri.tvb = tempb.y;
		tri.tuc = tempc.x;
		tri.tvc = tempc.y;
	}
}

void Geometry::CalculateUVCoordinateSystems() {
	if (verticesHaveTextur && !trianglesHaveTexture) {
		for (Triangle &tri : t) {
			tri.tua = v[tri.va].u;
			tri.tva = v[tri.va].v;
			tri.tub = v[tri.vb].u;
			tri.tvb = v[tri.vb].v;
			tri.tuc = v[tri.vc].u;
			tri.tvc = v[tri.vc].v;
		}
		trianglesHaveTexture = true;
	}
	if (trianglesHaveTexture) {
		bool texturesFlat = true;
		// Calculate 't' and 'b' of the TBN-matrix.
		for (Triangle &tri : t) {

			const double dU1 = (tri.flip ? tri.tuc : tri.tub) - tri.tua;
			const double dV1 = (tri.flip ? tri.tvc : tri.tvb) - tri.tva;
			const double dU2 = (tri.flip ? tri.tub : tri.tuc) - tri.tua;
			const double dV2 = (tri.flip ? tri.tvb : tri.tvc) - tri.tva;
			const double det = dU1 * dV2 - dU2 * dV1;
			if (fabs(det) < FLT_EPSILON) {
				tri.t.Zero();
				tri.b.Zero();
				continue;
			}
			texturesFlat = false;
			const double f = 1.0 / det;

			const Vector3 dE1 = (tri.flip ? v[tri.vc] : v[tri.vb]) - v[tri.va];
			const Vector3 dE2 = (tri.flip ? v[tri.vb] : v[tri.vc]) - v[tri.va];

			tri.b.x = f * (-dU2 * dE1.x + dU1 * dE2.x);
			tri.b.y = f * (-dU2 * dE1.y + dU1 * dE2.y);
			tri.b.z = f * (-dU2 * dE1.z + dU1 * dE2.z);

			tri.t.x = f * (dV2 * dE1.x - dV1 * dE2.x);
			tri.t.y = f * (dV2 * dE1.y - dV1 * dE2.y);
			tri.t.z = f * (dV2 * dE1.z - dV1 * dE2.z);
		}
		if (texturesFlat)
			trianglesHaveTexture = false;
	}
	if (trianglesHaveTexture && !verticesHaveTextur) {
		for (Triangle &tri : t) {
			v[tri.va].u = tri.tua;
			v[tri.va].v = tri.tva;
			v[tri.vb].u = tri.tub;
			v[tri.vb].v = tri.tvb;
			v[tri.vc].u = tri.tuc;
			v[tri.vc].v = tri.tvc;
		}
		verticesHaveTextur = true;
	}
}

void Geometry::CalculateSharpEdges(double angle) {
	// Every edge that connects two triangles that are oriented
	// more than alpha radians relative to each other is considered
	// a sharp edge.
	const float sharplimit = cos(angle);
	auto dot = [](const Vector3 &a, const Vector3 &b) -> double {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	};
	// Set sharp edges based on enclosed angle
	for (Edge &ed : e) {
		if (ed.trianglecount <= 1) {
			ed.sharp = true;
		} else {
			const double sl = dot(t[ed.ta].n, t[ed.tb].n);
			if (sl < sharplimit) {
				ed.sharp = true;
			} else {
				ed.sharp = false;
			}
		}
	}
}

void Geometry::ResetGroups() {
	// Reset all triangles to group -1
	for (Triangle &tri : this->t)
		tri.group = nothing;
	// Reset all edges to group -1
	for (Edge &ed : this->e)
		ed.group = nothing;
	// Reset all vertices to group -1
	for (Vertex &vert : this->v)
		vert.group = nothing;
}

void Geometry::CalculateGroups(double angle) {

	CalculateSharpEdges(angle);

	ResetGroups();
	size_t currentGroup = nothing;

	for (size_t i = 0; i < t.size(); ++i) {
		if (t[i].group != nothing)
			continue;

		++currentGroup;
		std::set<size_t> checklist;
		checklist.insert(i);
		while (!checklist.empty()) {
			size_t j = *(checklist.begin());
			{
				const size_t k = t[j].ea;
				if (!e[k].sharp && e[k].trianglecount >= 2) {
					size_t p = e[k].GetOtherTriangle(j);
					if (t[p].group == nothing)
						checklist.insert(p);
				}
			}
			{
				const size_t k = t[j].eb;
				if (!e[k].sharp && e[k].trianglecount >= 2) {
					size_t p = e[k].GetOtherTriangle(j);
					if (t[p].group == nothing)
						checklist.insert(p);
				}
			}
			{
				const size_t k = t[j].ec;
				if (!e[k].sharp && e[k].trianglecount >= 2) {
					size_t p = e[k].GetOtherTriangle(j);
					if (t[p].group == nothing)
						checklist.insert(p);
				}
			}
			t[j].group = currentGroup;
			checklist.erase(j);
		}
	}

	// Count the number of sharp edges ending or starting in each vertex.
	// Store the first two sharp edges connecting to a vertex.
	std::vector<size_t> vecount;
	std::vector<size_t> vea;
	std::vector<size_t> veb;
	vecount.assign(v.size(), 0);
	vea.resize(v.size());
	veb.resize(v.size());
	for (size_t n = 0; n < e.size(); ++n) {
		if (!e[n].sharp)
			continue;
		const size_t ja = e[n].va;
		const size_t jb = e[n].vb;
		vecount[ja]++;
		if (vecount[ja] == 1)
			vea[ja] = n;
		if (vecount[ja] == 2)
			veb[ja] = n;
		vecount[jb]++;
		if (vecount[jb] == 1)
			vea[jb] = n;
		if (vecount[jb] == 2)
			veb[jb] = n;
	}

	// Assign groups
	currentGroup = nothing;
	for (size_t n = 0; n < e.size(); ++n) {
		if (e[n].group != nothing || !e[n].sharp)
			continue;
		currentGroup++;
		e[n].group = currentGroup;

		// Forward pass
		{
			size_t m = n;
			size_t i = e[m].va;
			while (vecount[i] == 2) {
				if (vea[i] == m) {
					m = veb[i];
				} else {
					m = vea[i];
				}
				if (e[m].group != nothing)
					break;
				i = e[m].GetOtherVertex(i);
				e[m].group = currentGroup;
			}
		}
		// Backward pass
		{
			size_t m = n;
			size_t i = e[m].vb;
			while (vecount[i] == 2) {
				if (veb[i] == m) {
					m = vea[i];
				} else {
					m = veb[i];
				}
				if (e[m].group != nothing)
					break;
				i = e[m].GetOtherVertex(i);
				e[m].group = currentGroup;
			}
		}
	}
}

size_t Geometry::CalculateObjects() {

	// If the Finish() method was called and everything is sorted, this group
	// assignment should work with optimal speed. It always converges.

	for (size_t i = 0; i < v.size(); ++i)
		v[i].group = i;

	bool runagain = true;
	while (runagain) {
		runagain = false;
		for (Edge &ed : e) {
			if (v[ed.va].group < v[ed.vb].group) {
				v[ed.vb].group = v[ed.va].group;
				runagain = true;
				continue;
			}
			if (v[ed.vb].group < v[ed.va].group) {
				v[ed.va].group = v[ed.vb].group;
				runagain = true;
				continue;
			}
		}
	}

	// Remap the vertex groups.
	gmap.clear();
	for (const Vertex &vert : v) {
		if (gmap.find(vert.group) == gmap.end())
			gmap[vert.group] = gmap.size();
	}
	const size_t groupcount = gmap.size();
	for (Vertex &vert : v)
		vert.group = gmap[vert.group];
	gmap.clear();

	// The transfer to edges and groups assumes, that the geometry is
	// consistent and passes SelfCheckPassed().

	// Transfer groups to edges
	for (Edge &ed : e)
		ed.group = v[ed.va].group;

	// Transfer groups to triangles
	for (Triangle &tri : t)
		tri.group = v[tri.va].group;

	return groupcount;
}

size_t Geometry::CountVertices() const {
	return v.size();
}

size_t Geometry::CountEdges() const {
	return e.size();
}

size_t Geometry::CountTriangles() const {
	return t.size();
}

const Geometry::Vertex& Geometry::operator [](size_t index) const {
	return v[index];
}

Geometry::Vertex& Geometry::operator [](size_t index) {
	return v[index];
}

const Geometry::Vertex& Geometry::GetVertex(size_t index) const {
	return v[index];
}

Geometry::Vertex& Geometry::GetVertex(size_t index) {
	return v[index];
}

const Geometry::Vertex& Geometry::GetEdgeVertex(const size_t indexEdge,
		uint_fast8_t indexVertex) const {
	const auto ed = e[indexEdge];
	const size_t vidx = ed.GetVertexIndex(indexVertex);
	if (vidx >= v.size())
		throw std::range_error(
				std::string(__FUNCTION__)
						+ " - Index not found in vertex array.");
	return v[vidx];
}

const Geometry::Edge& Geometry::GetEdge(const size_t index) const {
	return e[index];
}

Geometry::Edge& Geometry::GetEdge(const size_t index) {
	return e[index];
}

const Geometry::Triangle& Geometry::GetTriangle(const size_t index) const {
	return t[index];
}

Geometry::Triangle& Geometry::GetTriangle(const size_t index) {
	return t[index];
}

const Geometry::Vertex& Geometry::GetTriangleVertex(const size_t indexTriangle,
		uint_fast8_t indexVertex) const {
	const auto &tri = t[indexTriangle];
	const size_t vidx = tri.GetVertexIndex(indexVertex);
	if (vidx >= v.size())
		throw std::range_error(
				std::string(__FUNCTION__)
						+ " - Index not found in vertex array.");
	return v[vidx];
}

void Geometry::Transform(const AffineTransformMatrix &matrix) {
	AffineTransformMatrix::Orientation orientation = matrix.CheckOrientation();

	AffineTransformMatrix matrixnormal = matrix.GetNormalMatrix();
	matrixnormal.Normalize();

//	Vector3 ex = matrixnormal.GetEx();
//	Vector3 ey = matrixnormal.GetEy();
//	Vector3 ez = matrixnormal.GetEz();
//
//	matrixnormal.SetEx(ex.Normal());
//	matrixnormal.SetEy(ey.Normal());
//	matrixnormal.SetEz(ez.Normal());

	if (orientation == AffineTransformMatrix::Orientation::LHS)
		FlipInsideOutside();

	for (Vertex &vertex : v)
		vertex.Transform(matrix, matrixnormal);
	for (Edge &edge : e)
		edge.n = matrixnormal.Transform(edge.n);
	for (Triangle &tri : t)
		tri.n = matrixnormal.Transform(tri.n);
}

void Geometry::Transform(std::function<Vector3(Vector3)> func) {
	//TODO Modify the normals as well.
	for (Vertex &vertex : v) {
		//FIXME Check if the temp vector is still needed
//		Vector3 temp(vertex.x, vertex.y, vertex.z);
		Vector3 temp = func(vertex);
		vertex.x = temp.x;
		vertex.y = temp.y;
		vertex.z = temp.z;
	}
}

void Geometry::ApplyTransformationMatrix() {
	Transform(this->matrix);
	this->matrix.SetIdentity();
}

Vector3 Geometry::GetTriangleCenter(size_t idx) const {
	const Triangle &tri = t[idx];
	const Vertex &va = v[tri.va];
	const Vertex &vb = v[tri.vb];
	const Vertex &vc = v[tri.vc];
	return (va + vb + vc) / 3.0;
}

Vector3 Geometry::GetTetraederCenter(size_t idx) const {
	return GetTriangleCenter(idx) * (3.0 / 4.0);
}

double Geometry::GetTetraederVolume(size_t idx) const {
	const Triangle &tri = t[idx];
	const Vertex &v0 = v[tri.GetVertexIndex(0)];
	const Vertex &v1 = v[tri.GetVertexIndex(1)];
	const Vertex &v2 = v[tri.GetVertexIndex(2)];
	double Vt = (v0.x * v1.y - v0.y * v1.x) * v2.z
			+ (-v0.x * v1.z + v0.z * v1.x) * v2.y
			+ (v0.y * v1.z - v0.z * v1.y) * v2.x;
	return Vt / 6.0;
}

double Geometry::GetTriangleArea(size_t idx) const {
	const Triangle &tri = t[idx];
	const Vertex &v0 = v[tri.va];
	const Vertex &v1 = v[tri.vb];
	const Vertex &v2 = v[tri.vc];
	const Vector3 d1 = v1 - v0;
	const Vector3 d2 = v2 - v0;
	const Vector3 x = d1 * d2;
	return x.Abs() / 2.0;
}

Vector3 Geometry::GetCenterOfVertices() const {
	Vector3 center;
	double V = 0.0;
	for (const Vertex &vert : v) {
		center += vert;
		V += 1.0;
	}
	return center / V;
}

Vector3 Geometry::GetCenterOfMass() const {
	Vector3 center;
	double V = 0.0;
	for (const Triangle &tri : t) {
		const Vertex &v0 = v[tri.GetVertexIndex(0)];
		const Vertex &v1 = v[tri.GetVertexIndex(1)];
		const Vertex &v2 = v[tri.GetVertexIndex(2)];
		double Vt = (v0.x * v1.y - v0.y * v1.x) * v2.z
				+ (-v0.x * v1.z + v0.z * v1.x) * v2.y
				+ (v0.y * v1.z - v0.z * v1.y) * v2.x;
		// dV is 6 fold larger then the volume of the tetraeder, but this
		// cancels out in the end.
		center += (v0 + v1 + v2) / 3.0 * Vt;
		V += Vt;
	}
	return center / V;
}

double Geometry::GetArea() const {
	double A = 0.0;
	for (const Triangle &tri : t) {
		const Vertex &v0 = v[tri.GetVertexIndex(0)];
		const Vertex &v1 = v[tri.GetVertexIndex(1)];
		const Vertex &v2 = v[tri.GetVertexIndex(2)];
		const Vector3 d1 = v1 - v0;
		const Vector3 d2 = v2 - v0;
		const Vector3 x = d1 * d2;
		A += x.Abs();
	}
	return A / 2.0;
}

double Geometry::GetVolume() const {
	double V = 0.0;
	for (const Triangle &tri : t) {
		const Vertex &v0 = v[tri.GetVertexIndex(0)];
		const Vertex &v1 = v[tri.GetVertexIndex(1)];
		const Vertex &v2 = v[tri.GetVertexIndex(2)];
		// Fricas code:
		// )set fortran optlevel 2
		// )set output fortran on
		//	v321 := v2x*v1y*v0z
		//	v231 := v1x*v2y*v0z
		//	v312 := v2x*v0y*v1z
		//	v132 := v0x*v2y*v1z
		//	v213 := v1x*v0y*v2z
		//	v123 := v0x*v1y*v2z
		//	(-v321 + v231 + v312 - v132 - v213 + v123)
		double Vt = (v0.x * v1.y - v0.y * v1.x) * v2.z
				+ (-v0.x * v1.z + v0.z * v1.x) * v2.y
				+ (v0.y * v1.z - v0.z * v1.y) * v2.x;
		V += Vt;
	}
	return V / 6.0;
}

double Geometry::GetNormalCurvature() const {
	double A = 0.0;

	for (const Edge &ed : e) {
		if (ed.trianglecount < 2)
			continue;
		const Triangle &ta = t[ed.ta];
		const Triangle &tb = t[ed.tb];
		const Vector3 na = ta.n;
		const Vector3 nb = tb.n;
		const Vector3 d = (v[tb.va] + v[tb.vb] + v[tb.vc] - v[ta.va] - v[ta.vb]
				- v[ta.vc]).Normal();

//		const Vector3 pa = na * d;
//		const Vector3 pb = nb * d;

		const double aa = asin(na.Dot(d));
		const double ab = asin(nb.Dot(d));
		const double da = ab - aa;
//		const double da = acos(na.Dot(nb));
		A += da;
	}
	return A;
}

Polygon3 Geometry::IntersectPlane(const Vector3 &n_, double d) const {
	Vector3 n_unit = n_.Normal();
	Polygon3 temp;

	// Map all vertices onto the plane normal, shift by d.
	std::vector<double> vd;
	vd.reserve(v.size());
	for (const Vertex &vert : v) {
		const double h = vert.Dot(n_unit) - d;
		vd.push_back(h);
	}

	// Check all triangles, if the vertices of the triangle intersect with the
	// plane. i.e. two vertices are on one side and one is on the other side
	// of the plane.
	for (const Triangle &tri : t) {
		const size_t va = tri.va;
		const size_t vb = tri.vb;
		const size_t vc = tri.vc;

		const double da = vd[va];
		const double db = vd[vb];
		const double dc = vd[vc];

		// Two vertices might sit on the plane if one edge is in the plane.
		if (da < DBL_EPSILON && db < DBL_EPSILON && dc < DBL_EPSILON)
			continue;
		if (da > -DBL_EPSILON && db > -DBL_EPSILON && dc > -DBL_EPSILON)
			continue;

		Vertex vea;
		Vertex veb;
		int8_t ia = -1;
		int8_t ib = -1;
		int8_t dira = 0;
		int8_t dirb = 0;

		// Find the edges, that need to be cut.
		if ((da <= DBL_EPSILON && db > DBL_EPSILON)
				|| (db <= DBL_EPSILON && da > DBL_EPSILON)) {
			const double f =
					(fabs(da - db) < DBL_EPSILON) ? (0.5) : (da / (da - db));
			vea = v[va].Interp(v[vb], f);
			ia = 0;
			dira = (da <= DBL_EPSILON) ? 1 : -1;
			if (e[tri.ea].sharp)
				vea.n = tri.n;
			else
				vea.n = e[tri.ea].n;
		}
		if ((db <= DBL_EPSILON && dc > DBL_EPSILON)
				|| (dc <= DBL_EPSILON && db > DBL_EPSILON)) {
			const double f =
					(fabs(db - dc) < DBL_EPSILON) ? (0.5) : (db / (db - dc));
			if (ia == -1) {
				vea = v[vb].Interp(v[vc], f);
				ia = 1;
				dira = (db <= DBL_EPSILON) ? 1 : -1;
				if (e[tri.eb].sharp)
					vea.n = tri.n;
				else
					vea.n = e[tri.eb].n;
			} else {
				veb = v[vb].Interp(v[vc], f);
				ib = 1;
				dirb = (db <= DBL_EPSILON) ? 1 : -1;
				if (e[tri.eb].sharp)
					veb.n = tri.n;
				else
					veb.n = e[tri.eb].n;
			}
		}
		if ((dc <= DBL_EPSILON && da > DBL_EPSILON)
				|| (da <= DBL_EPSILON && dc > DBL_EPSILON)) {
			const double f =
					(fabs(dc - da) < DBL_EPSILON) ? (0.5) : (dc / (dc - da));
			if (ia == -1) {
				vea = v[vc].Interp(v[va], f);
				ia = 2;
				dira = (dc <= DBL_EPSILON) ? 1 : -1;
				if (e[tri.ec].sharp)
					vea.n = tri.n;
				else
					vea.n = e[tri.ec].n;
			} else {
				veb = v[vc].Interp(v[va], f);
				ib = 2;
				dirb = (dc <= DBL_EPSILON) ? 1 : -1;
				if (e[tri.ec].sharp)
					veb.n = tri.n;
				else
					veb.n = e[tri.ec].n;
			}
		}
		const size_t idx = temp.v.size();
		temp.AddVertex(vea);
		temp.AddVertex(veb);

		if (tri.flip)
			dira = -dira;
		if (dira < 0)
			temp.AddEdge(idx, idx + 1);
		else
			temp.AddEdge(idx + 1, idx);

//			temp.e.back().flip = !temp.e.back().flip;
		temp.e.back().n = tri.n;
		temp.e.back().c = tri.c;
	}

#ifndef NDEBUG
	// Correctness has to be checked before joining the vertices.
	int edge_correct = 0;
	int edge_wrong = 0;
	for (const Edge &ed : temp.e) {
		// The vector r should point outside and in the same direction as the
		// normals of the edges.
		Vector3 r = (temp.v[ed.vb] - temp.v[ed.va]) * n_unit;
		// Project the vector r on the normal of the edge for checking.
		double c = r.x * ed.n.x + r.y * ed.n.y + r.z * ed.n.z;
		// The result should always be positive. (Assuming the object cut has
		// correctly oriented normals.
		edge_correct += (c > 0.0) ? 1 : 0;
		edge_wrong += (c <= 0.0) ? 1 : 0;
	}
	if (edge_wrong > 0) {
		std::cout << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__
				<< " - ";
		std::cout << "correct edges: " << edge_correct << ", ";
		std::cout << "wrong edges: " << edge_wrong << "\n";
	}
#endif
	temp.Join();
	temp.SortLoop();
	return temp;
}

/*
 // Find all edges, that intersect with the plane. I.e one point is on or
 // smaller than the plane, the other is larger than the plane.
 std::set<size_t> edges;
 for (size_t i = 0; i < e.size(); ++i) {
 const Vector3 a = v[e[i].va];
 const Vector3 b = v[e[i].vb];
 const double da = a.Dot(n_unit);
 const double db = b.Dot(n_unit);
 if ((da > d && db <= d) || (da <= d && db > d))
 edges.insert(i);
 }

 while (!edges.empty()) {

 // Start anywhere
 size_t ne = *(edges.begin());
 size_t nt = e[ne].ta;

 // Find other edge.
 uint_fast8_t i0 = t[nt].PositionEdge(ne);
 uint_fast8_t i1 = i0;
 if (i0 != 0 && edges.find(t[nt].ea) != edges.end()) {
 i1 = 3;
 }
 if (i0 != 1 && edges.find(t[nt].eb) != edges.end()) {
 i1 = (i0 == 0) ? 1 : 4;
 }
 if (i0 != 2 && edges.find(t[nt].ec) != edges.end()) {
 i1 = 2;
 }
 // Always i1 > i0 but the distance is either 1 or 2.
 // And i1==i0 if there is a defect.
 int_fast8_t di = i1 - i0;

 // Based on the distance and the flippedness of the triangle, change the
 // order of the vertices.
 if (di <= 0 || di > 2) {
 std::ostringstream out;
 out << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " - ";
 out << "Algorithmic error in edge distance calculation.";
 throw std::runtime_error(out.str());
 }
 if ((di == 2 && t[nt].flip) || (di == 1 && !t[nt].flip)) {
 nt = e[ne].tb;
 }

 while (!edges.empty()) {
 const Vector3 a = v[e[ne].va];
 const Vector3 b = v[e[ne].vb];
 const double sa = a.Dot(n) - d;
 const double sb = b.Dot(n) - d;
 const double f =
 (fabs(sa - sb) < DBL_EPSILON) ? (0.5) : (sa / (sa - sb));
 const Vector3 na = v[e[ne].va].n;
 const Vector3 nb = v[e[ne].vb].n;
 const Vector3 i = a.Interp(b, f);
 const Vector3 ni = na.Interp(nb, f).Normal();

 temp.AddEdgeToVertex(Vertex(i, ni));

 //			if (!temp.e.empty()) {
 //				auto &ed = temp.e.back();
 //				Vector3 r = (temp.v[ed.vb] - temp.v[ed.va]) * n;
 //				// Project the vector r on the normal of the edge for checking.
 //				double c = r.x * ni.x + r.y * ni.y + r.z * ni.z;
 //				std::cout << c << '\n';
 //			}

 //		ret.InsertPoint(PlaneProjection(a, b, n, d));
 edges.erase(ne);

 if (edges.find(t[nt].ea) != edges.end()) {
 ne = t[nt].ea;
 nt = e[ne].OtherTriangle(nt);
 continue;
 }
 if (edges.find(t[nt].eb) != edges.end()) {
 ne = t[nt].eb;
 nt = e[ne].OtherTriangle(nt);
 continue;
 }
 if (edges.find(t[nt].ec) != edges.end()) {
 ne = t[nt].ec;
 nt = e[ne].OtherTriangle(nt);
 continue;
 }
 break;
 }
 temp.CloseLoopNextGroup();
 }
 temp.PropagateNormals();
 temp.finished = true;

 */

Vector3 Geometry::IntersectArrow(const Vector3 &p0, const Vector3 &dir) const {

	// TODO Change to ranged loop.
	for (size_t i = 0; i < t.size(); ++i) {
		const Vertex a = GetTriangleVertex(i, 0);
		const Vertex b = GetTriangleVertex(i, 1);
		const Vertex c = GetTriangleVertex(i, 2);
		// Check if point of (p0 + x*dir) lies inside the triangle
		if (((b - a) * (p0 - a)).Dot(dir) < 0)
			continue;
		if (((c - b) * (p0 - b)).Dot(dir) < 0)
			continue;
		if (((a - c) * (p0 - c)).Dot(dir) < 0)
			continue;
		// Find intersection point
		// solve((p0x+x*dirx)*nx + (p0y+x*diry)*ny + (p0z+x*dirz)*nz = ax*nx + ay*ny + az*nz, x);
		const double den = (t[i].n).Dot(dir);
		const double x = (t[i].n).Dot(a - p0) / den;
		return p0 + dir * x;
	}
	return Vector3();
}

size_t Geometry::Select(const std::set<size_t> &select) {
	selected = select;
	std::set<size_t>::iterator n = selected.upper_bound(v.size());
	if (n != selected.end())
		selected.erase(n, selected.end());
	return selected.size();
}

size_t Geometry::SelectAll() {
	const size_t n0 = selected.size();
	for (size_t n = 0; n < v.size(); ++n)
		selected.insert(n);
	return selected.size() - n0;
}

size_t Geometry::UnselectAll() {
	const size_t n0 = selected.size();
	selected.clear();
	return n0;
}

size_t Geometry::SelectByPlane(const Vector3 &n, double d) {
	const size_t n0 = selected.size();
	for (size_t i = 0; i < v.size(); ++i) {
		const double temp = v[i].Dot(n);
		if (temp >= d)
			selected.insert(i);
	}
	return selected.size() - n0;
}

size_t Geometry::UnselectByPlane(const Vector3 &n, double d) {
	const size_t n0 = selected.size();
	for (size_t i = 0; i < v.size(); ++i) {
		const double temp = v[i].Dot(n);
		if (temp >= d)
			selected.erase(i);
	}
	return n0 - selected.size();
}

size_t Geometry::SelectByNormal(const Vector3 &n, double limit) {
	const size_t n0 = selected.size();
	for (size_t i = 0; i < v.size(); ++i) {
		const double temp = v[i].n.Dot(n);
		if (temp >= limit)
			selected.insert(i);
	}
	return selected.size() - n0;
}

size_t Geometry::UnselectByNormal(const Vector3 &n, double limit) {
	const size_t n0 = selected.size();
	for (size_t i = 0; i < v.size(); ++i) {
		const double temp = v[i].n.Dot(n);
		if (temp >= limit)
			selected.erase(i);
	}
	return n0 - selected.size();
}

size_t Geometry::SelectByGroup(size_t group) {
	const size_t n0 = selected.size();
	for (const auto &tri : t) {
		if (tri.group == group) {
			selected.insert(tri.va);
			selected.insert(tri.vb);
			selected.insert(tri.vc);
		}
	}
	return n0 - selected.size();
}

size_t Geometry::UnselectByGroup(size_t group) {
	const size_t n0 = selected.size();
	for (const auto &tri : t) {
		if (tri.group == group) {
			selected.erase(tri.va);
			selected.erase(tri.vb);
			selected.erase(tri.vc);
		}
	}
	return n0 - selected.size();
}

size_t Geometry::CountSelected() const {
	return selected.size();
}

void Geometry::Paint() const {

	glPointSize(dotSize);

	if (paintTriangles) {
		PaintTriangles();
	}
	if (paintEdges) {
		PaintEdges();
	}
	if (paintVertices) {
		PaintVertices();
	}
	if (paintSelected) {
		PaintSelected();
	}

	glPointSize(0);
}

void Geometry::PaintTriangles(const std::set<size_t> &sel, bool invert) const {
	const double normalscale = 0.005;
	glPushMatrix();
	matrix.GLMultMatrix();
	//	OpenGLMaterial::EnableColors();
	GLuint group = 0;
	bool skip = false;
	glPushName(group);
	glBegin(GL_TRIANGLES);
	if (smooth) {
		for (const Triangle &tri : t) {

			if (tri.group != group) {
				group = (GLuint) (tri.group % (nothing));
				skip = ((!invert && sel.find(group) == sel.end())
						|| (invert && sel.find(group) != sel.end()));
				if (skip)
					continue;

				glEnd();
				glLoadName(group);
				glBegin(GL_TRIANGLES);
			}
			if (skip)
				continue;

			const size_t idx_a = tri.va;
			const size_t idx_b = (tri.flip) ? tri.vc : tri.vb;
			const size_t idx_c = (tri.flip) ? tri.vb : tri.vc;

			const size_t idx_ea = (tri.flip) ? tri.ec : tri.ea;
			const size_t idx_eb = tri.eb;
			const size_t idx_ec = (tri.flip) ? tri.ea : tri.ec;

			auto va = v[idx_a];
			auto vb = v[idx_b];
			auto vc = v[idx_c];
			auto ca = v[idx_a].c;
			auto cb = v[idx_b].c;
			auto cc = v[idx_c].c;

			// ca = cb = cc = Geometry::Color();

			Vector3 na;
			Vector3 nb;
			Vector3 nc;

			bool useNormals = false;

			if (edgesHaveNormal && trianglesHaveNormal) {
				useNormals = true;
				if (e[idx_ea].sharp) {
					if (e[idx_eb].sharp) {
						if (e[idx_ec].sharp) {
							na = nb = nc = tri.n;
						} else {
							na = nc = e[idx_ec].n;
							nb = tri.n;
						}
					} else {
						if (e[idx_ec].sharp) {
							nb = nc = e[idx_eb].n;
							na = tri.n;
						} else {
							na = e[idx_ec].n;
							nb = e[idx_eb].n;
							nc = (na + nb) / 2.0;
						}
					}
				} else {
					if (e[idx_eb].sharp) {
						if (e[idx_ec].sharp) {
							na = nb = e[idx_ea].n;
							nc = tri.n;
						} else {
							nb = e[idx_ea].n;
							nc = e[idx_ec].n;
							na = (nb + nc) / 2.0;
						}
					} else {
						if (e[idx_ec].sharp) {
							na = e[idx_ea].n;
							nc = e[idx_eb].n;
							nb = (na + nc) / 2.0;
						} else {
							if (verticesHaveNormal) {
								useNormals = true;
								na = v[idx_a].n;
								nb = v[idx_b].n;
								nc = v[idx_c].n;
							} else {
								const auto ha = e[idx_ea].n;
								const auto hb = e[idx_eb].n;
								const auto hc = e[idx_ec].n;
								na = (hc + ha) / 2.0;
								nb = (ha + hb) / 2.0;
								nc = (hb + hc) / 2.0;
							}
						}
					}
				}
			} else {
				if (verticesHaveNormal) {
					useNormals = true;
					na = v[idx_a].n;
					nb = v[idx_b].n;
					nc = v[idx_c].n;
				}
			}

			if (useNormals)
				GLNormal(na);
			if (verticesHaveTextur)
				glTexCoord2d(va.u, va.v);
			if (verticesHaveColor)
				GLColor(ca);
			GLVertex(va);
			if (useNormals)
				GLNormal(nb);
			if (verticesHaveTextur)
				glTexCoord2d(vb.u, vb.v);
			if (verticesHaveColor)
				GLColor(cb);
			GLVertex(vb);
			if (useNormals)
				GLNormal(nc);
			if (verticesHaveTextur)
				glTexCoord2d(vc.u, vc.v);
			if (verticesHaveColor)
				GLColor(cc);
			GLVertex(vc);

//			glNormal3d(v[tri.va].n.x, v[tri.va].n.y, v[tri.va].n.z);
//			glVertex3d(v[tri.va].x, v[tri.va].y, v[tri.va].z);
//			glNormal3d(v[tri.vb].n.x, v[tri.vb].n.y, v[tri.vb].n.z);
//			glVertex3d(v[tri.vb].x, v[tri.vb].y, v[tri.vb].z);
//			glNormal3d(v[tri.vc].n.x, v[tri.vc].n.y, v[tri.vc].n.z);
//			glVertex3d(v[tri.vc].x, v[tri.vc].y, v[tri.vc].z);
		}

	} else {
		for (const Triangle &tri : t) {
//			const double a = 2.0 * M_PI * i / t.size();
//			const double r = (1.0 + cos(a)) / 2.0;
//			const double g = (1.0 + cos(a + M_PI / 1.5)) / 2.0;
//			const double b = (1.0 + cos(a + 2.0 * M_PI / 1.5)) / 2.0;
//			glColor3d(r, g, b);

			if (tri.group != group) {
				group = (GLuint) (tri.group % (nothing));
				skip = ((!invert && sel.find(tri.group) == sel.end())
						|| (invert && sel.find(tri.group) != sel.end()));
				if (skip)
					continue;
				glEnd();
				glLoadName(group);
				glBegin(GL_TRIANGLES);
			}
			if (skip)
				continue;

			//			glColor3f(0.5 + cos(2 * M_PI / 20.0 * tri.group) * 0.5,
			//					0.5
			//							+ cos(
			//									2 * M_PI / 20.0 * tri.group
			//											+ M_PI * 2.0 / 3.0) * 0.5,
			//					0.5
			//							+ cos(
			//									2 * M_PI / 20.0 * tri.group
			//											+ M_PI * 4.0 / 3.0) * 0.5);

			const size_t idx_a = tri.va;
			const size_t idx_b = (tri.flip) ? tri.vc : tri.vb;
			const size_t idx_c = (tri.flip) ? tri.vb : tri.vc;

			const auto n = tri.n;
			const auto va = v[idx_a];
			const auto vb = v[idx_b];
			const auto vc = v[idx_c];
			const auto ca = v[idx_a].c;
			const auto cb = v[idx_b].c;
			const auto cc = v[idx_c].c;

			if (trianglesHaveNormal)
				GLNormal(n);
			if (verticesHaveTextur && !trianglesHaveTexture)
				glTexCoord2d(va.u, va.v);
			if (trianglesHaveTexture)
				glTexCoord2d(tri.tua, tri.tva);
			if (verticesHaveColor)
				GLColor(ca);
			GLVertex(va);
			if (verticesHaveTextur && !trianglesHaveTexture)
				glTexCoord2d(vb.u, vb.v);
			if (trianglesHaveTexture)
				if (tri.flip)
					glTexCoord2d(tri.tuc, tri.tvc);
				else
					glTexCoord2d(tri.tub, tri.tvb);
			if (verticesHaveColor)
				GLColor(cb);
			GLVertex(vb);
			if (verticesHaveTextur && !trianglesHaveTexture)
				glTexCoord2d(vc.u, vc.v);
			if (tri.flip)
				glTexCoord2d(tri.tub, tri.tvb);
			else
				glTexCoord2d(tri.tuc, tri.tvc);
			if (verticesHaveColor)
				GLColor(cc);
			GLVertex(vc);
		}
	}
	glEnd();
	glPopName();

	if (paintNormals && trianglesHaveNormal) {
		glBegin(GL_LINES);
		for (const Triangle &tri : t) {
			GLNormal(tri.n);
			const Vector3 center = (v[tri.va] + v[tri.vb] + v[tri.vc]) / 3.0;
			glColor3f(1.0, 0.0, 0.0);
			GLVertex(center - tri.n * normalscale * 0.0);
			glColor3f(0.0, 1.0, 0.0);
			GLVertex(center + tri.n * normalscale);

		}
		glEnd();
	}
	glPopMatrix();
}

void Geometry::PaintEdges(const std::set<size_t> &sel, bool invert) const {
	const double normalscale = 0.05;
	glPushMatrix();
	matrix.GLMultMatrix();
	GLuint group = 0;
	bool skip = false;
	glPushName(group);
	glBegin(GL_LINES);
	if (smooth) {
		for (const Edge &ed : e) {
			if (ed.group != group) {
				group = (GLuint) (ed.group % (nothing));
				skip = ((!invert && sel.find(group) == sel.end())
						|| (invert && sel.find(group) != sel.end()));
				if (skip)
					continue;
				glEnd();
				glLoadName(group);
				glBegin(GL_LINES);
			}
			if (skip)
				continue;
			if (!ed.sharp)
				continue;

			if (edgesHaveColor && !verticesHaveColor)
				GLColor(ed.c);
			if (edgesHaveNormal && !verticesHaveNormal)
				GLNormal(ed.n);
			if (verticesHaveColor)
				GLColor(v[ed.va].c);
			if (verticesHaveNormal)
				GLNormal(v[ed.va]);
			GLVertex(v[ed.va]);
			if (verticesHaveColor)
				GLColor(v[ed.vb].c);
			if (verticesHaveNormal)
				GLNormal(v[ed.vb]);
			GLVertex(v[ed.vb]);
		}
	} else {
		for (const Edge &ed : e) {
			if (ed.group != group) {
				group = (GLuint) (ed.group % (nothing));
				skip = ((!invert && sel.find(group) == sel.end())
						|| (invert && sel.find(group) != sel.end()));
				if (skip)
					continue;
				glEnd();
				glLoadName(group);
				glBegin(GL_LINES);
			}
			if (skip)
				continue;
//			if (!ed.sharp)
//				continue;

//			const double a = 2.0 * M_PI / 20.0 * (double) i;
//			glColor3f(0.5 + cos(a) * 0.5, 0.5 + cos(a + M_PI * 2.0 / 3.0) * 0.5,
//					0.5 + cos(a + M_PI * 4.0 / 3.0) * 0.5);
//			glColor3f(0.8, 0.8, 0.8);
			if (edgesHaveColor)
				GLColor(ed.c);
			if (edgesHaveNormal)
				GLNormal(ed.n);
			if (!edgesHaveColor && verticesHaveColor)
				GLColor(v[ed.va].c);
			if (!edgesHaveNormal && verticesHaveNormal)
				GLNormal(v[ed.va]);
			GLVertex(v[ed.va]);
			if (!edgesHaveColor && verticesHaveColor)
				GLColor(v[ed.vb].c);
			if (!edgesHaveNormal && verticesHaveNormal)
				GLNormal(v[ed.vb]);
			GLVertex(v[ed.vb]);
		}
	}
	glEnd();
	glPopName();

	if (paintDirection) {
		for (const auto &ed : e) {
			size_t idx0 = ed.GetVertexIndex(0);
			size_t idx1 = ed.GetVertexIndex(1);
			Vertex v0 = v[idx0];
			Vertex v1 = v[idx1];
			Vector3 d = v1 - v0;
			double s = d.Abs();
			AffineTransformMatrix t;
			t.SetOrigin(v0);
			t.SetEx(d.Normal());
			t.SetEy(d.Orthogonal());
			t.CalculateEz();
			t.ScaleLocal(s, s, s);
			if (edgesHaveColor)
				GLColor(ed.c);
			glBegin(GL_TRIANGLE_FAN);
			GLNormal(t.TransformWithoutShift(1, 0, 0).Normal());
			GLVertex(t(0.7, 0, 0));
			for (double a = 0.0; a < 2 * M_PI; a += M_PI / 3.0) {
				GLNormal(
						t.TransformWithoutShift(0.0, 0.2 * cos(a), 0.2 * sin(a)).Normal());
				GLVertex(t(0.3, 0.2 * cos(a), 0.2 * sin(a)));
			}
			glEnd();
		}
	}

	if (paintNormals) {
		glBegin(GL_LINES);
		if (edgesHaveNormal) {
			for (const auto &ed : e) {
				if (edgesHaveColor)
					GLColor(ed.c);
				GLNormal(ed.n);
				const Vector3 center = (v[ed.va] + v[ed.vb]) / 2.0;
				GLVertex(center);
				GLVertex(center + ed.n * normalscale);
			}
		} else if (verticesHaveNormal) {
			for (const auto &vert : v) {
				if (verticesHaveColor)
					GLColor(vert.c);
				GLNormal(vert.n);
				GLVertex(vert);
				GLVertex(vert + vert.n * normalscale);
			}
		}
		glEnd();
	}
	glPopMatrix();
}

void Geometry::PaintVertices() const {

	const double normalscale = 0.1;
	glPushMatrix();
	matrix.GLMultMatrix();
	int name = 0;
	glPushName(0);
	for (const auto &vert : v) {
		glLoadName(name);
		glBegin(GL_POINTS);
		if (verticesHaveColor)
			GLColor(vert.c);
		GLNormal(vert.n);
		GLVertex(vert);
		glEnd();
		name++;
	}
	glPopName();

	if (paintNormals) {
		glBegin(GL_LINES);
		for (const auto &vert : v) {
			if (verticesHaveColor)
				GLColor(vert.c);
			GLNormal(vert.n);
			GLVertex(vert);
			glVertex3d(vert.x + vert.n.x * normalscale,
					vert.y + vert.n.y * normalscale,
					vert.z + vert.n.z * normalscale);
		}
		glEnd();
	}
	glPopMatrix();
}

void Geometry::PaintSelected() const {
	glPushMatrix();
	matrix.GLMultMatrix();
	glBegin(GL_POINTS);
	for (std::set<size_t>::iterator n = selected.begin(); n != selected.end();
			++n) {
		size_t i = *n;
		GLNormal(v[i].n);
		GLVertex(v[i]);
	}
	glEnd();
	glPopMatrix();
}

inline void Geometry::GLVertex(const Vector3 &v_) {
	glVertex3d(v_.x, v_.y, v_.z);
}

inline void Geometry::GLNormal(const Vector3 &n) {
	glNormal3d(n.x, n.y, n.z);
}

inline void Geometry::GLColor(const Color &c) {
	glColor4d(c.r, c.g, c.b, c.a);
}

void Geometry::SendToGLVertexArray(const std::string fields) {

#ifndef USE_GLAD
	return;
#endif

// Note: A glVertexAttributePointer can only handle up to 4 floats.

	std::regex reFields("XYZ|XY|X|RGBA|RGB|RG|R|T|B|N|UV|.");
	enum Op {
		XYZ, XY, X, RGBA, RGB, RG, RA, R, T, B, N, UV
	};
	std::vector<Op> ops;
	std::vector<unsigned int> widths;
	std::regex_iterator it(fields.begin(), fields.end(), reFields);
	while (it != std::sregex_iterator()) {
		if (it->str() == "X") {
			ops.push_back(X);
			widths.push_back(1);
		} else if (it->str() == "XY") {
			ops.push_back(XY);
			widths.push_back(2);
		} else if (it->str() == "XYZ") {
			ops.push_back(XYZ);
			widths.push_back(3);
		} else if (it->str() == "R") {
			ops.push_back(R);
			widths.push_back(1);
		} else if (it->str() == "RA") {
			ops.push_back(RA);
			widths.push_back(2);
		} else if (it->str() == "RG") {
			ops.push_back(RG);
			widths.push_back(2);
		} else if (it->str() == "RGB") {
			ops.push_back(RGB);
			widths.push_back(3);
		} else if (it->str() == "RGBA") {
			ops.push_back(RGBA);
			widths.push_back(4);
		} else if (it->str() == "T") {
			ops.push_back(T);
			widths.push_back(3);
		} else if (it->str() == "B") {
			ops.push_back(B);
			widths.push_back(3);
		} else if (it->str() == "N") {
			ops.push_back(N);
			widths.push_back(3);
		} else if (it->str() == "UV") {
			ops.push_back(UV);
			widths.push_back(2);
		} else {
			throw std::runtime_error(
					"Geometry::SendToGLVertexArray - Fieldname not known.");
		}
		++it;
	}

	std::vector<GLfloat> vbo;
	std::vector<GLuint> ebo;
	GLuint vidx = 0;
	for (const Triangle &tri : t) {
		for (uint_fast8_t idx = 0; idx < 3; ++idx) {
			const Vertex &vert = v[tri.GetVertexIndex(idx)];
			for (const Op op : ops) {
				switch (op) {
				case Op::X:
					vbo.push_back(vert.x);
					break;
				case Op::XY:
					vbo.push_back(vert.x);
					vbo.push_back(vert.y);
					break;
				case Op::XYZ:
					vbo.push_back(vert.x);
					vbo.push_back(vert.y);
					vbo.push_back(vert.z);
					break;
				case Op::R:
					vbo.push_back(vert.c.r);
					break;
				case Op::RA:
					vbo.push_back(vert.c.r);
					vbo.push_back(vert.c.a);
					break;
				case Op::RG:
					vbo.push_back(vert.c.r);
					vbo.push_back(vert.c.g);
					break;
				case Op::RGB:
					vbo.push_back(vert.c.r);
					vbo.push_back(vert.c.g);
					vbo.push_back(vert.c.b);
					break;
				case Op::RGBA:
					vbo.push_back(vert.c.r);
					vbo.push_back(vert.c.g);
					vbo.push_back(vert.c.b);
					vbo.push_back(vert.c.a);
					break;
				case Op::UV:
					if (idx == 0) {
						vbo.push_back(tri.tua);
						vbo.push_back(tri.tva);
					} else if (idx == 1) {
						if (tri.flip) {
							vbo.push_back(tri.tuc);
							vbo.push_back(tri.tvc);
						} else {
							vbo.push_back(tri.tub);
							vbo.push_back(tri.tvb);
						}
					} else {
						if (tri.flip) {
							vbo.push_back(tri.tub);
							vbo.push_back(tri.tvb);
						} else {
							vbo.push_back(tri.tuc);
							vbo.push_back(tri.tvc);
						}
					}
					break;
				case Op::T:
					vbo.push_back(tri.t.x);
					vbo.push_back(tri.t.y);
					vbo.push_back(tri.t.z);
					break;
				case Op::B:
					vbo.push_back(tri.b.x);
					vbo.push_back(tri.b.y);
					vbo.push_back(tri.b.z);
					break;
				case Op::N:
					vbo.push_back(tri.n.x);
					vbo.push_back(tri.n.y);
					vbo.push_back(tri.n.z);
//					vbo.push_back(vert.n.x);
//					vbo.push_back(vert.n.y);
//					vbo.push_back(vert.n.z);
					break;
				default:
					throw std::runtime_error(
							"Geometry::SendToGLVertexArray - Missing operation.");
				}
			}
			ebo.push_back(vidx);
			++vidx;
		}
	}

	if (vertexArrayObject == 0)
		glGenVertexArrays(1, &vertexArrayObject);
	if (vertexBufferObject == 0)
		glGenBuffers(1, &vertexBufferObject);
	if (elementBufferObject == 0)
		glGenBuffers(1, &elementBufferObject);

	Bind();

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * vbo.size(), vbo.data(),
	GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * ebo.size(),
			ebo.data(),
			GL_STATIC_DRAW);

	const unsigned int width = std::accumulate(widths.begin(), widths.end(),
			(unsigned int) 0);
	unsigned int offs = 0;
	for (unsigned int i = 0; i < widths.size(); ++i) {
		glVertexAttribPointer(i, widths[i], GL_FLOAT, GL_FALSE,
				width * sizeof(GLfloat), (void*) (offs * sizeof(GLfloat)));
		glEnableVertexAttribArray(i);
		offs += widths[i];
	}
}

void Geometry::DeleteGLVertexArray() {
	if (vertexArrayObject > 0) {
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &vertexArrayObject);
		vertexArrayObject = 0;
	}
	if (elementBufferObject > 0) {
		glDeleteBuffers(1, &elementBufferObject);
		elementBufferObject = 0;
	}
	if (vertexBufferObject > 0) {
		glDeleteBuffers(1, &vertexBufferObject);
		vertexBufferObject = 0;
	}
}

void Geometry::Bind() {
	glBindVertexArray(vertexArrayObject);
}

//bool Geometry::Triangle::FromString(const std::string &string) {
//
//	//TODO: Move the regex into a global static variable when this function is really used a lot.
//	// Regex for a Vector3: (\\{[^\\{]+\\})
//	std::regex e(
//			"^\\{p:\\[(\\{[^\\{]+\\}),(\\{[^\\{]+\\}),(\\{[^\\{]+\\})\\],n:\\[(\\{[^\\{]+\\}),(\\{[^\\{]+\\}),(\\{[^\\{]+\\})\\],c:\\[(\\{[^\\{]+\\}),(\\{[^\\{]+\\}),(\\{[^\\{]+\\})\\]\\}$");
//	std::smatch sm;
//	std::regex_match(string.begin(), string.end(), sm, e);
//
//	if (sm.size() != 9)
//		return false;
//	p[0] = Vector3(sm[0]);
//	p[1] = Vector3(sm[1]);
//	p[2] = Vector3(sm[2]);
//	n[0] = Vector3(sm[3]);
//	n[1] = Vector3(sm[4]);
//	n[2] = Vector3(sm[5]);
//	c[0] = Vector3(sm[6]);
//	c[1] = Vector3(sm[7]);
//	c[2] = Vector3(sm[8]);
//	return true;
//}
//
//std::string Geometry::Triangle::ToString() const {
//	std::ostringstream os;
//	os << "{p:[";
//	os << p[0].ToString() << ',' << p[1].ToString() << ',' << p[2].ToString();
//	os << "],n:[";
//	os << n[0].ToString() << ',' << n[1].ToString() << ',' << n[2].ToString();
//	os << "],c:[";
//	os << c[0].ToString() << ',' << c[1].ToString() << ',' << c[2].ToString();
//	os << "]}";
//	return os.str();
//}

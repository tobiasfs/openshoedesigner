///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleFlatten.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 29.12.2024
// Copyright          : (C) 2024 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "InsoleFlatten.h"

#include <cfloat>
#include <sstream>
#include <stdexcept>
#include <iostream>

InsoleFlatten::InsoleFlatten() {
	out = std::make_shared<Insole>();
}

std::string InsoleFlatten::GetName() const {
	return "InsoleFlatten";
}

bool InsoleFlatten::CanRun() {
	std::string missing;

	if (!in)
		missing += missing.empty() ? "\"in\"" : ", \"in\"";
	if (!out)
		missing += missing.empty() ? "\"out\"" : ", \"out\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	return error.empty();
}

bool InsoleFlatten::Propagate() {
	if (!in || !out)
		return false;

	bool modify = false;

	if (!in->IsValid()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	if (out->IsNeeded()) {
		modify |= !in->IsNeeded();
		in->MarkNeeded(true);
	}
	return modify;
}

bool InsoleFlatten::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void InsoleFlatten::Run() {
	*out = *in;

	out->MarkValid(true);
	out->MarkNeeded(false);
	return;

	//TODO Bugfix and activate below

	// Find the start triangle closest to the origin.
	size_t n;
	for (n = 0; n < out->TriangleCount(); ++n) {
		const auto &v0 = out->GetTriangleVertex(n, 0);
		const auto &v1 = out->GetTriangleVertex(n, 1);
		const auto &v2 = out->GetTriangleVertex(n, 2);
		if (v0.x < 0.0 && v1.x < 0.0 && v2.x < 0.0)
			continue;
		if (v0.x > 0.0 && v1.x > 0.0 && v2.x > 0.0)
			continue;
		break;
	}
	if (n >= out->TriangleCount()) {
		out->MarkNeeded(false);
		error += GetName() + ": The Insole does not reach to the origin.";
		return;
	}

	// UV-Map the insole starting from the start triangle
	// Reset UV Map
	for (n = 0; n < out->VertexCount(); ++n) {
		auto &v = out->GetVertex(n);
		v.u = -DBL_MAX;
		v.v = -DBL_MAX;
	}

	// Map the start triangle.
	{
		const auto &t0 = out->GetTriangle(n);
		Geometry::Vertex &v0 = out->GetVertex(t0.va);
		Geometry::Vertex &v1 = out->GetVertex(t0.vb);
		Geometry::Vertex &v2 = out->GetVertex(t0.vc);

		AffineTransformMatrix m;
		m.SetEz(t0.n);
		m.CalculateEx();
		m.CalculateEy();
		m.SetOrigin(
				{ 0, 0, v0.z + v0.x * t0.n.x / t0.n.z + v0.y * t0.n.y / t0.n.z });

		v0.u = m.LocalX(v0);
		v1.u = m.LocalX(v1);
		v2.u = m.LocalX(v2);
		v0.v = m.LocalY(v0);
		v1.v = m.LocalY(v1);
		v2.v = m.LocalY(v2);
	}

	bool again = true;

	while (again) {

		for (size_t m2 = 0; m2 < out->TriangleCount(); ++m2) {
			again = false;
			const auto &t = out->GetTriangle(m2);
			Geometry::Vertex &v0 = out->GetVertex(t.va);
			Geometry::Vertex &v1 = out->GetVertex(t.vb);
			Geometry::Vertex &v2 = out->GetVertex(t.vc);

			if (v0.u < -FLT_MAX && v0.v < -FLT_MAX && v1.u < -FLT_MAX
					&& v1.v < -FLT_MAX && v2.u < -FLT_MAX && v2.v < -FLT_MAX)
				continue;
			if (v0.u >= -FLT_MAX && v0.v >= -FLT_MAX && v1.u >= -FLT_MAX
					&& v1.v >= -FLT_MAX && v2.u >= -FLT_MAX && v2.v >= -FLT_MAX)
				continue;

			again = true;
			AffineTransformMatrix m;
			m.SetEz(t.n);
			m.CalculateEx();
			m.CalculateEy();

			if (v0.u > -FLT_MAX && v0.v > -FLT_MAX) {
				Vector3 c = m.Transform(v0.u, v0.v);
				m.SetOrigin(-c);
			}
			if (v1.u > -FLT_MAX && v1.v > -FLT_MAX) {
				Vector3 c = m.Transform(v1.u, v1.v);
				m.SetOrigin(-c);
			}
			if (v2.u > -FLT_MAX && v2.v > -FLT_MAX) {
				Vector3 c = m.Transform(v2.u, v2.v);
				m.SetOrigin(-c);
			}
			if (v0.u < -FLT_MAX || v0.v < -FLT_MAX) {
				v0.u = m.LocalX(v0);
				v0.v = m.LocalY(v0);
			}
			if (v1.u < -FLT_MAX || v1.v < -FLT_MAX) {
				v1.u = m.LocalX(v1);
				v1.v = m.LocalY(v1);
			}
			if (v2.u < -FLT_MAX || v2.v < -FLT_MAX) {
				v2.u = m.LocalX(v2);
				v2.v = m.LocalY(v2);
			}
		}
	}

	for (n = 0; n < out->VertexCount(); ++n) {
		auto &v = out->GetVertex(n);
		v.x = v.u;
		v.y = v.v;
		v.z = 0.0;
	}

	out->MarkValid(true);
	out->MarkNeeded(false);
}

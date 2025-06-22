///////////////////////////////////////////////////////////////////////////////
// Name               : InsoleFlatten.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -isystem /usr/include/eigen3
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

#include "../../math/EnergyRelease.h"
#include "../../math/Exporter.h"
#include "../../math/Kernel.h"
#include "../../math/PCA.h"
#ifdef USE_EIGEN
#include <Eigen/Dense>
#else
#include "../math/SVD.h"
#endif

#include <algorithm>
#include <cfloat>
#include <cmath>
#include <iostream>
#include <sstream>
#include <stdexcept>

using std::min;

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
	if (!debugMIDI_48)
		missing += missing.empty() ? "\"debugMIDI_48\"" : ", \"debugMIDI_48\"";
	if (!debugMIDI_49)
		missing += missing.empty() ? "\"debugMIDI_49\"" : ", \"debugMIDI_49\"";
	if (!debugMIDI_50)
		missing += missing.empty() ? "\"debugMIDI_50\"" : ", \"debugMIDI_50\"";
	if (!debugMIDI_51)
		missing += missing.empty() ? "\"debugMIDI_51\"" : ", \"debugMIDI_51\"";
	if (!debugMIDI_52)
		missing += missing.empty() ? "\"debugMIDI_52\"" : ", \"debugMIDI_52\"";
	if (!debugMIDI_53)
		missing += missing.empty() ? "\"debugMIDI_53\"" : ", \"debugMIDI_53\"";
	if (!debugMIDI_54)
		missing += missing.empty() ? "\"debugMIDI_54\"" : ", \"debugMIDI_54\"";
	if (!debugMIDI_55)
		missing += missing.empty() ? "\"debugMIDI_55\"" : ", \"debugMIDI_55\"";

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
	if (!in || !out || !debugMIDI_48 || !debugMIDI_49 || !debugMIDI_50
			|| !debugMIDI_51 || !debugMIDI_52 || !debugMIDI_53 || !debugMIDI_54
			|| !debugMIDI_55)
		return false;

	bool parameterModified = false;
	parameterModified |= !in->IsValid();
	parameterModified |= debugMIDI_48->IsModified();
	parameterModified |= debugMIDI_49->IsModified();
	parameterModified |= debugMIDI_50->IsModified();
	parameterModified |= debugMIDI_51->IsModified();
	parameterModified |= debugMIDI_52->IsModified();
	parameterModified |= debugMIDI_53->IsModified();
	parameterModified |= debugMIDI_54->IsModified();
	parameterModified |= debugMIDI_55->IsModified();

	bool modify = false;
	if (parameterModified) {
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

	// Find the uniform axis of the insole.
	// This is the axis along which the insole is NOT bend.
	out->Geometry::CalculateNormals();
	AffineTransformMatrix m;

#ifdef USE_EIGEN
	Eigen::MatrixXd M;
	M.resize(out->CountTriangles(), 3);

	// Transpose and map to Eigen matrices.
	for (size_t i = 0; i < out->CountTriangles(); i++) {
		const Geometry::Triangle &t = out->GetTriangle(i);
		const double a = out->GetTriangleArea(i);
		M(i, 0) = t.n.x * a;
		M(i, 1) = t.n.y * a;
		M(i, 2) = t.n.z * a;
	}

	Eigen::BDCSVD<Eigen::MatrixXd> svd(M, Eigen::ComputeThinV);

	Eigen::MatrixXd S = svd.singularValues();
	Eigen::MatrixXd V = svd.matrixV();
	// Eigen guarantees, that the eigenvalues are always in descending order.

	DEBUGOUT << "Eigenvalues:\n" << S << '\n';
	DEBUGOUT << "V - Matrix:\n" << V << '\n';

	// The column in V for which the eigenvalue in S is smallest is the axis
	// that is least (or not at all) bent. The major eigenvalue points in the
	// main surface direction. The middle eigenvalue is the one, that the
	// U values are ordered in. Thus the mapping is YXZ: XY -> U, Z -> V.
	m.SetEy( { V(0, 0), V(1, 0), V(2, 0) });
	m.SetEx( { V(0, 1), V(1, 1), V(2, 1) });
	m.SetEz( { V(0, 2), V(1, 2), V(2, 2) });

#else
	{
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " - ";
		err << "Uniform axis detection not implemented without using Eigen3.";
		error = err.str();
		throw std::logic_error(err.str());
	}
#endif
	Vector3 uniqueDimension = m.GetEz().Normal();
	m.Invert();
	EnergyRelease::InitByUniformDimension(*out, m);

	// The UV coordinates are copied to XY with Z = 0.
	for (size_t idx = 0; idx < out->CountVertices(); idx++) {
		auto &v = out->GetVertex(idx);
		v.x = v.u;
		v.y = v.v;
		v.z = 0.0;
	}
	// Move the UV values from the vertices to the triangles
	out->FlagUV(true, false);
	out->CalculateUVCoordinateSystems();

	// Fix the normal vectors for the triangles

	std::vector<double> areas(out->CountTriangles(), 0.0);
	for (size_t idx = 0; idx < out->CountTriangles(); idx++)
		areas[idx] = out->GetTriangleArea(idx);
	std::vector<double> defect(out->CountTriangles(), 0.0);
	for (size_t idx = 0; idx < out->CountTriangles(); idx++) {
		Geometry::Triangle &t = out->GetTriangle(idx);
		Vector3 local = m.Transform(t.n);
		// Check if the normal vector points into the unique dimension
		// or the has a negative normal. To determine this, the rotation of the
		// normals around the main axis of the insole is calculated. This
		// should be close to zero.
		defect[idx] = atan2(local.z, local.y);
	}
	for (size_t idx = 0; idx < out->CountTriangles(); idx++) {
		Geometry::Triangle &t = out->GetTriangle(idx);
		// Collapse the unique dimension for all normal-vectors.
		t.n = (t.n - uniqueDimension * t.n.Dot(uniqueDimension)).Normal();
	}
	// Replace the defect normals using interpolation over nearby (in U)
	// triangles.
	Kernel::Function k = Kernel::Stretch(Kernel::Triweight, 0.1);
	const double defectLimit = 0.01;
	for (size_t idx = 0; idx < out->CountTriangles(); idx++) {
		if (fabs(defect[idx]) < defectLimit)
			continue;
		Geometry::Triangle &t = out->GetTriangle(idx);
		const double u0 = (t.tua + t.tub + t.tuc) / 3.0;
		Vector3 n1;
		double L = 0.0;
		for (size_t idx1 = 0; idx1 < out->CountTriangles(); idx1++) {
			if (defect[idx1] >= defectLimit)
				continue;
			Geometry::Triangle &t1 = out->GetTriangle(idx1);
			const double u1 = (t1.tua + t1.tub + t1.tuc) / 3.0;
			const double w = k(u1 - u0);
			n1 += t1.n * w;
			L += w;
		}
		if (fabs(L) > FLT_EPSILON)
			t.n = n1.Normal();
	}

	// Move the normals and the UV from the vertices to the edges
	out->UpdateNormals(true, true, false);

	out->outline.Clear();
	out->outline.ExtractOutline(*out);
	// At this point the insole-outline has also the XY coordinates in UV.

	// Orient outline mathematically positive.
	// Each edge points mathematically positive direction in relation to the
	// area-center of the polygon.
	// Top-view:
	//  +--<--+
	//  |     |
	//  +-->--+

	Vector3 axis = out->outline.GetRotationalAxis();
	if (axis.z < 0.0)
		out->outline.Reverse();

#ifdef DEBUG
	{
		Matrix M1("out", out->CountVertices(), 8);
		for (size_t idx = 0; idx < out->CountVertices(); idx++) {
			const auto &v = out->GetVertex(idx);
			M1.Insert(v.x, idx, 0);
			M1.Insert(v.y, idx, 1);
			M1.Insert(v.z, idx, 2);
			M1.Insert(v.n.x, idx, 3);
			M1.Insert(v.n.y, idx, 4);
			M1.Insert(v.n.z, idx, 5);
			M1.Insert(v.u, idx, 6);
			M1.Insert(v.v, idx, 7);
		}
		Matrix M2("outline", out->outline.CountVertices(), 8);
		for (size_t idx = 0; idx < out->outline.CountVertices(); idx++) {
			const auto &v = out->outline.GetVertex(idx);
			M2.Insert(v.x, idx, 0);
			M2.Insert(v.y, idx, 1);
			M2.Insert(v.z, idx, 2);
			M2.Insert(v.n.x, idx, 3);
			M2.Insert(v.n.y, idx, 4);
			M2.Insert(v.n.z, idx, 5);
			M2.Insert(v.u, idx, 6);
			M2.Insert(v.v, idx, 7);
		}

		Exporter ex("/tmp/insoleflatten.mat");
		ex.WriteMatrix(M1);
		ex.WriteMatrix(M2);
	}
#endif

	out->MarkValid(true);
	out->MarkNeeded(false);
}

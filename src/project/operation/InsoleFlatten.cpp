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

#include "../../math/EnergyRelease.h"
#include "../../math/Exporter.h"
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

	if (debugMIDI_48->GetString().empty()) {
		error += " Input \"debugMIDI_48\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_49->GetString().empty()) {
		error += " Input \"debugMIDI_49\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_50->GetString().empty()) {
		error += " Input \"debugMIDI_50\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_51->GetString().empty()) {
		error += " Input \"debugMIDI_51\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_52->GetString().empty()) {
		error += " Input \"debugMIDI_52\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_53->GetString().empty()) {
		error += " Input \"debugMIDI_53\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_54->GetString().empty()) {
		error += " Input \"debugMIDI_54\" for InsoleFlatten is empty.";
	}
	if (debugMIDI_55->GetString().empty()) {
		error += " Input \"debugMIDI_55\" for InsoleFlatten is empty.";
	}

	return error.empty();
}

bool InsoleFlatten::Propagate() {
	if (!in || !out || !debugMIDI_48 || !debugMIDI_49 || !debugMIDI_50
			|| !debugMIDI_51 || !debugMIDI_52 || !debugMIDI_53 || !debugMIDI_54
			|| !debugMIDI_55)
		return false;

	bool modify = false;
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
	out->Geometry::CalculateNormals();

#ifdef USE_EIGEN

	Eigen::MatrixXd M;
	M.resize(out->CountTriangles(), 3);

	// Transpose and map to Eigen matrices.
	size_t p = 0;
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

//	Eigen::MatrixXd pinvM = M.completeOrthogonalDecomposition().pseudoInverse();
//	Eigen::MatrixXd S = Eigen::MatrixXd::Identity(3, 3) - pinvM * M;

	std::cout << "Eigenvalues:\n" << S << '\n';
	std::cout << "V - Matrix:\n" << V << '\n';

	//TODO: Use the result above to calculate the uniform dimension.
	EnergyRelease::InitByUniformDimension(*out, { 0, 1, 0 });
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
	out->outline.Clear();
	out->outline.ExtractOutline(*out);
	out->CalculateUVCoordinateSystems();

#ifdef DEBUG
	{
		Matrix M1("out", out->CountVertices(), 5);
		for (size_t idx = 0; idx < out->CountVertices(); idx++) {
			const auto &v = out->GetVertex(idx);
			M1.Insert(v.x, idx, 0);
			M1.Insert(v.y, idx, 1);
			M1.Insert(v.z, idx, 2);
			M1.Insert(v.u, idx, 3);
			M1.Insert(v.v, idx, 4);
		}
		Matrix M2("outline", out->outline.CountVertices(), 5);
		for (size_t idx = 0; idx < out->outline.CountVertices(); idx++) {
			const auto &v = out->outline.GetVertex(idx);
			M2.Insert(v.x, idx, 0);
			M2.Insert(v.y, idx, 1);
			M2.Insert(v.z, idx, 2);
			M2.Insert(v.u, idx, 3);
			M2.Insert(v.v, idx, 4);
		}

		Exporter ex("/tmp/insoleflatten.mat");
		ex.WriteMatrix(M1);
		ex.WriteMatrix(M2);
	}
#endif

	out->MarkValid(true);
	out->MarkNeeded(false);
}

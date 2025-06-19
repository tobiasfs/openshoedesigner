///////////////////////////////////////////////////////////////////////////////
// Name               : HeelExtractInsole.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 27.12.2024
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
#include "HeelExtractInsole.h"

#include "../../3D/FileSTL.h"
#include "../../math/DependentVector.h"
#include "../../math/Exporter.h"

#include <sstream>
#include <stdexcept>

HeelExtractInsole::HeelExtractInsole() {
	out = std::make_shared<Insole>();
	debug = std::make_shared<Matrix>();
}

std::string HeelExtractInsole::GetName() const {
	return "HeelExtractInsole";
}

bool HeelExtractInsole::CanRun() {
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

bool HeelExtractInsole::Propagate() {
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

bool HeelExtractInsole::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void HeelExtractInsole::Run() {

	if (in->PassedSelfCheck()) {
		DEBUGOUT << "in is OK\n";
	} else {
		DEBUGOUT << "in is NOK\n";
	}

	const Vector3 up(0.707, 0.0, 0.707);

	in->CalculateGroups(22.5 / 180.0 * M_PI);
	in->SelectFacesCloseTo(up);

	out->Clear();
	out->AddSelectedFrom(*in);
	if (out->PassedSelfCheck()) {
		DEBUGOUT << "out is OK\n";
	} else {
		DEBUGOUT << "out is NOK\n";
	}

#ifdef DEBUG
	// Areas of all triangles. (Broken triangles have typically a very small
	// area; they are almost collapsed. The calculation of the normal might
	// be therefore off.
	std::vector<double> areas(out->CountTriangles(), 0.0);
	for (size_t idx = 0; idx < out->CountTriangles(); idx++)
		areas[idx] = out->GetTriangleArea(idx);

	// For an example insole the sum of the areas is around 200 cm^2.
	// The median is 0.8cm^2; the mean is 2cm^2. This indicates many sliver-
	// triangles.
	std::vector<double> nlength(out->CountTriangles(), 0.0);
	for (size_t idx = 0; idx < out->CountTriangles(); idx++) {
		const Geometry::Triangle &t = out->GetTriangle(idx);
		nlength[idx] = t.n.Dot(up);
	}

	{
		Exporter ex("/tmp/tri_areas.mat");
		ex.Add(areas, "areas");
		ex.Add(nlength, "nlength");
	}
#endif

	out->UpdateNormals(true, true, false);
	out->outline.Clear();

	// Fixing and sorting is already done here, so that insole and insoleFlat
	// have the same vertex-ordering.
	out->Fix();
	out->Sort(); // This mitigates, that EnergyRelease::InitByUniformDimension is broken for certain edge-cases.

#ifdef DEBUG
	{

//		DependentVector sample = DependentVector::Chebyshev(-1, 1, 11);
//		Matrix A = Matrix::Vandermonde(sample, 3);
//		A.PseudoInvert();
//
//		const size_t Nsample = 10;
//		Polynomial poss = Polynomial::ByValue(0, 0, Nsample, L);
//
//		Matrix Y(sample.Length(), 3 * Nsample);
//		for (size_t n = 0; n < Nsample; n++) {
//			const double pos = poss(n);
//			for (size_t m = 0; m < sample.Length(); m++) {
//				const auto &res = outline.AtU(pos + sample.X()[m] * L / 8);
//				Y.Insert(res.pos.x, m, n * 3 + 0);
//				Y.Insert(res.pos.y, m, n * 3 + 1);
//				Y.Insert(res.pos.z, m, n * 3 + 2);
//			}
//		}
//		Matrix X = A * Y;
//
//		{
//			Exporter ex("/tmp/van_inv.mat");
//			ex.Add(A, "Ainv");
//			ex.Add(Y, "Y");
//			ex.Add(X, "X");
//		}
//
//		*debug = Matrix::Zeros(outline.Size(), 2);
//		for (size_t idx1 = 0; idx1 < outline.Size(); idx1++) {
//			size_t idx0 = (idx1 + outline.Size() - 6) % outline.Size();
//			size_t idx2 = (idx1 + 6) % outline.Size();
//
//			const auto v0 = outline[idx0];
//			const auto v1 = outline[idx1];
//			const auto v2 = outline[idx2];
//			// Local coordinate system
//			AffineTransformMatrix m;
//			m.SetEx((v1 - v0).Normal());
//			m.SetEz(v1.n.Normal());
//			m.CalculateEy();
//			Vector3 dv = v2 - v1;
//			double dx = dv.Dot(m.GetEx());
//			double dy = dv.Dot(m.GetEy());
//
//			debug->Insert(v1.u, idx1, 0);
//			debug->Insert(std::atan2(dy, dx), idx1, 1);
//		}
//		DEBUGOUT << "debug filled with " << outline.Size() << " values.\n";

//		FileSTL stl("/tmp/insole.stl");
//		stl.Write(*out);
	}
#endif

	out->MarkValid(true);
	out->MarkNeeded(false);
}

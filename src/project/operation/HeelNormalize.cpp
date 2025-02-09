///////////////////////////////////////////////////////////////////////////////
// Name               : HeelNormalize.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 26.12.2024
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
#include "HeelNormalize.h"

#include "../../3D/Polygon3.h"
#include "../../math/DependentVector.h"
#include "../../math/Exporter.h"
#include "../../math/Polynomial.h"

#include <stdexcept>
#include <sstream>

HeelNormalize::HeelNormalize() {
	out = std::make_shared<ObjectGeometry>();
	debug = std::make_shared<Matrix>();
}

std::string HeelNormalize::GetName() const {
	return "HeelNormalize";
}

bool HeelNormalize::CanRun() {
	std::string missing;

	if (!heelReorient)
		missing += missing.empty() ? "\"heelReorient\"" : ", \"heelReorient\"";
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

bool HeelNormalize::Propagate() {
	if (!heelReorient || !in || !out)
		return false;

	bool modify = false;

	if (!in->IsValid() || heelReorient->IsModified()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	if (out->IsNeeded()) {
		modify |= !in->IsNeeded();
		in->MarkNeeded(true);
	}
	return modify;
}

bool HeelNormalize::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void HeelNormalize::Run() {
	*out = *in;

	if (heelReorient->GetSelectionIdx() == 1) {
		AffineTransformMatrix mo;
		mo.ScaleGlobal(-1, -1, 1); //TODO Automate flipping and rotation
		out->Transform(mo);
	}

	// Coarse alignment
	{
		out->UpdateBoundingBox();

		AffineTransformMatrix m;
		m.TranslateGlobal(-out->BB.xmin, -(out->BB.ymin + out->BB.ymax) / 2.0,
				-out->BB.zmin);
		m.TranslateGlobal(-out->BB.GetSizeX() / 5, 0, 0);
		out->Transform(m);
	}

	// Extract the insole
	{
		out->CalcGroups(22.5 / 180.0 * M_PI);
		out->SelectFacesCloseTo( { 1, 0, 1 });
		Geometry insole;
		insole.AddSelectedFrom(*out);
		insole.UpdateNormals(true, true, false);
		Polygon3 outline;
		outline.ExtractOutline(insole);
		const double L = outline.MapU(false);

		DependentVector sample = DependentVector::Chebyshev(-1, 1, 11);
		Matrix A = Matrix::Vandermonde(sample, 3);
		A.PseudoInvert();

		const size_t Nsample = 10;
		Polynomial poss = Polynomial::ByValue(0, 0, Nsample, L);

		Matrix Y(sample.Length(), 3 * Nsample);
		for (size_t n = 0; n < Nsample; n++) {
			const double pos = poss(n);
			for (size_t m = 0; m < sample.Length(); m++) {
				const auto &res = outline.AtU(pos + sample.X()[m] * L / 8);
				Y.Insert(res.pos.x, m, n * 3 + 0);
				Y.Insert(res.pos.y, m, n * 3 + 1);
				Y.Insert(res.pos.z, m, n * 3 + 2);
			}
		}
		Matrix X = A * Y;

		{
			Exporter ex("/tmp/van_inv.mat");
			ex.Add(A, "Ainv");
			ex.Add(Y, "Y");
			ex.Add(X, "X");
		}

		*debug = Matrix::Zeros(outline.Size(), 2);
		for (size_t idx1 = 0; idx1 < outline.Size(); idx1++) {
			size_t idx0 = (idx1 + outline.Size() - 6) % outline.Size();
			size_t idx2 = (idx1 + 6) % outline.Size();

			const auto v0 = outline[idx0];
			const auto v1 = outline[idx1];
			const auto v2 = outline[idx2];
			// Local coordinate system
			AffineTransformMatrix m;
			m.SetEx((v1 - v0).Normal());
			m.SetEz(v1.n.Normal());
			m.CalculateEy();
			Vector3 dv = v2 - v1;
			double dx = dv.Dot(m.GetEx());
			double dy = dv.Dot(m.GetEy());

			debug->Insert(v1.u, idx1, 0);
			debug->Insert(std::atan2(dy, dx), idx1, 1);
		}
		DEBUGOUT << "debug filled with " << outline.Size() << " values.\n";
	}

	out->MarkValid(true);
	out->MarkNeeded(false);
}

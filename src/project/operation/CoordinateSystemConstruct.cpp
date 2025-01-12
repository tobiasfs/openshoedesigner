///////////////////////////////////////////////////////////////////////////////
// Name               : CoordinateSystemConstruct.cpp
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
#include "CoordinateSystemConstruct.h"

#include "../../3D/Ellipse.h"
#include <sstream>
#include <stdexcept>
#include <iostream>

CoordinateSystemConstruct::CoordinateSystemConstruct() {
	out = std::make_shared<CoordinateSystem>();
}

std::string CoordinateSystemConstruct::GetName() const {
	return "CoordinateSystemConstruct";
}

bool CoordinateSystemConstruct::CanRun() {
	std::string missing;

	if (!in)
		missing += missing.empty() ? "\"in\"" : ", \"in\"";
	if (!out)
		missing += missing.empty() ? "\"out\"" : ", \"out\"";
	if (!belowCrutchGirth)
		missing +=
				missing.empty() ?
						"\"belowCrutchGirth\"" : ", \"belowCrutchGirth\"";
	if (!belowCrutchLevel)
		missing +=
				missing.empty() ?
						"\"belowCrutchLevel\"" : ", \"belowCrutchLevel\"";
	if (!middleOfCalfGirth)
		missing +=
				missing.empty() ?
						"\"middleOfCalfGirth\"" : ", \"middleOfCalfGirth\"";
	if (!middleOfCalfLevel)
		missing +=
				missing.empty() ?
						"\"middleOfCalfLevel\"" : ", \"middleOfCalfLevel\"";
	if (!aboveKneeGirth)
		missing +=
				missing.empty() ? "\"aboveKneeGirth\"" : ", \"aboveKneeGirth\"";
	if (!aboveKneeLevel)
		missing +=
				missing.empty() ? "\"aboveKneeLevel\"" : ", \"aboveKneeLevel\"";
	if (!overKneeCapGirth)
		missing +=
				missing.empty() ?
						"\"overKneeCapGirth\"" : ", \"overKneeCapGirth\"";
	if (!overKneeCapLevel)
		missing +=
				missing.empty() ?
						"\"overKneeCapLevel\"" : ", \"overKneeCapLevel\"";
	if (!belowKneeGirth)
		missing +=
				missing.empty() ? "\"belowKneeGirth\"" : ", \"belowKneeGirth\"";
	if (!belowKneeLevel)
		missing +=
				missing.empty() ? "\"belowKneeLevel\"" : ", \"belowKneeLevel\"";
	if (!middleOfShankGirth)
		missing +=
				missing.empty() ?
						"\"middleOfShankGirth\"" : ", \"middleOfShankGirth\"";
	if (!middleOfShankLevel)
		missing +=
				missing.empty() ?
						"\"middleOfShankLevel\"" : ", \"middleOfShankLevel\"";
	if (!aboveAnkleGirth)
		missing +=
				missing.empty() ?
						"\"aboveAnkleGirth\"" : ", \"aboveAnkleGirth\"";
	if (!aboveAnkleLevel)
		missing +=
				missing.empty() ?
						"\"aboveAnkleLevel\"" : ", \"aboveAnkleLevel\"";
	if (!overAnkleBoneGirth)
		missing +=
				missing.empty() ?
						"\"overAnkleBoneGirth\"" : ", \"overAnkleBoneGirth\"";
	if (!overAnkleBoneLevel)
		missing +=
				missing.empty() ?
						"\"overAnkleBoneLevel\"" : ", \"overAnkleBoneLevel\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	if (belowCrutchGirth->GetString().empty()) {
		error +=
				" Input \"belowCrutchGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (belowCrutchLevel->GetString().empty()) {
		error +=
				" Input \"belowCrutchLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfCalfGirth->GetString().empty()) {
		error +=
				" Input \"middleOfCalfGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfCalfLevel->GetString().empty()) {
		error +=
				" Input \"middleOfCalfLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveKneeGirth->GetString().empty()) {
		error +=
				" Input \"aboveKneeGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveKneeLevel->GetString().empty()) {
		error +=
				" Input \"aboveKneeLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (overKneeCapGirth->GetString().empty()) {
		error +=
				" Input \"overKneeCapGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (overKneeCapLevel->GetString().empty()) {
		error +=
				" Input \"overKneeCapLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (belowKneeGirth->GetString().empty()) {
		error +=
				" Input \"belowKneeGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (belowKneeLevel->GetString().empty()) {
		error +=
				" Input \"belowKneeLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfShankGirth->GetString().empty()) {
		error +=
				" Input \"middleOfShankGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (middleOfShankLevel->GetString().empty()) {
		error +=
				" Input \"middleOfShankLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveAnkleGirth->GetString().empty()) {
		error +=
				" Input \"aboveAnkleGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (aboveAnkleLevel->GetString().empty()) {
		error +=
				" Input \"aboveAnkleLevel\" for CoordinateSystemConstruct is empty.";
	}
	if (overAnkleBoneGirth->GetString().empty()) {
		error +=
				" Input \"overAnkleBoneGirth\" for CoordinateSystemConstruct is empty.";
	}
	if (overAnkleBoneLevel->GetString().empty()) {
		error +=
				" Input \"overAnkleBoneLevel\" for CoordinateSystemConstruct is empty.";
	}

	return error.empty();
}

bool CoordinateSystemConstruct::Propagate() {
	if (!in || !out || !belowCrutchGirth || !belowCrutchLevel
			|| !middleOfCalfGirth || !middleOfCalfLevel || !aboveKneeGirth
			|| !aboveKneeLevel || !overKneeCapGirth || !overKneeCapLevel
			|| !belowKneeGirth || !belowKneeLevel || !middleOfShankGirth
			|| !middleOfShankLevel || !aboveAnkleGirth || !aboveAnkleLevel
			|| !overAnkleBoneGirth || !overAnkleBoneLevel)
		return false;

	bool modify = false;
	bool parameterModified = false;
	parameterModified |= !in->IsValid();
	parameterModified |= belowCrutchGirth->IsModified();
	parameterModified |= belowCrutchLevel->IsModified();
	parameterModified |= middleOfCalfGirth->IsModified();
	parameterModified |= middleOfCalfLevel->IsModified();
	parameterModified |= aboveKneeGirth->IsModified();
	parameterModified |= aboveKneeLevel->IsModified();
	parameterModified |= overKneeCapGirth->IsModified();
	parameterModified |= overKneeCapLevel->IsModified();
	parameterModified |= belowKneeGirth->IsModified();
	parameterModified |= belowKneeLevel->IsModified();
	parameterModified |= middleOfShankGirth->IsModified();
	parameterModified |= middleOfShankLevel->IsModified();
	parameterModified |= aboveAnkleGirth->IsModified();
	parameterModified |= aboveAnkleLevel->IsModified();
	parameterModified |= overAnkleBoneGirth->IsModified();
	parameterModified |= overAnkleBoneLevel->IsModified();

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

bool CoordinateSystemConstruct::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void CoordinateSystemConstruct::Run() {

	out->Clear();
	out->Setup(4, 2, -M_PI, M_PI, 0, 1);
	out->SetOrder(4, 4);
	out->ClearBoundaries();
	out->Stitch(2, true, false);
	out->SoftBoundaries();

	Polygon3 outline0;
	outline0.ExtractOutline(*in);

	// Rule the surface
	{
		Vector3 side(0, 1, 0);
		Vector3 ra = outline0.GetRotationalAxis();

		for (size_t n = 0; n < outline0.EdgeCount(); ++n) {
			Geometry::Edge &ed = outline0.GetEdge(n);
			Vector3 nv = ed.n.Normal();

			if (ra.Dot(nv) < 0.9999) {
				auto &v0 = outline0.GetEdgeVertex(n, 0);
				auto &v1 = outline0.GetEdgeVertex(n, 1);
				nv = ((v1 - v0) * side).Normal();
				if (ra.Dot(nv) < 0.0)
					nv = -nv;
				ed.n = nv;
			}
		}
		outline0.UpdateNormals(true, false, true);
	}

	//		outline0.paintNormals = true;

	double L = outline0.MapU();
	Polynomial m = Polynomial::ByValue(0, 0, L, 2.0 * M_PI);
	auto inter = outline0.Intersect( { 0, 1, 0 }, 0);
	m.ShiftX(inter.positive[0].u);
	for (size_t n = 0; n < outline0.VertexCount(); ++n) {
		auto &vert = outline0.GetVertex(n);
		vert.u = m(vert.u);
		while (vert.u > M_PI)
			vert.u -= 2.0 * M_PI;
		while (vert.u < -M_PI)
			vert.u += 2.0 * M_PI;
	}

	{
		Vector3 center = outline0.GetCenter();
		auto inter2 = outline0.Intersect( { 0, 1, 0 }, center.y);
		if (!inter2.positive.empty())
			out->Add(inter2.positive[0]);
		if (!inter2.negative.empty())
			out->Add(inter2.negative[0]);
	}
	{
		Ellipse el;
		el.SetIdentity();
		el *= AffineTransformMatrix::RotationAroundVector( { 0, 1, 0 }, -0.1);
		el.SetOrigin( { 0, 0, 0.25 });
		el.ScaleLocal(0.04, 0.02, 1.0);
		out->HardBoundaries();
		Polynomial mapu = Polynomial::ByValue(0, -M_PI, 4, M_PI);
		for (size_t n = 0; n < 4; ++n)
			out->AddPolynomial(mapu(n), mapu(n + 1), 1, 1, el.GetSpline(4, n));

		out->SoftBoundaries();
	}
	for (size_t n = 0; n < outline0.VertexCount(); ++n)
		out->Add(outline0.GetVertex(n), 0.05);
	out->Calculate();
	out->Update();

	out->MarkValid(true);
	out->MarkNeeded(false);
}


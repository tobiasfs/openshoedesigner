///////////////////////////////////////////////////////////////////////////////
// Name               : HeelCenter.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 17.03.2025
// Copyright          : (C) 2025 Tobias Schaefer <tobiassch@users.sourceforge.net>
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
#include "HeelCenter.h"
#include "../../math/Exporter.h"

#include <sstream>
#include <stdexcept>

HeelCenter::HeelCenter() {
	heel_out = std::make_shared<ObjectGeometry>();
	insole_out = std::make_shared<Insole>();
}

std::string HeelCenter::GetName() const {
	return "HeelCenter";
}

bool HeelCenter::CanRun() {
	std::string missing;

	if (!overAnkleBoneLevel)
		missing +=
				missing.empty() ?
						"\"overAnkleBoneLevel\"" : ", \"overAnkleBoneLevel\"";
	if (!heel_in)
		missing += missing.empty() ? "\"heel_in\"" : ", \"heel_in\"";
	if (!heel_out)
		missing += missing.empty() ? "\"heel_out\"" : ", \"heel_out\"";
	if (!insole_in)
		missing += missing.empty() ? "\"insole_in\"" : ", \"insole_in\"";
	if (!insole_out)
		missing += missing.empty() ? "\"insole_out\"" : ", \"insole_out\"";

	if (!missing.empty()) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << GetName() << "::"
				<< __FUNCTION__ << " -";
		err << "The variables " << missing << " are not connected.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	error.clear();

	if (overAnkleBoneLevel->GetString().empty()) {
		error += " Input \"overAnkleBoneLevel\" for HeelCenter is empty.";
	}

	return error.empty();
}

bool HeelCenter::Propagate() {
	if (!overAnkleBoneLevel || !heel_in || !heel_out || !insole_in
			|| !insole_out)
		return false;

	bool parameterModified = false;
	parameterModified |= !heel_in->IsValid();
	parameterModified |= !insole_in->IsValid();
	parameterModified |= overAnkleBoneLevel->IsModified();

	bool modify = false;
	if (parameterModified) {
		modify |= heel_out->IsValid();
		modify |= insole_out->IsValid();
		heel_out->MarkValid(false);
		insole_out->MarkValid(false);
	}
	if (heel_out->IsNeeded() || insole_out->IsNeeded()) {
		modify |= !heel_in->IsNeeded();
		modify |= !insole_in->IsNeeded();
		heel_in->MarkNeeded(true);
		insole_in->MarkNeeded(true);
	}
	return modify;
}

bool HeelCenter::HasToRun() {
	return heel_in && heel_in->IsValid() && insole_in && insole_in->IsValid()
			&& heel_out && insole_out
			&& ((!heel_out->IsValid() && heel_out->IsNeeded())
					|| (!insole_out->IsValid() && insole_out->IsNeeded()));
}

void HeelCenter::Run() {
	*heel_out = *heel_in;
	*insole_out = *insole_in;

	// Find the center of the ankle.
	const Vector3 coa = insole_in->J
			+ insole_in->J.n * overAnkleBoneLevel->ToDouble();

	AffineTransformMatrix m;
	// Describe the coordinate system forwards
	m.SetOrigin( { coa.x, coa.y, 0.0 });
	Vector3 front = insole_in->B - insole_in->A;
	front.z = 0.0;
	m.SetEx(front.Normal());
	m.SetEz( { 0, 0, 1 });

	// If the insole is a right insole, flip the coordinate system.
	Vector3 test = (insole_in->B - insole_in->A)
			* (insole_in->F - insole_in->E);
	if (test.z < 0.0)
		m.SetOrientation(AffineTransformMatrix::Orientation::LHS);
	m.CalculateEy();
	m.Invert();

	heel_out->Transform(m);
	insole_out->Transform(m);

//	insole_out->outline.paintNormals = true;
//	insole_out->paintNormals = true;
//	heel_out->paintNormals = true;

#ifdef DEBUG
	{
//		Exporter ex("/tmp/geo.mat");
////		ex.Add(*insoleFlat_out, "insoleflat");
////		ex.Add(insoleFlat_out->outline, "outlineflat");
//		ex.Add(*insole_out, "insole");
//		ex.Add(insole_out->outline, "outline");
	}
#endif

	heel_out->MarkValid(true);
	insole_out->MarkValid(true);
	heel_out->MarkNeeded(false);
	insole_out->MarkNeeded(false);
}

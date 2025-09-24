///////////////////////////////////////////////////////////////////////////////
// Name               : UpperConstruct.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 23.06.2025
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

#include "UpperConstruct.h"

#include <sstream>
#include <stdexcept>

UpperConstruct::UpperConstruct() {
	out = std::make_shared<Upper>();
}

std::string UpperConstruct::GetName() const {
	return "UpperConstruct";
}

bool UpperConstruct::CanRun() {
	std::string missing;

	if (!design_in)
		missing += missing.empty() ? "\"design_in\"" : ", \"design_in\"";
	if (!cs_in)
		missing += missing.empty() ? "\"cs_in\"" : ", \"cs_in\"";
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

bool UpperConstruct::Propagate() {
	if (!design_in || !cs_in || !out)
		return false;

	bool modify = false;
	if (!design_in->IsValid() || !cs_in->IsValid()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	if (out->IsNeeded()) {
		modify |= !design_in->IsNeeded();
		modify |= !cs_in->IsNeeded();
		design_in->MarkNeeded(true);
		cs_in->MarkNeeded(true);
	}
	return modify;
}

bool UpperConstruct::HasToRun() {
	return design_in && design_in->IsValid() && cs_in && cs_in->IsValid() && out
			&& !out->IsValid() && out->IsNeeded();
}

void UpperConstruct::Run() {
	out->patches.clear();
	design_in->Update();

//	for (const Design::Edge &e : design_in->edges) {
//		out->patches.push_back(e.geo);
//	}
	for (const Design::Patch &p : design_in->patches) {
		const Geometry *g = dynamic_cast<const Geometry*>(&p);
		out->patches.push_back(*g);
	}

	for (Geometry &geo : out->patches) {
//		geo.paintEdges = true;
		geo.FlagUV(true, false);

		cs_in->Apply(geo);

//		for (size_t vidx = 0; vidx < geo.CountVertices(); vidx++) {
//			Geometry::Vertex &v = geo.GetVertex(vidx);
//			v.x = v.u / 10.0;
//			v.z = v.v / 2.0;
//			v.y = 0.0;
//			v.n = { 0, -1, 0 };
//		}
		geo.UpdateNormals(false, true, true);
	}

	out->MarkValid(true);
	out->MarkNeeded(false);
}

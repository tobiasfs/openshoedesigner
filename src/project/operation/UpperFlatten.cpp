///////////////////////////////////////////////////////////////////////////////
// Name               : UpperFlatten.cpp
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

#include "UpperFlatten.h"

#include <sstream>
#include <stdexcept>

UpperFlatten::UpperFlatten() {
	out = std::make_shared<Upper>();
}

std::string UpperFlatten::GetName() const {
	return "UpperFlatten";
}

bool UpperFlatten::CanRun() {
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

bool UpperFlatten::Propagate() {
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

bool UpperFlatten::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void UpperFlatten::Run() {
	*out = *in;

	for (Geometry &geo : out->patches) {
		for (size_t vidx = 0; vidx < geo.CountVertices(); vidx++) {
			Geometry::Vertex &v = geo.GetVertex(vidx);
			v.u = v.u / 10.0;
			v.v = v.v / 2.0;
		}
	}

	out->MarkValid(true);
	out->MarkNeeded(false);
}

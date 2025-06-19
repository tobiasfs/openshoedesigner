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

#include <stdexcept>
#include <sstream>

HeelNormalize::HeelNormalize() {
	out = std::make_shared<ObjectGeometry>();
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

	if (in->PassedSelfCheck()) {
		DEBUGOUT << "in is OK\n";
	} else {
		DEBUGOUT << "in is NOK\n";
	}

	if (heelReorient->GetSelectionIdx() == 1) {
		AffineTransformMatrix mo;
		mo.ScaleGlobal(1, -1, 1);
		out->Transform(mo);
	}

	// Coarse alignment
	{
		out->UpdateBoundingBox();

		AffineTransformMatrix m;
		m.TranslateGlobal(-out->BB.xmin, -(out->BB.ymin + out->BB.ymax) / 2.0,
				-out->BB.zmin);
		out->Transform(m);
	}

	out->MarkValid(true);
	out->MarkNeeded(false);
}

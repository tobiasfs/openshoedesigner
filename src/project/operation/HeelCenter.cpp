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

	if (!heel_in)
		missing += missing.empty() ? "\"heel_in\"" : ", \"heel_in\"";
	if (!heel_out)
		missing += missing.empty() ? "\"heel_out\"" : ", \"heel_out\"";
	if (!insole_in)
		missing += missing.empty() ? "\"insole_in\"" : ", \"insole_in\"";
	if (!insole_out)
		missing += missing.empty() ? "\"insole_out\"" : ", \"insole_out\"";
//	if (!debug)
//		missing += missing.empty() ? "\"debug\"" : ", \"debug\"";

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

bool HeelCenter::Propagate() {
	if (!heel_in || !heel_out || !insole_in || !insole_out)
		return false;

	bool modify = false;

	if (!heel_in->IsValid() || !insole_in->IsValid()) {
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

	heel_out->MarkValid(true);
	insole_out->MarkValid(true);
	heel_out->MarkNeeded(false);
	insole_out->MarkNeeded(false);
}

///////////////////////////////////////////////////////////////////////////////
// Name               : LastConstruct.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 12.01.2025
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
#include "LastConstruct.h"

#include <sstream>
#include <iostream>
#include <stdexcept>

LastConstruct::LastConstruct() {
	out = std::make_shared<ObjectGeometry>();
}

std::string LastConstruct::GetName() const {
	return "LastConstruct";
}

bool LastConstruct::CanRun() {
	std::string missing;

	if (!upperLevel)
		missing = "\"upperLevel\"";
	if (!insole)
		missing += missing.empty() ? "\"insole\"" : ", \"insole\"";
	if (!cs)
		missing += missing.empty() ? "\"cs\"" : ", \"cs\"";
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

	if (upperLevel->GetString().empty()) {
		error += " Input \"upperLevel\" for LastConstruct is empty.";
	}

	return error.empty();
}

bool LastConstruct::Propagate() {
	if (!upperLevel || !insole || !cs || !out)
		return false;

	bool parameterModified = false;
	parameterModified |= !insole->IsValid();
	parameterModified |= !cs->IsValid();
	parameterModified |= upperLevel->IsModified();

	bool modify = false;
	if (parameterModified) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	if (out->IsNeeded()) {
		modify |= !insole->IsNeeded();
		modify |= !cs->IsNeeded();
		insole->MarkNeeded(true);
		cs->MarkNeeded(true);
	}
	return modify;
}

bool LastConstruct::HasToRun() {
	return insole && insole->IsValid() && cs && cs->IsValid() && out
			&& !out->IsValid() && out->IsNeeded();
}

void LastConstruct::Run() {
	*out = cs->ExtractByUVPlane(0, -1, -upperLevel->ToDouble());

	//TODO Extend algorithm to stitch the insole to the bottom of the last.
	DEBUGOUT << GetName() << ": Operation not fully implemented.\n";

	out->MarkValid(true);
	out->MarkNeeded(false);
}

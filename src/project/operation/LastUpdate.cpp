///////////////////////////////////////////////////////////////////////////////
// Name               : LastUpdate.cpp
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
#include "LastUpdate.h"

#include <sstream>
#include <stdexcept>

LastUpdate::LastUpdate() {
	out = std::make_shared<LastModel>();
}

std::string LastUpdate::GetName() const {
	return "LastUpdate";
}

bool LastUpdate::CanRun() {
	std::string missing;

	if (!lastModify)
		missing += missing.empty() ? "\"lastModify\"" : ", \"lastModify\"";
	if (!heelPitch)
		missing += missing.empty() ? "\"heelPitch\"" : ", \"heelPitch\"";
	if (!toeSpring)
		missing += missing.empty() ? "\"toeSpring\"" : ", \"toeSpring\"";
	if (!heelHeight)
		missing += missing.empty() ? "\"heelHeight\"" : ", \"heelHeight\"";
	if (!ballHeight)
		missing += missing.empty() ? "\"ballHeight\"" : ", \"ballHeight\"";
	if (!legLengthDifference)
		missing +=
				missing.empty() ?
						"\"legLengthDifference\"" : ", \"legLengthDifference\"";
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

	if (heelPitch->GetString().empty()) {
		error += " Input \"heelPitch\" for LastUpdate is empty.";
	}
	if (toeSpring->GetString().empty()) {
		error += " Input \"toeSpring\" for LastUpdate is empty.";
	}
	if (heelHeight->GetString().empty()) {
		error += " Input \"heelHeight\" for LastUpdate is empty.";
	}
	if (ballHeight->GetString().empty()) {
		error += " Input \"ballHeight\" for LastUpdate is empty.";
	}
	if (legLengthDifference->GetString().empty()) {
		error += " Input \"legLengthDifference\" for LastUpdate is empty.";
	}

	return error.empty();
}

bool LastUpdate::Propagate() {
	if (!lastModify || !heelPitch || !toeSpring || !heelHeight || !ballHeight
			|| !legLengthDifference || !in || !out)
		return false;

	bool modify = false;
	bool parameterModified = false;
	parameterModified |= !in->IsValid();
	parameterModified |= lastModify->IsModified();
	parameterModified |= heelPitch->IsModified();
	parameterModified |= toeSpring->IsModified();
	parameterModified |= heelHeight->IsModified();
	parameterModified |= ballHeight->IsModified();
	parameterModified |= legLengthDifference->IsModified();

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

bool LastUpdate::HasToRun() {
	return in && in->IsValid() && out && !out->IsValid() && out->IsNeeded();
}

void LastUpdate::Run() {
	*out = *in;

	std::cerr << GetName() << ": Operation not implemented.\n";

	out->MarkValid(true);
	out->MarkNeeded(false);
}

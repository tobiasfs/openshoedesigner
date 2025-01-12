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

#include <sstream>
#include <stdexcept>

HeelExtractInsole::HeelExtractInsole() {
	out = std::make_shared<Insole>();
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

	in->CalcGroups(22.5 / 180.0 * M_PI);
	in->SelectFacesCloseTo( { 1, 0, 1 });
	out->Clear();
	out->AddSelectedFrom(*in);

#ifdef DEBUG
	{
//		FileSTL stl("/tmp/insole.stl");
//		stl.Write(*out);
	}
#endif

	out->MarkValid(true);
	out->MarkNeeded(false);
}

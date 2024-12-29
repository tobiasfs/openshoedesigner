///////////////////////////////////////////////////////////////////////////////
// Name               : ObjectLoad.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 10.11.2024
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

#include "ObjectLoad.h"

#include "../../3D/FileDXF.h"
#include "../../3D/FileGTS.h"
#include "../../3D/FileOBJ.h"
#include "../../3D/FilePLY.h"
#include "../../3D/FileSTL.h"
#include "../../3D/PolyCylinder.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

ObjectLoad::ObjectLoad() {
	out = std::make_shared<ObjectGeometry>();
}

std::string ObjectLoad::GetName() const {
	return "ObjectLoad";
}

bool ObjectLoad::CanRun() {
	std::string missing;

	if (!filename)
		missing += missing.empty() ? "\"filename\"" : ", \"filename\"";
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

	if (filename->GetString().empty()) {
		error += " Input \"filename\" for ObjectLoad is empty.";
	} else {
		std::filesystem::path fn(filename->GetString());
		if (!std::filesystem::exists(fn)) {
			error += " The file \"" + filename->GetString()
					+ "\" does not exist.";
		}
	}

	return error.empty();
}

bool ObjectLoad::Propagate() {
	if (!filename || !out)
		return false;

	bool modify = false;

	if (filename->IsModified() || filename->GetString().empty()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}

	if (modify)
		return modify;

	std::filesystem::path fn(filename->GetString());
	if (!std::filesystem::exists(fn)) {
		modify |= out->IsValid();
		out->MarkValid(false);
		return modify;
	}
	// Check, if the file on drive is modified.
	auto timeModified = std::filesystem::last_write_time(fn);
	if (timeModified != lastModified) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}
	return modify;
}

bool ObjectLoad::HasToRun() {
	return out && !out->IsValid() && out->IsNeeded();
}

void ObjectLoad::Run() {
	std::filesystem::path filepath(filename->GetString());
	std::string extension = filepath.extension().string();
	for (auto &ch : extension)
		ch = std::tolower(ch);

	if (extension.compare(".dxf") == 0) {
		FileDXF obj(filename->GetString());
		out->Clear();
		obj.Read(*out);
	}
	if (extension.compare(".gts") == 0) {
		FileGTS obj(filename->GetString());
		out->Clear();
		obj.Read(*out);
	}
	if (extension.compare(".obj") == 0) {
		FileOBJ obj(filename->GetString());
		out->Clear();
		obj.Read(*out);
	}
	if (extension.compare(".ply") == 0) {
		FilePLY stl(filename->GetString());
		out->Clear();
		stl.Read(*out);
	}
	if (extension.compare(".stl") == 0) {
		FileSTL stl(filename->GetString());
		out->Clear();
		stl.Read(*out);
	}

	if (extension.compare(".dat") == 0) {
		PolyCylinder pc;
		pc.Load(filepath.string());
		*out = pc.GenerateGeometry(false);
	}
	out->CalculateNormals();
	if (out->IsClosed()) {
		DEBUGOUT << "Fully closed hull loaded." << "\n";
	} else {
		DEBUGOUT << "Geometry has open edges." << "\n";
	}
	lastModified = std::filesystem::last_write_time(filepath);
	out->MarkValid(true);
	out->MarkNeeded(false);
}


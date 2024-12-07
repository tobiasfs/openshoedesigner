///////////////////////////////////////////////////////////////////////////////
// Name               : LastLoad.cpp
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
#include "LastLoad.h"

#include "../../3D/FileOBJ.h"
#include "../../3D/FileSTL.h"
#include "../../3D/PolyCylinder.h"

#include <filesystem>
#include <iostream>
#include <sstream>
#include <stdexcept>

LastLoad::LastLoad() {
	out = std::make_shared<LastRaw>();
}

bool LastLoad::CanRun() {
	if (filename && out)
		return true;
	std::ostringstream err;
	err << __FILE__ << ":" << __LINE__ << ":" << __func__ << " -";
	if (!filename)
		err << " Input \"filename\" not connected.";
	if (!out)
		err << " Output \"out\" not set.";
	throw std::runtime_error(err.str());
}

bool LastLoad::Propagate() {
	bool modify = false;
	if (!CanRun())
		return modify;
	if (filename->IsModified()) {
		modify = out->IsValid();
		out->MarkValid(false);
	}
	return modify;
}

bool LastLoad::HasToRun() {
	if (!CanRun())
		return false;
	return !out->IsValid() && out->IsNeeded();
}

void LastLoad::Run() {
	std::filesystem::path filepath(filename->GetString());
	std::string extension = filepath.extension().string();
	for (auto &ch : extension)
		ch = std::tolower(ch);

	if (extension.compare(".dat") == 0) {
		PolyCylinder pc;
		pc.Load(filepath.string());
		*out = pc.GenerateGeometry(false);
	}
	if (extension.compare(".stl") == 0) {
		FileSTL stl(filename->GetString());
		out->Clear();
		stl.Read(*out);
	}
	if (extension.compare(".obj") == 0) {
		FileOBJ obj(filename->GetString());
		out->Clear();
		obj.Read(*out);
	}
	out->CalculateNormals();
	if (out->IsClosed()) {
		DEBUGOUT << "Fully closed hull loaded." << "\n";
	} else {
		DEBUGOUT << "Geometry has open edges." << "\n";
	}
	out->MarkValid(true);
	out->MarkNeeded(false);
}


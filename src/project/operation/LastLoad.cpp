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

#include "../../Config.h"

bool LastLoad::CanRun() {
	return filename.use_count() > 0;
}

bool LastLoad::HasToRun() {
	return filename->IsModified();
}

void LastLoad::Run() {
	std::filesystem::path filepath(filename->GetString());
	std::string extension = filepath.extension().string();
	for (auto &ch : extension)
		ch = std::tolower(ch);

	if (extension.compare("dat") == 0) {
		PolyCylinder pc;
		pc.Load(filepath.string());
		raw = std::make_shared<LastRaw>(pc.GenerateGeometry(false));
	}
	if (extension.compare("stl") == 0) {
		FileSTL stl(filename->GetString());
		raw = std::make_shared<LastRaw>();
		stl.Read(*raw);
	}
	if (extension.compare("obj") == 0) {
		FileOBJ obj(filename->GetString());
		raw = std::make_shared<LastRaw>();
		obj.Read(*raw);
	}
	raw->CalculateNormals();
	if (raw->IsClosed()) {
		DEBUGOUT << "Fully closed hull loaded." << "\n";
	} else {
		DEBUGOUT << "Geometry has open edges." << "\n";
	}
	raw->Modify(true);
}

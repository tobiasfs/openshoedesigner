///////////////////////////////////////////////////////////////////////////////
// Name               : FootModelLoad.cpp
// Purpose            : 
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   : -lm
// Author             : Tobias Schaefer
// Created            : 23.12.2024
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
#include "FootModelLoad.h"

FootModelLoad::FootModelLoad() {
	out = std::make_shared<FootModel>();
}

std::string FootModelLoad::GetName() const {
	return "FootModelLoad";
}

bool FootModelLoad::CanRun() {
	if (!filename || !out) {
		std::ostringstream err;
		err << __FILE__ << ":" << __LINE__ << ":" << __FUNCTION__ << " -";
		if (!filename)
			err << " Input \"filename\" not connected.";
		if (!out)
			err << " Output \"out\" not set.";
		error = err.str();
		throw std::logic_error(err.str());
	}

	if (filename->GetString().empty()) {
		error = "Input \"filename\" is empty.";
		return false;
	}

	std::filesystem::path fn(filename->GetString());
	if (!std::filesystem::exists(fn)) {
		error = "The file " + filename->GetString() + " does not exist.";
		return false;
	}

	error.clear();
	return true;
}

bool FootModelLoad::Propagate() {
	bool modify = false;
	if (!out || !filename)
		return false;

	if (filename->IsModified()) {
		modify |= out->IsValid();
		out->MarkValid(false);
	}

	if (filename->GetString().empty()) {
		modify |= out->IsValid();
		out->MarkValid(false);
		return modify;
	}

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

bool FootModelLoad::HasToRun() {
	return out && !out->IsValid() && out->IsNeeded();
}

void FootModelLoad::Run() {
	std::filesystem::path filepath(filename->GetString());
	std::string extension = filepath.extension().string();
	for (auto &ch : extension)
		ch = std::tolower(ch);

	out->LoadJSON(filepath.string());
	try {
		out->GetBone("Tibia")->matrixinit.SetOrigin(Vector3(0, 0, 0));
	} catch (const std::exception &ex) {
		error = "The file " + filepath.string()
				+ " does not contain a \"Tibia\" bone.";
		out->Clear();
	}
	out->CalculateBones();
	out->CalculateSkin();
	out->Update();

	lastModified = std::filesystem::last_write_time(filepath);
	out->MarkValid(true);
	out->MarkNeeded(false);
}


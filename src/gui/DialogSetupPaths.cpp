///////////////////////////////////////////////////////////////////////////////
// Name               : DialogSetupPaths.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.12.2019
// Copyright          : (C) 2019 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "DialogSetupPaths.h"

#include <cstdio>
DialogSetupPaths::DialogSetupPaths(wxWindow *parent,
		const CollectionFilepaths *collection) :
		GUISetupPaths(parent) {
	m_dirPickerMeasurementsDirectory->SetPath(collection->measurementDirectory);
	m_dirPickerPresetDirectory->SetPath(collection->presetDirectory);
	m_dirPickerLastDirectory->SetPath(collection->lastDirectory);
	m_dirPickerFabricDirectory->SetPath(collection->fabricDirectory);
	m_dirPickerOutputDirectory->SetPath(collection->outputDirectory);
}

void DialogSetupPaths::OnClose(wxCommandEvent &event) {
	EndModal(wxID_OK);
}

void DialogSetupPaths::UpdateCollection(CollectionFilepaths *collection) const {
	collection->measurementDirectory =
			m_dirPickerMeasurementsDirectory->GetPath();
	collection->presetDirectory = m_dirPickerPresetDirectory->GetPath();
	collection->lastDirectory = m_dirPickerLastDirectory->GetPath();
	collection->fabricDirectory = m_dirPickerFabricDirectory->GetPath();
	collection->outputDirectory = m_dirPickerOutputDirectory->GetPath();
}


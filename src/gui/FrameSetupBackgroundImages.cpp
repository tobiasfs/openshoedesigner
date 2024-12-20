///////////////////////////////////////////////////////////////////////////////
// Name               : FrameSetupBackgroundImages.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.02.2019
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

#include "FrameSetupBackgroundImages.h"

FrameSetupBackgroundImages::FrameSetupBackgroundImages(wxWindow *parent,
		wxWindowID id, const wxString &title, const wxPoint &pos,
		const wxSize &size, long style) :
		GUIFrameSetupBackgroundImages(parent, id, title, pos, size, style) {

}

void FrameSetupBackgroundImages::OnButtonAdd(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameSetupBackgroundImages::OnButtonRemove(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameSetupBackgroundImages::OnTextEnter(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameSetupBackgroundImages::OnSpin(wxSpinEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameSetupBackgroundImages::OnRightDown(wxMouseEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameSetupBackgroundImages::OnScroll(wxScrollEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameSetupBackgroundImages::OnCheckBox(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

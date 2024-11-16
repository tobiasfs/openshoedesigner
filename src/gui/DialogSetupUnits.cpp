///////////////////////////////////////////////////////////////////////////////
// Name               : DialogSetupUnits.cpp
// Purpose            :
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 03.07.2011
// Copyright          : (C) 2011 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "../StdInclude.h"
#include "DialogSetupUnits.h"
#include "IDs.h"

DialogSetupUnits::DialogSetupUnits(wxWindow *parent, CollectionUnits *units) :
		GUIFrameSetupUnits(parent) {
	this->units = units;

	TransferDataToWindow();
}

void DialogSetupUnits::OnClose(wxCommandEvent &event) {
	Hide();
}

bool DialogSetupUnits::TransferDataToWindow() {
	int i;

	m_choiceUnitLength->Clear();
	for (const Unit &unit : units->unitsOfLength)
		m_choiceUnitLength->Append(wxString(unit.GetOtherName()));
	i = m_choiceUnitLength->FindString(units->Distance.GetOtherName());
	if (i == wxNOT_FOUND)
		i = 6;
	m_choiceUnitLength->SetSelection(i);

	m_choiceUnitSmallDistance->Clear();
	for (const Unit &unit : units->unitsOfLength)
		m_choiceUnitSmallDistance->Append(wxString(unit.GetOtherName()));

	i = m_choiceUnitSmallDistance->FindString(
			units->SmallDistance.GetOtherName());
	if (i == wxNOT_FOUND)
		i = 6;
	m_choiceUnitSmallDistance->SetSelection(i);

	m_choiceUnitAngle->Clear();
	for (const Unit &unit : units->unitsOfAngle)
		m_choiceUnitAngle->Append(wxString(unit.GetOtherName()));
	i = m_choiceUnitAngle->FindString(units->Angle.GetOtherName());
	if (i == wxNOT_FOUND)
		i = 0;
	m_choiceUnitAngle->SetSelection(i);

	return true;
}

bool DialogSetupUnits::TransferDataFromWindow() {
	int idx;

	idx = m_choiceUnitLength->GetSelection();
	units->Distance = units->unitsOfLength[idx];

	idx = m_choiceUnitSmallDistance->GetSelection();
	units->SmallDistance = units->unitsOfLength[idx];

	idx = m_choiceUnitAngle->GetSelection();
	units->Angle = units->unitsOfAngle[idx];

	return true;
}

void DialogSetupUnits::OnCloseX(wxCloseEvent &event) {
	Hide();
}

void DialogSetupUnits::OnChangeUnit(wxCommandEvent &event) {
	TransferDataFromWindow();
	wxCommandEvent selectEvent(wxEVT_COMMAND_MENU_SELECTED, ID_REFRESHALL);
	ProcessEvent(selectEvent);
}

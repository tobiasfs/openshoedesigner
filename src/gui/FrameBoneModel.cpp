///////////////////////////////////////////////////////////////////////////////
// Name               : FrameBoneModel.cpp
// Purpose            : 
// Thread Safe        : Yes
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 18.11.2018
// Copyright          : (C) 2018 Tobias Schaefer <tobiassch@users.sourceforge.net>
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

#include "FrameBoneModel.h"

FrameBoneModel::FrameBoneModel(wxWindow *parent, wxWindowID id,
		const wxString &title, const wxPoint &pos, const wxSize &size,
		long style) :
		GUIFrameBoneModel(parent, id, title, pos, size, style) {
}

bool FrameBoneModel::TransferDataToWindow() {
//	const unsigned int N = foot->GetBoneCount();
//	if(m_gridBoneLength->GetNumberRows() == 0) m_gridBoneLength->AppendRows(N);
//	for(size_t n = 0; n < foot->bones.size(); n++){
//		m_gridBoneLength->SetRowLabelValue(n, foot->bones[n].name);
//		m_gridBoneLength->SetCellValue(n, 0, foot->bones[n].lengthv);
//		m_gridBoneLength->SetCellValue(n, 1,
//				settings.SmallDistance.TextFromSIWithUnit(foot->bones[n].length,
//						1));
//		m_gridBoneLength->SetReadOnly(n, 1);
//		m_gridBoneLength->SetCellAlignment(n, 1, wxALIGN_RIGHT, wxALIGN_TOP);
//	}
//
//	if(m_gridBoneDiameter->GetNumberRows() == 0) m_gridBoneDiameter->AppendRows(
//			N);
//	for(size_t n = 0; n < foot->bones.size(); n++){
//		m_gridBoneDiameter->SetRowLabelValue(n, foot->bones[n].name);
//		m_gridBoneDiameter->SetCellValue(n, 0, foot->bones[n].r1v);
//		m_gridBoneDiameter->SetCellValue(n, 1,
//				settings.SmallDistance.TextFromSIWithUnit(foot->bones[n].r1,
//						1));
//		m_gridBoneDiameter->SetCellValue(n, 2, foot->bones[n].r2v);
//		m_gridBoneDiameter->SetCellValue(n, 3,
//				settings.SmallDistance.TextFromSIWithUnit(foot->bones[n].r2,
//						1));
//		m_gridBoneDiameter->SetReadOnly(n, 1);
//		m_gridBoneDiameter->SetReadOnly(n, 3);
//		m_gridBoneDiameter->SetCellAlignment(n, 1, wxALIGN_RIGHT, wxALIGN_TOP);
//		m_gridBoneDiameter->SetCellAlignment(n, 3, wxALIGN_RIGHT, wxALIGN_TOP);
//	}
//
//	if(m_gridSkin->GetNumberRows() == 0) m_gridSkin->AppendRows(N);
//	for(size_t n = 0; n < foot->bones.size(); n++){
//		m_gridSkin->SetRowLabelValue(n, foot->bones[n].name);
//		m_gridSkin->SetCellValue(n, 0, foot->bones[n].s1v);
//		m_gridSkin->SetCellValue(n, 1,
//				settings.SmallDistance.TextFromSIWithUnit(foot->bones[n].s1,
//						1));
//		m_gridSkin->SetCellValue(n, 2, foot->bones[n].s2v);
//		m_gridSkin->SetCellValue(n, 3,
//				settings.SmallDistance.TextFromSIWithUnit(foot->bones[n].s2,
//						1));
//		m_gridSkin->SetReadOnly(n, 1);
//		m_gridSkin->SetReadOnly(n, 3);
//		m_gridSkin->SetCellAlignment(n, 1, wxALIGN_RIGHT, wxALIGN_TOP);
//		m_gridSkin->SetCellAlignment(n, 3, wxALIGN_RIGHT, wxALIGN_TOP);
//	}
	return true;
}

bool FrameBoneModel::TransferDataFromWindow() {
//	for(size_t n = 0; n < foot->bones.size(); n++){
//		wxString temp = m_gridBoneLength->GetCellValue(n, 0);
//		temp = temp.Trim(false).Trim(true);
//		if(foot->bones[n].lengthv.Cmp(temp) != 0){
//			project->GetCommandProcessor()->Submit(
//					new CommandFootModelSetParameter(
//							_("Set ") + foot->bones[n].name
//									+ _(" length to ") + temp, project, n,
//							Bone::stringIdentifier::stringLength, temp));
//		}
//	}
//	for(size_t n = 0; n < foot->bones.size(); n++){
//		wxString temp = m_gridBoneDiameter->GetCellValue(n, 0);
//		temp = temp.Trim(false).Trim(true);
//		if(foot->bones[n].r1v.Cmp(temp) != 0){
//			project->GetCommandProcessor()->Submit(
//					new CommandFootModelSetParameter(
//					_("Set ") + foot->bones[n].name + _(" r1 to ") + temp,
//							project, n, Bone::stringIdentifier::stringR1,
//							temp));
//		}
//
//		temp = m_gridBoneDiameter->GetCellValue(n, 2);
//		temp = temp.Trim(false).Trim(true);
//		if(foot->bones[n].r2v.Cmp(temp) != 0){
//			project->GetCommandProcessor()->Submit(
//					new CommandFootModelSetParameter(
//					_("Set ") + foot->bones[n].name + _(" r2 to ") + temp,
//							project, n, Bone::stringIdentifier::stringR2,
//							temp));
//		}
//	}
//	for(size_t n = 0; n < foot->bones.size(); n++){
//		wxString temp = m_gridSkin->GetCellValue(n, 0);
//		temp = temp.Trim(false).Trim(true);
//		if(foot->bones[n].s1v.Cmp(temp) != 0){
//			project->GetCommandProcessor()->Submit(
//					new CommandFootModelSetParameter(
//					_("Set ") + foot->bones[n].name + _(" s1 to ") + temp,
//							project, n, Bone::stringIdentifier::stringS1,
//							temp));
//		}
//
//		temp = m_gridSkin->GetCellValue(n, 2);
//		temp = temp.Trim(false).Trim(true);
//		if(foot->bones[n].s2v.Cmp(temp) != 0){
//			project->GetCommandProcessor()->Submit(
//					new CommandFootModelSetParameter(
//					_("Set ") + foot->bones[n].name + _(" s2 to ") + temp,
//							project, n, Bone::stringIdentifier::stringS2,
//							temp));
//		}
//	}
	return true;
}

void FrameBoneModel::OnCellChange(wxGridEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameBoneModel::OnReset(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameBoneModel::OnLoad(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameBoneModel::OnSaveAs(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

void FrameBoneModel::OnSave(wxCommandEvent &event) {
	DEBUGOUT << "Line " << __LINE__ << ": " << __FUNCTION__ << "( " << event.GetId()
			<< " )\n";
}

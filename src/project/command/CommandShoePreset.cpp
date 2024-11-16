///////////////////////////////////////////////////////////////////////////////
// Name               : CommandShoePreset.cpp
// Purpose            :
// Thread Safe        : No
// Platform dependent : No
// Compiler Options   :
// Author             : Tobias Schaefer
// Created            : 01.05.2018
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

#include "CommandShoePreset.h"
#include "../../gui/gui.h"

CommandShoePreset::CommandShoePreset(const wxString &name, Project *project,
		int preset) :
		wxCommand(true, name) {
	this->project = project;
	this->preset = preset;
}

bool CommandShoePreset::Do(void) {
	if (project == NULL)
		return false;

	oldHeelHeight = project->shoe.heelHeight->GetFormula();
	oldBallHeight = project->shoe.ballHeight->GetFormula();
	oldToeAngle = project->shoe.toeSpring->GetFormula();

	// 0.26 m is the average footlength (= size EU 39).
	switch (preset) {
//	case ID_PRESETFLATS:
//		project->shoe.exprHeelHeight = _T("0 cm");
//		project->shoe.exprBallHeight = _T("0 cm");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETCLASSIC:
//		project->shoe.exprHeelHeight = _T("3 cm");
//		project->shoe.exprBallHeight = _T("1.5 cm");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETPLATEAU:
//		project->shoe.exprHeelHeight = _T("11 cm");
//		project->shoe.exprBallHeight = _T("9 cm");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETHHLOW:
//		project->shoe.exprHeelHeight = _T("7 cm * (L/26cm)");
//		project->shoe.exprBallHeight = _T("0 cm");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETHHMID:
//		project->shoe.exprHeelHeight = _T("10 cm * (L/26cm)");
//		project->shoe.exprBallHeight = _T("0 cm");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETHHHIGH:
//		project->shoe.exprHeelHeight = _T("13 cm * (L/26cm)");
//		project->shoe.exprBallHeight = _T("0 cm");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETPLATFORM:
//		project->shoe.exprHeelHeight = _T("15 cm * (L/26cm)");
//		project->shoe.exprBallHeight = _T("5 cm * (L/26cm)");
//		project->shoe.exprToeAngle = _T("0 deg");
//		break;
//	case ID_PRESETPONY:
//		project->shoe.exprHeelHeight = _T("16 cm * (L/26cm)");
//		project->shoe.exprBallHeight = _T("0 cm");
//		project->shoe.exprToeAngle = _T("45 deg");
//		break;
//	case ID_PRESETBALLET:
//		project->shoe.exprHeelHeight = _T("20 cm * (L/26cm)");
//		project->shoe.exprBallHeight = _T("0 cm");
//		project->shoe.exprToeAngle = _T("90 deg");
//		break;
	default:
		return false;
	}
//	project->Update(Project::UpdateFoot, Project::Both);
	return true;
}

bool CommandShoePreset::Undo(void) {
	if (project == NULL)
		return false;

	project->shoe.heelHeight->SetFormula(oldHeelHeight);
	project->shoe.ballHeight->SetFormula(oldBallHeight);
	project->shoe.toeSpring->SetFormula(oldToeAngle);

//	project->Update(Project::UpdateFoot, Project::Both);
	return true;
}

